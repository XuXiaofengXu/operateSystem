#include"FileOperate.h"
#include<unistd.h>
#include<string.h>
#include<stdio.h>


extern char* systemStartAddr;  //�ⲿ������ϵͳ��ʼ��ַ
dirTable* rootDirTable; //��Ŀ¼
dirTable* currentDirTable;  //��ǰλ��
char path[200]; //���浱ǰ����·��


//��ʼ����Ŀ¼
void initRootDir()
{
    int bitMapSize = block_count * sizeof(char) / block_szie;//λʾͼռ���̿���:100
    int startBlock;
    if(systemStartAddr[bitMapSize] !='1' ){  //�����101���̿�δ��ʹ�ã�˵����Ŀ¼δ����ռ䡣
        //����һ���̿�ռ��rootDirTable
        startBlock = getBlock(1);
        if(startBlock == -1)
            return;
    }else{
        startBlock=bitMapSize; //�����101���̿��Ѿ���ʹ�ã�˵���Ѿ�����ռ䡣�������̿�����Ϊ100��
    }

    printf("Bit map startBlock is %d/n",startBlock);


    rootDirTable = (dirTable*)getBlockAddr(startBlock);
    rootDirTable->dirUnitAmount = 0;

    //��������Ϊ����Ŀ¼
    //addDirUnit(rootDirTable, "..", 0, startBlock);

       //һ��СԲ���ʾ��ǰ·��????????????????????????????�ǲ���Ҫ��һ�£�������
    //addDirUnit(rootDirTable, ".", 0, startBlock);

    currentDirTable = rootDirTable;
    //��ʼ����ʼ����·��
    path[0]='\\';   //˫б�ܣ�һ��б����Ϊ��ת�壬ʵ�ʵ�����Ч������һ��б��
    path[1]='\0';   //������
}

//��þ���·��
char* getPath()
{
    return path;
}

//չʾ��ǰĿ¼ ls
void showDir()
{
    int unitAmount = currentDirTable->dirUnitAmount;
    printf("total:%d\n", unitAmount);
    printf("name\ttype\tsize\tFCB\tdataStartBlock\n");
    //�������б���
    for(int i=0; i<unitAmount; i++)
    {
        //��ȡĿ¼��
        dirUnit unitTemp = currentDirTable->dirs[i];
        printf("%s\t%d\t", unitTemp.fileName, unitTemp.type);
        //�ñ������ļ������������С����ʼ�̿��
        if(unitTemp.type == 1)   //��Ϊ�ļ�����ʼ�̿�ָʾ��PCB����Ϣ�������FCB��Ϣ
        {
            int FCBBlock = unitTemp.startBlock;
            FCB* fileFCB = (FCB*)getBlockAddr(FCBBlock);
            printf("%d\t%d\t%d\n", fileFCB->fileSize, FCBBlock, fileFCB->blockNum);
        }else{                  //��ΪĿ¼����ʼ�̿�ָʾ��Ŀ¼�����ڵ��̿�ţ������Ŀ¼��������Ϣ
            int dirBlock = unitTemp.startBlock;
            dirTable* myTable = (dirTable*)getBlockAddr(dirBlock);
            printf("%d\t%d\n",myTable->dirUnitAmount, unitTemp.startBlock);
        }
    }
}

//�л�Ŀ¼ cd
int changeDir(char dirName[])
{
    //Ŀ¼����Ŀ¼λ��
    int unitIndex = findUnitInTable(currentDirTable, dirName);
    //������
    if(unitIndex == -1)
    {
        printf("file not found\n");
        return -1;
    }
    if(currentDirTable->dirs[unitIndex].type == 1)
    {
        printf("not a dir\n");
        return -1;
    }
    //�޸ĵ�ǰĿ¼
    int dirBlock = currentDirTable->dirs[unitIndex].startBlock;
    currentDirTable = (dirTable*)getBlockAddr(dirBlock);

    //�޸�ȫ�־���·��
    if(strcmp(dirName, "..") == 0)  //����Ƿ�����һ���˵�����ô�ͰѾ���·��ɾ�������һ��б�ܵĵط�
    {
        //���˾���·��
        int len = strlen(path);
        for(int i=len-2;i>=0;i--)
            if(path[i] == '\\')
            {
                path[i+1]='\0';
                break;
            }
    }else {
        strcat(path, dirName);
        strcat(path, "\\");
    }

    return 0;
}



