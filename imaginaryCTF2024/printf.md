# printf

by Eth007

Description:

%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%64949c%*c%c%c%hn%c%c%c%c%c%c%c%545c%hn%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%hhn%*c%*74$c%*74$c%*74$c%*74$c%*74$c%*74$c%*74$c%*74$c%*74$c%*74$c%*74$c%*74$c%*74$c%*74$c%8c%75$hhn

---

Ta chạy file elf cùng với libc đề cho và thấy rằng chương trình chỉ đơn giản là nhập flag và check flag. Ta mở file trong IDA, hàm main chỉ gọi hàm printf với tham số như sau : 
```c
%2$0.2M%1$0.1511994458M%4$39272589.0M%1$407988693.504450890LF%5$2
81082297.302305860hE%1$39860509.126357916hE%4$113638530.17014068L
N%2$140781494.457680494LI%1$459244445.140810536LN%1$431556502.638
54045hB%3$0.2M%5$0.160398938M%2$90541966.0M%4$306933670.104139236
LF%5$359106186.467047824hE%4$368143182.87842953hE%5$531977237.137
...
```

Ngoài ra ta thấy rằng bên trong chương trình có một số hàm như sau:

```c
__int64 __fastcall sub_15DF(__int64 a1, __int64 a2)
{
  unsigned __int64 v2; // rax

  _BitScanReverse64(&v2, (unsigned __int8)BYTE4(*(_QWORD *)(a2 + 8)));
  qword_1C060[v2] = qword_1A040[dword_1C040 - 1] ^ qword_1A040[dword_1C040];
  return 1LL;
}
```

```c
__int64 __fastcall sub_133A(__int64 a1, __int64 a2)
{
  unsigned __int64 v2; // rax

  _BitScanReverse64(&v2, (unsigned __int8)BYTE4(*(_QWORD *)(a2 + 8)));
  qword_1C060[v2] = qword_1A040[dword_1C040 - 1] + qword_1A040[dword_1C040];
  return 1LL;
}
```

Các hàm này đều trông rất giống nhau và chỉ khác nhau mỗi phép tính, các phép tính bao gồm : ^,-,+,%,/,*

Bên cạnh còn có hàm sẽ in ra một chữ, có một hàm khác sẽ đọc một chữ:

```c
int sub_11E5()
{
  return putchar(qword_1A040[dword_1C040]);
}
```

```c
__int64 sub_11A8()
{
  __int64 result; // rax

  LODWORD(result) = getchar();
  ++dword_1C040;
  result = (int)result;
  qword_1A040[dword_1C040] = (int)result;
  return result;
}
```

Từ các hàm có bên trong chương trình ta có thể đoán rằng đây là một VM với các instruction là tham số của hàm printf có trong hàm main, với qword_1A040 chính là stack, dword_1C040 là stackpointer, qword_1C060 là mảng chứa các register.

Ta sẽ mở libc trong IDA và tìm hiểu xem các instruction hoạt động như thế nào, thông qua debug ta biết được các instrucion sẽ được giải mã ở sub_7D380.
Tóm tắt cách hoạt động của các instruction:

Các instruction được ngăn cách bởi dấu "%" và từng instruction sẽ có format như sau :
```c
2$#522854986.61681829N
4$407416399.62198782LK
2$65683313.238254650hE
3$73715752.399793033hE
1$84772383.442624706LN
4$0.1357935714M
2$471630577.0M
2$200014357.216634891LF
5$42988953.98867208hE
5$38673113.310185853hE
4$191959667.133606507LN
5$254808773.168134055LK
1$ 381510086.230507543X
5$-461247306.140868513A
2$0.777601541M
```

Chữ cái ở cuối sẽ quyết định chức năng của instruction, chức năng ứng với từng trường hợp như sau :
```python
if functype=='B':
    write()
if functype=='M':
    push(var)
if functype=='F':
    add(reg)
if functype=='E':
    mov(reg)
if functype=='N':
    pushreg(reg)
if functype=='I':
    div(reg)
if functype=='H':
    mul(reg)
if functype=='K':
    xor(reg)
if functype=='A':
    read()
if functype=='X':
    out()
```

