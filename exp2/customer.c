#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "shm_com_sem.h"

int main(void){
	void *shared_memory=(void *)0;//共享内存（缓冲区指针）
	struct shared_mem_st * shared_stuff; //将无类型共享存储区域转换为share_mem_st 类型的指针
	char key_line[256];
	int shmid; //共享内存的id
	pid_t fork_result;
	
	//访问共享内存的互斥量、空缓冲区、满缓冲区信号量，皆为信号量指针
	sem_t *sem_queue,*sem_queue_empty,*sem_queue_full,*sem_clear_memory;
	
	//获取共享内存区
	shmid=shmget((key_t)2333,sizeof(struct shared_mem_st),0666|IPC_CREAT);
	//判断是否获取共享内存区成功，若失败则退出
	if(shmid==-1){
		printf("获取共享内存区失败！");
		exit(-1);
	}
	//若成功则将共享内存区映射到本进程的进程空间
	shared_memory=shmat(shmid,(void*)0,0);	
	//判断是否映射成功，若失败则退出
	if(shared_memory == (void *)(-1)){
		printf("映射共享内存区失败！");
		exit(-1);
	}
	
	//将缓冲区指针转换为shared_mem_st 类型
	shared_stuff =(struct shared_mem_st*) shared_memory;
	
	//下面创建4个信号量，最后一个参数表示信号量的初始值
	sem_clear_memory=sem_open(clear_memory,O_CREAT);
	sem_queue=sem_open(queue_mutex,O_CREAT);
	//缓冲数组有16行，所以空可写信号量初始值为16
	sem_queue_empty=sem_open(queue_empty,O_CREAT);
	//一开始缓冲数组为空，故满的初始值为0
	sem_queue_full=sem_open(queue_full,O_CREAT);
	
	//创建子进程
	fork_result=fork();
	if(fork_result==-1){
		fprintf(stderr,"Fork failure\n");
	}
	if(fork_result==0){
		while(1){
			
			//信号量的p操作(Proberen 尝试)，缓冲区不为空
			sem_wait(sem_queue_full);
			//信号量的p操作(Proberen 尝试),读写、写写互斥
			sem_wait(sem_queue);
			//将输入字符串拷贝到临时变量
			strcpy(key_line,shared_stuff->buffer[shared_stuff->line_read]);
			if(strcmp(key_line,"quit")!=0){
				//打印出缓冲区信息
				printf("PID %d read : %s \n",getpid(),key_line);
				//移动写指针
				shared_stuff->line_read=(shared_stuff->line_read+1)%NUM_LINE;
				//信号量的v操作(verhogen 增加),释放
				sem_post(sem_queue);
				//信号量的v操作(verhogen 增加)，空缓冲区加1
				sem_post(sem_queue_empty);
			}else {
				//移动写指针
				shared_stuff->line_read=(shared_stuff->line_read+1)%NUM_LINE;
				//信号量的v操作(verhogen 增加),释放
				sem_post(sem_queue);
				//信号量的v操作(verhogen 增加)，空缓冲区加1
				sem_post(sem_queue_empty);
				break;	
			}
			
		}		
		//因键入“quit”从前面while()循环中跳出到此处，程序退出前，释放信号量
		sem_unlink(queue_mutex);
		sem_unlink(queue_empty);
		sem_unlink(queue_full);
		sem_unlink(clear_memory);
		return 0;
	}else {
		while(1){
			
			//信号量的p操作(Proberen 尝试)，缓冲区不为空
			sem_wait(sem_queue_full);
			//信号量的p操作(Proberen 尝试),读写、写写互斥
			sem_wait(sem_queue);
			//将输入字符串拷贝到临时变量
			strcpy(key_line,shared_stuff->buffer[shared_stuff->line_read]);
			if(strcmp(key_line,"quit")!=0){
				//打印出缓冲区信息
				printf("PID %d read : %s \n",getpid(),key_line);
				//移动写指针
				shared_stuff->line_read=(shared_stuff->line_read+1)%NUM_LINE;
				//信号量的v操作(verhogen 增加),释放
				sem_post(sem_queue);
				//信号量的v操作(verhogen 增加)，空缓冲区加1
				sem_post(sem_queue_empty);
			}else {
				//移动写指针
				shared_stuff->line_read=(shared_stuff->line_read+1)%NUM_LINE;
				//信号量的v操作(verhogen 增加),释放
				sem_post(sem_queue);
				//信号量的v操作(verhogen 增加)，空缓冲区加1
				sem_post(sem_queue_empty);
				break;	
			}
			
		}	
	        waitpid(fork_result,NULL,0);
		//增加信号量
		sem_post(sem_clear_memory);
		//因键入“quit”从前面while()循环中跳出到此处，程序退出前，释放信号量
		sem_unlink(queue_mutex);
		sem_unlink(queue_empty);
		sem_unlink(queue_full);
		sem_unlink(clear_memory);
		return 0;


	}
}