//�޸��ļ�������Ŀ¼�� mv
int changeName(char oldName[], char newName[])
{
    int unitIndex = findUnitInTable(currentDirTable, oldName);
    if(unitIndex == -1)
    {
        printf("file not found\n");
        return -1;
    }
    strcpy(currentDirTable->dirs[unitIndex].fileName, newName);
    return 0;
}


//******************������ɾ���ļ�
//�����ļ� touch
int creatFile(char fileName[], int fileSize)
{
    //����ļ����ֳ���
    if(strlen(fileName) >= 59)
    {
        printf("file name too long\n");
        return -1;
    }
    //���FCB�Ŀռ�
    int FCBBlock = getBlock(1);
    if(FCBBlock == -1)
        return -1;
    //��ȡ�ļ����ݿռ�
    int FileBlock = getBlock(fileSize);
    if(FileBlock == -1)
        return -1;
    //����FCB
    if(creatFCB(FCBBlock, FileBlock, fileSize) == -1)
        return -1;
    //��ӵ�Ŀ¼��
    if(addDirUnit(currentDirTable, fileName, 1, FCBBlock) == -1)
        return -1;

    return 0;
}

//����Ŀ¼ mkdir
int creatDir(char dirName[])
{
    if(strlen(dirName) >= 59)
    {
        printf("file name too long\n");
        return -1;
    }
    //ΪĿ¼�����ռ�
    int dirBlock = getBlock(1);
    if(dirBlock == -1)
        return -1;
    //��Ŀ¼��ΪĿ¼����ӵ���ǰĿ¼
    if(addDirUnit(currentDirTable, dirName, 0, dirBlock) == -1)
        return -1;
    //Ϊ�½���Ŀ¼���һ�������ĸ�Ŀ¼��Ŀ¼��
    dirTable* newTable = (dirTable*)getBlockAddr(dirBlock);
    newTable->dirUnitAmount = 0;
    char parent[] = "..";
    if(addDirUnit(newTable, parent, 0, getAddrBlock((char*)currentDirTable)) == -1)
        return -1;
    return 0;
}

//����FCB
int creatFCB(int fcbBlockNum, int fileBlockNum, int fileSize)
{
    //�ҵ�fcb�Ĵ洢λ��
    FCB* currentFCB = (FCB*) getBlockAddr(fcbBlockNum);
    currentFCB->blockNum = fileBlockNum;//�ļ�������ʼλ��
    currentFCB->fileSize = fileSize;//�ļ���С
    currentFCB->link = 1;//�ļ�������
    currentFCB->dataSize = 0;//�ļ���д�����ݳ���
    currentFCB->readptr = 0;//�ļ���ָ��
    currentFCB->reader_num=0;//���ߵ�����
    currentFCB->sem_write=1;//д�ź���
    currentFCB->sem_mutex=1;//��д�����ź���
    return 0;
}

//���Ŀ¼��
int addDirUnit(dirTable* myDirTable, char fileName[], int type, int FCBBlockNum)
{
    //���Ŀ¼��
    int dirUnitAmount = myDirTable->dirUnitAmount;
    //���Ŀ¼��ʾ�Ƿ�����
    if(dirUnitAmount == dirTable_max_size)
    {
        printf("dirTables is full, try to delete some file\n");
        return -1;
    }

    //�Ƿ����ͬ���ļ�
    if(findUnitInTable(myDirTable, fileName) != -1)
    {
        printf("file already exist\n");
           return -1;
    }
    //������Ŀ¼��
    dirUnit* newDirUnit = &myDirTable->dirs[dirUnitAmount];  //�½�һ��ָ��ָ����Ҫ�����Ŀ¼���λ��
    myDirTable->dirUnitAmount++;//��ǰĿ¼���Ŀ¼������+1
    //������Ŀ¼������
    strcpy(newDirUnit->fileName, fileName);
    newDirUnit->type = type;
    newDirUnit->startBlock = FCBBlockNum;

    return 0;
}


//ɾ���ļ� rm
int deleteFile(char fileName[])
{
    //����ϵͳ���Զ������ĸ�Ŀ¼
    if(strcmp(fileName, "..") == 0)
    {
        printf("can't delete ..\n");
        return -1;
    }
    //�����ļ���Ŀ¼��λ��
    int unitIndex = findUnitInTable(currentDirTable, fileName);
    if(unitIndex == -1)
    {
        printf("file not found\n");
        return -1;
    }
    dirUnit myUnit = currentDirTable->dirs[unitIndex];
    //�ж�����
    if(myUnit.type == 0)//Ŀ¼
    {
        printf("not a file\n");
        return -1;
    }
    int FCBBlock = myUnit.startBlock;
    //�ͷ��ڴ�
    releaseFile(FCBBlock);
    //��Ŀ¼�����޳�
    deleteDirUnit(currentDirTable, unitIndex);
    return 0;
}

