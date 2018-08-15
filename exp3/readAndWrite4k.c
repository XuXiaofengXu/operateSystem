#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>


/**
 *编写一个程序，分配256MB内存空间（或其他足够大的空间），检查分配前后/proc/pid/status文件中关于虚拟内存和物理内存的使用情况，然后每隔4KB间隔将相应地址进行读操作，再次检查/proc/pid/status文件中关于内存的情况，对比前后两次内存情况，说明所分配物理内存（物理内存块）的变化。然后重复上面操作，不过此时为写操作，再观察其变化.
 */



//*********系统函数*****************
void _sleep(int n){	//停顿n秒
	clock_t goal;
	goal=(clock_t)n*CLOCKS_PER_SEC+clock();
	while(goal>clock());	
}


char _keygo(){
	char c;
	printf("Press any key to continue......\n");
	c=getchar();
	return c;	
}

//**********主函数*****************
int main(){	

	int i;
	char* qstr;
	char temp;
	
	printf("开始分配256MB内存......\n");
	_keygo();

	//分配256MB内存空间
	qstr=(char*)malloc(256*1024*1024);
	if(qstr!=NULL){
		printf("分配256MB空间成功！\n");
	}else{
		
		printf("分配256MB空间失败！\n“");
		return -1;
	}


	printf("按回车开始每隔4kB地址进行读操作......\n");
	_keygo();
	//每隔4KB间隔将相应地址进行读操作
	for(i=0;i<256*1024*1024;i=i+4*1024){
		temp=qstr[i];
	}
	

	printf("按回车开始每隔4kB地址进行写操作......\n");
	_keygo();
	//每隔4KB间隔将相应地址进行写操作
	for(i=0;i<256*1024*1024;i=i+4*1024){
		qstr[i]='a';
		//_keygo();
	}

	printf("按回车开始每隔4kB地址进行第二次进行写操作......\n");
	_keygo();
	//每隔4KB间隔将相应地址进行写操作
	for(i=0;i<256*1024*1024;i=i+4*1024){
		qstr[i]='a';
		//_keygo();
	}


	//
	printf("按回车释放内存后退出.....\n");
	_keygo();
	
	//释放所有空间
	if(qstr!=NULL){
		free(qstr);
		qstr=NULL;
	}
		
	return 0;

}

