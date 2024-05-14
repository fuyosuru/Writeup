# ROVM

Tóm tắt sơ về challenge này thì ta sẽ được cho 3 file là chall, opcode và chain. File chall sẽ map 2 file opcode và chain vào memory để chạy một program bằng các gadget.

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  struct sigaction s; // [rsp+0h] [rbp-150h] BYREF
  struct stat stat_buf; // [rsp+A0h] [rbp-B0h] BYREF
  int fd; // [rsp+138h] [rbp-18h]
  int fildes; // [rsp+13Ch] [rbp-14h]
  void *v8; // [rsp+140h] [rbp-10h]
  void *buf; // [rsp+148h] [rbp-8h]
  __int64 savedregs; // [rsp+150h] [rbp+0h]
  __int64 retaddr; // [rsp+158h] [rbp+8h]

  memset(&s, 0, sizeof(s));
  s.sa_flags = 4;
  s.sa_handler = (__sighandler_t)handler;
  sigaction(11, &s, 0LL);
  buf = mmap((void *)0x1224000, 0x1000uLL, 7, 33, -1, 0LL);
  if ( buf == (void *)-1LL )
  {
    perror("mmap failed");
    exit(0);
  }
  fd = open("opcode", 0);
  if ( fd == -1 )
  {
    perror("open failed");
    exit(0);
  }
  if ( (unsigned int)fstat(fd, &stat_buf) == -1 )
  {
    perror("fstat failed");
    exit(0);
  }
  if ( read(fd, buf, stat_buf.st_size) == -1 )
  {
    perror("read failed");
    exit(0);
  }
  v8 = mmap((void *)0x1225000, 0x3000uLL, 7, 33, -1, 0LL);
  if ( v8 == (void *)-1LL )
  {
    perror("mmap failed");
    exit(0);
  }
  fildes = open("chain", 0);
  if ( fildes == -1 )
  {
    perror("open failed");
    exit(0);
  }
  if ( (unsigned int)fstat(fildes, &stat_buf) == -1 )
  {
    perror("fstat failed");
    exit(0);
  }
  if ( read(fildes, v8, stat_buf.st_size) == -1 )
  {
    perror("read failed");
    exit(0);
  }
  savedregs = 19025920LL;
  retaddr = 19021825LL;
  return 0;
}
```

Các gadget có trong file opcode ( đã được rebase lại cho giống trong memory lúc program được chạy ):

![image](https://hackmd.io/_uploads/Hy6qLzbQA.png)

File chain sẽ chứa address của các gadget theo thứ tự để chạy được program:

![image](https://hackmd.io/_uploads/SJHKPMZQA.png)

Ta sẽ debug để xem flow chương trình như thế nào khi bắt đầu chạy bằng các gadget:

![image](https://hackmd.io/_uploads/SkWSufbXR.png)

Đầu tiên ta sẽ bị yêu cầu nhập một input. Sau khi nhập input bất kỳ thì program sẽ bị Segmentation fault từ đó sẽ đi đến handler và xuất "FAIL!"

```c
void __noreturn handler()
{
  puts("Fail!");
  exit(0);
}
```

Sau khi debug lại vài lần thì ta thấy program sẽ duyệt qua từng ký tự của input và sau đó sẽ trừ độ dài input ( tính cả '\n' ) với 0x24 và lấy kết quả có được nhân với 8 và cộng kết quả với rsp.

![image](https://hackmd.io/_uploads/Hy1qjMZmC.png)

Ở hình trên với input có độ dài bằng 2 thì kết quả để cộng vào rsp sẽ là 0x6f0 từ đó sẽ đi đến 1 gadget có nội dung như sau:

![image](https://hackmd.io/_uploads/B1TRof-Q0.png)

Ta thấy rằng khi chạy gadget trên chắc chắn sẽ bị Segmentation fault vì vậy ta cần phải xem xét số được cộng với rsp ở bước trước để không đi đến gadget trên. Vì ta biết rằng file chain sẽ chứa các address của các gadget nên ta có thể biết được flow program từ đó.

![image](https://hackmd.io/_uploads/SyHphfb7R.png)

Ta thấy phía dưới chứa rất nhiều address dẫn đến gadget khiến program bị Segmentation fault vì vậy ta có hiểu rằng program đang kiểm tra độ dài input có bằng 0x24 hay không vì nếu kết quả có được khi trừ độ dài input cho 0x24 khác 0 thì program sẽ bị Segmentation fault.

Vậy ta đã biết được độ dài input nên ta sẽ tạo input có độ dài tượng và xem program làm gì tiếp theo.

![image](https://hackmd.io/_uploads/H1jRRM-mC.png)

Ở đoạn này ta thấy rằng input của ta đang bị nhân với các byte bắt đầu từ offset 0x1224950 của file opcode

![image](https://hackmd.io/_uploads/Skl4Jm-7R.png)

Ta thấy rằng dãy byte bắt đầu từ 0x1224950 cho đến string thông báo ta đã tìm được flag có đúng 0x24 byte vì vậy ta không nghi ngờ gì nữa.

Ta tiếp chạy program và thấy rằng sau khi nhân từng byte của input sau khi được nhân với byte tương ứng sẽ được chia cho 0xfb và lấy phần dư trừ cho 1, sau đó lấy kết quả cuối cùng nhân với 8 và cộng rsp với 8. Ta thấy rằng bước cộng kết quả có được sau cùng với rsp hoạt động giống với lúc check độ dài input đó là nếu kết quả phép trừ số dư với 1 khác 0 thì ta sẽ lại nhảy đến gadget khiến program bị Segmentation fault vì vậy ta có thể hiểu rằng các byte của input sau khi nhân với byte tương ứng phải chia 0xfb dư 1 nếu không program sẽ bị Segmentation fault. Khi đã hiểu program chạy như nào ta chỉ cần tìm input phù hợp.

Code tìm input :
```python

enc = [
  0x96, 0x44, 0x5B, 0x25, 0x47, 0x8C, 0x59, 0x25, 0x92, 0x5A, 
  0x25, 0x41, 0xF0, 0x44, 0x27, 0xF0, 0x8C, 0x4C, 0x4C, 0x29, 
  0x59, 0x27, 0x25, 0x29, 0x2C, 0x59, 0x27, 0x76, 0x8C, 0x27, 
  0x5A, 0x25, 0x29, 0xC7, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
]

for i in enc:
    for char in range(33,127):
        if (i*char) % 0xfb == 1:
            print(chr(char),end='')
```

Input tìm được : R0P_c4n_b5_pr0gr4mm1ng_1angu4g5_1o1

![image](https://hackmd.io/_uploads/Sk9TbXbQA.png)

Nhập thử input này vào program thì ta biết rằng đây chính là flag.

Flag : R0P_c4n_b5_pr0gr4mm1ng_1angu4g5_1o1
