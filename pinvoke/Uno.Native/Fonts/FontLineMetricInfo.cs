using System.Runtime.InteropServices;

namespace Uno.Native.Fonts
{
    [StructLayout(LayoutKind.Sequential)]
    struct FontLineMetricInfo
    {
        public float Ascender;
        public float Descender;
        public float LineHeight;
    }
}