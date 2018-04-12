#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>

#define LINE_SIZE 256
#define NUM_LINE 16

//用于创建信号量时的识别id
const char * queue_mutex="queue_mutex";
const char * queue_empty="queue_empty";
const char * queue_full="queue_full";
const char * clear_memory="clear_memory";

//生产者消费者公用的缓存区，含读写指针line_write 和 line_read ，以及缓冲数据区buffer[NUM_LINE][LINE_SIZE],buffer[X] 指向第x行信息，buffer[x][y]指向x行的信息的y个字符

struct shared_mem_st{
	char buffer[NUM_LINE][LINE_SIZE];
	int line_write;
	int line_read;
};
