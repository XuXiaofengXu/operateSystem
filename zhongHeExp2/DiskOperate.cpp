#include"DiskOperate.h"

//���ڴ����ź���ʱ��ʶ��id
const char * mutex_id="mutex_id";  //��д�����ź���
const char * write_id="write_id";  //д�ź���

char* systemStartAddr;  //ϵͳ��ʼ��ַ
void *shared_memory=(void *)0;//�����ڴ棨������ָ�룩
struct shared_mem_st * shared_stuff; //�������͹���洢����ת��Ϊshare_mem_st ���͵�ָ��
int shmid; //�����ڴ��id


//��ʼ��ϵͳ
void initSystem()
{
	//��ȡ�����ڴ���
	shmid=shmget((key_t)2333,sizeof(struct shared_mem_st),0666|IPC_CREAT);
	//�ж��Ƿ��ȡ�����ڴ����ɹ�����ʧ�����˳�
	if(shmid==-1){
		printf("Failed to create Share Memory Segments\n");
		exit(-1);
	}
	//���ɹ��򽫹����ڴ���ӳ�䵽�����̵Ľ��̿ռ�
	shared_memory=shmat(shmid,(void*)0,0);
	//�ж��Ƿ�ӳ��ɹ�����ʧ�����˳�
	if(shared_memory == (void *)(-1)){
		printf("Failed to map Share Memory Segments\n");
		exit(-1);
	}

	//��������ָ��ת��Ϊshared_mem_st ����
	shared_stuff =(struct shared_mem_st *) shared_memory;

	//����ռ�Ŀ�ʼ��ַָ��buffer����
    systemStartAddr=shared_stuff->buffer;


    //�ɸ���bitmapλ��ֵ���ж��Ƿ��Ѿ���ʼ��
    if(systemStartAddr[0] !='1' ){
        //��ʼ���̿��λʾͼ��ǰblock_count�ֽ����ڴ洢λʾͼ���Ƚ�ȫ����ֵΪ0
        for(int i=0; i<block_count; i++)
            systemStartAddr[i] = '0';
        //���ڴ��λʾͼ�Ŀռ��ѱ�ռ��
        int bitMapSize = block_count * sizeof(char) / block_szie;//λʾͼռ�ÿ���Ϊ100��
        for(int i=0; i<bitMapSize; i++)//���㿪ʼ����
            systemStartAddr[i] = '1';   //˵��ǰ100���̿��ѱ�ʹ�ã����ڴ洢bitMap
        shared_stuff->user=1; //���û�����ʼ��Ϊ1

    }else {
          shared_stuff->user=shared_stuff->user+1; //ʹ���û�����1
    }
}

//�˳�ϵͳ
void exitSystem()
{

    //free(systemStartAddr);

    if(shared_stuff->user==1){ //��������һ���û�
         //���������ڴ��ڱ����̵Ĺ���ӳ��
        if(shmdt(shared_memory)<0){
            printf("Failed to unload Share Memory Segments\n");
            exit(-1);
        }
        //ɾ�������ڴ�����
        if(shmctl(shmid,IPC_RMID,0)==-1){
            printf("Failed to delete Share Memory Segments\n");
            exit(-1);
        }
    }else{
        shared_stuff->user=shared_stuff->user-1; //ʹ���û�����1
    }
}

//���̷��䣬�������Ϊ��Ҫһ���Է���Ŀ�ĸ��������õ����״���Ӧ�ķ������з���
int getBlock(int blockSize)
{
    int startBlock = 0;
    int sum=0;
    for(int i=0; i<block_count; i++)
    {
        if(systemStartAddr[i] == '0')//�����̿�
        {
            if(sum == 0)//�տ�ʼ�����ÿ�ʼ�̿��
                startBlock = i;
            sum++;
            if(sum == blockSize)//�����̿��Ƿ���������
            {
                //������䣬��1
                for(int j=startBlock; j<startBlock+blockSize; j++)
                    systemStartAddr[j] = '1';
                return startBlock;
            }

        }
        else//�ѱ�ʹ��,�����Ѿ������
            sum = 0;
    }
    //���û���ҵ��������㹻��Ŀ����ڷ����ڴ棬�򷵻�-1
    printf("not found such series memory Or memory is full\n");
    return -1;
}

//����̿�������ַ
char* getBlockAddr(int blockNum)
{
    return systemStartAddr + blockNum * block_szie; //ƫ������λΪ�ֽ�
}


//��������ַ���̿��
int getAddrBlock(char* addr)
{
    return (addr - systemStartAddr)/block_szie;
}


//�ͷ��̿�
int releaseBlock(int blockNum, int blockSize)
{
    int endBlock = blockNum + blockSize;
    //�޸�λʾͼ�̿��λ��Ϊ0
    for(int i=blockNum; i<endBlock; i++)
        systemStartAddr[i] = '0';
    return 0;
}
