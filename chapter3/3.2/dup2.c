#include <fcntl.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

int mydup2(int fd, int fd2)
{
    /*
    * 按照man dup2的说法，如果fd没有打开或者fd2超过已打开fd的上限，返回-1，并且置errno为EBADF
    * 其他多线程或系统中断相关的err暂不处理。
    * */
    int fd_temp = 0;
    int top = 0;
    int fd_stack[sysconf(_SC_OPEN_MAX)];
    
    if(fd > sysconf(_SC_OPEN_MAX) || fd2 > sysconf(_SC_OPEN_MAX) || fcntl(fd, F_GETFD) == -1 || fd < 0 || fd2 < 0) {
        errno = EBADF;
        return -1;
    }
    
    if(fd == fd2) {
        return fd2;
    }

    while((fd_temp = dup(fd), fd_temp != -1) && fd_temp <= fd2) {
        fd_stack[top++] = fd_temp;
    }

    /*
    * 跳出while循环的原因之一是fd_temp > fd2。
    * 对fd_stack[]，如果里面有元素，表明是一步步执行dup直到fd2的，此时关闭fd_stack中的其他fd以及fd_temp，返回fd_stack的数组尾元素，即fd2.
    * 		    如果里面没有元素，表明执行了一次dup就已经超过了fd2，也就是说fd2已经是打开状态。此时，关闭fd_temp和fd2，再次执行一次dup，返回。
    * */
    
    if(fd_temp != -1) {
        close(fd_temp);
    }

    if(top == 0 && fd_temp != -1) {
        close(fd2);
        return dup(fd);
    }

    while(--top) {
        close(fd_stack[top - 1]);
    }

    if(fd_temp == -1)
	return -1;
    
    return fd2;
}

int main(int argc, char *argv[])
{
    int fd = 0;
    int fd2 = 0;
    if(argc != 3) {
        printf("Parameter error!\n");
	return -1;
    }

    fd = openat(AT_FDCWD, argv[1], O_RDWR | O_APPEND);
    if(fd == -1) {
        printf("Open file %s error!\n", argv[1]);
	return -1;
    }
    
    fd2 = atoi(argv[2]);
    if(mydup2(fd, fd2) != -1) {
        if(lseek(fd2, SEEK_SET, 1) == -1)
	    printf("lseek fail!\n");
	write(fd2, "test", sizeof("test"));
    }
    return 0;
}
