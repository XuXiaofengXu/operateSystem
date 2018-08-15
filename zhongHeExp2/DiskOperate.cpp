#include"DiskOperate.h"

//用于创建信号量时的识别id
const char * mutex_id="mutex_id";  //读写互斥信号量
const char * write_id="write_id";  //写信号量

char* systemStartAddr;  //系统起始地址
void *shared_memory=(void *)0;//共享内存（缓冲区指针）
struct shared_mem_st * shared_stuff; //将无类型共享存储区域转换为share_mem_st 类型的指针
int shmid; //共享内存的id


//初始化系统
void initSystem()
{
	//获取共享内存区
	shmid=shmget((key_t)2333,sizeof(struct shared_mem_st),0666|IPC_CREAT);
	//判断是否获取共享内存区成功，若失败则退出
	if(shmid==-1){
		printf("Failed to create Share Memory Segments\n");
		exit(-1);
	}
	//若成功则将共享内存区映射到本进程的进程空间
	shared_memory=shmat(shmid,(void*)0,0);
	//判断是否映射成功，若失败则退出
	if(shared_memory == (void *)(-1)){
		printf("Failed to map Share Memory Segments\n");
		exit(-1);
	}

	//将缓冲区指针转换为shared_mem_st 类型
	shared_stuff =(struct shared_mem_st *) shared_memory;

	//共享空间的开始地址指向buffer数组
    systemStartAddr=shared_stuff->buffer;


    //可根据bitmap位的值来判断是否已经初始化
    if(systemStartAddr[0] !='1' ){
        //初始化盘块的位示图，前block_count字节用于存储位示图，先将全部赋值为0
        for(int i=0; i<block_count; i++)
            systemStartAddr[i] = '0';
        //用于存放位示图的空间已被占用
        int bitMapSize = block_count * sizeof(char) / block_szie;//位示图占用块数为100块
        for(int i=0; i<bitMapSize; i++)//从零开始分配
            systemStartAddr[i] = '1';   //说明前100块盘块已被使用，用于存储bitMap
        shared_stuff->user=1; //将用户数初始化为1

    }else {
          shared_stuff->user=shared_stuff->user+1; //使用用户数加1
    }
}

//退出系统
void exitSystem()
{

    //free(systemStartAddr);

    if(shared_stuff->user==1){ //如果是最后一个用户
         //结束共享内存在本进程的挂载映像
        if(shmdt(shared_memory)<0){
            printf("Failed to unload Share Memory Segments\n");
            exit(-1);
        }
        //删除共享内存区域
        if(shmctl(shmid,IPC_RMID,0)==-1){
            printf("Failed to delete Share Memory Segments\n");
            exit(-1);
        }
    }else{
        shared_stuff->user=shared_stuff->user-1; //使用用户数减1
    }
}

//磁盘分配，传入参数为需要一次性分配的块的个数，采用的是首次适应的方法进行分配
int getBlock(int blockSize)
{
    int startBlock = 0;
    int sum=0;
    for(int i=0; i<block_count; i++)
    {
        if(systemStartAddr[i] == '0')//可用盘块
        {
            if(sum == 0)//刚开始，设置开始盘块号
                startBlock = i;
            sum++;
            if(sum == blockSize)//连续盘块是否满足需求
            {
                //满足分配，置1
                for(int j=startBlock; j<startBlock+blockSize; j++)
                    systemStartAddr[j] = '1';
                return startBlock;
            }

        }
        else//已被使用,连续已经被打断
            sum = 0;
    }
    //如果没有找到连续的足够大的块用于分配内存，则返回-1
    printf("not found such series memory Or memory is full\n");
    return -1;
}

//获得盘块的物理地址
char* getBlockAddr(int blockNum)
{
    return systemStartAddr + blockNum * block_szie; //偏移量单位为字节
}


//获得物理地址的盘块号
int getAddrBlock(char* addr)
{
    return (addr - systemStartAddr)/block_szie;
}


//释放盘块
int releaseBlock(int blockNum, int blockSize)
{
    int endBlock = blockNum + blockSize;
    //修改位示图盘块的位置为0
    for(int i=blockNum; i<endBlock; i++)
        systemStartAddr[i] = '0';
    return 0;
}
