#include<unistd.h>
#include<stdarg.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

/*实现创建多个子进程的功能
 *
 */

int tprintf(const char* fmt,...);
pid_t createSubProcess(int index);//此函数用来创建子进程
const int N = 5;  //表示创建的子进程的数量

int main(void){
	int i=0,j=0;
	pid_t* pids=(pid_t*)malloc(N*sizeof(pid_t));  //动态申请一个数组，用来保存子进程号
	
	printf("Hello from Parent Process,PID is %d.\n",getpid());
	for(i=0;i<N;i++){
		pids[i]=createSubProcess(i);  //调用创建子进程的函数，传入参数是一个子进程的序号
		tprintf("Parent forked child process %d --%d.\n",i,pids[i]);
	}
	
	tprintf("Parent is waiting for child to exit.\n");
	for(i=0;i<N;i++){
		waitpid(pids[i],NULL,0);      //等待各个子进程
		tprintf("Child Process %d  has exited.\n",i);
	}
	tprintf("Parent has exited.\n");

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


pid_t createSubProcess(int index){
	pid_t pid;
	int  i;
	pid=fork();   //创建子进程
	if(pid==0){   //子进程执行的代码块
		sleep(3);
		for(i=0;i<3;i++){
			printf("Hello from Child Process %d %d.%d times\n",index,getpid(),i+1);
			sleep(1);
		}
		exit(0);   //子进程执行完成后退出
	}else if(pid!=-1)   //父进程直接将进程号返回
		return pid;
}

