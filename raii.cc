#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

CRITICAL_SECTION cs;
int gGlobal = 0;

class MyLock
{
public:
    MyLock()
    {
        EnterCriticalSection(&cs);
    }

    ~MyLock()
    {
        LeaveCriticalSection(&cs);
    }
    MyLock(const MyLock&) = delete;
    MyLock operator =(const MyLock&) = delete;
};

void DoComplex(MyLock& lock)
{
}

unsigned int __stdcall ThreadFun(PVOID pv)
{
    // 利用lock变量构造加锁，析构解锁
    MyLock lock;
    int* para = (int*)pv;

    // 业务代码.......

    DoComplex(lock);

    for (int i = 0; i < 10; ++i)
    {
        ++gGlobal;
        cout << "Thread " << *para << endl;
        cout << gGlobal << endl;
    }
    return 0;
}

int main()
{
    InitializeCriticalSection(&cs);

    int thread1, thread2;
    thread1 = 1;
    thread2 = 2;

    HANDLE handle[2];
    handle[0] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFun, (void*)&thread1, 0, nullptr);
    handle[1] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFun, (void*)&thread2, 0, nullptr);
    WaitForMultipleObjects(2, handle, TRUE, INFINITE);
    return 0;
}
