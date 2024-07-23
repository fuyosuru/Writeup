# SVM Revenge

by ksaweryr

Description:

As foretold, the revenge of SVM from round 46 is here!

---

Ta mở file trong ida và thấy ở hàm main chương trình sẽ đọc input từ flag.txt sau đó encrypt data và lưu vào trong output.bin . Mỗi lần chương trình sẽ encrypt 16 byte của flag và độ dài của flag là 64.

Hàm encrypt :
```c
__int64 __fastcall sub_1396(__int64 a1, __int64 a2)
{
  int v2; // eax
  __int64 result; // rax
  unsigned int v4; // [rsp+18h] [rbp-8h]
  int v5; // [rsp+1Ch] [rbp-4h]

  v5 = 0;
  while ( 1 )
  {
    v4 = *(unsigned __int8 *)(v5 + a2);
    v2 = v5 + 1;
    v5 += 2;
    result = *(unsigned __int8 *)(v2 + a2);
    if ( !v4 )
      break;
    sub_1253(a1, v4, (unsigned __int8)result);
  }
  return result;
}
```

Ta thấy chương trình sẽ lần lượt lấy 2 byte từ mảng a2 ( đã có sẵn trong file binary ) để làm tham số cho hàm sub_1253().

```c
unsigned __int64 __fastcall sub_1253(__int64 a1, unsigned int a2, unsigned __int8 a3)
{
  unsigned __int64 result; // rax
  char v5; // [rsp+18h] [rbp-8h]
  char v6; // [rsp+18h] [rbp-8h]
  char v7; // [rsp+1Ch] [rbp-4h]
  char v8; // [rsp+1Ch] [rbp-4h]
  char v9; // [rsp+1Ch] [rbp-4h]

  result = a2;
  switch ( a2 )
  {
    case 1u:
      v9 = sub_11F7(a1 + 40);
      v6 = sub_11F7(a1 + 40);
      result = (unsigned __int64)sub_1189((_QWORD *)(a1 + 40), v6 * v9);
      break;
    case 2u:
      result = (unsigned __int64)sub_1189((_QWORD *)(a1 + 40), *(_BYTE *)(a1 + a3));
      break;
    case 3u:
      v8 = sub_11F7(a1 + 40);
      v5 = sub_11F7(a1 + 40);
      result = (unsigned __int64)sub_1189((_QWORD *)(a1 + 40), v8 + v5);
      break;
    case 4u:
      v7 = sub_11F7(a1 + 40);
      result = a3;
      *(_BYTE *)(a1 + a3) = v7;
      break;
    case 5u:
      result = (unsigned __int64)sub_1189((_QWORD *)(a1 + 40), a3);
      break;
    default:
      return result;
  }
  return result;
}
```

Ở đây có 2 hàm là sub_11F7() và sub_1189() như sau:
```c
__int64 __fastcall sub_11F7(unsigned __int8 **a1)
{
  unsigned int v2; // [rsp+14h] [rbp-Ch]
  _QWORD *ptr; // [rsp+18h] [rbp-8h]

  ptr = *a1;
  v2 = **a1;
  *a1 = (unsigned __int8 *)*((_QWORD *)*a1 + 1);
  if ( !*a1 )
    a1[1] = 0LL;
  free(ptr);
  return v2;
}
```

```c
_QWORD *__fastcall sub_1189(_QWORD *a1, char a2)
{
  _QWORD *result; // rax
  _QWORD *v3; // [rsp+18h] [rbp-8h]

  v3 = malloc(0x10uLL);
  *(_BYTE *)v3 = a2;
  v3[1] = 0LL;
  if ( a1[1] )
    *(_QWORD *)(a1[1] + 8LL) = v3;
  else
    *a1 = v3;
  result = a1;
  a1[1] = v3;
  return result;
}
```

2 hàm này chính là chức năng push() và pop() của cấu trúc dữ liệu queue, sub_1189() sẽ thêm tham số a2 vào queue, sub_11F7() sẽ xoá số ở đầu queue và trả về giá trị của số này

Vậy ta có thể viết hàm sub_1253() thành :
```python
    if a2==1:
        x=pop()
        y=pop()
        push(x*y)
    if a2==2:
        push(tmp[a3])
    if a2==3:
        x=pop()
        y=pop()
        push(x+y)
    if a2==4:
        x=pop()
        tmp[a3]=x
    if a2==5:
        push(a3)
```

Vì mảng chứa các thao tác encrypt quá lớn cho nên ta sẽ dùng python để mô phỏng quá trình encrypt dưới dạng text : 

