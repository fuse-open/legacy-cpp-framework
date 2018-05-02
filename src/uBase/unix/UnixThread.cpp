#include <uBase/Console.h>
#include <uBase/Thread.h>
#include <pthread.h>
#include <time.h>
#include <cstdlib>
#include <inttypes.h>

namespace uBase
{
    Cond* CreateCond()
    {
        pthread_cond_t* handle = new pthread_cond_t;
        pthread_cond_init(handle, NULL);
        return (Cond*)handle;
    }

    void DeleteCond(Cond* handle)
    {
        pthread_cond_destroy((pthread_cond_t*)handle);
        delete (pthread_cond_t*)handle;
    }

    void CondWait(Cond* handle, Mutex* mutex)
    {
        pthread_cond_wait((pthread_cond_t*)handle, (pthread_mutex_t*)mutex);
    }

    void CondSignal(Cond* handle)
    {
        pthread_cond_signal((pthread_cond_t*)handle);
    }

    void CondBroadcast(Cond* handle)
    {
        pthread_cond_broadcast((pthread_cond_t*)handle);
    }

    Mutex* CreateMutex()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

        pthread_mutex_t* handle = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(handle, &attr);
        return (Mutex*)handle;
    }

    void DeleteMutex(Mutex* handle)
    {
        pthread_mutex_destroy((pthread_mutex_t*)handle);
        free(handle);
    }

    void LockMutex(Mutex* handle)
    {
        pthread_mutex_lock((pthread_mutex_t*)handle);
    }

    bool TryLockMutex(Mutex* handle)
    {
        return pthread_mutex_trylock((pthread_mutex_t*)handle) == 0;
    }

    void UnlockMutex(Mutex* handle)
    {
        pthread_mutex_unlock((pthread_mutex_t*)handle);
    }

    struct ThreadData
    {
        void (*Entrypoint)(void*);
        void* Arg;
    };

    void* ThreadFunc(void* arg)
    {
        ThreadData* data = (ThreadData*)arg;
        data->Entrypoint(data->Arg);
        free(data);
        return NULL;
    }

    Thread* CreateThread(void (*entrypoint)(void*), void* arg)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        ThreadData* data = (ThreadData*)malloc(sizeof(ThreadData));
        data->Entrypoint = entrypoint;
        data->Arg = arg;

        pthread_t handle;
        if (pthread_create(&handle, &attr, ThreadFunc, data))
            throw Exception("pthread_create() failed");

        pthread_attr_destroy(&attr);
        return (Thread*)handle;
    }

    void JoinThread(Thread* handle)
    {
        int result = pthread_join((pthread_t)handle, NULL);

        if (result)
            Error->WriteLine("pthread_join() failed: " + String::HexFromInt(result));
    }

    void SetThreadName(const String& name)
    {
#ifdef __APPLE__
        pthread_setname_np(name.Ptr());
#else
        pthread_setname_np(pthread_self(), name.Ptr());
#endif
    }

    Thread* GetThread()
    {
        return (Thread*)pthread_self();
    }

    void Sleep(int ms)
    {
        struct timespec t, r;
        t.tv_sec = ms / 1000;
        t.tv_nsec = (ms % 1000) * 1000 * 1000;

        int result = nanosleep(&t, &r);

        if (result)
            Error->WriteLine("nanosleep() failed: " + String::HexFromInt(result));
    }

    ThreadLocal* CreateThreadLocal(void (*destructor)(void*))
    {
        pthread_key_t handle;
        if (pthread_key_create(&handle, destructor))
            throw Exception("pthread_key_create() failed");

        return (ThreadLocal*)(intptr_t)handle;
    }

    void DeleteThreadLocal(ThreadLocal* handle)
    {
        pthread_key_delete((pthread_key_t)(intptr_t)handle);
    }

    void SetThreadLocal(ThreadLocal* handle, void* data)
    {
        pthread_setspecific((pthread_key_t)(intptr_t)handle, data);
    }

    void* GetThreadLocal(ThreadLocal* handle)
    {
        return pthread_getspecific((pthread_key_t)(intptr_t)handle);
    }
}