- Với nhóm thao tác toán học (+,-,*,%,/,^) phép tính sẽ được thực hiện giữa 2 số trên cùng của stack và lưu vào trong một register.
-  push() sẽ đẩy tham số được truyền vào lên trên cùng của stack.
-  pushreg() cũng tương tự push nhưng tham số được đẩy lên stack và giá trị của một register.
-  read() và write() lần lượt là đọc và in một ký tự.
-  out() sẽ kiểm tra nếu giá trị của register 0 bằng 0 thì chương trình sẽ gọi hàm exit().
-  mov() sẽ lấy giá trị trên đầu stack và gán vào một register sau đó pop giá trị đó khỏi stack.

Register được sử dụng sẽ được quyết định thông qua 2 cách: ký tự trước chữ cái cuối cùng hoặc ký tự phía sau dấu "$", bảng chuyển đổi như sau:
```python
" ":4
"#":3
"-":5
"'":7
"I":8
"h":1
"l":2
"L":0
"+":9
```
Nếu không có trong danh sách trên thì mặc định register được sử dụng là register 0.

Về tham số của instruction được tính như sau:
```c
2$200014357.216634891LF
```
Ta sẽ chỉ quan tâm phần số sau dấu "$": tức là 200014357.216634891

Dấu "." là để tách thành 2 số lần lượt là 200014357 và 216634891. Chuyển 2 số này thành hex : 0xBEBFA15 và 0xCE9960B. Kết quả cuối cùng được tính như sau : 8 byte đầu của tham số sẽ là số phía sau tức là 0xCE9960B, 8 byte tiếp theo sẽ là số đầu tiên tức là 0xBEBFA15. Vậy tham số của instruction trên là 0x0BEBFA150CE9960B

Ta viết disassembler và có được flow chương trình đơn giản như sau:
- Đọc 4 ký tự.
- Nhân mỗi ký tự với một số cho trước.
- Tính tổng của 4 số vừa tính được.
- Xor tổng vừa tính được với 2 số cho trước.
- Kiểm tra kết quả với 0, nếu khác 0 thì chương trình sẽ exit.

Vì mỗi 4 ký tự được tính độc lập với nhau nên ta hoàn toàn có thể bruteforce.

Code tìm flag (trong code có cả disassembler):

