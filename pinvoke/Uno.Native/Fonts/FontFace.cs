using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

namespace Uno.Native.Fonts
{
    public class FontFace : NativeObject
    {
        public string FamilyName
        {
            get;
            private set;
        }

        public string StyleName
        {
            get;
            private set;
        }

        public FontFace(string filename)
        {
            FontFaceInfo result;
            CheckError(NativeLib.uLoadFontFace(Handle, filename, out result));

            FamilyName = GetStringValue(result.FamilyName);
            StyleName = GetStringValue(result.StyleName);
        }

        public FontFace(byte[] bytes)
        {
            var ptr = GCHandle.Alloc(bytes, GCHandleType.Pinned);

            try
            {
                FontFaceInfo result;
                CheckError(NativeLib.uLoadFontFaceFromMemory(Handle, ptr.AddrOfPinnedObject(), bytes.Length, out result));

                FamilyName = GetStringValue(result.FamilyName);
                StyleName = GetStringValue(result.StyleName);
            }
            finally
            {
                ptr.Free();
            }
        }

        public FontFace(Stream stream)
            : this(stream.ReadAllBytes())
        {
        }

        public float GetAscender(float pixelSize)
        {
            FontLineMetricInfo result;
            CheckError(NativeLib.uFontFace_GetLineMetrics(Handle, pixelSize, out result));
            return result.Ascender;
        }

        public float GetDescender(float pixelSize)
        {
            FontLineMetricInfo result;
            CheckError(NativeLib.uFontFace_GetLineMetrics(Handle, pixelSize, out result));
            return result.Descender;
        }

        public float GetLineHeight(float pixelSize)
        {
            FontLineMetricInfo result;
            CheckError(NativeLib.uFontFace_GetLineMetrics(Handle, pixelSize, out result));
            return result.LineHeight;
        }

        public bool ContainsGlyph(float pixelSize, char glyph)
        {
            int result;
            CheckError(NativeLib.uFontFace_ContainsGlyph(Handle, pixelSize, glyph, out result));
            return result != 0;
        }

        public RenderedGlyph RenderGlyph(float pixelSize, char glyph, FontRenderMode mode)
        {
            FontGlyphInfo result;
            CheckError(NativeLib.uFontFace_RenderGlyph(Handle, pixelSize, glyph, mode, out result));

            // TODO: Also use size of PixelFormat
            var bitmap = new byte[result.BitmapWidth * result.BitmapHeight];
            CheckError(NativeLib.uFontFace_ReadGlyphBitmap(Handle, bitmap, bitmap.Length));

            return new RenderedGlyph
            {
                AdvanceX = result.AdvanceX,
                AdvanceY = result.AdvanceY,
                BearingX = result.BearingX,
                BearingY = result.BearingY,
                Width = result.BitmapWidth,
                Height = result.BitmapHeight,
                PixelFormat = (Textures.PixelFormat)result.PixelFormat,
                Bitmap = bitmap
            };
        }

        public bool TryGetKerning(float pixelSize, char left, char right, out float kerningX, out float kerningY)
        {
            FontKerningInfo result;
            CheckError(NativeLib.uFontFace_GetKerning(Handle, pixelSize, left, right, out result));
            kerningX = result.KerningX;
            kerningY = result.KerningY;
            return kerningX != 0 || kerningY != 0;
        }
    }
}
