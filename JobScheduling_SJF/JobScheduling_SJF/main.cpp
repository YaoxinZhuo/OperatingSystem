//
//  main.cpp
//  JobScheduling_SJF
//
//  Created by YaoxinZhuo on 02/11/2016.
//  Copyright © 2016 YaoxinZhuo. All rights reserved.
//

#include <iostream>
#define jobNumber (8)//jobNumber为任务数+1
#define route ("/users/yaoxinzhuo/Desktop/OperatingSystem/jobs.txt")//文件路径

int sjfOrder[jobNumber];
int fakeTime = 0;//排序时候的虚拟时间

typedef struct node* pNode;
typedef struct node
{
    int number;         //作业号
    int reach_time;     //作业抵达时间
    int need_time;      //作业的执行时间
    int privilege;	  //作业优先权
    float excellent;   //响应比
    int start_time;     //作业开始时间
    int wait_time;      //等待时间
    int tr_time;        //周转时间
    double wtr_time;    //带权周转时间
    int run_time;       //作业累计已执行时间
    int visited;		//作业是否被访问过
    bool isreached;	//作业是否已抵达
}job;
typedef pNode Job;
node jobs[jobNumber];

void initial_jobs()
{
    for(int i = 1;i < jobNumber;i ++)
    {
        jobs[i].excellent = 0.0;
        jobs[i].start_time = 0;
        jobs[i].wait_time = 0;
        jobs[i].tr_time = 0;
        jobs[i].wtr_time = 0.0;
        jobs[i].run_time = 0;
        jobs[i].visited = 0;
        jobs[i].isreached = false;
    }
}

void read_Jobdata()
{
    FILE *fp;
    if((fp=fopen(route, "r")))
    {
        printf("读入成功！\n");
        initial_jobs();
        printf("作业的数量：%d\n",jobNumber-1);
        for(int i = 1;i < jobNumber;i ++)
        {
            fscanf(fp, "%d %d %d %d\n",&jobs[i].number,&jobs[i].reach_time,&jobs[i].need_time,&jobs[i].privilege);
            printf("%d %d %d %d\n",jobs[i].number,jobs[i].reach_time,jobs[i].need_time,jobs[i].privilege);
        }
    }
    else
    {
        printf("读入失败！\n");
    }
    fclose(fp);
}

void initialVisited()//全部初始化为未访问过
{
    for(int i = 1;i < jobNumber;i ++)
    {
        jobs[i].visited = false;
    }
}

int findFirstInJob()//每次返回没有找到过的reach最早的job顺序，加入到order中
{
    int result = -1;
    while(result == -1)
    {
        for(int i = 1;i < jobNumber; i ++)//第一个没有处理过的任务
        {
            if(jobs[i].visited == false && jobs[i].reach_time <= fakeTime)
            {
                result = i;
            }
        }
        
        if(result == -1)//没有找到当前时间有到达的任务
        {
            fakeTime++;//时间在流逝
        }
        else//为防万一遍历一遍看有没有同时到达时间更短的任务
        {
            for(int i = 1;i < jobNumber && i!= result;i ++)
            {
                if(jobs[i].visited == false && (jobs[i].reach_time <= fakeTime) && (jobs[i].need_time < jobs[result].need_time) )
                {
                    result = i;
                }
            }
            fakeTime += jobs[result].need_time;
            jobs[result].visited = true;
            break;
        }
        
    }
    
    return result;
}

void shorestJobFirstSort()
{
    int k = 1;
    initialVisited();
    for(int i = 1;i < jobNumber;i++)
    {
        sjfOrder[k++] = findFirstInJob();
    }
}

void shorestJobFirst()
{
    shorestJobFirstSort();
    initialVisited();//得到order后初始化都未未访问的
    printf("目前fifs任务顺序:\n");
    for(int i = 1;i < jobNumber;i++)
    {
        printf("%d ",sjfOrder[i]);
    }
    printf("\n");
    
    int startTime = jobs[sjfOrder[1]].reach_time;
    int currentTime = startTime;
    
    //第一个任务处理
    jobs[sjfOrder[1]].wait_time = 0;
    currentTime = startTime + jobs[sjfOrder[1]].need_time;//run this job
    jobs[sjfOrder[1]].tr_time = currentTime - jobs[sjfOrder[1]].reach_time;
    jobs[sjfOrder[1]].wtr_time = (jobs[sjfOrder[1]].tr_time+jobs[sjfOrder[1]].wait_time)/(jobs[sjfOrder[1]].tr_time);
    
    //其余任务处理
    for(int i = 2;i < jobNumber; i++)
    {
        if(currentTime - jobs[sjfOrder[i]].reach_time > 0)
        {
            jobs[sjfOrder[i]].wait_time = (currentTime - jobs[sjfOrder[i]].reach_time);
        }
        else
        {
            currentTime = jobs[sjfOrder[i]].reach_time;//若这个进程没有等待则重制当前时间
            jobs[sjfOrder[i]].wait_time = 0;
        }
        currentTime = currentTime + jobs[sjfOrder[i]].need_time; //run this job
        jobs[sjfOrder[i]].tr_time = (currentTime - jobs[sjfOrder[i]].reach_time);
        jobs[sjfOrder[i]].wtr_time = 1.0*(jobs[sjfOrder[i]].tr_time + jobs[sjfOrder[i]].wait_time)/(1.0*jobs[sjfOrder[i]].tr_time);
    }
    
    for(int i = 1;i < jobNumber;i++)
    {
        printf("执行完的作业:%d作业 \t等待时间%d \t周转时间%d \t带权周转时间:%.2f \n",sjfOrder[i],jobs[sjfOrder[i]].wait_time,jobs[sjfOrder[i]].tr_time,jobs[sjfOrder[i]].wtr_time);
    }
    
}

void findMinStartTime()
{
    fakeTime = jobs[1].reach_time;
    for(int i = 1;i < jobNumber;i++)
    {
        if(jobs[i].reach_time < fakeTime)
        {
            fakeTime = jobs[i].reach_time;
        }
    }
}

int main(int argc, const char * argv[]) {
    
    read_Jobdata();
    
    findMinStartTime();//生成队列用的时间，就是第一个任务开始时间需要确定
    
    shorestJobFirst();
    
    return 0;
}
