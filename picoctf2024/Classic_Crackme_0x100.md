### Classic Crackme 0x100
---

Ta chạy thử file và có vẻ chỉ là check password bình thường nên ta sẽ mở file trong IDA

![image](https://github.com/fuyosuru/Writeup/assets/150824829/51d34675-a584-4c60-aded-97502afcd7b7)

Có vẻ có một chút mã hoá ở đây. Input của ta sẽ đi qua một quá trình mã hoá 3 lần và sau đó được so sánh với xâu "ztqittwtxtieyfrslgtzuxovlfdnbrsnlrvyhhsdxxrfoxnjbl". Ta sẽ viết code để tìm ra input thoả mãn.

```cpp=
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
