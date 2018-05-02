#pragma once

namespace uBase
{
    class String;

    /** \addtogroup uThread */
    /** @{ */
    struct Cond;
    struct Mutex;
    struct Thread;
    struct ThreadLocal;

    Cond* CreateCond();
    void DeleteCond(Cond* cond);

    void CondWait(Cond* cond, Mutex* mutex);
    void CondSignal(Cond* cond);
    void CondBroadcast(Cond* cond);

    Mutex* CreateMutex();
    void DeleteMutex(Mutex* mutex);

    void LockMutex(Mutex* mutex);
    bool TryLockMutex(Mutex* mutex);
    void UnlockMutex(Mutex* mutex);

    Thread* CreateThread(void (*entrypoint)(void*), void* arg);
    void JoinThread(Thread* thread);

    void SetThreadName(const String& name);
    Thread* GetThread();

    void Sleep(int ms);

    ThreadLocal* CreateThreadLocal(void (*destructor)(void*));
    void DeleteThreadLocal(ThreadLocal* tls);

    void SetThreadLocal(ThreadLocal* tls, void* value);
    void* GetThreadLocal(ThreadLocal* tls);
    /** @} */

    /** \ingroup uThread */
    class MutexLock
    {
        Mutex* _mutex;
        MutexLock(const MutexLock& copy);
        MutexLock& operator =(const MutexLock& copy);
    public:
        MutexLock(Mutex* mutex) { LockMutex(_mutex = mutex); }
        ~MutexLock() { UnlockMutex(_mutex); }
    };
}
