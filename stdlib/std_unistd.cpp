#include <cstdio>

//系统调用（System Calls）:
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

//  1. 文件相关的系统调用
// - `open()`: 打开或创建一个文件
// - `close()`: 关闭一个文件
// - `read()`: 从文件中读取数据
// - `write()`: 将数据写入文件
// - `lseek()`: 在文件中定位
// - `unlink()`: 删除一个文件
// - `mkdir()`: 创建一个目录
// - `rmdir()`: 删除一个目录
// - `mkdirat()`: 在指定路径下创建一个目录
// - `link()`: 创建一个硬链接
// - `unlinkat()`: 删除一个文件或目录
// - `rename()`: 重命名一个文件
// - `stat()`: 获取文件的状态信息
// - `fstat()`: 获取文件描述符所指文件的状态信息
// - `chmod()`: 修改文件权限
// - `chown()`: 修改文件所有者
// - `chdir()`: 修改当前工作目录
// - `getcwd()`: 获取当前工作目录
// - `opendir()`: 打开一个目录
// - `readdir()`: 读取目录中的项
// - `closedir()`: 关闭目录


// 2. 进程和CPU的系统调用
#include <sys/wait.h>
#include <sys/resource.h>
#include <sched.h>

// - `fork()`: 创建一个新进程
// - `exec()` 系列函数（如`execve()`, `execl()`, `execle()`等）: 执行一个新程序
// - `exit()`: 终止当前进程
// - `wait()` 系列函数（如`waitpid()`, `wait3()`, `wait4()`等）: 等待子进程终止
// - `kill()`: 给进程发送信号
// - `getpid()`: 获取当前进程ID
// - `getppid()`: 获取父进程ID
// - `getuid()`: 获取用户ID
// - `getgid()`: 获取组ID
// - `nice()`: 修改进程优先级
// - `sched_setaffinity()`, `sched_getaffinity()`: 设置和获取进程的CPU亲和性
// - `getrusage()`: 获取进程资源使用情况
// - `times()`: 获取进程的执行时间信息
// - `setrlimit()`, `getrlimit()`: 设置和获取进程资源限制

// 3. 内存相关的系统调用

// - brk() 系统调用可以将程序的数据段结束位置（堆的起始地址）设置到指定的位置，从而直接控制堆的大小。
// - sbrk() 系统调用则可以将堆的结束地址递增指定的字节数，从而调整堆的大小。
// - malloc() 函数用于动态分配指定大小的堆内存，并返回分配的内存区域的首地址。
// - free() 函数可以释放之前由`malloc()`分配的内存。
// - mmap() 系统调用用于将文件或设备映射到内存中，创建一个新的内存映射区域。
// - munmap() 系统调用则用于解除内存映射关系，释放对应的内存区域。
// - mprotect() 该系统调用用于更改内存页的保护标志，以控制对内存的读、写和执行权限。
// - memalign() 该函数用于以特定的对齐方式分配内存。通常用于需要特定对齐要求的数据结构。
// - mlock() 和 munlock() 这两个系统调用分别用于将内存锁定到物理内存中，以防止被交换到磁盘上，以及解除内存锁定。

int main(int argc, char *argv[]) {
    int fd = open("test.txt", O_CREAT | O_RDWR);
    if(fd == -1) {
        printf("faild to open file %s \n", "test.txt");
    }

    return 0;
}