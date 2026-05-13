extern gadget_addr : qword
extern target_api  : qword

extern gadget_size : dword
extern btit_size   : dword
extern rtl_size    : dword


extern btit        : qword
extern rtl         : qword

extern g_CallContext    : qword

.code

DodgeCall PROC

    push rbx
    push rax
    push rbp
    push rdi
    push rcx
    push rdx
    push r8
    push r9
    mov rbx, rsp

    sub rsp, 2000h

    ; --------------------------------

    xor rax, rax
    mov eax, dword ptr [rtl_size]
    add eax, dword ptr [btit_size]
    add eax, dword ptr [gadget_size]

    ; eax = total adjustment

    and eax, 0Fh        ; eax = eax % 16

    cmp eax, 0
    je  aligned_ok

    sub rsp, 8         ; fix alignment

    aligned_ok:
    ;----------------------------
    

    lea rax, Go_back
    mov [rsp + 48h], rax
    mov rbp, rsp

    mov eax, dword ptr [rtl_size]
    sub rsp, rax

    mov rax, qword ptr [rtl]
    mov [rsp], rax

    mov eax, dword ptr [btit_size]
    sub rsp, rax

    mov rax, qword ptr [btit]
    mov [rsp], rax

    mov eax, dword ptr [gadget_size]
    sub rsp, rax

    mov rax, qword ptr [gadget_addr]
    mov [rsp], rax



    ; ===========

    lea rdi, g_CallContext

    ; Register arguments
    mov rcx, [rdi + 00h]
    mov rdx, [rdi + 08h]
    mov r8,  [rdi + 10h]
    mov r9,  [rdi + 18h]

    ; -----------------------------------------
    ; Stack arguments
    ; args[4+] go after shadow space
    ; -----------------------------------------

    mov rax, [rdi + 20h]
    mov [rsp + 28h], rax

    mov rax, [rdi + 28h]
    mov [rsp + 30h], rax

    mov rax, [rdi + 30h]
    mov [rsp + 38h], rax

    mov rax, [rdi + 38h]
    mov [rsp + 40h], rax

    mov rax, [rdi + 40h]
    mov [rsp + 48h], rax

    mov rax, [rdi + 48h]
    mov [rsp + 50h], rax

    mov rax, [rdi + 50h]
    mov [rsp + 58h], rax

    mov rax, [rdi + 58h]
    mov [rsp + 60h], rax

    mov rax, [rdi + 60h]
    mov [rsp + 68h], rax

    mov rax, [rdi + 68h]
    mov [rsp + 70h], rax

    mov rax, [rdi + 70h]
    mov [rsp + 78h], rax

    mov rax, [rdi + 78h]
    mov [rsp + 80h], rax

    ;========



    mov rax, qword ptr [target_api]

    jmp rax

Go_back:

    ; --------------------------------

    mov rsp, rbx
    pop r9
    pop r8
    pop rdx
    pop rcx
    pop rdi
    pop rbp
    pop rax
    pop rbx

    ret

DodgeCall ENDP

END