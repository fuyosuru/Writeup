#include <Windows.h>
#include <ehdata.h>
#include <iostream>
#include <map>
#include <subauth.h>
#include <TlHelp32.h>
#include <vector>

#pragma comment(lib, "ntdll.lib")

//#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) == 0)
#define STATUS_SUCCESS   ((NTSTATUS)0x00000000L)

#define MAX_STACK_SIZE 12000
//
// Definitions and structs required to call NtOpenProcess.
//
#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }


typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, * PCLIENT_ID;
CLIENT_ID clientTest = {};

typedef struct _OBJECT_ATTRIBUTES {
    ULONG           Length;
    HANDLE          RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

typedef struct CALL_CONTEXT
{
    uint64_t Args[16];
};

//
// Unwind op codes: https://docs.microsoft.com/en-us/cpp/build/exception-handling-x64?view=msvc-170
//
typedef enum _UNWIND_OP_CODES {
    UWOP_PUSH_NONVOL = 0, /* info == register number */
    UWOP_ALLOC_LARGE,     /* no info, alloc size in next 2 slots */
    UWOP_ALLOC_SMALL,     /* info == size of allocation / 8 - 1 */
    UWOP_SET_FPREG,       /* no info, FP = RSP + UNWIND_INFO.FPRegOffset*16 */
    UWOP_SAVE_NONVOL,     /* info == register number, offset in next slot */
    UWOP_SAVE_NONVOL_FAR, /* info == register number, offset in next 2 slots */
    UWOP_SAVE_XMM128 = 8, /* info == XMM reg number, offset in next slot */
    UWOP_SAVE_XMM128_FAR, /* info == XMM reg number, offset in next 2 slots */
    UWOP_PUSH_MACHFRAME   /* info == 0: no error-code, 1: error-code */
} UNWIND_CODE_OPS;



extern "C" 
{
    void* gadget_addr = nullptr;
    void* target_api = nullptr;

    ULONG  gadget_size = 0;
    ULONG  btit_size = 0;
    ULONG  rtl_size = 0;

    CALL_CONTEXT g_CallContext;

    void* btit = nullptr;
    void* rtl = nullptr;
    // Assembly function declaration
    void DodgeCall();
}


void* CallFunction(void* target_api_address, int arg_count, ...)
{
    target_api = target_api_address;

    g_CallContext = {};
    va_list args;
    va_start(args, arg_count);

    for (int i = 0; i < arg_count && i < 16; i++)
        g_CallContext.Args[i] = va_arg(args, uint64_t);

    va_end(args);
    DodgeCall();
    return nullptr;
}

ULONG ChatGPTCalculateFunctionStackSize(void* ReturnAddress)
{
    DWORD64 imageBase = 0;

    PRUNTIME_FUNCTION runtime =RtlLookupFunctionEntry((DWORD64)ReturnAddress,&imageBase,nullptr);

    if (!runtime)
        return 0;

    PUNWIND_INFO unwind = (PUNWIND_INFO)(imageBase + runtime->UnwindData);

    ULONG totalStackSize = 8; // return address

    for (ULONG i = 0; i < unwind->CountOfCodes; ++i)
    {
        BYTE op = unwind->UnwindCode[i].UnwindOp;

        BYTE info = unwind->UnwindCode[i].OpInfo;

        switch (op)
        {
            case UWOP_PUSH_NONVOL:
            {
                totalStackSize += 8;
                break;
            }

            case UWOP_ALLOC_SMALL:
            {
                totalStackSize += (info * 8) + 8;
                break;
            }

            case UWOP_ALLOC_LARGE:
            {
                ++i;

                ULONG size = unwind->UnwindCode[i].FrameOffset;

                if (info == 0)
                {
                    size *= 8;
                }
                else
                {
                    ++i;
                    size += (unwind->UnwindCode[i].FrameOffset << 16);
                }

                totalStackSize += size;
                break;
            }

            case UWOP_SAVE_NONVOL:
            {
                ++i;
                break;
            }

            case UWOP_SET_FPREG:
            {
                break;
            }

            default:
            {
                return 0;
            }
        }
    }

    return totalStackSize;
}

void* FindGadget(HMODULE mod) 
{
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)mod;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((BYTE*)mod + dos->e_lfanew);
    PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(nt);
    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; i++, sec++) 
    {
        if (memcmp(sec->Name, ".text", 5) == 0) 
        {
            BYTE* start = (BYTE*)mod + sec->VirtualAddress;
            for (DWORD j = 0; j < sec->Misc.VirtualSize - 3; j++) 
            {
                // Looking for: jmp qword ptr [rbp+48h] (FF 65 48)
                if (start[j] == 0xFF && start[j + 1] == 0x65 && start[j + 2] == 0x48)
                    return (void*)(start + j);
            }
        }
    }
    return nullptr;
}

int main() 
{
    gadget_addr = FindGadget(GetModuleHandleA("KernelBase.dll"));
    //target_api = (void*)GetProcAddress(LoadLibraryA("kernel32.dll"), "GetCurrentProcessId");
    //target_api = (void*)GetProcAddress(LoadLibraryA("User32.dll"), "MessageBoxA");
    target_api = (void*)GetProcAddress(LoadLibraryA("kernel32.dll"), "CreateFileW");

    if (!gadget_addr || !target_api) return 1;

    rtl = (void*)((BYTE*)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlUserThreadStart") + 0x21);
    btit = (void*)((BYTE*)GetProcAddress(GetModuleHandleA("kernel32.dll"), "BaseThreadInitThunk") + 0x14);

    gadget_size = ChatGPTCalculateFunctionStackSize(gadget_addr);

    btit_size = ChatGPTCalculateFunctionStackSize(btit);

    rtl_size = ChatGPTCalculateFunctionStackSize(rtl);

    
    HANDLE hFile = CreateFileW(
        L"C:\\Users\\Public\\test.txt", // lpFileName
        GENERIC_WRITE,                  // dwDesiredAccess
        FILE_SHARE_READ,                // dwShareMode
        NULL,                           // lpSecurityAttributes
        CREATE_ALWAYS,                  // dwCreationDisposition
        FILE_ATTRIBUTE_NORMAL,          // dwFlagsAndAttributes
        NULL                            // hTemplateFile
    );
    
    /*
    CALL_CONTEXT ctx =
       BuildCallContext(
            L"C:\\Users\\Public\\test.txt",
            GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    
    CALL_CONTEXT ctx =
        BuildCallContext(NULL,"hehe","haha",MB_OK);

    g_CallContext = ctx;
    */
    CallFunction(
        GetProcAddress(LoadLibraryA("kernel32.dll"), "CreateFileW"),
        7,
        L"C:\\Users\\Public\\test.txt",
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );


    std::cout << "[+] Back in Main!" << std::endl;
    return 0;
}