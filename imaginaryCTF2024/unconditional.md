# unconditional 
Description:

Can you reverse this flag mangler? The output is b4,31,8e,02,af,1c,5d,23,98,7d,a3,1e,b0,3c,b3,c4,a6,06,58,28,19,7d,a3,c0,85,31,68,0a,bc,03,5d,3d,0b

The input only contains lowercase letters, numbers, underscore, and braces .

---

Ở bài này thì ta mở file trong ida và thấy rằng file chỉ đơn giản là encrypted flag và code cũng không quá khó đọc cho nên ta có thể viết lại bằng python và giải bằng z3.

Code giải :
```python
enc=[0xb4,0x31,0x8e,0x02,0xaf,0x1c,0x5d
     ,0x23,0x98,0x7d,0xa3
     ,0x1e,0xb0,0x3c,0xb3,0xc4,0xa6,0x06,0x58
     ,0x28,0x19,0x7d,0xa3,0xc0,0x85
     ,0x31,0x68,0x0a,0xbc
     ,0x03,0x5d,0x3d,0x0b]
table2=[1, 3, 4, 2, 6, 5]
table1=[0x52, 0x64, 0x71, 0x51, 0x54, 0x76]
flag = [0] * 100
counter1 = 0  
counter2 = 0  
def process(a1, flag, table1, table2, counter1, counter2):
    v3 = flag[a1]
    v4 = (a1 & 1) != 0
    v1 = (v3 > 0x60 and v3 <= 0x7A)
    if v1:
        rotated_v3 = ((v3 >> table2[counter2]) | (v3 << (8 - table2[counter2]))) & 0xFF
    else:
        rotated_v3 = (((v3 << 6) | (v3 >> 2)) ^ table1[counter1])&0xff
    if (a1 & 1) == 0:
        flag[a1] = rotated_v3
    else:
        if v1:
            flag[a1] = (v3 ^ table1[counter1]) & 0xFF
        else:
            flag[a1] = ((4 * v3) | (v3 >> 6)) & 0xFF
    counter1 = (v4 + counter1) % 6
    counter2 = (v4 + counter2) % 6
    
    return counter1, counter2
flagchar = "_}{abcdefghijklmnopqrstuvwxyz0123456789"

a1 = 0  
for i in range(len(enc)):
    r1=0
    r2=0
    for j in flagchar:
        flag[i]=ord(j)
        ncounter1,ncounter2=process(i,flag,table1,table2,counter1,counter2)
        if flag[i]==enc[i]:
            print(j,end='')
            r1,r2=ncounter1,ncounter2
    counter1,counter2=r1,r2
```

Flag : ictf{m0r3_than_1_way5_t0_c0n7r0l}

Không biết là do đề lỗi hay mình code sai nhưng có một số vị trí của flag có tận 2 ký tự thoả mãn nhưng may mắn là chỉ có 3 vị trí như vậy nên mình vẫn có thể đoán được flag đúng.
