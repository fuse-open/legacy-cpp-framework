#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#undef CreateMutex

// From: http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

static void SetThreadName(DWORD dwThreadID, const char* threadName)
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;

    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
}

#include <uBase/Thread.h>
#include <uBase/Console.h>

namespace uBase
{
    Cond* CreateCond()
    {
        CONDITION_VARIABLE* cv = new CONDITION_VARIABLE;
        InitializeConditionVariable(cv);
        return (Cond*)cv;
    }

    void DeleteCond(Cond* handle)
    {
        delete (CONDITION_VARIABLE*)handle;
    }

    void CondWait(Cond* handle, Mutex* mutex)
    {
        SleepConditionVariableCS((CONDITION_VARIABLE*)handle, (CRITICAL_SECTION*)mutex, INFINITE);
    }

    void CondSignal(Cond* handle)
    {
        WakeConditionVariable((CONDITION_VARIABLE*)handle);
    }

    void CondBroadcast(Cond* handle)
    {
        WakeAllConditionVariable((CONDITION_VARIABLE*)handle);
    }

    Mutex* CreateMutex()
    {
        CRITICAL_SECTION* cs = new CRITICAL_SECTION;
        InitializeCriticalSection(cs);
        return (Mutex*)cs;
    }

    void DeleteMutex(Mutex* mutex)
    {
        DeleteCriticalSection((CRITICAL_SECTION*)mutex);
        delete (CRITICAL_SECTION*)mutex;
    }

    void LockMutex(Mutex* mutex)
    {
        EnterCriticalSection((CRITICAL_SECTION*)mutex);
    }

    bool TryLockMutex(Mutex* mutex)
    {
        return TryEnterCriticalSection((CRITICAL_SECTION*)mutex) == TRUE;
    }

    void UnlockMutex(Mutex* mutex)
    {
        LeaveCriticalSection((CRITICAL_SECTION*)mutex);
    }

    struct Win32ThreadData
    {
        void (*Entrypoint)(void*);
        void* Arg;
    };

    unsigned int __stdcall Win32ThreadFunc(void* data)
    {
        Win32ThreadData* win32Data = reinterpret_cast<Win32ThreadData*>(data);
        win32Data->Entrypoint(win32Data->Arg);
        delete win32Data;
        return 0;
    }

    Thread* CreateThread(void (*entrypoint)(void*), void* arg)
    {
        Win32ThreadData* data = new Win32ThreadData;
        data->Entrypoint = entrypoint;
        data->Arg = arg;

        unsigned threadId;
        HANDLE thread = (HANDLE)_beginthreadex(NULL, 0, &Win32ThreadFunc, data, 0, &threadId);

        if (!thread)
            throw Exception("Failed to create thread");

        return (Thread*)thread;
    }

    void JoinThread(Thread* handle)
    {
        DWORD res = ::WaitForSingleObject((HANDLE)handle, INFINITE);

        if (res != WAIT_OBJECT_0)
            Error->WriteLine("WaitForSingleObject() failed");
        else
            ::CloseHandle((HANDLE)handle);
    }

    void SetThreadName(const String& name)
    {
        ::SetThreadName(-1, name.Ptr());
    }

    Thread* GetThread()
    {
        return (Thread*)::GetCurrentThread();
    }

    void Sleep(int ms)
    {
        ::Sleep(ms);
    }

    ThreadLocal* CreateThreadLocal(void (*destructor)(void*))
    {
        return (ThreadLocal*)::TlsAlloc();
    }

    void DeleteThreadLocal(ThreadLocal* handle)
    {
        ::TlsFree((DWORD)handle);
    }

    void SetThreadLocal(ThreadLocal* handle, void* data)
    {
        ::TlsSetValue((DWORD)handle, data);
    }

    void* GetThreadLocal(ThreadLocal* handle)
    {
        return ::TlsGetValue((DWORD)handle);
    }
}
