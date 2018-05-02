using System.Runtime.InteropServices;

namespace Uno.Native.Fonts
{
    [StructLayout(LayoutKind.Sequential)]
    struct FontGlyphInfo
    {
        public float AdvanceX;
        public float AdvanceY;
        public float BearingX;
        public float BearingY;
        public int BitmapWidth;
        public int BitmapHeight;
        public int PixelFormat;
    }
}