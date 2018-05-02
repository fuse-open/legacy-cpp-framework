using System;
using System.Runtime.InteropServices;

namespace Uno.Native.Fonts
{
    [StructLayout(LayoutKind.Sequential)]
    struct FontFaceInfo
    {
        public IntPtr FamilyName;
        public IntPtr StyleName;
    }
}