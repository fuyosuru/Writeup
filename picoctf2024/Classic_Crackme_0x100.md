## Classic Crackme 0x100

Ta chạy thử file:

![image](https://hackmd.io/_uploads/HkUMrrn1A.png)

Có vẻ là một bài check password, ta mở trong IDA và xem nội dung hàm main trong IDA:

![image](https://github.com/fuyosuru/Writeup/assets/150824829/51d34675-a584-4c60-aded-97502afcd7b7)

Có vẻ chương trình sẽ nhận input và mã hoá input ta nhập vào. Input của ta sẽ đi qua cùng một quá trình mã hoá 3 lần và sau đó được so sánh với xâu "ztqittwtxtieyfrslgtzuxovlfdnbrsnlrvyhhsdxxrfoxnjbl".
Ta có các biến secret1, secret2, secret3, fix là cố định.
Ở mỗi lần mã hoá chương trình sẽ mã hoá từng ký tự.

![image](https://hackmd.io/_uploads/BkIcIBnJR.png)

Vì fix đã cho cố định bằng 97 cho nên các chữ cái sau khi mã hoá chỉ có thể thuộc 'a-z' và ký tự tại vị trí i_0 sau khi mã hoá chỉ phụ thuộc vào chữ cái tại vị trí i_0 trước khi bị mã hoá cho nên tại mọi ví trí ta hoàn toàn có thể brute force để tìm được chữ cái trước khi bị mã hoá.

Code giải mã:


```cpp
#include <bits/stdc++.h>
using namespace std;
int main()
{
  string s="ztqittwtxtieyfrslgtzuxovlfdnbrsnlrvyhhsdxxrfoxnjbl";
  int i = 0;
  int len = s.size();
  int secret1 = 85;
  int secret2 = 51;
  int secret3 = 15;
  int fix = 97;
  while ( i <= 2 )
  {
    string t="";
    for (int i_0 = 0; i_0 < len; ++i_0 )
        for(int j='a';j<='z';j++)
    {
      int random1 = (secret1 & (i_0 % 255)) + (secret1 & ((i_0 % 255) >> 1));
      int random2 = (random1 & secret2) + (secret2 & (random1 >> 2));
      char check = char(((random2 & secret3) + j - fix + (secret3 & (random2 >> 4))) % 26 + fix);
        if(check==s[i_0])
        {
            t+=char(j);
            break;
        }
    }
    s=t;
    cout<<s<<"\n";
    ++i;
  }
  cout<<s;
}
```

Sau khi chạy code ta tìm được password : zqncqnqkuncvswigianqoofjfwubsfgyilppbyjrroitflbuvc

Ta nộp password và có flag


![image](https://github.com/fuyosuru/Writeup/assets/150824829/ec289557-7290-4abd-8e2f-aab4b6f78852)

flag :  picoCTF{s0lv3_angry_symb0ls_4656b68e}
