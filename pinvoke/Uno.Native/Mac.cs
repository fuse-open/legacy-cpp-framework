using System;
using System.Runtime.InteropServices;

namespace Uno.Native
{
    public static class Mac
    {
        [DllImport("/usr/lib/libSystem.dylib")]
        public static extern IntPtr dlopen(string path, int mode);

        [DllImport("/usr/lib/libSystem.dylib")]
        public static extern IntPtr dlsym(IntPtr handle, string symbol);
    }
}