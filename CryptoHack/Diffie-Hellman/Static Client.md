#    Static Client
You've just finished eavesdropping on a conversation between Alice and Bob. Now you have a chance to talk to Bob. What are you going to say?

Connect at socket.cryptohack.org 13373
## Solution
Ta thử tương tác với server:
```
Intercepted from Alice: {"p": "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff", "g": "0x02", "A": "0x77761ff7686a9b0a4d0de99bffdc757dc528d2f6fb710d2ded76ee9107b84b5d0dfa3cf124445be12ebee0ec2de94eb2c04d2485b83f06d3b45d7f1880ba5cfb4a7fb5def97760d38ab1412e09b2a33423f215e72e47cd664eb36862195848526d3d164c52eb2ca229d6a3266898f98e9b101cf6527636475b1a1af2f1be1ff12af24e9721ce04e69caba09bea0bd1bbe41972a46def057f2bf60e54fc0b0023c37e808f9b99e09be160c92c23c653ca7127d0ff7aff998f7c841e01fb735535"}
Intercepted from Bob: {"B": "0x8d79b69390f639501d81bdce911ec9defb0e93d421c02958c8c8dd4e245e61ae861ef9d32aa85dfec628d4046c403199297d6e17f0c9555137b5e8555eb941e8dcfd2fe5e68eecffeb66c6b0de91eb8cf2fd0c0f3f47e0c89779276fa7138e138793020c6b8f834be20a16237900c108f23f872a5f693ca3f93c3fd5a853dfd69518eb4bab9ac2a004d3a11fb21307149e8f2e1d8e1d7c85d604aa0bee335eade60f191f74ee165cd4baa067b96385aa89cbc7722e7426522381fc94ebfa8ef0"}
Intercepted from Alice: {"iv": "237428fd9028bdf2aa06e6ec585b0c09", "encrypted": "30d0d797a4e276a74f0609004cefbbf2a3642ff383b3761070fd48a48220ec3a"}
Bob connects to you, send him some parameters:
```

Ta có được các thông tin như p, g, A, B, iv, encrypted_flag của quá trình trao đổi khoá giữa Alice với Bob. Sau đó Bob connect với ta và ta là bên được chọn các tham số p và g cho quá trình trao đổi khoá.

Diffie–Hellman key exchange thông thường giữa Alice và Bob:
```
Alice->Bob  p,g,A
Bob->Alice  B
Alice tính shared key: B^a (mod p) 
Bob tính shared key: A^b (mod p)

Với A = g^a (mod p), B = g^b (mod p)
```

Để có thể decrypt được flag ta cần biết shared key của quá trình trao đổi khoá giữa Alice với Bob.

Vì ta được chọn các tham số p và g cho nên ta có thể chọn các tham số này để lừa Bob tính lại shared key của quá trình trao đổi khoá trước đó với Alice.

Nhận xét:
```
Shared key = A^b (mod p)
B = g^b (mod p)
```
Vậy nếu ta chọn g = A thì B trong quá trình Bob trao đổi khoá với ta sẽ là shared key.

