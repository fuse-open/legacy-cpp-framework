using System.Runtime.InteropServices;

namespace Uno.Native.Textures
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct TextureInfo
    {
        public int Width;
        public int Height;
        public int Depth;
        public int FaceCount;
        public int MipCount;
        [MarshalAs(UnmanagedType.I4)]
        public TextureType TextureType;
        [MarshalAs(UnmanagedType.I4)]
        public PixelFormat PixelFormat;
    }
}