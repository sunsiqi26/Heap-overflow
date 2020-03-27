#include <windows.h>  
#include <stdio.h>  
typedef void (*MYPROC)(LPTSTR);  
int main()  
{       
              HINSTANCE LibHandle;  
              MYPROC ProcAdd;  
              LibHandle = LoadLibrary("msvcrt");  
              printf("msvcrt LibHandle = //x%x\n", LibHandle);  
              ProcAdd=(MYPROC)GetProcAddress(LibHandle,"system");   
              printf("system = //x%x\n", ProcAdd);  
                
              LibHandle = LoadLibrary("kernel32");  
              printf("kernel32 LibHandle = //x%x\n", LibHandle);  
              ProcAdd=(MYPROC)GetProcAddress(LibHandle,"LoadLibraryA");   
              printf("LoadLibraryA = //x%x\n", ProcAdd);  
              return 0;  
}  
//获取默认异常处理函数地址
#include <stdio.h>   
#include <windows.h>   
void main()   
{   
    unsigned int sehaddr;   
    int *un;   
    HMODULE hk = LoadLibrary("KERNEL32.dll");   
    un = (int *)GetProcAddress(hk,"SetUnhandledExceptionFilter");   
    _asm  
    {   
        mov eax,un   
        add eax,5   
        mov ebx,[eax]   
        mov sehaddr,ebx   
    }   
    printf("the top seh: 0x%x\r\n",sehaddr);   
    _getch();   
    return;   
}
