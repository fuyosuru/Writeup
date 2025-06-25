#    Parameter Injection
You're in a position to not only intercept Alice and Bob's DH key exchange, but also rewrite their messages. Think about how you can play with the DH equation that they calculate, and therefore sidestep the need to crack any discrete logarithm problem.

Use the script from "Deriving Symmetric Keys" to decrypt the flag once you've recovered the shared secret.

Connect at socket.cryptohack.org 13371

## Solution

Ta thử tương tác với server:
```
fuyosuru@HuuTung:~$ nc socket.cryptohack.org 13371
Intercepted from Alice: {"p": "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff", "g": "0x02", "A": "0xa41587c3e8bcc6c6cd2594fffc2dd4b7b002c6b5b56ca9cc32df61d216e844146f15c95b8ba9de02c59e49908e98bd21b75769499ec0a140288edd16b9881ca5dd4bbe239884577f0d3296e0fc8a83ffeca64b5f41d7ee8813950c5577fa0ae5bfb72dac4eed943fbb33637fc74cb76e72d500259366277d1ab60615f3ef9c6feb3a6531f61e57d9fdba38c4919fc7ccc73a99e2092610e707637aff81f442fd761f0f6117e682b7581e2d6fc3936d1f3b3526883a41917e42c1b87fc66d4c60"}
Send to Bob:
```
Alice đang dùng DH để trao đổi khoá với Bob

Diffie–Hellman key exchange thông thường giữa A và B:
```
A->B  p,g,A
B->A  B
A tính shared key: B^a (mod p) 
B tính shared key: A^b (mod p)
```

Ta dùng MITM attack để thay đổi giá trị của A và B trong quá trình truyền dữ liệu bằng p. Khi đó B^a hay A^b tương đương với             p^a (mod p) = 0 đồng nghĩa với shared key = 0.

MITM attack:
```
A->M p,g,A
M->B p,g,p
B->M B
M->A p
A tính shared key: p^a (mod p) 
B tính shared key: p^b (mod p)
```
Thực hiện:
```
Intercepted from Alice: {"p": "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff", "g": "0x02", "A": "0xa41587c3e8bcc6c6cd2594fffc2dd4b7b002c6b5b56ca9cc32df61d216e844146f15c95b8ba9de02c59e49908e98bd21b75769499ec0a140288edd16b9881ca5dd4bbe239884577f0d3296e0fc8a83ffeca64b5f41d7ee8813950c5577fa0ae5bfb72dac4eed943fbb33637fc74cb76e72d500259366277d1ab60615f3ef9c6feb3a6531f61e57d9fdba38c4919fc7ccc73a99e2092610e707637aff81f442fd761f0f6117e682b7581e2d6fc3936d1f3b3526883a41917e42c1b87fc66d4c60"}
Send to Bob: {"p": "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff", "g": "0x02", "A": "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff"}
Intercepted from Bob: {"B": "0x1cca2a5b1f942d4679f52953f0d4681ac57dc73090927b6b8f19c4bd1381186a6baf18ab457ba0f5c6979cf81d723690c00b01306450498b723437674e98eba5bee57b4157a25b113248af351f73db55dc2a16e27e0d9cc10a0a3d8c8fb4de7bfed0bdccf94044e50f59e337b114bbfbc58a49a087adfc19cf3b2a0ee3228ffb5ada2c84afb0116824def2f732eaaf7a847f314c6159444d142897de5aab587b88d9179f300c2dd47784bb9ac8436d7afe0bdcdde4d07d0b395654a5b7e755d4"}
Send to Alice: {"B": "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff"}
Intercepted from Alice: {"iv": "000d960da3702307fdb061cd87191a74", "encrypted_flag": "05f2764499da8b3253877da7f550e397331bd57ae70bafdb9c0df70714dd28b5"}
```


Dùng file decrypt mà đề bài cung cấp để decrypt flag với các tham số:
```
******
shared_secret = 0
iv = "000d960da3702307fdb061cd87191a74"
ciphertext = "05f2764499da8b3253877da7f550e397331bd57ae70bafdb9c0df70714dd28b5"

print(decrypt_flag(shared_secret, iv, ciphertext))
```

Flag: crypto{n1c3_0n3_m4ll0ry!!!!!!!!}


