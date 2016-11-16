#include <stdio.h>
#include <Windows.h>

#define N 5 //设置哲学家总人数
#define LEFT (i+N-1)%N //左边的哲学家的位置
#define RIGHT (i+1)%N //右边的哲学家的位置
#define THINKING 0  //思考状态
#define HUNGRY 1    //饥饿尝试进餐状态
#define EATING 2       //进餐状态
#define  MAX_SEM_COUNT 1 //设置最大信号量

HANDLE ghSemaphore;//声明信号量句柄数组
HANDLE SemPhliosopher[N]; //声明哲学家信号量数组
HANDLE aThread[N];//声明线程数组
int state[N];//声明哲学家状态记录数组

CRITICAL_SECTION cs;

VOID take_forks(int i); //取筷子
VOID put_forks(int i);  //放筷子
VOID tryEat(int i);     //试吃

VOID WINAPI PhilosopherProc(LPARAM lpParam)
{
    INT PhilosopherNo = (INT)lpParam;
    while(TRUE)
    {
        take_forks(PhilosopherNo);
        put_forks(PhilosopherNo);
    }
}

VOID take_forks(int i)
{
    WaitForSingleObject(ghSemaphore,INFINITE);
    state[i] = HUNGRY;
    tryEat(i);
    WaitForSingleObject(SemPhliosopher[i],INFINITE);
}

VOID put_forks(int i)
{
    state[i] = THINKING;
    printf("哲学家%d开始思考.\n\n",i+1);
    tryEat(LEFT);
    tryEat(RIGHT);
    ReleaseSemaphore(ghSemaphore,MAX_SEM_COUNT,NULL);

}

VOID tryEat(int i)
{
    printf("哲学家%d尝试拿筷子\n",i+1);
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] == EATING;
        printf("哲学家%d正在进餐.\n\n",i+1);
        Sleep(1000);
        ReleaseSemaphore(SemPhliosopher[i],MAX_SEM_COUNT,NULL);
    }
    else
    {
        printf("还没有筷子噢！(⊙o⊙)\n\n");
    }
}
int main(int argc,char * argv[])
{
    INT index = 0;
    DWORD dwThreadID[N];
    ghSemaphore = CreateSemaphore(NULL,2,2,TEXT("Sem"));//初始和最大信号量均设置为2可以使2人同时就餐
    if(ghSemaphore == NULL)
    {
        printf("创建信号量失败!\n");
        return -1;
    }

    for(index=0;index<N;index++)
    {
        SemPhliosopher[index] = CreateSemaphore(NULL,MAX_SEM_COUNT,MAX_SEM_COUNT,NULL);
        if(SemPhliosopher[index] == NULL)
        {
            printf("ERROR:%d\n",GetLastError());
            return -1;
        }
    }

    for(index=0;index<N;index++)//为每个哲学家创建一个线程
    {
        aThread[index] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)PhilosopherProc,(LPVOID)index,0,&dwThreadID[index]);
        if(aThread[index] == NULL)
        {
            printf("ERROR:%d\n",GetLastError());
            return -1;
        }
    }

    WaitForMultipleObjects(N,aThread,TRUE,5000);

    CloseHandle(ghSemaphore);

    for(index=0;index<N;index++)
    {
        CloseHandle(SemPhliosopher[index]);
        CloseHandle(ghSemaphore);
        CloseHandle(aThread[index]);
    }

    return 0;
}
