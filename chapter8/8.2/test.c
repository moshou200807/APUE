#include <apue.h>

static void f1(void),f2(void);

int main(void)
{
	f1();
	f2();
	exit(0);
}

static void f1()
{
	pid_t pid;
	if((pid == vfork()) < 0)
		err_sys("vfork error");
}

static void f2()
{
	char buf[1000];
	int i;
	for(i = 0; i < 1000; i++) {
		buf[i] = 0;
	}
}
