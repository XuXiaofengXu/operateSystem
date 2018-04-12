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
	shared_memory=shmat(shmid,0,0);	
	//判断是否映射成功，若失败则退出
	if(shared_memory == (void *)(-1)){
		printf("映射共享内存区失败！");
		exit(-1);
	}
	
	//将缓冲区指针转换为shared_mem_st 类型
	shared_stuff =(struct shared_mem_st*) shared_memory;
	
	//下面创建4个信号量，最后一个参数表示信号量的初始值
	sem_clear_memory=sem_open(clear_memory,O_CREAT,0644,0);
	sem_queue=sem_open(queue_mutex,O_CREAT,0644,1);
	//缓冲数组有16行，所以空可写信号量初始值为16
	sem_queue_empty=sem_open(queue_empty,O_CREAT,0644,16);
	//一开始缓冲数组为空，故满的初始值为0
	sem_queue_full=sem_open(queue_full,O_CREAT,0644,0);
	
	//读写指针初始化，开始时都指向第0行
	shared_stuff->line_write=0;
	shared_stuff->line_read=0;
	
	//不断从控制台读入按键输入的字符行
	while(1){
		//提示可以输入，并用get()读入按键行的key_line 中
		printf("Enter your text('quit' for exit):");
		gets(key_line);
		
		//信号量的p操作(Proberen 尝试)，有空余缓冲区
		sem_wait(sem_queue_empty);
		//信号量的p操作(Proberen 尝试),读写互斥
		sem_wait(sem_queue);
		//将输入字符串拷贝到缓冲数组
		strcpy(shared_stuff->buffer[shared_stuff->line_write],key_line);
		//移动写指针
		shared_stuff->line_write=(shared_stuff->line_write+1)%NUM_LINE;
		//信号量的v操作(verhogen 增加),释放
		sem_post(sem_queue);
		//信号量的v操作(verhogen 增加)，已写缓冲区加1
		sem_post(sem_queue_full);
		
		//如果键入“quit”,再写一次，然后退出
		if(strcmp(key_line,"quit")==0){
			
			//信号量的p操作(Proberen 尝试)，有空余缓冲区
			sem_wait(sem_queue_empty);
			//信号量的p操作(Proberen 尝试),读写互斥
			sem_wait(sem_queue);
			//将输入字符串拷贝到缓冲数组
			strcpy(shared_stuff->buffer[shared_stuff->line_write],key_line);
			//移动写指针
			shared_stuff->line_write=(shared_stuff->line_write+1)%NUM_LINE;
			//信号量的v操作(verhogen 增加),释放
			sem_post(sem_queue);
			//信号量的v操作(verhogen 增加)，已写缓冲区加1
			sem_post(sem_queue_full);
			break;
		}

	}	
	//等待消费者完成并释放信号量
	sem_wait(sem_clear_memory);
	//因键入“quit”从前面while()循环中跳出到此处，程序退出前，释放信号量
	sem_unlink(queue_mutex);
	sem_unlink(queue_empty);
	sem_unlink(queue_full);
	sem_unlink(clear_memory);
	//结束共享内存在本进程的挂载映像
	if(shmdt(shared_memory)<0){
		printf("结束挂载镜像失败！");
		exit(-1);
	}	
	//删除共享内存区域
	if(shmctl(shmid,IPC_RMID,0)==-1){
		printf("删除内存共享区域失败！");
		exit(-1);
	}
	return 0;
}
