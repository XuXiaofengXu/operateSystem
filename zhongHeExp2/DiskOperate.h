#ifndef DISKOPERATE_H_INCLUDED
#define DISKOPERATE_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>

//磁盘操作接口
#define system_size 100*1024*1024   //系统大小,为100M
#define block_szie 1024 //盘块大小 ，默认一个块大小为1K
#define block_count system_size/block_szie //系统盘块数目，为100K

//初始化系统
void initSystem();
//磁盘分配
int getBlock(int blockSize) ;
//获得盘块的物理地址
char* getBlockAddr(int blockNum);
//获得物理地址的盘块号
int getAddrBlock(char* addr);
//释放盘块、
int releaseBlock(int blockNum, int blockSize);
//退出系统
void exitSystem();


struct shared_mem_st{
	char buffer[system_size];  //代表分配的空间
	int user; //使用共享内存空间的用户数
};

#endif // DISKOPERATE_H_INCLUDED
