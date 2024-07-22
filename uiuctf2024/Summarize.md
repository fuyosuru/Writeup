# Summarize

uthor: Nikhil

All you have to do is find six numbers. How hard can that be?

---

Đề bài cho ta một file binary, ta chạy thử và thấy yêu cầu nhập 6 số và check xem 6 số ta nhập vào có đúng không. Ta mở file trong ida:

```c
__int64 __fastcall main(int a1, char **a2, char **a3)
{
  unsigned int v4; // [rsp+18h] [rbp-58h] BYREF
  unsigned int v5; // [rsp+1Ch] [rbp-54h] BYREF
  unsigned int v6; // [rsp+20h] [rbp-50h] BYREF
  unsigned int v7; // [rsp+24h] [rbp-4Ch] BYREF
  unsigned int v8; // [rsp+28h] [rbp-48h] BYREF
  unsigned int v9; // [rsp+2Ch] [rbp-44h] BYREF
  char s[56]; // [rsp+30h] [rbp-40h] BYREF
  unsigned __int64 v11; // [rsp+68h] [rbp-8h]

  v11 = __readfsqword(0x28u);
  puts("To get the flag, you must correctly enter six 9-digit positive integers: a, b, c, d, e, and f.");
  putchar(10);
  printf("a = ");
  __isoc99_scanf("%d", &v4);
  printf("b = ");
  __isoc99_scanf("%d", &v5);
  printf("c = ");
  __isoc99_scanf("%d", &v6);
  printf("d = ");
  __isoc99_scanf("%d", &v7);
  printf("e = ");
  __isoc99_scanf("%d", &v8);
  printf("f = ");
  __isoc99_scanf("%d", &v9);
  if ( (unsigned __int8)sub_40137B(v4, v5, v6, v7, v8, v9, a2) )
  {
    puts("Correct.");
    sprintf(s, "uiuctf{%x%x%x%x%x%x}", v4, v5, v6, v7, v8, v9);
    puts(s);
  }
  else
  {
    puts("Wrong.");
  }
  return 0LL;
}
```

Ta thấy rằng chương trình sẽ check 6 số ta nhập vào ở hàm sub_40137B(), Ta sẽ xem nội dung hàm này:

```c
_BOOL8 __fastcall sub_40137B(
        unsigned int a1,
        unsigned int a2,
        unsigned int a3,
        unsigned int a4,
        unsigned int a5,
        unsigned int a6)
{
  unsigned int v7; // eax
  unsigned int v8; // ebx
  unsigned int v9; // eax
  unsigned int v10; // ebx
  unsigned int v11; // eax
  unsigned int v12; // eax
  unsigned int v18; // [rsp+20h] [rbp-30h]
  unsigned int v19; // [rsp+24h] [rbp-2Ch]
  unsigned int v20; // [rsp+28h] [rbp-28h]
  unsigned int v21; // [rsp+2Ch] [rbp-24h]
  unsigned int v22; // [rsp+30h] [rbp-20h]
  unsigned int v23; // [rsp+34h] [rbp-1Ch]
  unsigned int v24; // [rsp+38h] [rbp-18h]
  unsigned int v25; // [rsp+3Ch] [rbp-14h]

  if ( a1 <= 0x5F5E100 || a2 <= 0x5F5E100 || a3 <= 0x5F5E100 || a4 <= 0x5F5E100 || a5 <= 0x5F5E100 || a6 <= 0x5F5E100 )
    return 0LL;
  if ( a1 > 0x3B9AC9FF || a2 > 0x3B9AC9FF || a3 > 0x3B9AC9FF || a4 > 0x3B9AC9FF || a5 > 0x3B9AC9FF || a6 > 0x3B9AC9FF )
    return 0LL;
  v7 = sub_4016D8(a1, a2);
  v18 = (unsigned int)sub_40163D(v7, a3) % 0x10AE961;
  v19 = (unsigned int)sub_40163D(a1, a2) % 0x1093A1D;
  v8 = sub_4016FE(2LL, a2);
  v9 = sub_4016FE(3LL, a1);
  v10 = sub_4016D8(v9, v8);
  v20 = v10 % (unsigned int)sub_40174A(a1, a4);
  v11 = sub_40163D(a3, a1);
  v21 = (unsigned int)sub_4017A9(a2, v11) % 0x6E22;
  v22 = (unsigned int)sub_40163D(a2, a4) % a1;
  v12 = sub_40163D(a4, a6);
  v23 = (unsigned int)sub_40174A(a3, v12) % 0x1CE628;
  v24 = (unsigned int)sub_4016D8(a5, a6) % 0x1172502;
  v25 = (unsigned int)sub_40163D(a5, a6) % 0x2E16F83;
  return v18 == 4139449
      && v19 == 9166034
      && v20 == 556569677
      && v21 == 12734
      && v22 == 540591164
      && v23 == 1279714
      && v24 == 17026895
      && v25 == 23769303;
}
```

Ta sẽ đi qua nội dung của các hàm.

```c
__int64 __fastcall sub_4017A9(unsigned int a1, unsigned int a2)
{
  unsigned int v5; // [rsp+8h] [rbp-10h]
  int v6; // [rsp+Ch] [rbp-Ch]
  int v7; // [rsp+10h] [rbp-8h]
  int v8; // [rsp+14h] [rbp-4h]

  v5 = 0;
  v6 = 0;
  while ( a1 || a2 )
  {
    v7 = a1 & 1;
    v8 = a2 & 1;
    a1 >>= 1;
    a2 >>= 1;
    v5 += (v8 & v7) << v6++;
  }
  return v5;
}
```

