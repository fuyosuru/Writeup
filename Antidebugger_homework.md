# Chal1
ở bài này anti-debugger trick đầu tiên ta thấy trong hàm main đó là ptrace.

![image](https://hackmd.io/_uploads/SJ5WP-cZR.png)

Ptrace là một syscall của linux, dùng để trace process, nhưng vì mỗi process chỉ có thể có một tracer nên khi ta debug một process có gọi ptrace thì ptrace sẽ báo "Operation not permitted" và người code có thể dùng nó để kiểm tra process có đang bị debug hay không.

Để bypass thì có một số cách: cách đầu tiên là trong quá trình debug thì ta sẽ thay đổi giá trị trả về của ptrace vì nếu ta đang debug thì ptrace sẽ trả về -1 cho nên sau khi đi qua hàm ptrace ta chỉ cần thay đổi giá trị trở về thành 0 thì ta sẽ bypass được. Cách thứ hai là ta sẽ nop đoạn call toàn bộ đoạn assembly call ptrace. Ở bài này thì em dùng cách đầu tiên.

![image](https://hackmd.io/_uploads/SJqhw-9bC.png)

ở đây ta có thể sau khi ptrace được gọi thì giá trị eax là 0xffffffff. Ta chỉ cần set giá trị của eax về 0 là có thể bypass được ptrace

![image](https://hackmd.io/_uploads/BkUNu-5-0.png)

ở đoạn code tiếp theo ta thấy rằng chương trình đang check các byte ở trong đoạn code từ main to đến marker với breakpointvalue (0xCC) 

Đây là một anti-debugger vì khi ta đặt breakpoint ở một vị trí nào đó trong code thì tại đó sẽ xuất hiện byte 0xCC, khi chương trình thực thi đến byte 0xCC thì chương trình sẽ tạm dừng lại để ta có thể theo dõi giá trị các biến của chương trình. Và ở đoạn code trên chương trình đang kiểm tra xem trong đoạn code từ main đến marker có breakpoint nào không, nếu có thì chương trình sẽ exit.

Để bypass thì vì em nhận thấy rằng đoạn code này không ảnh hường gì đến quá trình decrypt flag cho nên em sẽ patch để không thực hiện đoạn code này.

Code sau khi patch:
![image](https://hackmd.io/_uploads/Hyts3Z5bR.png)

Vậy là ta có thể debug chương trình bình thường

Để có flag ta cần phải thay đổi username của ta thành string username 

![image](https://hackmd.io/_uploads/SyaC0Z9ZA.png)

Ở đây ta có thể thấy username của ta đang là 'fuyosuru' ta cần phải thay đổi thành username. Ta sẽ thay đổi nó bằng cách thay vì lấy username của máy ta để so sánh và tính toán thì ta sẽ thay vào con trỏ của string username luôn. Từ assembly ta biết được username đang ở 0x804a05c vì vậy ta thay eax = 0x804a05c và tiếp tục thực hiện chương trình.

![image](https://hackmd.io/_uploads/H1ayJf5WA.png)

Ta đã thay đổi thành công. Cho chương trình thực thi đến hết.

![image](https://hackmd.io/_uploads/Bkz1lfcZR.png)



FLAG : HTB{L00k1ng_f0r_4_w31rd_n4m3}

# Chal2

Trong hàm main ta thấy có rất nhiều anti-debugger khác nhau nên ta sẽ xử lý lần lượt từng trick.

Đầu tiên là IsDebuggerPresent()

![image](https://hackmd.io/_uploads/SkIo4f5bC.png)

IsDebuggerPresent() sẽ trả về giá trị dựa trên việc process có đang bị debug hay không.

Để bypass thì ta chỉ cần patch lại chương trình

Ban đầu:

 ![image](https://hackmd.io/_uploads/S1cRzQcZR.png)

Sau khi patch:

![image](https://hackmd.io/_uploads/HkTLmX9ZA.png)

Anti debugger tiếp theo là NtGlobalFlag

![image](https://hackmd.io/_uploads/Bk2nX7cb0.png)!

![image](https://hackmd.io/_uploads/rydAX79b0.png)

NtGlobalFlag là 1 field của PEB (Process Environment Block) bình thường thì nó sẽ bằng 0 nhưng trong quá trình debug thì nó sẽ bằng 0x70 (112) 

Để bypass thì ta làm tương tự như IsDebuggerPresent()

Sau khi patch:
![image](https://hackmd.io/_uploads/rJ2QHmq-0.png)

Tiếp theo là CheckRemoteDebuggerPresent().

![image](https://hackmd.io/_uploads/r1VUHX5ZA.png)

CheckRemoteDebuggerPresent() sẽ set pbDebuggerPresent bằng -1 nếu process hiện tại đang bị debug.

Để bypass thì ta làm tương tự 2 trick trên.

![image](https://hackmd.io/_uploads/SJm_Um9WA.png)

Tiếp theo là GetTickCount()
![image](https://hackmd.io/_uploads/H10h8QcZ0.png)

GetTickCount() sẽ trả về số milliseconds đã trôi qua tính từ khi system được khởi động. Bởi vì khi ta debug thì chương trình sẽ bị dừng lại tại từng instruction cho nên nếu ta Gọi 2 hàm GetTickCount() bằng cách đi qua từng instruction thì chắc chắn giữa chúng sẽ mất nhiều hơn 1s (0x3e8 ms) và khi đó chương trình sẽ phát hiện được rằng ta đang debug nó.

Để bypass thì khi đến đoạn này ta không đi qua từng instruction mà đặt breakpoint để thực thi 2 hàm GetTickCount() < 1s

![image](https://hackmd.io/_uploads/rJ_OYmcZA.png)

![image](https://hackmd.io/_uploads/rkgFKQ5bR.png)

Tiếp theo là

![image](https://hackmd.io/_uploads/S1V9H95ZA.png)

"\\\\.\\Global\\ProcmonDebugLogger" được tạo ra bởi process monitor trong quá trình debug. Vì vậy nếu file này tồn tại trong quá trình thực thi code thì có nghĩa là nó đang bị debug.

Để bypass ta chỉ patch để đảo ngược câu lệnh điều kiện thành như sau

![image](https://hackmd.io/_uploads/SymKLcqbC.png)

Tiếp theo là đoạn code sau:
![image](https://hackmd.io/_uploads/Bkuo855Z0.png)

![image](https://hackmd.io/_uploads/S1oaI99ZR.png)

Ta có struct PROCESSENTRY32 như sau
```cpp=
typedef struct tagPROCESSENTRY32 {
  DWORD     dwSize;
  DWORD     cntUsage;
  DWORD     th32ProcessID;
  ULONG_PTR th32DefaultHeapID;
  DWORD     th32ModuleID;
  DWORD     cntThreads;
  DWORD     th32ParentProcessID;
  LONG      pcPriClassBase;
  DWORD     dwFlags;
  CHAR      szExeFile[MAX_PATH];
} PROCESSENTRY32;
```
Trường szExeFile sẽ chứa tên file thực thi của process. Vậy ở đây chương trình sẽ check xem process được thực thi bởi executable file  nào.

Để bypass thì ngay sau khi sub_401130()  được thi (tại 0x00401510) thì ta sẽ thay đổi giá trị của eax về 0 ứng với không trường hợp nào cả.
![image](https://hackmd.io/_uploads/H1rnd55ZC.png)

Tiếp theo là đoạn code:
![image](https://hackmd.io/_uploads/Hyj4Y5qZC.png)
![image](https://hackmd.io/_uploads/HJ35Y59WA.png)

__indword(0x5658u) sẽ đọc data từ port 0x5658, đây là 1 backdoor I/O của VMware. Nếu chương trình chạy bên trong VMware thì nó sẽ thực thi được còn không thì nó sẽ báo lỗi và trigger exception

![image](https://hackmd.io/_uploads/rJ1NSHobC.png)

Trong assembly code ta thấy rằng nếu câu lệnh __indword(0x5658u) lỗi tức là ta không dùng VMware thì chương trình sẽ try loc_4012A4 và chương trình sẽ không return 1 vì vậy nó sẽ không exit

![image](https://hackmd.io/_uploads/BypyLBj-A.png)

Nếu ta dùng VMware thì chương trình sẽ chạy bình thường và đi đến loc_4012C1, ở đây hàm sẽ return 1 từ đó thoả điều kiện và chương trình sẽ exit và báo VMware detected.

![image](https://hackmd.io/_uploads/HJJUGUob0.png)


![image](https://hackmd.io/_uploads/SyYCLroWA.png)

![image](https://hackmd.io/_uploads/Skm_zIsZC.png)


Để bypass ta sữa điều kiện lại thành ( Nếu ta sử dụng VMware ):

![image](https://hackmd.io/_uploads/SJCna55WR.png)

Đoạn code kế tiếp có chút kỳ lạ

![image](https://hackmd.io/_uploads/rk9rei9WA.png)

Ta thấy có thực hiện phép tính 1/0 sẽ trigger exception vì vậy ta sẽ xem assembly code

![image](https://hackmd.io/_uploads/HJ432HiWC.png)

Có vẻ khi sau trigger exception thì chương trình sẽ nhảy đến loc_4015F6

![image](https://hackmd.io/_uploads/HyGOTBjZA.png)

Ta thấy rằng var_88 được gán bằng sau đó lập tức bị so sánh với 1 và theo sau đó là lệnh jnz vì vậy lệnh jnz sẽ luôn nhảy đến loc_401627 và sau đó nếu ta không can thiệp gì thì nó sẽ tiếp tục chạy bình thường và đi đến loc_401652

![image](https://hackmd.io/_uploads/Hytw18sWA.png)

Ở đoạn code trên ta thấy rằng có xâu rất lạ ở đây nhưng trước hết ở 3 lệnh đầu tiên có 1 điều rất lạ là var_78 được gán bằng 0 và ngay lập tức được đem đi so sánh với 1 theo sau là jnz vì vậy lệnh jnz sẽ luôn được thực thi và đi đến kết thúc chương trình mà không thực thi đoạn code chứa xâu bên dưới.

Ta sẽ sửa lệnh jnz thành jz và để cho chương trình chạy đoạn code phía dưới và bất ngờ ta có flag.

![image](https://hackmd.io/_uploads/rJAPg8sZC.png)

Flag: SECCON{check_Ascii85}

# prob

Đầu tiên ta sẽ mở file exe này trong IDA và quan sát hàm main:

![image](https://hackmd.io/_uploads/rkVKkNpbR.png)

Ta thầy rằng có rất nhiều hàm khác nhau được gọi vì vậy ta sẽ xem qua từng hàm và ta thấy ngoại trừ hàm đầu tiên thì các hàm còn lại rất quan trọng.

Đầu tiên, câu lệnh ```dword_4054F4 = strlen(Str)``` đang lấy độ dài của xâu Str và xâu Str có nội dung : ```"have a good day! enjoy wargame!" ```

Sau đó là ```sub_401690()```:

```cpp=
int sub_401690()
{
  FILE *Stream; // [esp+1Ch] [ebp-Ch]

  Stream = fopen("flag.txt", "r");
  if ( !Stream )
  {
    puts("file error!");
    exit(1);
  }
  fscanf(Stream, "%[^;]s", &unk_405900);
  fclose(Stream);
  return 0;
}
```

Ta thấy hàm này chỉ đơn thuần là đọc file và lưu data vào ```unk_405900``` nên mình đặt tên lại biến này thành ```filedata```

Tiếp theo là ```sub_401500(&filedata)```:

```cpp
int __cdecl sub_401500(int a1)
{
  __int64 v1; // rax
  __int64 v2; // rcx
  __int64 v3; // rax
  __int64 v4; // rax
  int i; // [esp+Ch] [ebp-14h]

  for ( i = 0; i <= 80; ++i )
  {
    v1 = (Str[i % dword_4054F4] * Str[i % dword_4054F4] + i) ^ *(char *)(i + a1);
    dword_405040[2 * i] = v1;
    dword_405044[2 * i] = HIDWORD(v1);
    LODWORD(v2) = dword_405040[2 * i];
    HIDWORD(v2) = dword_405044[2 * i];
    v3 = v2 - Str[i % dword_4054F4];
    dword_405040[2 * i] = v3;
    dword_405044[2 * i] = HIDWORD(v3);
    LODWORD(v2) = dword_405040[2 * i];
    HIDWORD(v2) = dword_405044[2 * i];
    v4 = v2 + i;
    dword_405040[2 * i] = v4;
    dword_405044[2 * i] = HIDWORD(v4);
  }
  return 0;
}
```

Ta chưa đi vào phân tích kỹ nhưng nhìn sơ qua thì hàm này đang thực hiện quá trình mã hoá ```filedata``` và kết quả của quá trình mã hoá được lưu vào dword_405040 và dword_405044 nên em sẽ đặt tên 2 mảng này lại lần lượt là ketqua1 và ketqua2

Tiếp theo là ```sub_401608()```:

```cpp
int sub_401608()
{
  __int64 v1; // [esp+8h] [ebp-20h]
  char *Buffer; // [esp+18h] [ebp-10h]
  int i; // [esp+1Ch] [ebp-Ch]

  Buffer = (char *)calloc(1u, 0x10u);
  Destination = 0;
  for ( i = 0; i <= 80; ++i )
  {
    LODWORD(v1) = ketqua1[2 * i];
    HIDWORD(v1) = ketqua2[2 * i];
    sprintf(Buffer, "%#llX", v1);
    strcat(&Destination, Buffer);
  }
  free(Buffer);
  return 0;
}
```

Ta thấy hàm này đang kết hợp 2 mảng ketqua1 và ketqua2 và lưu chúng vào Destination

Cuối cùng là ```sub_4016F8()```:

```cpp
int sub_4016F8()
{
  FILE *Stream; // [esp+1Ch] [ebp-Ch]

  Stream = fopen("tmp.enc", "w");
  if ( !Stream )
  {
    puts("file error!");
    exit(1);
  }
  fputs(&Destination, Stream);
  fclose(Stream);
  return 0;
}
```

Hàm này chỉ đơn giản lưu Destination vào file tmp.enc

Sau khi phân tích các hàm có trong hàm main ta thấy rằng flow chương trình là đọc file flag.txt sau đó mã hoá nó và lưu vào tmp.enc vì vậy ta chỉ cần đảo ngược lại quá trình mã hoá ở ```sub_401500()``` là được.

```python
key="have a good day! enjoy wargame!"
tmp="0X29AF0X24930X35A90X27290X4140X24530X4580X28EF0X2F9E0X2FFC0X26D00X4670X26EB0X24390X39140X42C0X43F0X275F0X2EDD0X2B2B0X300F0X389C0X41D0X36A60X24740X32290X29790X24A90X2E890X27560X4270X29EE0X24480X36980X27500X44E0X247D0X41F0X29670X302F0X2FCF0X26CD0X4260X26D00X24A70X391D0X46B0X42A0X28090X2F100X2BF70X302B0X39120X4160X37710X24A30X32940X296D0X24A80X2E610X27F80X4680X2A220X25130X365C0X28050X4950X25120X4970X296C0X30350X2FED0X273C0X4720X27400X24F60X39500X4BA0X47C0X28120X2F76"
arr = []
p=tmp.split("0X")[1:]
for i in p:
    arr.append(int(i,16))
s=''
for i in range(81):
    var=ord(key[i%len(key)])
    word=arr[i]
    v2=word-i
    v3=v2
    v2=v3+var
    v1=v2
    bandau=(var*var+i)^v1
    s=s+chr(bandau)
print(s)
```
flag : W1{W4it7,H0w_C@n_You_F1nd_M3_huhhhhh_???_162171478932455214742256314789}