Thực hiện:
```
fuyosuru@HuuTung:~$ nc socket.cryptohack.org 13373
Intercepted from Alice: {"p": "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff", "g": "0x02", "A": "0x864d2c8d39b8fa65144009aba26deb4611f63fc1d9dd69e630b2e0eebd68dc0e441d42800fc613d84868470d288681b7bfc840183b2cb70ae9fa1bb0814419c623543718b78e81802c91025573e0807b9339ac69c1e2bf87c797aa4c4ee03592d79f043b02ef90ff5f2506dc0cd54cc41ea958590cfa7d1855549a80b15aa33545a29908b733fe130f7f6ca129db1a39e3d1dd5b96a3d549eebb37dcfd018d2f984e1514c848557807cef91d1231544876a8aa64180e44892232b974db948b4f"}
Intercepted from Bob: {"B": "0x8d79b69390f639501d81bdce911ec9defb0e93d421c02958c8c8dd4e245e61ae861ef9d32aa85dfec628d4046c403199297d6e17f0c9555137b5e8555eb941e8dcfd2fe5e68eecffeb66c6b0de91eb8cf2fd0c0f3f47e0c89779276fa7138e138793020c6b8f834be20a16237900c108f23f872a5f693ca3f93c3fd5a853dfd69518eb4bab9ac2a004d3a11fb21307149e8f2e1d8e1d7c85d604aa0bee335eade60f191f74ee165cd4baa067b96385aa89cbc7722e7426522381fc94ebfa8ef0"}
Intercepted from Alice: {"iv": "f1d818adea119d0742d739dbbadea651", "encrypted": "ce717cda0d1f3941ccda0cf2f6afcc44a35da13147b32a093cb9d6fd52c39a9f"}
Bob connects to you, send him some parameters: {"p": "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff", "g": "0x864d2c8d39b8fa65144009aba26deb4611f63fc1d9dd69e630b2e0eebd68dc0e441d42800fc613d84868470d288681b7bfc840183b2cb70ae9fa1bb0814419c623543718b78e81802c91025573e0807b9339ac69c1e2bf87c797aa4c4ee03592d79f043b02ef90ff5f2506dc0cd54cc41ea958590cfa7d1855549a80b15aa33545a29908b733fe130f7f6ca129db1a39e3d1dd5b96a3d549eebb37dcfd018d2f984e1514c848557807cef91d1231544876a8aa64180e44892232b974db948b4f", "A": "0x0"}
Bob says to you: {"B": "0xde384de27616f4ec6bb434dcb6936c03029703a1901265a822980745a2eaad309dfd4c26d90d671a7e7e4f8b5321703565805d0d9fb1f8059340e098c4bae613553c421d32aff0910c7fcba87388f33b394e106db42d30ee9fca2c40a552f77dbd1269e45e8787c288c737244070c8196a821fa4482a03a6ca7f38c05821b3a80df78c42ce3416208a48417f518ffe4dc6b708088f9ba9fa92e86372d1fcac57439b84fbe78601a93395b5bb3aca915a396a9fe17db5b63bb17223eff4ce2f0f"}
Bob says to you: {"iv": "dde5828acf54e653d312356bed329403", "encrypted": "97870a327c92cb0b825c5bc03c751fa002eba8ad3444c51116f8a1e02f4c2ed15d6242c29bf28198b31d7f5d8231210a9abaf8af7370a7e4744330d1f7b03ec197445c55548861a3d337aec8970e0b53"}
```
Script:
```
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import hashlib


def is_pkcs7_padded(message):
    padding = message[-message[-1]:]
    return all(padding[i] == len(padding) for i in range(0, len(padding)))

def decrypt_flag(shared_secret: int, iv: str, ciphertext: str):
    sha1 = hashlib.sha1()
    sha1.update(str(shared_secret).encode('ascii'))
    key = sha1.digest()[:16]
    ciphertext = bytes.fromhex(ciphertext)
    iv = bytes.fromhex(iv)
    cipher = AES.new(key, AES.MODE_CBC, iv)
    plaintext = cipher.decrypt(ciphertext)
    if is_pkcs7_padded(plaintext):
        return unpad(plaintext, 16).decode('ascii')
    else:
        return plaintext.decode('ascii')
    
iv = "f1d818adea119d0742d739dbbadea651"
encrypted_flag = "ce717cda0d1f3941ccda0cf2f6afcc44a35da13147b32a093cb9d6fd52c39a9f"
shared_secret = 0xde384de27616f4ec6bb434dcb6936c03029703a1901265a822980745a2eaad309dfd4c26d90d671a7e7e4f8b5321703565805d0d9fb1f8059340e098c4bae613553c421d32aff0910c7fcba87388f33b394e106db42d30ee9fca2c40a552f77dbd1269e45e8787c288c737244070c8196a821fa4482a03a6ca7f38c05821b3a80df78c42ce3416208a48417f518ffe4dc6b708088f9ba9fa92e86372d1fcac57439b84fbe78601a93395b5bb3aca915a396a9fe17db5b63bb17223eff4ce2f0f
print(decrypt_flag(shared_secret, iv, encrypted_flag))
```

Flag: crypto{n07_3ph3m3r4l_3n0u6h}



