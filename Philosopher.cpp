#include <stdio.h>
#include <Windows.h>

#define N 5 //������ѧ��������
#define LEFT (i+N-1)%N //��ߵ���ѧ�ҵ�λ��
#define RIGHT (i+1)%N //�ұߵ���ѧ�ҵ�λ��
#define THINKING 0  //˼��״̬
#define HUNGRY 1    //�������Խ���״̬
#define EATING 2       //����״̬
#define  MAX_SEM_COUNT 1 //��������ź���

HANDLE ghSemaphore;//�����ź����������
HANDLE SemPhliosopher[N]; //������ѧ���ź�������
HANDLE aThread[N];//�����߳�����
int state[N];//������ѧ��״̬��¼����

CRITICAL_SECTION cs;

VOID take_forks(int i); //ȡ����
VOID put_forks(int i);  //�ſ���
VOID tryEat(int i);     //�Գ�

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
    printf("��ѧ��%d��ʼ˼��.\n\n",i+1);
    tryEat(LEFT);
    tryEat(RIGHT);
    ReleaseSemaphore(ghSemaphore,MAX_SEM_COUNT,NULL);

}

VOID tryEat(int i)
{
    printf("��ѧ��%d�����ÿ���\n",i+1);
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] == EATING;
        printf("��ѧ��%d���ڽ���.\n\n",i+1);
        Sleep(1000);
        ReleaseSemaphore(SemPhliosopher[i],MAX_SEM_COUNT,NULL);
    }
    else
    {
        printf("��û�п����ޣ�(��o��)\n\n");
    }
}
int main(int argc,char * argv[])
{
    INT index = 0;
    DWORD dwThreadID[N];
    ghSemaphore = CreateSemaphore(NULL,2,2,TEXT("Sem"));//��ʼ������ź���������Ϊ2����ʹ2��ͬʱ�Ͳ�
    if(ghSemaphore == NULL)
    {
        printf("�����ź���ʧ��!\n");
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

    for(index=0;index<N;index++)//Ϊÿ����ѧ�Ҵ���һ���߳�
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