Code:
```python
ops = "lấy ở trong file binary"

tmp=[0]*100
queue=[]
for i in range(16):
    queue.append(f'flag[{i}]')
def lay():
    ans=queue[0]
    queue.pop(0)
    return ans

def them(x):
    queue.append(x)
    return

for i in range(0,len(ops),2):
    if ops[i]==1:
        x=lay()
        y=lay()
        them(f'{x}*{y}')
    if ops[i]==2:
        them(tmp[ops[i+1]])
    if ops[i]==3:
        x=lay()
        y=lay()
        them(f'{x}+{y}')
    if ops[i]==4:
        x=lay()
        tmp[ops[i+1]]=x
    if ops[i]==5:
        them(ops[i+1])
for i in range(16):
    j=lay()
    print(j)
```
 
 Ta sẽ có quá trình tính toán như sau :
 ```
 flag[0]*170+flag[1]*237+flag[2]*236+flag[3]*93+flag[4]*142+flag[5]*135+flag[6]*65+flag[7]*255+flag[8]*166+flag[9]*166+flag[10]*16+flag[11]*91+flag[12]*198+flag[13]*1+flag[14]*122+flag[15]*253
flag[0]*61+flag[1]*113+flag[2]*174+flag[3]*90+flag[4]*79+flag[5]*220+flag[6]*48+flag[7]*235+flag[8]*141+flag[9]*151+flag[10]*254+flag[11]*40+flag[12]*64+flag[13]*76+flag[14]*131+flag[15]*127
flag[0]*115+flag[1]*204+flag[2]*168+flag[3]*225+flag[4]*233+flag[5]*242+flag[6]*141+flag[7]*102+flag[8]*80+flag[9]*175+flag[10]*32+flag[11]*117+flag[12]*30+flag[13]*15+flag[14]*213+flag[15]*91
flag[0]*60+flag[1]*35+flag[2]*61+flag[3]*255+flag[4]*133+flag[5]*212+flag[6]*75+flag[7]*110+flag[8]*81+flag[9]*35+flag[10]*159+flag[11]*111+flag[12]*146+flag[13]*237+flag[14]*215+flag[15]*142
flag[0]*179+flag[1]*68+flag[2]*170+flag[3]*215+flag[4]*255+flag[5]*48+flag[6]*250+flag[7]*58+flag[8]*77+flag[9]*39+flag[10]*49+flag[11]*22+flag[12]*83+flag[13]*93+flag[14]*73+flag[15]*96
flag[0]*130+flag[1]*47+flag[2]*195+flag[3]*93+flag[4]*192+flag[5]*178+flag[6]*12+flag[7]*43+flag[8]*151+flag[9]*29+flag[10]*29+flag[11]*55+flag[12]*86+flag[13]*76+flag[14]*161+flag[15]*55
flag[0]*128+flag[1]*107+flag[2]*107+flag[3]*112+flag[4]*214+flag[5]*51+flag[6]*36+flag[7]*132+flag[8]*207+flag[9]*213+flag[10]*25+flag[11]*166+flag[12]*254+flag[13]*206+flag[14]*248+flag[15]*98
flag[0]*21+flag[1]*72+flag[2]*129+flag[3]*228+flag[4]*192+flag[5]*190+flag[6]*109+flag[7]*203+flag[8]*19+flag[9]*35+flag[10]*56+flag[11]*202+flag[12]*249+flag[13]*95+flag[14]*176+flag[15]*159
flag[0]*39+flag[1]*66+flag[2]*218+flag[3]*115+flag[4]*168+flag[5]*25+flag[6]*32+flag[7]*137+flag[8]*73+flag[9]*185+flag[10]*80+flag[11]*213+flag[12]*69+flag[13]*68+flag[14]*59+flag[15]*16
flag[0]*12+flag[1]*197+flag[2]*214+flag[3]*249+flag[4]*215+flag[5]*165+flag[6]*171+flag[7]*176+flag[8]*79+flag[9]*65+flag[10]*251+flag[11]*16+flag[12]*142+flag[13]*112+flag[14]*6+flag[15]*17
flag[0]*205+flag[1]*251+flag[2]*189+flag[3]*70+flag[4]*254+flag[5]*140+flag[6]*181+flag[7]*213+flag[8]*252+flag[9]*112+flag[10]*106+flag[11]*243+flag[12]*66+flag[13]*193+flag[14]*147+flag[15]*180
flag[0]*149+flag[1]*205+flag[2]*80+flag[3]*192+flag[4]*174+flag[5]*110+flag[6]*160+flag[7]*121+flag[8]*103+flag[9]*78+flag[10]*174+flag[11]*90+flag[12]*205+flag[13]*99+flag[14]*2+flag[15]*174
flag[0]*79+flag[1]*144+flag[2]*161+flag[3]*243+flag[4]*140+flag[5]*55+flag[6]*149+flag[7]*18+flag[8]*60+flag[9]*201+flag[10]*149+flag[11]*216+flag[12]*2+flag[13]*226+flag[14]*49+flag[15]*81
flag[0]*114+flag[1]*188+flag[2]*139+flag[3]*5+flag[4]*60+flag[5]*238+flag[6]*7+flag[7]*114+flag[8]*202+flag[9]*44+flag[10]*75+flag[11]*118+flag[12]*130+flag[13]*100+flag[14]*8+flag[15]*91
flag[0]*18+flag[1]*91+flag[2]*153+flag[3]*190+flag[4]*44+flag[5]*223+flag[6]*146+flag[7]*193+flag[8]*250+flag[9]*24+flag[10]*25+flag[11]*51+flag[12]*158+flag[13]*102+flag[14]*122+flag[15]*164
flag[0]*211+flag[1]*205+flag[2]*34+flag[3]*52+flag[4]*227+flag[5]*81+flag[6]*76+flag[7]*70+flag[8]*147+flag[9]*203+flag[10]*66+flag[11]*176+flag[12]*190+flag[13]*199+flag[14]*34+flag[15]*48
```