//�ͷ��ļ��ڴ�
int releaseFile(int FCBBlock)
{
    FCB* myFCB = (FCB*)getBlockAddr(FCBBlock);
    myFCB->link--;  //��������һ
    //�����ӣ�ɾ���ļ�
    if(myFCB->link == 0)
    {
        //�ͷ��ļ������ݿռ�
        releaseBlock(myFCB->blockNum, myFCB->fileSize);
    }
    //�ͷ�FCB�Ŀռ�
    releaseBlock(FCBBlock, 1);
    return 0;
}

//ɾ��Ŀ¼��
int deleteDirUnit(dirTable* myDirTable, int unitIndex)
{

    int dirUnitAmount = myDirTable->dirUnitAmount;
    //Ǩ�Ƹ��ǣ������������ǰŲ
    for(int i=unitIndex; i<dirUnitAmount-1; i++)
    {
        myDirTable->dirs[i] = myDirTable->dirs[i+1];
    }
    myDirTable->dirUnitAmount--;
    return 0;
}

//ɾ��Ŀ¼�ļ� rmdir
int deleteDir(char dirName[])
{
    //����ϵͳ���Զ������ĸ�Ŀ¼
    if(strcmp(dirName, "..") == 0)
    {
        printf("can't delete ..\n");
        return -1;
    }
    //�����ļ�
    int unitIndex = findUnitInTable(currentDirTable, dirName);
    if(unitIndex == -1)
    {
        printf("file not found\n");
        return -1;
    }
    dirUnit myUnit = currentDirTable->dirs[unitIndex];
    //�ж�����
    if(myUnit.type == 0)//Ŀ¼
    {
        deleteFileInTable(currentDirTable, unitIndex);
    }else {
        printf("not a dir\n");
        return -1;
    }
    //��Ŀ¼�����޳�
    deleteDirUnit(currentDirTable, unitIndex);
    return 0;
}

//ɾ���ļ�/Ŀ¼��
int deleteFileInTable(dirTable* myDirTable, int unitIndex)
{
   //�����ļ�
    dirUnit myUnit = myDirTable->dirs[unitIndex];
    //�ж�����
    if(myUnit.type == 0)//Ŀ¼
    {
        //�ҵ�Ŀ¼λ��
        int FCBBlock = myUnit.startBlock;
        dirTable* table = (dirTable*)getBlockAddr(FCBBlock);
        //�ݹ�ɾ��Ŀ¼�µ������ļ�
        printf("cycle delete dir %s\n", myUnit.fileName);
        int unitCount = table->dirUnitAmount;
        for(int i=1; i<unitCount; i++)//���ԡ�..��
        {
            printf("delete %s\n", table->dirs[i].fileName);
            deleteFileInTable(table, i);
        }
        //�ͷ�Ŀ¼��ռ�
        releaseBlock(FCBBlock, 1);
    }else {//�ļ�
        //�ͷ��ļ��ڴ�
        int FCBBlock = myUnit.startBlock;
        releaseFile(FCBBlock);
    }
    return 0;
}



//**********************��д����
//���ļ� read
int read(char fileName[], int length)
{


    int unitIndex = findUnitInTable(currentDirTable, fileName);
    if(unitIndex == -1)
    {
        printf("file no found\n");
        return -1;
    }
    //���ƿ�
    int FCBBlock = currentDirTable->dirs[unitIndex].startBlock;
    FCB* myFCB = (FCB*)getBlockAddr(FCBBlock);


    printf("waiting for read\n");
    wait(myFCB->sem_mutex);
    if(myFCB->reader_num==0)  //��һ������
        wait(myFCB->sem_write);
    myFCB->reader_num=myFCB->reader_num+1;
    signal(myFCB->sem_mutex);

    printf("reading....\n");
    sleep(10);
    doRead(myFCB, length);

    wait(myFCB->sem_mutex);
    myFCB->reader_num=myFCB->reader_num-1;
    if(myFCB->reader_num==0) //���һ������
        signal(myFCB->sem_write);
    signal(myFCB->sem_mutex);
    printf("finished read\n");


    return 0;
}

