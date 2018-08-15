#include<stdio.h>
#include<stdlib.h>
#include<time.h>


/**
 *
 * 编写程序，连续申请分配六个128MB空间（记为1~6号），然后释放第2、3、5号的128MB空间。然后再分配1024MB，记录该进程的虚存空间变化（/proc/pid/maps），每次操作前后检查/proc/pid/status文件中关于内存的情况，简要说明虚拟内存变化情况。推测此时再分配64M内存将出现在什么位置，实测后是否和你的预测一致？解释说明用户进程空间分配属于课本中的离散还是连续分配算法？首次适应还是最佳适应算法？用户空间存在碎片问题吗？
 */



//*********系统函数*****************
char _keygo(){
	char c;
	c=getchar();
	return c;	
}

//**********主函数*****************
int main(){	

	int i;
	char* array[6];
	//连续分配6个128m的空间
	for(i=0;i<6;i++){
		printf("开始分配第%d号内存空间.......",i+1);
		_keygo();
		array[i]=(char*)malloc(128*1024*1024);	
		if(array[i]==NULL){
			printf("分配第%d号内存失败！\n\n",i+1);	
		}else {
			printf("分配第%d号内存成功！\n\n",i+1);	
		}
	}


	//释放第2,3,5号空间
	for(i=0;i<6;i++){
		if( (i+1)==2 || (i+1)==3  || (i+1)==5){

			printf("准备释放第%d号内存空间.......",i+1);
			_keygo();
			free(array[i]);
			array[i]=NULL;	//将指针赋值为NULL，因为free只是释放空间，但指针还是指向原来的位置
			printf("释放第%d号内存空间成功\n\n",i+1);
		}
	}

	//再分配512MB空间
	char *another512;
	printf("开始分配 512MB 内存空间.......");
	_keygo();
	another512=(char*)malloc(512*1024*1024);	
	if(another512==NULL){
		printf("分配 512MB 内存空间失败！\n\n");	
	}else {
		printf("分配 512MB 内存空间成功！\n\n");	
	}

	//再分配64MB空间
	char *another64;
	printf("开始分配 64MB 内存空间.......");
	_keygo();
	another64=(char*)malloc(64*1024*1024);	
	if(another64==NULL){
		printf("分配 64MB 内存空间失败！\n\n");	
	}else {
		printf("分配 64MB 内存空间成功！\n\n");	
	}

	printf("程序即将结束，按回车退出.....");
	_keygo();
	
	//释放所有空间
	if(another512!=NULL){
		free(another512);
		another512=NULL;
		}
	if(another64!=NULL){
		free(another64);
		another64=NULL;
		}
	for(i=0;i<6;i++){
		if(array[i]!=NULL){
			free(array[i]);
			array[i]=NULL;
			}
	
	}
		
	return 0;

}

