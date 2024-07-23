# watchdog

by cleverbear57

Description:

The keepers of the Watchdog vault have forgotten their password. Can you help them retrieve it?

---

Ta chạy thử file binary và thấy file chỉ đơn giản là check flag. Ta mở file trong ida:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int v3; // ebx
  __int64 v4; // rax
  __int64 v5; // rdx
  __int64 v6; // rdx
  __int64 v7; // rax
  _BOOL4 v8; // r12d
  __int64 v9; // rdx
  __int64 *v10; // rax
  __int64 v11; // rax
  int v12; // r12d
  __int64 v13; // rax
  int i; // [rsp+Ch] [rbp-C4h]
  char v16[32]; // [rsp+10h] [rbp-C0h] BYREF
  char v17[32]; // [rsp+30h] [rbp-A0h] BYREF
  char v18[32]; // [rsp+50h] [rbp-80h] BYREF
  char v19[32]; // [rsp+70h] [rbp-60h] BYREF
  char v20[40]; // [rsp+90h] [rbp-40h] BYREF
  unsigned __int64 v21; // [rsp+B8h] [rbp-18h]

  v21 = __readfsqword(0x28u);
  v4 = std::operator<<<std::char_traits<char>>(&std::cout, &unk_2B40, envp);
  std::ostream::operator<<(v4, &std::endl<char,std::char_traits<char>>);
  std::operator<<<std::char_traits<char>>(&std::cout, "Enter the password to the Watchdog vault: ", v5);
  std::string::basic_string(v19);
  std::operator>><char>(&std::cin, v19);
  if ( std::string::length(v19) == 43 )
  {
    std::string::basic_string(v20, v19);
    flagToNum(v16, v20);
    std::string::~string(v20);
    std::vector<unsigned long>::vector(v18, v16);
    evalMultiPoly(v17, v18);
    std::vector<unsigned long>::~vector(v18);
    for ( i = 0; i <= 43; ++i )
    {
      v10 = (__int64 *)std::vector<unsigned long>::operator[](v17, i);
      v9 = *v10;
      if ( *v10 != answer[i] )
      {
        v11 = std::operator<<<std::char_traits<char>>(&std::cout, "Try Again :(", v9);
        std::ostream::operator<<(v11, &std::endl<char,std::char_traits<char>>);
        v3 = 0;
        v12 = 0;
        goto LABEL_9;
      }
    }
    v13 = std::operator<<<std::char_traits<char>>(&std::cout, "Correct!", v9);
    std::ostream::operator<<(v13, &std::endl<char,std::char_traits<char>>);
    v12 = 1;
LABEL_9:
    std::vector<unsigned long>::~vector(v17);
    std::vector<unsigned long>::~vector(v16);
    v8 = v12 == 1;
  }
  else
  {
    v7 = std::operator<<<std::char_traits<char>>(&std::cout, "Try Again :(", v6);
    std::ostream::operator<<(v7, &std::endl<char,std::char_traits<char>>);
    v3 = 0;
    v8 = 0;
  }
  std::string::~string(v19);
  if ( v8 )
    return 0;
  return v3;
}
```

Đầu tiên ta thấy rằng flag có độ dài là 43 và bị encrypt ở hàm evalMultiPoly() và được check ở cuối.

```c
__int64 __fastcall evalMultiPoly(__int64 a1, __int64 a2)
{
  int i; // [rsp+14h] [rbp-3Ch]
  __int64 v4; // [rsp+18h] [rbp-38h] BYREF
  char v5[24]; // [rsp+20h] [rbp-30h] BYREF
  unsigned __int64 v6; // [rsp+38h] [rbp-18h]

  v6 = __readfsqword(0x28u);
  std::vector<unsigned long>::vector(a1);
  for ( i = 2; i < (unsigned __int64)(std::vector<unsigned long>::size(a2) + 3); ++i )
  {
    std::vector<unsigned long>::vector(v5, a2);
    v4 = evalVectPoly(v5, i);
    std::vector<unsigned long>::push_back(a1, &v4);
    std::vector<unsigned long>::~vector(v5);
  }
  return a1;
}
```

Hàm evalMultiPoly() sẽ loop các giá trị từ 2 đến 45, với mỗi giá trị sẽ gọi hàm evalVectPoly()

```c
__int64 __fastcall evalVectPoly(__int64 a1, unsigned __int64 a2)
{
  __int64 v2; // rbx
  _BYTE v4[12]; // [rsp+1Ch] [rbp-24h]
  __int64 v5; // [rsp+28h] [rbp-18h]

  *(_DWORD *)&v4[8] = 0;
  *(_QWORD *)v4 = (unsigned int)std::vector<unsigned long>::size(a1) - 1;
  while ( *(int *)v4 >= 0 )
  {
    v5 = std::vector<unsigned long>::size(a1) - *(int *)v4 - 1;
    v2 = *(_QWORD *)std::vector<unsigned long>::operator[](a1, v5);
    *(_QWORD *)&v4[4] += v2 * my_pow(a2, *(int *)v4);
    --*(_DWORD *)v4;
  }
  return *(_QWORD *)&v4[4];
}
```

Hàm này sẽ tính giá trị của phương trình bậc 43 với x là các giá trị từ 2 đến 45 đã được truyền vào trước đó và hệ số là flag.

Vậy tóm lại file sẽ tính tích của 2 ma trận, ma trận bên trái sẽ là luỹ thừa từ 0 đến 43 của các số từ 2 đến 45, ma trận bên phải là flag. Kết quả của phép tính sẽ được so với answer.

Sau khi biết được chương trình làm gì ta sẽ tìm flag bằng sagemath

Code sagemath:

```python
from sage.all import *
answer = [924303399913049, 2830611888841555553, 11417572481992307149, 9399377228148865497, 4466300653933784009, 6929356405062948441, 1060599744128290109, 2413293955258720417, 12532996834583759385, 924446205882046577, 953636631316484173, 8178891442779449225, 5149328285628553929, 14848938488663696553, 10695623300750768253, 331402136485900945, 7526029785593005913, 528067901588867841, 17934877594923492173, 1823014476946146745, 4474927539439552841, 11848375034122087289, 14775913573091536445, 6221187839989261569, 8706220284657781273, 12220531501790844433, 6881877824236885709, 1709544903114568809, 13840396538186662729, 1672874960389122761, 12890717860805876349, 12512590028788979697, 17022931942054141529, 4432258044383820193, 17829278568412944077, 15934592270780519321, 9284508610800604361, 8084337873092305561, 9905442585384757565, 9968445791360002913, 2334598992640348185, 11395810839179523505, 16674150723980103501, 1359876212648330057]
mat = [[x**i for i in range(43)] for x in range(2, 46)]
A = matrix(Zmod(2**64), mat)
B = vector(Zmod(2**64), answer)
flag = "".join([chr(int(x) % 128) for x in A.solve_right(B)])
print(flag[::-1])
```

Flag : ictf{i_l0ve_interp0lati0n_2ca38d6ef0a709e0}
