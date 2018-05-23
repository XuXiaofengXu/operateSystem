/*
 * 本程序用于模拟各种进程调度算法
 */


#include <stdio.h>
#include<stdlib.h>
#include <curses.h>
#include<time.h>

//define NULL0
//getch有一个int型的返回值.当程序调用getch时.程序就等着用户按键.
//用户输入的字符被存放在键盘缓冲区中.直到用户按回车为止(回车字符也放在缓冲区中).
//当用户键入回车之后,getch才开始从stdio流中每次读入一个字符
#define getpch(type) (type*)malloc(sizeof(type))
#define maxnum 10	//最大进程数量
typedef struct pcb PCB;

struct pcb{
	int id;           //标识符
	char name[10];    //名称
	int time_start;   //到达时间
	int time_need;    //服务时间
	int time_left;    //剩余运行时间
	int time_used;    //已使用的cpu时间
	//在时间片机制中，time_need=time_left+time_used
	char state;	//进程状态

};


//**********系统函数*******************
void _sleep(int n){  //停顿n秒

	clock_t goal;
	goal=(clock_t)n * CLOCKS_PER_SEC+clock();
	while(goal>clock());

}


char _keygo(){   //程序暂停，按任意键继续
	char c;
	printf("按任意键继续……\n");
	c=getchar();
	return c;
}



//************用户函数****************

//-----------数据设置区域-------------
int time_unit=2;	//时间片长度
int num=5;		//实际进程数量
PCB pcbdata[maxnum]={	//在程序内设计进程数据，不必手工输入
	{1000,"A",0,4,4,0,'R'},
	{1001,"B",1,3,3,0,'R'},
	{1002,"C",2,5,5,0,'R'},
	{1003,"D",3,2,2,0,'R'},
	{1004,"E",4,4,4,0,'R'},
};


int ready[maxnum];	//就绪队列，存放进程在pcbdata中的位置
/*例如上面的数据经过某种调度算法排序为CBADE
 * 就绪队列的第一个进程为C，
 * 存放在ready数组的第一个数值为2，因为pcbdata[2]是C进程的PCB
 * 就绪队列的第二个进程为B，
 * 存放在ready数组的第一个数值为1，因为pcbdata[1]是B进程的PCB
 */

int order[maxnum];	//记录排序使用哪个数值作为排序对象

void input(){		//手工输入数据
	int i;
	printf("进程总数为：");
	scanf("%d",&num);
	for(i=0;i<num;i++){
		pcbdata[i].id=1000+i;	//系统自动分配标识符
		printf("输入第%d个进程名：",i+1);
		scanf("%s",&pcbdata[i].name);
		printf("输入第%d个进程到达时间：",i+1);
		scanf("%s",&pcbdata[i].time_start);
		printf("输入第%d个进程服务时间：",i+1);
		scanf("%s",&pcbdata[i].time_need);
		pcbdata[i].time_left=pcbdata[i].time_need;	//剩余运行时间，初始值和服务时间相同
		printf("\n");
		pcbdata[i].time_used=0;	//已使用CPU时间初始值为0
		pcbdata[i].state='R';	//进程初始状态为就绪R
	}
}

//-------------------调度函数------------------
void FCFS(){	//先来先服务算法
	int i,j,temp;
	double k;
	for(i=0;i<num;i++){
		order[i]=pcbdata[i].time_start;
		ready[i]=i;
	}
	for(i=0;i<num;i++){	//冒泡排序
		for(j=i+1;j<num;j++){
			if(order[i]>order[j]){
				temp=order[i];
				order[i]=order[j];
				order[j]=temp;
				temp=ready[i];
				ready[i]=ready[j];
				ready[j]=temp;
			}
		}
	}
	printf("---先来先服务算法调度：非抢占，无时间片---\n");
	temp=pcbdata[ready[0]].time_start;
	for(i=0;i<num;i++){
			printf("第%d个进程-- %s",i+1,pcbdata[ready[i]].name);
			printf("到达时间 -- %d，服务时间 -- %d \n"
				,pcbdata[ready[i]].time_start
				,pcbdata[ready[i]].time_need);
			printf("本进程正在运行.........");
			_sleep(1);
			printf("运行完毕\n");
			temp+=pcbdata[ready[i]].time_need;	//进程完成时间
			j=temp-pcbdata[ready[i]].time_start;	//周转时间
			k=(float)j/pcbdata[ready[i]].time_need;	//带权周转时间
			printf("完成时间 -- %d，周转时间-- %d，带权周转时间 -- %.1f\n",temp,j,k);

		}
		printf("------所有的进程调度完毕-----\n");
}

