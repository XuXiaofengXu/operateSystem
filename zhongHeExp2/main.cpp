#include"DiskOperate.h"
#include"FileOperate.h"
#include <stdio.h>
#include <string.h>

/*
*���̿ռ������������ķ�ʽ����1kB��Ϊ�̿�Ϊ���䵥λ������λʾͼ��¼�̿���������λ���̿�0��
*ϵͳ����100k���̿飬ǰ100���̿����ڴ��λͼ��char[]��
*�ļ�Ŀ¼������״Ŀ¼��ϵͳ��ʼ��һ�Ÿ�Ŀ¼���û���ʼλ�ã���ռ��һ���̿�
*Ŀ¼��ļ�����FCB�̿�ţ��ļ�����
*�ļ����ƿ�FCB���ļ�������ʼ�̿�ţ��ļ���С��������,��ָ�룬���ݴ�С��ռ��һ���̿�,���ڶ�������,д�ź�������д�����ź���
*�ļ����ݺ�FCB�ֿ����
*/

//�������
void help();

int main()
{
    //---����ͬ����д

    //semaphore rmutex = 1; //readCount�������
    //semaphore mutex = 1;  //���̶��ļ��Ļ������

    //��ʼ��ϵͳ
    initSystem();
    //������Ŀ¼
    initRootDir();

    help();
    char command[10];
    printf("command:\\>");
    scanf("%s", command);
    while(strcmp(command, "quit") != 0)
    {
        if(strcmp(command, "ls") == 0)//�г�Ŀ¼�������ļ�
        {
            showDir();
            //printf("ls\n");
        }else if(strcmp(command, "help") == 0)//����
        {
            help();
        }else if(strcmp(command, "cd") == 0)//�л�Ŀ¼
        {
            char name[59];
            scanf("%s", name);
            changeDir(name);
        }else if(strcmp(command, "pwd") == 0)//��ʾĿ¼
        {
            printf("%s\n", getPath());
        }else if(strcmp(command, "mkdir") == 0)//����Ŀ¼
        {
            char name[59];
            scanf("%s", name);
            creatDir(name);
        }else if(strcmp(command, "rmdir") == 0)//ɾ��Ŀ¼
        {
            char name[59];
            scanf("%s", name);
            deleteDir(name);
        }else if(strcmp(command, "mv") == 0)//�޸��ļ�����Ŀ¼��
        {
            char oldname[59];
            scanf("%s", oldname);
            char newname[59];
            scanf("%s", newname);
            changeName(oldname, newname);
        }else if(strcmp(command, "touch") == 0)//�����ļ�
        {
            char name[59];
            scanf("%s", name);
            int fileSize;
            scanf("%d", &fileSize);
            creatFile(name, fileSize);
        }else if(strcmp(command, "rm") == 0)//ɾ���ļ�
        {
            char name[59];
            scanf("%s", name);
            deleteFile(name);
        }else if(strcmp(command, "read") == 0)//��ȡ�ļ�
        {
            char name[59];
            scanf("%s", name);
            int length;
            scanf("%d", &length);
            read(name, length);

        }else if(strcmp(command, "reread") == 0)//�����ָ��Ϊ���
        {
            char name[59];
            scanf("%s", name);
            int length;
            scanf("%d", &length);
            reread(name, length);
        }else if(strcmp(command, "write") == 0)//д�ļ�,ֻ֧�ִ�ĩβд��
        {
            char name[59];
            scanf("%s", name);
            char content[1024];
            scanf("%s", content);
            write(name, content);
        }else if(strcmp(command, "rewrite") == 0)//����д�ļ�
        {
            char name[59];
            scanf("%s", name);
            char content[1024];
            scanf("%s", content);
            rewrite(name, content);
        }else
        {
            printf("command error\n");
        }

        printf("command:%s>", getPath());
        scanf("%s", command);
    }
    exitSystem();

    return 0;
}
void help()
{
    printf("===About the command:===\n");
    printf("ls  ---- to list the files and directory in current directory\n");
    printf("pwd ---- to print the current path\n");
    printf("cd dirName ---- to change the current path \n");
    printf("mkdir dirName ---- to create a new directory \n");
    printf("rmdir dirName ---- to delete a directory\n");
    printf("mv fileName newName ---- to rename the file or directory\n");
    printf("touch fileName size(the size of file) ---- eg:touch a.txt 10\n");
    printf("rm fileName --- to remove a file\n");
    printf("read fileName size(the size to read) ---- to read from where last time read; eg:read a.txt 10\n");
    printf("reread fileName size(the size to reread) ---- to reread from where from the start of file; eg:read a.txt 10\n");
    printf("write fileName content(the content to write in) ---- to append at end eg:write a.txt helloworld\n");
    printf("rewrite fileName content ---- to rewrite the file\n");
    printf("\n");
}
