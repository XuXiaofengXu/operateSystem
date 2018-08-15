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
	int N=atoi(argv[1]),i,j,k;
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

	long times=0l;

	printf("开始对2块512MB空间每隔4KB空间将相应地址的字节数值增1,重复执行%d次\n",N);
	//_keygo();
	start=time(NULL);
	for(j=0;j<N;j++){
		for(i=0;i<512*1024*1024;i=i+4*1024){
			array[0][i]++;	//每隔4KB空间将相应地址的字节数值增1
			//_keygo();
			times++;
		}
		for(i=0;i<512*1024*1024;i=i+4*1024){
			array[1][i]++;	//每隔4KB空间将相应地址的字节数值增1
			//_keygo();
			times++;
		}
	}
	end=time(NULL);
	printf("按4KB的间隔逐个单元进行写操作，重复访问%d遍，总共花费%d s\n\n",N,(end-start));
	printf("总共的读写次数为 %ld\n",times);

	


	printf("开始对2块512MB空间每隔4KB空间将相应地址的字节数值增1,以16页为一组,重复执行%d次\n",N);
	//_keygo();
	start=time(NULL);

	
	times=0l;
	for(k=0;k<512*1024*1024;k+=16*4*1024){
		for(j=0;j<N;j++){
			for(i=0;i<16;i++){
				array[0][k+i*4*1024]++;	//每隔4KB空间将相应地址的字节数值增1
				//_keygo();
				times++;
			}
		}
	}
	for(k=0;k<512*1024*1024;k+=16*4*1024){
		for(j=0;j<N;j++){
			for(i=0;i<16;i++){
				array[1][k+i*4*1024]++;	//每隔4KB空间将相应地址的字节数值增1
				//_keygo();
				times++;
			}
		}
	}
	end=time(NULL);
	printf("按4KB的间隔逐个单元进行写操作，以16页为一组,重复访问%d 遍，总共花费%d s\n\n",N,(end-start));
	printf("总共的读写次数为 %ld\n",times);
	

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

