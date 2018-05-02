#include <uBase/Console.h>
#include <uBase/BaseLib.h>
#include <uBase/Bundle.h>
#include <uBase/Disk.h>
#include <uBase/File.h>
#include <uBase/Thread.h>
#include <cstdlib>

namespace uBase
{
    BundleAccessor Bundle;
    DiskAccessor Disk;
    StdOutAccessor Out;
    StdErrAccessor Error;
    StdInAccessor In;

    FileSystem* CreateBundleFileSystem();
    NativeFileSystem* CreateNativeFileSystem();

    static FileSystem* as;
    static NativeFileSystem* fs;
    static void(*ExceptionCallback)(const Exception&, const String&);
    static TextWriter* OutWriter;
    static TextWriter* ErrWriter;
    static TextReader* InReader;
    static Mutex* LibMutex;
    static bool LibInited;

    static void Terminate()
    {
        as->Release();
        fs->Release();
        OutWriter->GetStream()->Flush();
        ErrWriter->GetStream()->Flush();
        delete OutWriter;
        delete ErrWriter;
        delete InReader;
        DeleteMutex(LibMutex);
        LibInited = false;
        OutWriter = NULL;
        ErrWriter = NULL;
        InReader = NULL;
        as = fs = NULL;
    }

    void BaseLib::Init()
    {
        if (!LibInited)
        {
            LibInited = true;
            LibMutex = CreateMutex();
            OutWriter = new TextWriter(AutoPtr(new File(stdout, FileFlagsCanWrite | FileFlagsIgnoreWriteErrors)));
            ErrWriter = new TextWriter(AutoPtr(new File(stderr, FileFlagsCanWrite | FileFlagsIgnoreWriteErrors)));
            InReader = new TextReader(AutoPtr(new File(stdin, FileFlagsCanRead)));
            fs = CreateNativeFileSystem();
            as = CreateBundleFileSystem();
            atexit(Terminate);
        }
    }

    void BaseLib::SetUnhandledExceptionCallback(void(*callback)(const Exception&, const String&))
    {
        ExceptionCallback = callback;
    }

    void BaseLib::OnUnhandledException(const Exception& exception, const String& where)
    {
        Error->WriteLine();
        Error->WriteLine((String)"UNHANDLED EXCEPTION (" + where + "):");
        Error->WriteLine();
        Error->WriteLine(exception.GetMessage());
        Error->WriteLine();
        Error->WriteLine((String)"Function: " + exception.GetFunction());
        Error->WriteLine((String)"Line: " + exception.GetLine());
        Error->GetStream()->Flush();

        if (ExceptionCallback)
            ExceptionCallback(exception, where);
    }

    bool BaseLib::EnterCriticalIfFalse(bool* flag)
    {
        if (*flag)
            return false;

        BaseLib::Init();
        LockMutex(LibMutex);

        if (*flag)
        {
            UnlockMutex(LibMutex);
            return false;
        }

        return *flag = true;
    }

    void BaseLib::EnterCritical()
    {
        LockMutex(LibMutex);
    }

    void BaseLib::ExitCritical()
    {
        UnlockMutex(LibMutex);
    }

    void BundleAccessor::SetFilesystem(FileSystem* fs)
    {
        BaseLib::Init();
        as->Release();
        fs->Retain();
        as = fs;
    }

    FileSystem* BundleAccessor::operator ->()
    {
        BaseLib::Init();
        return as;
    }

    BundleAccessor::operator FileSystem*()
    {
        BaseLib::Init();
        return as;
    }

    NativeFileSystem* DiskAccessor::operator ->()
    {
        BaseLib::Init();
        return fs;
    }

    DiskAccessor::operator FileSystem*()
    {
        BaseLib::Init();
        return fs;
    }

    TextWriter* StdOutAccessor::operator ->()
    {
        BaseLib::Init();
        return OutWriter;
    }

    StdOutAccessor::operator TextWriter*()
    {
        BaseLib::Init();
        return OutWriter;
    }

    TextWriter* StdErrAccessor::operator ->()
    {
        BaseLib::Init();
        return ErrWriter;
    }

    StdErrAccessor::operator TextWriter*()
    {
        BaseLib::Init();
        return ErrWriter;
    }

    TextReader* StdInAccessor::operator ->()
    {
        BaseLib::Init();
        return InReader;
    }

    StdInAccessor::operator TextReader*()
    {
        BaseLib::Init();
        return InReader;
    }
}