Hàm này chỉ đơn giản là tính a1&a2.

```c
__int64 __fastcall sub_40174A(unsigned int a1, unsigned int a2)
{
  unsigned int v5; // [rsp+8h] [rbp-10h]
  int v6; // [rsp+Ch] [rbp-Ch]
  int v7; // [rsp+10h] [rbp-8h]
  int v8; // [rsp+14h] [rbp-4h]

  v5 = 0;
  v6 = 0;
  while ( a1 || a2 )
  {
    v7 = a1 & 1;
    v8 = a2 & 1;
    a1 >>= 1;
    a2 >>= 1;
    v5 += (v8 ^ v7) << v6++;
  }
  return v5;
}
```

Hàm này cũng chỉ đơn giản là tính a1^a2.

```c
__int64 __fastcall sub_4016FE(unsigned int a1, int a2)
{
  unsigned int v4; // [rsp+Ch] [rbp-Ch]
  int v5; // [rsp+10h] [rbp-8h]

  v4 = 0;
  v5 = 0;
  while ( a1 )
  {
    v4 += (a1 & 1) * (a2 << v5);
    a1 >>= 1;
    ++v5;
  }
  return v4;
}
```

Hàm này chỉ đơn giản là tính a1^a2 ( a1 mũ a2 )

```c
__int64 __fastcall sub_40163D(unsigned int a1, unsigned int a2)
{
  unsigned int v5; // [rsp+10h] [rbp-18h]
  char v6; // [rsp+14h] [rbp-14h]
  int v7; // [rsp+18h] [rbp-10h]
  int v8; // [rsp+1Ch] [rbp-Ch]
  __int64 v9; // [rsp+20h] [rbp-8h]

  v9 = 0LL;
  v5 = 0;
  v6 = 0;
  while ( a1 || a2 )
  {
    v7 = a1 & 1;
    v8 = a2 & 1;
    a1 >>= 1;
    a2 >>= 1;
    v9 += (v5 ^ v8 ^ v7) << v6;
    v5 = v5 & v7 | v8 & v7 | v5 & v8;
    ++v6;
  }
  return ((unsigned __int64)v5 << v6) + v9;
}
```

Hàm này đơn giản là tính a1+a2

Sau khi ta biết được tất cả các hàm làm gì thì ta chỉ việc code lại để tìm 6 số đúng.

Code tìm bằng z3:
```python
from z3 import *

a1 = BitVec('a1', 64)
a2 = BitVec('a2', 64)
a3 = BitVec('a3', 64)
a4 = BitVec('a4', 64)
a5 = BitVec('a5', 64)
a6 = BitVec('a6', 64)

def sub_40163D(a1, a2):
    return (a1+a2)& 0xFFFFFFFF

def sub_4016D8(a1, a2):
    return (a1-a2)& 0xFFFFFFFF

def sub_40174A(a1, a2):
    return (a1^a2)& 0xFFFFFFFF

def sub_4017A9(a1, a2):
    return (a1&a2)& 0xFFFFFFFF

def sub_4016FE(a1, a2):
    v4 = BitVecVal(0, 64)
    v5 = 0
    c=32
    while c>0:
        v4 = v4 + ((a1 & 1) * (a2 << v5))
        a1 = a1>>1
        v5 += 1
        c=c-1
    return v4 & 0xFFFFFFFF

solver = Solver()

solver.add(a1 > 0x5F5E100, a2 > 0x5F5E100, a3 > 0x5F5E100, a4 > 0x5F5E100, a5 > 0x5F5E100, a6 > 0x5F5E100)
solver.add(a1 <= 0x3B9AC9FF, a2 <= 0x3B9AC9FF, a3 <= 0x3B9AC9FF, a4 <= 0x3B9AC9FF, a5 <= 0x3B9AC9FF, a6 <= 0x3B9AC9FF)

v7 = sub_4016D8(a1, a2)
v18 = sub_40163D(v7, a3) % 0x10AE961
v19 = sub_40163D(a1, a2) % 0x1093A1D
v8 = sub_4016FE(2, a2)
v9 = sub_4016FE(3, a1)
v10 = sub_4016D8(v9, v8)
v20 = v10 % sub_40174A(a1, a4)
v11 = sub_40163D(a3, a1)
v21 = sub_4017A9(a2, v11) % 0x6E22
v22 = sub_40163D(a2, a4) % a1
v12 = sub_40163D(a4, a6)
v23 = sub_40174A(a3, v12) % 0x1CE628
v24 = sub_4016D8(a5, a6) % 0x1172502
v25 = sub_40163D(a5, a6) % 0x2E16F83

solver.add(v18 == 4139449)
solver.add(v19 == 9166034)
solver.add(v20 == 556569677)
solver.add(v21 == 12734)
solver.add(v22 == 540591164)
solver.add(v23 == 1279714)
solver.add(v24 == 17026895)
solver.add(v25 == 23769303)

if solver.check() == sat:
    model = solver.model()
    solution = {d.name(): model[d] for d in model.decls()}
    print("result:", solution)
else:
    print("Oh no!")

```

6 số cần tìm :
![image](https://hackmd.io/_uploads/By1-KAodR.png)

flag : uiuctf{2a142dd72e87fa9c1456a32d1bc4f77739975e5fcf5c6c0}
