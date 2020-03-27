# Heap overflow

* [实验内容](#实验内容)
* [堆简介](#堆简介)
    * [堆数据结构](#堆数据结构)
    * [堆与栈的区别](#堆与栈的区别)
    * [使用堆时的三种操作](#使用堆时的三种操作)
    * [和堆有关的API函数](#和堆有关的API函数)
    * [默认异常处理](#默认异常处理)
    * [几种执行shellcode的情形](#几种执行shellcode的情形)
* [实验步骤](#实验步骤)

### 实验内容
先建立一个HEAP，溢出buf定位堆溢出点,编写堆溢出的shellcode，把双向链表后一指针覆盖为默认异常处理函数地址从而跳转到shellcode。

### 堆简介

#### 堆数据结构
- 堆块：堆区的内存按不同大小组织成块，以堆块为单位进行标识。
	- 块首：是堆块头部几个字节，用来标识堆块自身信息
	- 块身：是最终分配给用户使用的数据区，堆块系统返回的指针一般指向块身的起始位置
- 堆表：一般位于堆区起始位置，用于索引堆区中所有堆块的重要信息，包括堆块位置、堆块的大小、空闲还是占用等。最重要的堆表有两种：
	- 空表：空闲双向链表freelist，简称空表，用于将空闲堆块组织成双向链表。
	- 快表：快速单向链表looklist，简称快表，用来加速堆块分配而采用的堆表
    
    <img src='https://pic.downk.cc/item/5e7d5bc2504f4bcb041382d9.jpg'/>空闲双向链表
    <img src='https://pic.downk.cc/item/5e7d5cc0504f4bcb0413eb24.jpg'/>快速单向链表


#### 堆与栈的区别
- 堆是一种在程序动态运行时分配的内存，（动态：内存大小在程序设计时不能预先决定，要在程序运行时参考用户反馈）
- 堆使用时需要用专用函数进行申请
- 堆一般用一个堆指针来使用申请到的内存
- 堆使用完后需要把堆指针传给堆释放函数回收这片内存，否则会内存泄露，free，delete
- 而栈只有pop和push两种操作，总是在线性变化，管理机制也相对简单。
- 堆内存与栈内存比较
<img src='https://pic.downk.cc/item/5e7d58d7504f4bcb04121475.jpg'/>

#### 使用堆时的三种操作
使用堆时只需要进行三种操作：申请一定大小的内存、使用内存、释放内存。对于堆管理系统来说，响应程序的内存使用申请意味着要在堆区中辨别哪些内存正在使用，哪些内存空闲，并最终找到适当的空闲内存以指针形式返回给程序。
#### 和堆有关的API函数
- HeapAlloc 在堆中申请内存空间 
- HeapCreate 创建一个新的堆对象 
- HeapDestroy 销毁一个堆对象 
- HeapFree 释放申请的内存 
- HeapWalk 枚举堆对象的所有内存块 
- GetProcessHeap 取得进程的默认堆对象 
- GetProcessHeaps 取得进程所有的堆对象
以上都是用户态的函数，最终都要调用ntdll里面Rtl相关核心函数

#### 默认异常处理
当链表中所有的异常处理函数都无法处理异常时，系统就会使用默认异常处理指针来处理异常情况。
默认异常处理指针通过如下函数来设置：
`SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTERlpTopLevelExceptionFilter)`

默认异常处理指针通过如下函数来调用：
`LONG UnhandledExceptionFilter(STRUCT _EXCEPTION_POINTERS *ExceptionInfo)`
它负责显示一个错误对话框，来指出出错的原因，这就是一般的程序出错时显示错误对话框的原因。

#### 几种执行shellcode的情形
<img src='https://pic.downk.cc/item/5e7d5a44504f4bcb0412cc02.jpg'/>
堆溢出利用的核心就是用精心构造的数据去溢出下一个堆块的块首，改写块首中中的前向指针flink和后向指针blink，然后在分配、释放、合并操作发生时伺机获得一次想内存任意地址写入数据的机会。

### 实验步骤
1. 通过HeapCreate函数先建立一个堆，将堆的初始大小设置为10000bytes，堆的最大大小设置为0xfffff。通过Windows API函数HeapAlloc在指定的堆上分配内存(分配后的内存不可移动)，动态分配200字节buf1和16字节buf2。
<img src='https://pic.downk.cc/item/5e7d5b7e504f4bcb041363b9.jpg'/>
注意：由于DEBUG版的程序与实际运行程序的内存结构是不同的，所以刚才的程序要用VC6按照RELEASE方式编译运行。
在VC下生成RELEASE版的方法是：选择菜单栏‘编译’→‘放置可远程配置’，然后在弹出的‘活动工程配置’中选择‘Win32 RELEASE’，这样，生成的程序就会在Release目录下，并且是发布版本。也可以右击工具栏，选中组件选项，将编译选项设置为win32 release。

2. 定位溢出点:
heapalloc函数先是为buf1分配8字节的管理结构，该结构对用户不可见；然后系统返回给用户能实际操作的200字节的空间，buf1就是从这儿开始的；接着是8字节的下一空闲堆的管理结构；最后是两个双链表指针，各4个字节共8个字节
<img src='https://pic.downk.cc/item/5e7d5cfb504f4bcb04140719.jpg'/>
实现堆溢出，就是要将后面8字节的两个双向链表指针覆盖，即为mybuf数组赋值的超长字符串。
<img src='https://pic.downk.cc/item/5e7d5d8b504f4bcb04144497.jpg'/>

3. 先为mybuf数组填充无意义字符，再在后面跟上开DOS窗口的shellcode。运行获取函数地址的程序得到win2000下system函数和loadlibrary函数的地址。

4. 跳转到shellcode：由于在用超长字符串覆盖buf1时覆盖了最后两个双向链表指针，当程序分配buf2时，就会因为要使用那两个指针而报错。eax为前一指针的值，而ecx为后一指针的值，并作如下操作： mov [ecx],eax; mov [eax+4], ecx ，该操作的目的是在分配内存时改变链表的指向。当系统重新分配‘buf2’，执行到 mov [ecx],eax 时，就等于执行 mov [BBBB],AAAA ，就会报0x42424242不能写的错误。
因此可以将eax覆盖成shellcode的地址，ecx覆盖成默认异常处理函数地址的值，当发生异常时，操作系统就会找异常处理函数，shellcode就可以执行了。
<img heigh=250 width=200 src='https://pic.downk.cc/item/5e7d5e9e504f4bcb0414d24a.jpg'/>
Windows结构化异常处理结构

0x00在内存中是会被截断的，因此要将最后8字节用来覆盖双向链表的shellcode中的0x00换为0xff，在通过strcpy赋值后在转换回0x00