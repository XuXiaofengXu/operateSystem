#include<stdio.h>
#include<stdlib.h>
#include<time.h>


/**设计一个程序测试出你的系统单个进程所能分配到的最大虚拟内存空间为多大。
 * 2018/5/24
 */
//***********系统函数***********
char _keygo(){
	char c;
	printf("Press any key to continue:");
	c=getchar();
	return c;
}




//***********主函数*************
int main(){
	
	int max=0;
	int i=1;
	int index=0;
	char* pstr[100000];
	
	
	//从小到大，查找单次分配的最大极限
	printf("按回车开始查找单次分配的最大极限 :\n");
	_keygo();
	while(1){
		
		pstr[index]=(char*)malloc(i*1024*1024);
		if(pstr[index]!=NULL){
			printf("I is :%d\n\n",i);
			_keygo();
			i*=2;
			index++;
		}else {
			printf("申请 :%d MB内存失败\n\n",i);
			i/=2;
			break;
		}		
	}
	printf("单次可以申请的最大内存为:%d\n",i);
	

	//释放查找过程中分配的内存，后面再从头开始分配空间
	printf("按回车开始释放内存 :\n");
	_keygo();
	//index--;
	while(index>=0){
		free(pstr[index]);
		pstr[index]=NULL;
		index--;
	}
	printf("释放内存成功\n");
	
	
	
	//开始以单次最大的极限分配内存，可能进行很多次
	printf("按回车开始以最大的极限分配内存 :\n");
	_keygo();
	index=0;
	while(1){
		pstr[index]=(char*)malloc(i*1024*1024);
		if(pstr[index]!=NULL){
			//printf("I is :%d\n",i);
			//_keygo();
			max+=i;
			index++;
		}else{
			break;
		}

	}
	printf("以最大的极限总共分配了%dMB内存\n",max);

	
	//以从大到小的方式继续分配内存
	printf("按回车开始以从大到小的方式分配内存 :\n");
	_keygo();
	while(i){

		pstr[index]=(char*)malloc(i*1024*1024);
		if(pstr[index]!=NULL){
			printf("I is :%d\n",i);
			_keygo();
			max+=i;
		}
		i/=2;
		index++;
	}
	printf("The max virtual space is :%d MB\n",max);
	
	//释放所有动态分配的内存
	printf("按回车开始释放内存 :\n");
	_keygo();
	index--;
	while(index>=0){
		free(pstr[index]);
		pstr[index]=NULL;
		index--;
	}
	printf("释放内存成功\n");
	

	return 0;
		
}
