#include<unistd.h>
#include<stdarg.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

/*例程2：创建子进程，然后子进程中执行外部命令（linux命令）
 *
 */

int tprintf(const char* fmt,...);

int main(void){
	pid_t pid;
	pid =fork();
	if(pid==0){ //child process
		sleep(5);
		tprintf("Hello from Child Process!\n");
		tprintf("I am calling exec.\n");
		execl("/bin/ps","-a",NULL);   //执行外部linux命令，其作用是打印出当前执行的进程号
		//execl("/bin/ls","-l","/etc",NULL);
		tprintf("You should never see this because the child is already gone.\n"); //这句话是不会执行的，因为执行外部命令的时候，子进程已经把使用权交给父进程
	}else if(pid!=-1){//parent process
		tprintf("Hello from Parent,pid %d.\n",getpid());
		sleep(1);
		tprintf("Parent forked process %d.\n",pid);
		sleep(1);
		tprintf("Parent is waiting for child to exit.\n");
		waitpid(pid,NULL,0);
		tprintf("Parent has exited.\n");
		
	}else {
		tprintf("Every thing was done without error.\n");
	}
	return 0;
}

int tprintf(const char*fmt,...){
	va_list args;
	struct tm *tstruct;
	time_t tsec;
	tsec=time(NULL);
	tstruct=localtime(&tsec);
	printf("%02d:%02d:%02d: %5d|",tstruct->tm_hour,tstruct->tm_min,tstruct->tm_sec,getpid());
	va_start(args,fmt);
	return vprintf(fmt,args);
}



