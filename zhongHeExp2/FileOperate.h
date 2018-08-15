#ifndef FILEOPERATE_H_INCLUDED
#define FILEOPERATE_H_INCLUDED


//文件操作
#include"DiskOperate.h"

#define dirTable_max_size 15    //目录表项最大值



//目录项结构：
//一个目录项包含了文件名和文件类型，当文件为目录时，起始盘块指示了目录表所在的盘块号，当文件为文件时，起始盘块指示了FCB所在的盘块号。
struct dirUnit{
    char fileName[59];  //文件名
    char type;  //文件类型,0目录， 1文件
    int startBlock; //FCB起始盘块
};



//目录表结构：
/*
本系统规定一个目录表只占用一个盘块，一个目录项大小为64B，所以一个目录表中最多可含15个目录项，dirUnitAmount记录每个目录表中已含有的目录项数目。系统在初始化时，会自动生成一个空的根目录表存放于磁盘中，作为用户的初始位置，用户所有的目录和文件都这个表为根进行树状目录结构的展开。
当创建一个目录表时，系统会自动为目录表加上一项名为”..”的目录项，指示父目录表的位置。
*/
struct dirTable {
    int dirUnitAmount;//目录项数目
    dirUnit dirs[dirTable_max_size];//目录项列表
};




//FCB结构：
/*
    文件控制块包含了文件数据的起始位置和大小。dataSize,readptr是为文件的读写操作而准备的，记录文件已写入的内容长度（不可超过文件大小），和当前读取的位置。Link记录了文件的链接数，用于文件的共享，当文件的链接数为0时，系统可以回收文件的空间。同样的，一个FCB大小为20B，但也用一个盘块保存。
    由于采用的是连续分配方式，所以系统规定文件被创建时，必须给出文件的大小，而且后期也不能修改文件的大小。
*/
struct FCB {
    int blockNum;   //文件数据起始盘块号
    int fileSize;   //文件大小，盘块为单位
    int dataSize;   //当前已写入的内容大小，字节为单位；写入内容的大小，小于等于该pcb所有盘块的大小之和
    int readptr;    //读指针，字节为单位
    int link;   //文件链接数
    int reader_num;   //读者的数量
    int sem_write;   //写信号量
    int sem_mutex;	//读写互斥变量
};




 //文件系统操作
//初始化根目录
void initRootDir();
//展示绝对路径
char* getPath();
//展示当前目录文件
void showDir();
//创建目录
int creatDir(char dirName[]);
//切换目录
int changeDir(char dirName[]);
//删除目录
int deleteDir(char fileName[]);
//修改文件名或者目录名
int changeName(char oldName[], char newName[]);

//创建文件
int creatFile(char fileName[], int fileSize);
//删除文件
int deleteFile(char fileName[]);


//读文件
int read(char fileName[], int length);
//重新读文件
int reread(char fileName[], int length);
//执行读操作
int doRead(FCB* myFCB, int length);
//写文件，从末尾写入
int write(char fileName[], char content[]);
//重新写覆盖
int rewrite(char fileName[], char content[]);
//执行写操作
int doWrite(FCB* myFCB, char content[]);

//释放文件内存
int releaseFile(int FCBBlock);
//添加目录项
int addDirUnit(dirTable* currentDirTable, char fileName[], int type, int FCBBlockNum);
//创建FCB
int creatFCB(int fcbBlockNum, int fileBlockNum, int fileSize);
//指定目录删除
int deleteFileInTable(dirTable* myDirTable, int unitIndex);
//删除目录项
int deleteDirUnit(dirTable* currentDirTable, int unitIndex);
//从目录中查找目录项目
int findUnitInTable(dirTable* currentDirTable, char unitName[]);

//控制信号量
void signal(int & sem);
void wait(int & sem);



#endif // FILEOPERATE_H_INCLUDED
