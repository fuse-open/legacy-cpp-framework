using System.Runtime.InteropServices;

namespace Uno.Native.Fonts
{
    [StructLayout(LayoutKind.Sequential)]
    struct FontKerningInfo
    {
        public float KerningX;
        public float KerningY;
    }
}