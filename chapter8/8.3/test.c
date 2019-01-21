#include <apue.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	siginfo_t status;

	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
		exit(7);
	
	if(waitid(P_PID, pid, &status, WEXITED) == -1)
		err_sys("wait error");
	//if(status.si_code == )
	printf("%d\n", status.si_code);
	
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
		abort();
	
	if(waitid(P_PID, pid, &status, WEXITED) == -1)
		err_sys("wait error");
	//if(status.si_code == )
	printf("%d\n", status.si_code);
	
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0) {
		int i = 0;
		i /= 0;
	}
	
	if(waitid(P_PID, pid, &status, WEXITED) == -1)
		err_sys("wait error");
	//if(status.si_code == )
	printf("%d\n", status.si_code);

	exit(0);
}
