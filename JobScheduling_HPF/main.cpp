#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;
int jobNumber;      //读取到的作业数

/*作业数据结构信息*/
struct work
{
    int number;//作业号
    int reach_time;//作业抵达时间
    int need_time;//作业的执行时间
    int privilege;//作业优先权
    float excellent;//响应比
    int start_time;//作业开始时间
    int wait_time;//等待时间
    int tr_time;//周转时间
    double wtr_time;//带权周转时间
    int run_time;//作业累计已执行时间
    int visited;//作业是否被访问过
    bool isreached;//作业是否已抵达
}w[10];

/*读取文件数据*/
void read_jobdata()
{
    int i=0;
    FILE *r;
    if((r=fopen("os1-data.txt","r")))
    {
        printf("读入成功！\n");
        printf("作业ID 到达时间 执行时间 优先级\n");
        while(fscanf(r,"%d%d%d%d",&w[i].number,&w[i].reach_time,&w[i].need_time,&w[i].privilege)!=EOF)
        {
            printf("%d\t%d\t%d\t%d\n",w[i].number,w[i].reach_time,w[i].need_time,w[i].privilege);
            i++;
        }
        jobNumber=i;
        printf("总共有%d个作业\n",jobNumber);
    }
    else
        printf("读入失败！\n");

    fclose(r);
}

/*初始化作业信息*/
void initial_jobs()
{
    int i;
    for(i=0;i<10;i++)
    {
        w[i].number=0;
        w[i].reach_time=0;
        w[i].need_time=0;
        w[i].privilege=0;
        w[i].excellent=0.0;
        w[i].start_time=0;
        w[i].wait_time=0;
        w[i].tr_time=0;
        w[i].wtr_time=0.0;
        w[i].visited=0;//标记为未被访问过
        w[i].isreached=false;
    }
}

/*作业依次按优先级、到达时间、最短作业的先后顺序排序的比较函数*/
int compare(work x,work y)
{
    if(x.privilege<y.privilege)
        return 1;
    else if(x.privilege==y.privilege)
    {
        if(x.reach_time<y.reach_time)
            return 1;
        else if(x.reach_time==y.reach_time)
        {
            if(x.need_time<y.need_time)
                return 1;
            else return 0;
        }
        else return 0;
    }
    else return 0;
}

/*将作业执行信息输出*/
void output(work *w)
{
    int i=0;
    w[0].start_time=w[0].reach_time;
    w[0].isreached=true;
    w[0].wait_time=0;
    w[0].tr_time=w[0].need_time+w[0].wait_time;
    w[0].wtr_time=w[0].tr_time*1.0/w[0].need_time;
    cout<<"作业的执行顺序如下："<<endl;
    cout<<"执行完的作业是："<<w[i].number<<",等待时间为"<<w[i].wait_time<<" 周转时间为"<<w[i].tr_time<<" 带权周转时间为"<<w[i].wtr_time<<endl;
    for(i=1;i<jobNumber;i++)
    {
        if(w[i].reach_time<w[i-1].start_time+w[i-1].need_time)
            w[i].start_time=w[i-1].start_time+w[i-1].need_time;
        else
            w[i].start_time=w[i].reach_time;
        w[i].wait_time=w[i].start_time-w[i].reach_time;
        w[i].tr_time=w[i].wait_time+w[i].need_time;
        w[i].wtr_time=w[i].tr_time*1.0/w[i].need_time;
        cout<<"执行完的作业是："<<w[i].number<<",等待时间为"<<w[i].wait_time<<" 周转时间为"<<w[i].tr_time<<" 带权周转时间为"<<w[i].wtr_time<<endl;
    }
}

/*优先级高者优先算法*/
void HPF(work*w)
{
    sort(w,w+jobNumber,compare);//按照优先级、到达时间、最短作业的先后顺序将作业进行排序
    output(w);
}



int main()
{
    initial_jobs();
    read_jobdata();
    HPF(w);
    return 0;
}
