#    Export-grade
Alice and Bob are using legacy codebases and need to negotiate parameters they both support. You've man-in-the-middled this negotiation step, and can passively observe thereafter. How are you going to ruin their day this time?

Connect at socket.cryptohack.org 13379

## Solution
Ta thử tương tác với server:
```
fuyosuru@HuuTung:~$ nc socket.cryptohack.org 13379
Intercepted from Alice: {"supported": ["DH1536", "DH1024", "DH512", "DH256", "DH128", "DH64"]}
Send to Bob:
```
Alice gửi cho Bob danh sách các parameter của DH để Bob chọn. Ta thấy rằng trong danh sách có "DH64" đây là bộ tham số yếu nhất và hoàn toàn có thể bị brute force cho nên ta có thể kết hợp giữa MITM attack cùng với Downgrade attack.

Vậy ta chỉ cần thay đổi danh sách các parameter mà Alice có thành ["DH64"] để ép Bob chọn bộ tham số này và sau đó lấy các tham số để brute force shared key.

Thực hiện:
```
fuyosuru@HuuTung:~$ nc socket.cryptohack.org 13379
Intercepted from Alice: {"supported": ["DH1536", "DH1024", "DH512", "DH256", "DH128", "DH64"]}
Send to Bob: {"supported": ["DH64"]}
Intercepted from Bob: {"chosen": "DH64"}
Send to Alice: {"chosen": "DH64"}
Intercepted from Alice: {"p": "0xde26ab651b92a129", "g": "0x2", "A": "0x16df125a24e7d381"}
Intercepted from Bob: {"B": "0x4eabfdc0e904ed3"}
Intercepted from Alice: {"iv": "c44b220440081470e095abc7f306d454", "encrypted_flag": "0e249964fb567f737e47156c3d81896783957809832dc6ff967a356214fb9acd"}
```

Script giải:
```
from sympy import discrete_log
from Crypto.Cipher import AES
from Crypto.Hash import SHA1

p = 0xde26ab651b92a129
g = 0x2
A = 0x16df125a24e7d381
B = 0x4eabfdc0e904ed3
iv = bytes.fromhex("c44b220440081470e095abc7f306d454")
encrypted_flag = bytes.fromhex("0e249964fb567f737e47156c3d81896783957809832dc6ff967a356214fb9acd")
b = discrete_log(p, B, g)
s = pow(A, b, p)
sha1 = SHA1.new(str(s).encode('ascii'))
key = sha1.digest()[:16]
print("AES Key:", key.hex())
cipher = AES.new(key, AES.MODE_CBC, iv)
decrypted = cipher.decrypt(encrypted_flag)
plaintext = decrypted.decode('utf-8')
print("Decrypted flag:", plaintext)
```

Flag: crypto{d0wn6r4d35_4r3_d4n63r0u5}



