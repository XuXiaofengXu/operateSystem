#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>


/**
 *分配足够大的内存空间，其容量超过系统现有的空闲物理内存的大小，1）按4KB的间隔逐个单元进行写操作，重复访问数遍（使得程序运行时间可测量）；2）与前面访问总量和次数不便，但是将访问分成16个连续页为一组，逐组完成访问，记录运行时间。观察系统的状态，比较两者运行时间，给出判断和解释。
 */



//*********系统函数*****************
char _keygo(){
	char c;
	printf("Press any key to continue......\n");
	c=getchar();
	return c;	
}

//**********主函数*****************
int main(int argc,char** argv){	
	
	time_t start,end;
	int N=atoi(argv[1]),i,j;
	char* array[2];
	//连续分配2个512MB的空间
	for(i=0;i<2;i++){
		printf("开始分配第%d号内存空间.......",i+1);
		//_keygo();
		array[i]=(char*)malloc(512*1024*1024);	
		if(array[i]==NULL){
			printf("分配第%d号内存失败！\n\n",i+1);	
			return -1;
		}else {
			memset(array[i],0,512*1024*1024);
			printf("分配第%d号内存成功,并已完成初始化！\n\n",i+1);	
		}
	}


	printf("开始对2块512MB空间每隔4KB空间将相应地址的字节数值增1,重复执行%d次\n",N);
	//_keygo();
	start=time(NULL);
	for(j=0;j<N;j++){
		for(i=0;i<512*1024*1024;i=i+4*1024){
			array[0][i]++;	//每隔4KB空间将相应地址的字节数值增1
			//_keygo();
		}
		for(i=0;i<512*1024*1024;i=i+4*1024){
			array[1][i]++;	//每隔4KB空间将相应地址的字节数值增1
			//_keygo();
		}
	}
	end=time(NULL);
	printf("按4KB的间隔逐个单元进行写操作，重复访问%d遍，总共花费%d s\n\n",N,(end-start));
	

	printf("开始对2块512MB空间每隔8KB空间(即2个连续页)将相应地址的字节数值增1,重复执行%d次\n",2*N);
	//_keygo();
	start=time(NULL);
	for(j=0;j<2*N;j++){
		for(i=0;i<512*1024*1024;i=i+2*4*1024){
			array[0][i]++;	//每隔64KB空间将相应地址的字节数值增1
			//_keygo();
		}
		for(i=0;i<512*1024*1024;i=i+2*4*1024){
			array[1][i]++;	//每隔64KB空间将相应地址的字节数值增1
			//_keygo();
		}
	}
	end=time(NULL);
	printf("按8KB的间隔逐个单元进行写操作，重复访问%d 遍，总共花费%d s\n\n",2*N,(end-start));


	printf("开始对2块512MB空间每隔16KB空间(即4个连续页)将相应地址的字节数值增1,重复执行%d次\n",4*N);
	//_keygo();
	start=time(NULL);
	for(j=0;j<4*N;j++){
		for(i=0;i<512*1024*1024;i=i+4*4*1024){
			array[0][i]++;	//每隔64KB空间将相应地址的字节数值增1
			//_keygo();
		}
		for(i=0;i<512*1024*1024;i=i+4*4*1024){
			array[1][i]++;	//每隔64KB空间将相应地址的字节数值增1
			//_keygo();
		}
	}
	end=time(NULL);
	printf("按16KB的间隔逐个单元进行写操作，重复访问%d 遍，总共花费%d s\n\n",4*N,(end-start));


	printf("开始对2块512MB空间每隔32KB空间(即8个连续页)将相应地址的字节数值增1,重复执行%d次\n",8*N);
	//_keygo();
	start=time(NULL);
	for(j=0;j<8*N;j++){
		for(i=0;i<512*1024*1024;i=i+8*4*1024){
			array[0][i]++;	//每隔64KB空间将相应地址的字节数值增1
			//_keygo();
		}
		for(i=0;i<512*1024*1024;i=i+8*4*1024){
			array[1][i]++;	//每隔64KB空间将相应地址的字节数值增1
			//_keygo();
		}
	}
	end=time(NULL);
	printf("按32KB的间隔逐个单元进行写操作，重复访问%d 遍，总共花费%d s\n\n",8*N,(end-start));





	printf("开始对2块512MB空间每隔64KB空间(即16个连续页)将相应地址的字节数值增1,重复执行%d次\n",16*N);
	//_keygo();
	start=time(NULL);
	for(j=0;j<16*N;j++){
		for(i=0;i<512*1024*1024;i=i+16*4*1024){
			array[0][i]++;	//每隔64KB空间将相应地址的字节数值增1
			//_keygo();
		}
		for(i=0;i<512*1024*1024;i=i+16*4*1024){
			array[1][i]++;	//每隔64KB空间将相应地址的字节数值增1
			//_keygo();
		}
	}
	end=time(NULL);
	printf("按64KB的间隔逐个单元进行写操作，重复访问%d 遍，总共花费%d s\n\n",16*N,(end-start));
	

	printf("开始释放内存空间...\n");
	//_keygo();
	//释放所有空间
	for(i=0;i<2;i++){
		if(array[i]!=NULL){
			free(array[i]);
			array[i]=NULL;
		}
	
	}
		
	return 0;

}