void SJF(){	//短作业优先算法
//以服务时间为排序对象，从小到大排列，可能有多次排序
	int i,j,temp,index,p;
	double k;

	for(i=0;i<num;i++){
		order[i]=pcbdata[i].time_need;  //将服务时间作为排序的标准
		ready[i]=i;
	}
	for(i=0;i<num;i++){	//冒泡排序,服务时间短的在前，长的在后
		for(j=i+1;j<num;j++){
			if(order[i]>order[j]){
				temp=order[i];
				order[i]=order[j];
				order[j]=temp;
				temp=ready[i];
				ready[i]=ready[j];
				ready[j]=temp;
			}
		}
	}

	//把到达时间最小的放到最前面，因为短作业优先算法实非抢占式的，第一个来的作业必须先做完再做其他的
	temp=pcbdata[ready[0]].time_start;
	index=0;    //index为最早到达的作业所处的位置
	for(i=1;i<num;i++){
            if(pcbdata[ready[i]].time_start<temp){
                index=i;
                temp=pcbdata[ready[i]].time_start;
            }
	}
	if(index!=0){
        temp=ready[index];
        for(i=index;i>0;i--){
            ready[i]=ready[i-1];
        }
        ready[0]=temp;
	}


	//如果作业a完成后，其紧接着的作业b还没到来时，需要从队列中寻找是否有已经到达的作业，并且其服务时间最短：
	//从前往后寻找，找到符合条件的就停止,因为作业已经更加服务时间排好序了

	temp=pcbdata[ready[0]].time_start;
	for(i=1;i<num;i++){
        if(pcbdata[ready[i]].time_start>temp){  //队头的作业还没到达

            for(j=i+1;j<num;j++){   //寻找是否有已经到达的作业
                if(pcbdata[ready[j]].time_start<temp){
                    temp=ready[j];
                    for(p=j;p>i+1;p--){
                        ready[p]=ready[p-1];
                    }
                    ready[i+1]=temp;
                    break;
                }
            }

        }
	}


	printf("---短作业优先算法调度：非抢占，无时间片---\n");
	temp=pcbdata[ready[0]].time_start;
	for(i=0;i<num;i++){
			printf("第%d个进程-- %s",i+1,pcbdata[ready[i]].name);
			printf("到达时间 -- %d，服务时间 -- %d \n"
				,pcbdata[ready[i]].time_start
				,pcbdata[ready[i]].time_need);
			printf("本进程正在运行.........");
			_sleep(1);
			printf("运行完毕\n");
			temp+=pcbdata[ready[i]].time_need;	//进程完成时间
			j=temp-pcbdata[ready[i]].time_start;	//周转时间
			k=(float)j/pcbdata[ready[i]].time_need;	//带权周转时间
			printf("完成时间 -- %d，周转时间-- %d，带权周转时间 -- %.1f\n",temp,j,k);

		}
		printf("------所有的进程调度完毕-----\n");
}

void HRF(){	//高响应比算法
//以响应比为排序对象，从大到小排序，可能有多次排序
}

void Timeslice(){	//按照先来先服务并使用时间片轮转
}

void MRLA(){	//多级反馈调度队列，抢占式调度
}

void main(){
	int i=0,sch=99;
	while(sch!=0){
		printf("\n请选择其中一种调度算法：\n");
		printf("(1)先来先服务FCFS\n");
		printf("(2)短作业优先SJF\n");
		printf("(3)高响应比HRF\n");
		printf("(4)时间片轮转Timeslice\n");
		printf("(5)多级反馈队列\n");
		printf("(0)退出:\n");
		printf("请输入上述一个数字：");
		scanf("%d",&sch);
		switch(sch){
			case 1:FCFS();break;
			case 2:SJF();break;
			case 3:HRF();break;
			case 4:Timeslice();break;
			case 5:MRLA();break;
			case 0:printf("退出程序\n");break;
		}

	}
	_keygo();
}



//-----------以下函数主要用于程序调试跟踪----------
void dis_pcb(PCB *pr){
	printf("%s的PCB:\n",pr->name);
	printf("标识符-- %d,状态 -- %c,到达时间-- %d\n",pr->id,pr->state,pr->time_start);
	printf("服务时间 --%d，剩余运行时间--%d，已用时间--d%\n",pr->time_need,pr->time_left,pr->time_used);
	printf("-----------------\n");
}


void dis_pcb_all(){
	int i;
	printf("****当前进程状态****\n");
	for(i=0;i<num;i++)
		dis_pcb(&pcbdata[i]);
}

void dis_ready(){
	int i;
	printf("当前就绪队列为：");
	for(i=0;i<num-1;i++){
		printf("%s --",pcbdata[order[i]].name);
	printf("%s\n",pcbdata[order[i]].name);
	}
}
