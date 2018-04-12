#include<unistd.h>
#include<stdarg.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

/*用于创建两个子进程
 * 
 */

int tprintf(const char* fmt,...);

int main(void){
	int i=0,j=0;
	pid_t pid1,pid2;
	printf("Hello from Parent Process,PID is %d.\n",getpid());
	pid1 =fork();   //创建第一个子进程
	if(pid1==0){ //child 1 process
		sleep(3);
		for(i=0;i<3;i++){
			printf("Hello from Child 1 Process %d.%d times\n",getpid(),i+1);
			sleep(1);
		}

	}else if(pid1!=-1){//parent process
		tprintf("Parent forked  child 1 process--%d.\n",pid1);
		pid2=fork();   //创建第二个子进程(创建第二个子进程必须在父进程所运行的代码块中创建，否则child 1 也会创建一个子进程，那总共就有3个子进程了)
		if(pid2==0){ //child 2 process
			sleep(3);
			for(j=0;j<3;j++){
				printf("Hello from Child 2 Process %d.%d times\n",getpid(),j+1);
				sleep(1);
			}
		}else if(pid2!=-1){
			tprintf("Parent forked  child 2 process--%d.\n",pid2);
			tprintf("Parent is waiting for children to exit.\n");
			waitpid(pid1,NULL,0);
			tprintf("Child 1 Process has exited.\n");
			waitpid(pid2,NULL,0);
			tprintf("Child 2 Process has exited.\n");
			tprintf("Parent has exited.\n");

		}else {
			tprintf("Every thing was done without error.\n");
		}
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



