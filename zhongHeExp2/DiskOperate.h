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

//���̲����ӿ�
#define system_size 100*1024*1024   //ϵͳ��С,Ϊ100M
#define block_szie 1024 //�̿��С ��Ĭ��һ�����СΪ1K
#define block_count system_size/block_szie //ϵͳ�̿���Ŀ��Ϊ100K

//��ʼ��ϵͳ
void initSystem();
//���̷���
int getBlock(int blockSize) ;
//����̿�������ַ
char* getBlockAddr(int blockNum);
//��������ַ���̿��
int getAddrBlock(char* addr);
//�ͷ��̿顢
int releaseBlock(int blockNum, int blockSize);
//�˳�ϵͳ
void exitSystem();


struct shared_mem_st{
	char buffer[system_size];  //�������Ŀռ�
	int user; //ʹ�ù����ڴ�ռ���û���
};

#endif // DISKOPERATE_H_INCLUDED
