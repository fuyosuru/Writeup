### FactCheck
---

Đầu tiên ta  chạy thử file và không thấy gì cả sau đó ta mở file trong IDA


![image](https://github.com/fuyosuru/Writeup/assets/150824829/860c9e3d-1e1e-4c29-9894-3f0693e7e2df)

ta đã có được phần đầu của flag : picoCTF{wELF_d0N3_mate_ , ta thấy ở đây có rất nhiều string được khai báo như v22,v23... và phần đầu của flag là v22. Ta sẽ xem tiếp phần tiếp theo của chương trình có gì 

![image](https://github.com/fuyosuru/Writeup/assets/150824829/ce15af7c-fb70-41ca-8ff0-3d7b3908665c)

Ta thấy ở đây chương trình đang ghép thêm phần còn lại của flag vì vậy ta sẽ vị trí mà tại đó flag đã được hoàn thiện và đặt breakpoint tại đó để lấy flag 


![image](https://github.com/fuyosuru/Writeup/assets/150824829/cd740a50-11d9-4e76-8166-9a15e6578703)

Ta thấy ở đây flag đã được thêm '}' vào cuối vì vậy có vẻ tại offset này flag đã được hoàn thiện, ta mở gdb lên để đặt breakpoint tại offset này để lấy flag

![image](https://github.com/fuyosuru/Writeup/assets/150824829/5ae17395-f321-4fe7-b870-08e1a2b6dbfe)

flag : picoCTF{wELF_d0N3_mate_93a9cb95}