Và lại là nhân ma trận.

Code python giải cho từng 16 byte bằng numpy:
```python
import numpy as np
from sympy import Matrix, mod_inverse

heso=[170, 237, 236, 93, 142, 135, 65, 255, 166, 166, 16, 91, 198, 1, 122, 253, 61, 113, 174, 90, 79, 220, 48, 235, 141, 151, 254, 40, 64, 76, 131, 127, 115, 204, 168, 225, 233, 242, 141, 102, 
80, 175, 32, 117, 30, 15, 213, 91, 60, 35, 61, 255, 133, 212, 75, 110, 81, 35, 159, 111, 146, 237, 215, 142, 179, 68, 170, 215, 255, 48, 250, 58, 77, 39, 49, 22, 83, 93, 73, 96, 130, 47, 195, 93, 192, 178, 12, 43, 151, 29, 29, 55, 86, 76, 161, 55, 128, 107, 107, 112, 214, 51, 36, 132, 207, 213, 25, 166, 254, 206, 248, 98, 21, 72, 129, 228, 192, 190, 109, 203, 19, 35, 56, 202, 249, 95, 176, 159, 39, 66, 218, 115, 168, 25, 32, 137, 73, 185, 80, 213, 69, 68, 59, 16, 12, 197, 214, 249, 215, 165, 171, 176, 79, 65, 251, 16, 142, 112, 6, 17, 205, 251, 189, 70, 254, 140, 181, 213, 252, 112, 106, 243, 66, 193, 147, 180, 149, 205, 80, 192, 174, 110, 160, 121, 103, 78, 174, 90, 205, 99, 2, 174, 79, 144, 161, 243, 140, 55, 149, 18, 60, 201, 149, 216, 2, 
226, 49, 81, 114, 188, 139, 5, 60, 238, 7, 114, 202, 44, 75, 118, 130, 100, 8, 91, 18, 91, 153, 190, 44, 223, 146, 193, 250, 24, 25, 51, 158, 102, 122, 164, 211, 205, 34, 52, 227, 81, 76, 
70, 147, 203, 66, 176, 190, 199, 34, 48]

b = np.array([ 46,  50,  22,  33, 141, 130,  61,  95,  34, 245, 190, 153,  95,  41,
  10,  58]) #16 byte đầu của output.bin

def solve(A, b, mod):
    A = Matrix(A) % mod
    b = Matrix(b) % mod
    A_inv = A.inv_mod(mod)
    x = A_inv @ b
    return np.array(x) % mod

A = np.zeros((16, 16), dtype=int)
for i in range(16):
    for j in range(16):
        A[i,j]=heso[i*16+j]
x = solve(A, b, 256)
solution_list = x.tolist()
print("Result:", solution_list)

```

Flag : ictf{S_d1dnt_5t4nd_f0r_5t4ck_b3c4u53_h3r3_I_us3d_4_L1nk3d_qu3u3}

