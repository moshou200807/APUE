#include <apue.h>
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";
char buf3[] = "0000000000";

int main(void)
{
    int fd1 = 0;
    int fd2 = 0;
    int i = 0;

    if((fd1 = creat("file", FILE_MODE)) < 0)
	err_sys("creat error");
    if((fd2 = creat("file.hole", FILE_MODE)) < 0)
	err_sys("creat error2");

    if(write(fd1, buf1, 10) != 10)
	err_sys("buf1 write error");

    if(write(fd2, buf1, 10) != 10)
	err_sys("buf1 write error2");

    for(i = 0; i < 1000; i++) {
	if(write(fd1, buf3, 10) != 10)
	    err_sys("buf3 write error");
    }

    if(lseek(fd2, 10000, SEEK_CUR) == -1)
	err_sys("lseek error");

    if(write(fd1, buf2, 10) != 10)
	err_sys("buf2 write error");

    if(write(fd2, buf2, 10) != 10)
	err_sys("buf1 write error2");

   exit(0);
}
