#include<unistd.h>
#include<stdarg.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
/*用来创建具有完全二叉树结构的进程树
 *总共4层
 */


int tprintf(const char* fmt,...);
pid_t createSubProcess(int index);
const int N = 2;   //父进程创建子进程的数量

int main(void){
	int i=0,j=0;
	pid_t* pids=(pid_t*)malloc(N*sizeof(pid_t));
	
	printf("Hello from Parent Process 1,PID is %d.\n",getpid());   //父进程序号为1
	for(i=2;i<N+2;i++){     //子进程的序号为2,3，也就是2*1=2和2*1+1=3
		pids[i]=createSubProcess(i);
	//	tprintf("Parent forked child process %d --%d.\n",i,pids[i]);
	}
	
	tprintf("Parent is waiting for child to exit.\n");
	for(i=2;i<N+2;i++){
		waitpid(pids[i],NULL,0);
	//	tprintf("Child Process %d  has exited.\n",i);
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
	pid=fork();
	if(pid==0){
		sleep(3);
		for(i=0;i<1;i++){
			printf("Hello from Child Process %d %d.%d times\n",index,getpid(),i+1);
			sleep(1);
		
		}
		if(index<=7){  //前3层总共有1+2+4=7个节点，所以属于前3层的就继续创建子进程
		pid_t pids[2];  
		pids[0]=createSubProcess(2*index);  //创建左子进程
		pids[1]=createSubProcess(2*index+1); //创建右子进程
		waitpid(pids[0],NULL,0);  //等待子进程
		waitpid(pids[1],NULL,0);
		}
		exit(0);
	}else if(pid!=-1)
		return pid;
}