```python
stackpointer=0
stack=[0]*4000
reg=[0]*30
nextchar=''
bad=0
attemp=['a','b','c','d']
attemp=attemp*100
mulnum=[0]*4
xor_num=0
flag_full=""
regchar={
    " ":4,
    "#":3,
    "-":5,
    "'":7,
    "I":8,
    "h":1,
    "l":2,
    "L":0,
    "+":9
}
funclist={}
def check(flag,mulnum,xor_num):
    s=0
    for i in range(4):
        s=(s+flag[i]*mulnum[i])&0xffffffffffffffff
    s=s&0xffffffffffffffff
    return s^xor_num
def read():
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    global attemp
    global xor_num
    xor_num=0
    print("read :",end='')
    stackpointer=stackpointer+1
    inp=ord(attemp[0])
    attemp=attemp[1:]
    nextchar=''
    stack[stackpointer]=inp
def write():
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    #print("write :",end='')
    print(f'|{chr(stack[stackpointer]&0xff)}|')
def mov(x):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    reg[x]=stack[stackpointer]
    print(f'load :{x} ({reg[x]})')
    stackpointer=stackpointer-1
def add(x):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    reg[x]=(stack[stackpointer]+stack[stackpointer-1])&0xffffffffffffffff
    print(f'add : {x} ({stack[stackpointer]} + {stack[stackpointer-1]} = {reg[x]})  ')
def sub(x):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    reg[x]=stack[stackpointer]-stack[stackpointer-1]
    print(f'sub : {x} ({stack[stackpointer]} - {stack[stackpointer-1]} = {reg[x]}) ')
def mul(x):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    global mulnum
    reg[x]=(stack[stackpointer]*stack[stackpointer-1])&0xffffffffffffffff
    if stack[stackpointer-1]==ord('a'):
        mulnum[0]=stack[stackpointer]
    if stack[stackpointer-1]==ord('b'):
        mulnum[1]=stack[stackpointer]
    if stack[stackpointer-1]==ord('c'):
        mulnum[2]=stack[stackpointer]
    if stack[stackpointer-1]==ord('d'):
        mulnum[3]=stack[stackpointer]
    print(f'mul : {x} ({stack[stackpointer]} * {stack[stackpointer-1]} = {reg[x]}) ')
def div(x):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    print(f'div : {x} ({stack[stackpointer]} / {stack[stackpointer-1]}) ')
    reg[x]=(stack[stackpointer]//stack[stackpointer-1])&0xffffffffffffffff
def mod(x):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    print(f'mod : {x} ({stack[stackpointer]} % {stack[stackpointer-1]}) ')
    reg[x]=stack[stackpointer]%stack[stackpointer-1]
def xor(x):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    global xor_num
    xor_num=xor_num^stack[stackpointer]
    print(f'xor : {x} ({stack[stackpointer]} ^ {stack[stackpointer-1]}) ')
    reg[x]=(stack[stackpointer]^stack[stackpointer-1])&0xffffffffffffffff
def push(x):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    print(f'push : {x}')
    stackpointer=stackpointer+1
    stack[stackpointer]=x
def pushreg(x):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    print(f'push : {x} ( {reg[x]} ) ')
    stackpointer=stackpointer+1
    stack[stackpointer]=reg[x]
def out():
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    global mulnum
    global xor_num
    global flag_full
    test=[0]*4
    print(f'check : {reg[0]}')
    print(flag_full)
    print(mulnum)
    print(xor_num)
    ok=0
    for i in range(32,127):
        for j in range(32,127):
            for k in range(32,127):
                for f in range(32,127):
                    test[0]=i
                    test[1]=j
                    test[2]=k
                    test[3]=f
                    if check(test,mulnum,xor_num)==0:
                        flag_full=flag_full+chr(i)+chr(j)+chr(k)+chr(f)
                        ok=1
    
    if ok==0:
        flag_full=flag_full+"????"
    """
    if reg[0]!=0:
        exit()
        #bad=1
    """
def execute(functype,var,reg):
    if functype=='B':
        write()
    if functype=='M':
        push(var)
    if functype=='F':
        add(reg)
    if functype=='E':
        mov(reg)
    if functype=='N':
        pushreg(reg)
    if functype=='I':
        div(reg)
    if functype=='H':
        mul(reg)
    if functype=='K':
        xor(reg)
    if functype=='A':
        read()
    if functype=='X':
        out()
def compile(x):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    x=x[2:]
    func=x[-1]
    funclist[func]="coá"
    x=x[:-1]
    regdes=0
    if x[0] in regchar:
        regdes=regchar[x[0]]
        x=x[1:]
    if x[-1] in regchar:
        regdes=regchar[x[-1]]
        x=x[:-1]
    numbers=x.split(".")
    for i in range(2):
        numbers[i]=int(numbers[i],10)
    number=(numbers[0]<<32)+numbers[1]
    execute(func,number,regdes)
def go(opcode):
    global stackpointer
    global stack
    global reg
    global nextchar
    global bad
    stackpointer=0
    stack=[0]*4000
    reg=[0]*30
    nextchar=''
    bad=0
    dem=0
    for i in opcode:
        compile(i)
    
def main():
    global flag_full
    with open(r"D:\BuiHuutung\linh_tinh\CTF\imaginiry\format.txt","r") as f:
        data=f.read()
    wtf=data.split(",")
    for i in range(len(wtf)):
        wtf[i]=wtf[i].strip()
    newdata=''.join([chr(int(i,16)) for i in wtf])
    newdata=newdata[:-1]
    opcode=newdata.split('%')[1:]
    go(opcode)
    print(flag_full)

if __name__ == "__main__":
    main()
```

Flag : ictf{n3v3r_too_m4ny_form4t_sp3cifi3rs_9a7837294d1633140433f51d13a033736}

