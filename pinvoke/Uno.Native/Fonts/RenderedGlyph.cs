namespace Uno.Native.Fonts
{
    public struct RenderedGlyph
    {
        public float AdvanceX, AdvanceY;
        public float BearingX, BearingY;
        public Textures.PixelFormat PixelFormat;
        public int Width, Height;
        public byte[] Bitmap;
    }
}