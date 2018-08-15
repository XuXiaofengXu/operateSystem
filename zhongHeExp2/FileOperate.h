#ifndef FILEOPERATE_H_INCLUDED
#define FILEOPERATE_H_INCLUDED


//�ļ�����
#include"DiskOperate.h"

#define dirTable_max_size 15    //Ŀ¼�������ֵ



//Ŀ¼��ṹ��
//һ��Ŀ¼��������ļ������ļ����ͣ����ļ�ΪĿ¼ʱ����ʼ�̿�ָʾ��Ŀ¼�����ڵ��̿�ţ����ļ�Ϊ�ļ�ʱ����ʼ�̿�ָʾ��FCB���ڵ��̿�š�
struct dirUnit{
    char fileName[59];  //�ļ���
    char type;  //�ļ�����,0Ŀ¼�� 1�ļ�
    int startBlock; //FCB��ʼ�̿�
};



//Ŀ¼��ṹ��
/*
��ϵͳ�涨һ��Ŀ¼��ֻռ��һ���̿飬һ��Ŀ¼���СΪ64B������һ��Ŀ¼�������ɺ�15��Ŀ¼�dirUnitAmount��¼ÿ��Ŀ¼�����Ѻ��е�Ŀ¼����Ŀ��ϵͳ�ڳ�ʼ��ʱ�����Զ�����һ���յĸ�Ŀ¼�����ڴ����У���Ϊ�û��ĳ�ʼλ�ã��û����е�Ŀ¼���ļ��������Ϊ��������״Ŀ¼�ṹ��չ����
������һ��Ŀ¼��ʱ��ϵͳ���Զ�ΪĿ¼�����һ����Ϊ��..����Ŀ¼�ָʾ��Ŀ¼���λ�á�
*/
struct dirTable {
    int dirUnitAmount;//Ŀ¼����Ŀ
    dirUnit dirs[dirTable_max_size];//Ŀ¼���б�
};




//FCB�ṹ��
/*
    �ļ����ƿ�������ļ����ݵ���ʼλ�úʹ�С��dataSize,readptr��Ϊ�ļ��Ķ�д������׼���ģ���¼�ļ���д������ݳ��ȣ����ɳ����ļ���С�����͵�ǰ��ȡ��λ�á�Link��¼���ļ����������������ļ��Ĺ������ļ���������Ϊ0ʱ��ϵͳ���Ի����ļ��Ŀռ䡣ͬ���ģ�һ��FCB��СΪ20B����Ҳ��һ���̿鱣�档
    ���ڲ��õ����������䷽ʽ������ϵͳ�涨�ļ�������ʱ����������ļ��Ĵ�С�����Һ���Ҳ�����޸��ļ��Ĵ�С��
*/
struct FCB {
    int blockNum;   //�ļ�������ʼ�̿��
    int fileSize;   //�ļ���С���̿�Ϊ��λ
    int dataSize;   //��ǰ��д������ݴ�С���ֽ�Ϊ��λ��д�����ݵĴ�С��С�ڵ��ڸ�pcb�����̿�Ĵ�С֮��
    int readptr;    //��ָ�룬�ֽ�Ϊ��λ
    int link;   //�ļ�������
    int reader_num;   //���ߵ�����
    int sem_write;   //д�ź���
    int sem_mutex;	//��д�������
};




 //�ļ�ϵͳ����
//��ʼ����Ŀ¼
void initRootDir();
//չʾ����·��
char* getPath();
//չʾ��ǰĿ¼�ļ�
void showDir();
//����Ŀ¼
int creatDir(char dirName[]);
//�л�Ŀ¼
int changeDir(char dirName[]);
//ɾ��Ŀ¼
int deleteDir(char fileName[]);
//�޸��ļ�������Ŀ¼��
int changeName(char oldName[], char newName[]);

//�����ļ�
int creatFile(char fileName[], int fileSize);
//ɾ���ļ�
int deleteFile(char fileName[]);


//���ļ�
int read(char fileName[], int length);
//���¶��ļ�
int reread(char fileName[], int length);
//ִ�ж�����
int doRead(FCB* myFCB, int length);
//д�ļ�����ĩβд��
int write(char fileName[], char content[]);
//����д����
int rewrite(char fileName[], char content[]);
//ִ��д����
int doWrite(FCB* myFCB, char content[]);

//�ͷ��ļ��ڴ�
int releaseFile(int FCBBlock);
//���Ŀ¼��
int addDirUnit(dirTable* currentDirTable, char fileName[], int type, int FCBBlockNum);
//����FCB
int creatFCB(int fcbBlockNum, int fileBlockNum, int fileSize);
//ָ��Ŀ¼ɾ��
int deleteFileInTable(dirTable* myDirTable, int unitIndex);
//ɾ��Ŀ¼��
int deleteDirUnit(dirTable* currentDirTable, int unitIndex);
//��Ŀ¼�в���Ŀ¼��Ŀ
int findUnitInTable(dirTable* currentDirTable, char unitName[]);

//�����ź���
void signal(int & sem);
void wait(int & sem);



#endif // FILEOPERATE_H_INCLUDED
