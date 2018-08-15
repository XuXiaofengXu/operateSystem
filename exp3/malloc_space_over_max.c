#include<stdio.h>
#include<stdlib.h>
#include<time.h>


/**
 *编写并运行（在第5步的程序未退出前）另一进程，分配等于或大于物理内存的空间，然后每隔4KB间隔将相应地址的字节数值增1，此时再查看前一个程序的物理内存变化，观察两个进程竞争物理内存的现象
 */



//*********系统函数*****************
char _keygo(){
	char c;
	printf("Press any key to continue......\n");
	c=getchar();
	return c;	
}

//**********主函数*****************
int main(){	

	int i;
	char* array[2];
	//连续分配2个512MB的空间
	for(i=0;i<2;i++){
		printf("开始分配第%d号内存空间.......",i+1);
		_keygo();
		array[i]=(char*)malloc(512*1024*1024);	
		if(array[i]==NULL){
			printf("分配第%d号内存失败！\n\n",i+1);	
			return -1;
		}else {
			printf("分配第%d号内存成功！\n\n",i+1);	
		}
	}

	printf("开始对第一块512MB空间每隔4KB空间将相应地址的字节数值增1\n");
	_keygo();
	for(i=0;i<512*1024*1024;i=i+4*1024){
		array[0][i]++;	//每隔4KB空间将相应地址的字节数值增1
		//_keygo();
	}
	printf("开始对第二块512MB空间每隔4KB空间将相应地址的字节数值增1\n");
	_keygo();
	for(i=0;i<512*1024*1024;i=i+4*1024){
		array[1][i]++;	//每隔4KB空间将相应地址的字节数值增1
		//_keygo();
	}
	
	
	printf("开始释放内存空间\n");
	_keygo();
	//释放所有空间
	for(i=0;i<2;i++){
		if(array[i]!=NULL){
			free(array[i]);
			array[i]=NULL;
		}
	
	}
		
	return 0;

}

