#include<unistd.h>
#include<stdarg.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

/*例程1：展示如何创建一个子进程
 *
 *
 */


//用来打印特定格式信息
int tprintf(const char* fmt,...);

int main(void){
	int i=0,j=0;
	pid_t pid;
	printf("Hello from Parent Process,PID is %d.\n",getpid());
	pid =fork();
	if(pid==0){ //child process 
		sleep(1);
		for(i=0;i<3;i++){
			printf("Hello from Child Process %d.%d times\n",getpid(),i+1);
			sleep(1);
		}
		
	}else if(pid!=-1){//parent process
		//sleep(1);
		tprintf("Parent forked one child process--%d.\n",pid);
		tprintf("Parent is waiting for child to exit.\n");
		waitpid(pid,NULL,0);
		tprintf("Child Process has exited.\n");
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