//���¶��ļ� reread
int reread(char fileName[], int length)
{
    int unitIndex = findUnitInTable(currentDirTable, fileName);
    if(unitIndex == -1)
    {
        printf("file no found\n");
        return -1;
    }
    //���ƿ�
    int FCBBlock = currentDirTable->dirs[unitIndex].startBlock;
    FCB* myFCB = (FCB*)getBlockAddr(FCBBlock);
    myFCB->readptr = 0;  //�Ѷ�ָ������Ϊ��ʼλ��0


    printf("waiting for read\n");
    wait(myFCB->sem_mutex);
    if(myFCB->reader_num==0)  //��һ������
        wait(myFCB->sem_write);
    myFCB->reader_num=myFCB->reader_num+1;
    signal(myFCB->sem_mutex);

    printf("reading....\n");
    sleep(10);
    doRead(myFCB, length);

    wait(myFCB->sem_mutex);
    myFCB->reader_num=myFCB->reader_num-1;
    if(myFCB->reader_num==0) //���һ������
        signal(myFCB->sem_write);
    signal(myFCB->sem_mutex);
    printf("finished read\n");


    return 0;
}

//ִ�ж�����
int doRead(FCB* myFCB, int length)
{


    //������
    int dataSize = myFCB->dataSize;
    char* data = (char*)getBlockAddr(myFCB->blockNum);  //��ȡ���ݵ���ʼ�����ڵĵ�ַ
    //�ڲ��������ݳ����£���ȡָ�����ȵ�����
    for(int i=0; i<length && myFCB->readptr < dataSize; i++, myFCB->readptr++)
    {
        printf("%c", *(data+myFCB->readptr));
    }
    if(myFCB->readptr == dataSize)//�����ļ�ĩβ��#��ʾ
        printf("#");
    //��������
    printf("\n");

    return 0;
}
//д�ļ�����ĩβд�� write
int write(char fileName[], char content[])
{
    int unitIndex = findUnitInTable(currentDirTable, fileName);
    if(unitIndex == -1)
    {
        printf("file no found\n");
        return -1;
    }
    //���ƿ�
    int FCBBlock = currentDirTable->dirs[unitIndex].startBlock;
    FCB* myFCB = (FCB*)getBlockAddr(FCBBlock);

    printf("waiting for write\n");
    wait(myFCB->sem_write);
    printf("writing\n");
    sleep(10);
    doWrite(myFCB, content);
    signal(myFCB->sem_write);
    printf("finished write\n");


    return 0;
}

//����д���� rewrite
int rewrite(char fileName[], char content[])
{
    int unitIndex = findUnitInTable(currentDirTable, fileName);
    if(unitIndex == -1)
    {
        printf("file no found\n");
        return -1;
    }
    //���ƿ�
    int FCBBlock = currentDirTable->dirs[unitIndex].startBlock;
    FCB* myFCB = (FCB*)getBlockAddr(FCBBlock);
    //�������ݿ�
    myFCB->dataSize = 0;
    myFCB->readptr = 0;


    wait(myFCB->sem_write);
    doWrite(myFCB, content);
    signal(myFCB->sem_write);

    return 0;
}
//ִ��д����
int doWrite(FCB* myFCB, char content[])
{
    int contentLen = strlen(content);
    int fileSize = myFCB->fileSize * block_szie;
    char* data = (char*)getBlockAddr(myFCB->blockNum);  //��ȡ���ݵ���ʼ�����ڵĵ�ַ
    //�ڲ������ļ��Ĵ�С�ķ�Χ��д��
    for(int i=0; i<contentLen && myFCB->dataSize<fileSize; i++, myFCB->dataSize++)
    {
        *(data+myFCB->dataSize) = content[i];
    }
    if(myFCB->dataSize == fileSize)
        printf("file is full,can't write in\n");
    return 0;
}



//��Ŀ¼�в���Ŀ¼��Ŀ
int findUnitInTable(dirTable* myDirTable, char unitName[])
{
    //���Ŀ¼��
    int dirUnitAmount = myDirTable->dirUnitAmount;
    int unitIndex = -1;
    for(int i=0; i<dirUnitAmount; i++)//����Ŀ¼��λ��
       if(strcmp(unitName, myDirTable->dirs[i].fileName) == 0)
            unitIndex = i;
    return unitIndex;
}



void signal(int & sem){
    sem=sem+1;
}

void wait(int & sem){
    while(sem<=0){
        sleep(1);
    }
    sem=sem-1;
}
