using System;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Uno.Native
{
    public class NativeObject : IDisposable
    {
        internal IntPtr Handle { get; private set; }
        public bool IsDisposed { get; private set; }

        internal NativeObject()
        {
            if (!NativeLib.IsInitialized)
                NativeLib.Initialize();

            Handle = NativeLib.uCreate();
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!IsDisposed)
            {
                if (disposing)
                    NativeLib.uRelease(Handle);

                Handle = IntPtr.Zero;
                IsDisposed = true;
            }
        }

        internal string GetStringValue(IntPtr ptr)
        {
            return Marshal.PtrToStringUni(ptr);
        }

        internal void CheckError(int result)
        {
            if (result != 0)
                throw new NativeException(GetStringValue(NativeLib.uGetErrorString(Handle)));
        }
    }
}
