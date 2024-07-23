# Rust

by NoobMaster

Description:

Rust! Enjoy 😃 Note: The message that produces the provided encryption is the flag.

---

Ta chạy thử file binary và thấy yêu cầu nhập như sau :

![image](https://github.com/user-attachments/assets/208b0eb3-047e-4481-aa69-3b89558bcc89)

Ta thấy độ dài của Encrypted bằng với độ dài của message ta nhập vào vì vậy có vẻ chương trình sẽ encrypt các chữ cái của message.

Ta mở file trong ida đi đến hàm encrypt :

```c
  v10 = *((_QWORD *)&key + 1);
  v11 = key;
  v20 = message;
  v21 = key;
  v2 = core::str::_$LT$impl$u20$str$GT$::len::hea9a06285cae512c(message);
  alloc::vec::Vec$LT$T$GT$::with_capacity::h0c09e3a492d17de2(
    (alloc::vec::Vec<i128,alloc::alloc::Global> *)&v12.__0.it.end_or_len,
    v2);
  v3 = core::str::_$LT$impl$u20$str$GT$::bytes::h3e8f599031eaef47(message);
  v13 = _$LT$I$u20$as$u20$core..iter..traits..collect..IntoIterator$GT$::into_iter::h28d3d56b117fadf6(v3);
  while ( 1 )
  {
    v14 = _$LT$core..str..iter..Bytes$u20$as$u20$core..iter..traits..iterator..Iterator$GT$::next::h8e93881ae4910881(&v13).gap0[0];
    v15 = v4;
    if ( (v14 & 1) == 0 )
      break;
    v22 = v15;
    v23 = v15 * (__int128)32LL;
    v5 = v23 >> 3;
    v25 = 0LL;
    v24 = v5;
    v27 = v10;
    v26 = v5 ^ v11;
    *(_OWORD *)&v9[40] = __PAIR128__(v10, v5 ^ v11) + 0x539;
    if ( __OFADD__(__CFADD__(v5 ^ v11, 1337LL), v10) )
    {
      v7.data_ptr = (u8 *)"attempt to add with overflow";
      v7.length = 28LL;
      core::panicking::panic::hf516d800ede7db8c(v7);
    }
    v28 = __PAIR128__(v10, v5 ^ v11) + 0x539;
    *(_OWORD *)&v9[8] = ~*(_OWORD *)&v9[40];
    v29 = ~*(_OWORD *)&v9[40];
    *(_QWORD *)&v9[24] = 2 * v11;
    v6 = __CFADD__(v11, v11) + v10;
    *(_QWORD *)&v9[32] = v10 + v6;
    if ( __CFADD__(__CFADD__(v11, v11), v10) | __CFADD__(v10, v6) )
    {
      v8.data_ptr = (u8 *)"attempt to multiply with overflowEnter the message:Enter the key (in hex): /build/rustc-kAv1jW"
                          "/rustc-1.75.0+dfsg0ubuntu1~bpo0/library/core/src/alloc/layout.rs";
      v8.length = 33LL;
      core::panicking::panic::hf516d800ede7db8c(v8);
    }
    v30 = 2 * v11;
    v31 = *(_QWORD *)&v9[32];
    alloc::vec::Vec$LT$T$C$A$GT$::push::h0f01f38e51e5f0bd(
      (alloc::vec::Vec<i128,alloc::alloc::Global> *)&v12.__0.it.end_or_len,
      *(i128 *)&v9[8]);
  }
  p_end_or_len = &v12.__0.it.end_or_len;
```

Thông qua đọc code và debug thì biết được rằng flow chương trình như sau: các chữ cái của message sẽ bị encrypt độc lập với nhau, quá trình encrypt như sau:
```Encrypted[i]=~((message[i]*4)^key+0x539)```
Code giải bằng python:
```python
def twos_complement_128bit_hex(number):
    positive_binary = bin(abs(number))[2:]
    positive_binary_padded = positive_binary.zfill(128)
    inverted_binary = ''.join('1' if bit == '0' else '0' for bit in positive_binary_padded)
    inverted_number = int(inverted_binary, 2)
    twos_complement = (inverted_number + 1)
    twos_complement_hex = hex(twos_complement)[10:]
    return twos_complement_hex

def hex_to_bin(hex_string):
    integer_value = int(hex_string, 16)
    binary_string = bin(integer_value)[2:]
    return binary_string

enc=[]
number = [-42148619422891531582255418903, -42148619422891531582255418927, -42148619422891531582255418851, -42148619422891531582255418907, -42148619422891531582255418831, -42148619422891531582255418859, -42148619422891531582255418855, -42148619422891531582255419111, -42148619422891531582255419103, -42148619422891531582255418687, -42148619422891531582255418859, -42148619422891531582255419119, -42148619422891531582255418843, -42148619422891531582255418687, -42148619422891531582255419103, -42148619422891531582255418907, -42148619422891531582255419107, -42148619422891531582255418915, -42148619422891531582255419119, -42148619422891531582255418935, -42148619422891531582255418823]
for i in number:
    enc.append(hex_to_bin(twos_complement_128bit_hex(i)))

for s in enc:
    t=""
    for i in s:
        if i=='1':
            t=t+'0'
        else:
            t=t+'1'
    print(chr(((int(t,2)-0x539)^0x83085554737383682184979)//4),end='')

```

Flag : ictf{ru57_r3v_7f4d3a}
