#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;
int jobNumber;      //��ȡ������ҵ��

/*��ҵ���ݽṹ��Ϣ*/
struct work
{
    int number;//��ҵ��
    int reach_time;//��ҵ�ִ�ʱ��
    int need_time;//��ҵ��ִ��ʱ��
    int privilege;//��ҵ����Ȩ
    float excellent;//��Ӧ��
    int start_time;//��ҵ��ʼʱ��
    int wait_time;//�ȴ�ʱ��
    int tr_time;//��תʱ��
    double wtr_time;//��Ȩ��תʱ��
    int run_time;//��ҵ�ۼ���ִ��ʱ��
    int visited;//��ҵ�Ƿ񱻷��ʹ�
    bool isreached;//��ҵ�Ƿ��ѵִ�
}w[10];

/*��ȡ�ļ�����*/
void read_jobdata()
{
    int i=0;
    FILE *r;
    if((r=fopen("os1-data.txt","r")))
    {
        printf("����ɹ���\n");
        printf("��ҵID ����ʱ�� ִ��ʱ�� ���ȼ�\n");
        while(fscanf(r,"%d%d%d%d",&w[i].number,&w[i].reach_time,&w[i].need_time,&w[i].privilege)!=EOF)
        {
            printf("%d\t%d\t%d\t%d\n",w[i].number,w[i].reach_time,w[i].need_time,w[i].privilege);
            i++;
        }
        jobNumber=i;
        printf("�ܹ���%d����ҵ\n",jobNumber);
    }
    else
        printf("����ʧ�ܣ�\n");

    fclose(r);
}

/*��ʼ����ҵ��Ϣ*/
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
        w[i].visited=0;//���Ϊδ�����ʹ�
        w[i].isreached=false;
    }
}

/*��ҵ���ΰ����ȼ�������ʱ�䡢�����ҵ���Ⱥ�˳������ıȽϺ���*/
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

/*����ҵִ����Ϣ���*/
void output(work *w)
{
    int i=0;
    w[0].start_time=w[0].reach_time;
    w[0].isreached=true;
    w[0].wait_time=0;
    w[0].tr_time=w[0].need_time+w[0].wait_time;
    w[0].wtr_time=w[0].tr_time*1.0/w[0].need_time;
    cout<<"��ҵ��ִ��˳�����£�"<<endl;
    cout<<"ִ�������ҵ�ǣ�"<<w[i].number<<",�ȴ�ʱ��Ϊ"<<w[i].wait_time<<" ��תʱ��Ϊ"<<w[i].tr_time<<" ��Ȩ��תʱ��Ϊ"<<w[i].wtr_time<<endl;
    for(i=1;i<jobNumber;i++)
    {
        if(w[i].reach_time<w[i-1].start_time+w[i-1].need_time)
            w[i].start_time=w[i-1].start_time+w[i-1].need_time;
        else
            w[i].start_time=w[i].reach_time;
        w[i].wait_time=w[i].start_time-w[i].reach_time;
        w[i].tr_time=w[i].wait_time+w[i].need_time;
        w[i].wtr_time=w[i].tr_time*1.0/w[i].need_time;
        cout<<"ִ�������ҵ�ǣ�"<<w[i].number<<",�ȴ�ʱ��Ϊ"<<w[i].wait_time<<" ��תʱ��Ϊ"<<w[i].tr_time<<" ��Ȩ��תʱ��Ϊ"<<w[i].wtr_time<<endl;
    }
}

/*���ȼ����������㷨*/
void HPF(work*w)
{
    sort(w,w+jobNumber,compare);//�������ȼ�������ʱ�䡢�����ҵ���Ⱥ�˳����ҵ��������
    output(w);
}



int main()
{
    initial_jobs();
    read_jobdata();
    HPF(w);
    return 0;
}
