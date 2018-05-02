using System;
using System.Reflection;
using System.Runtime.InteropServices;
using Uno.Native.Fonts;
using Uno.Native.Textures;

namespace Uno.Native
{
    // Generated using gen.sh
    public static partial class NativeLib
    {
        static void LoaduPInvoke(IntPtr dll)
        {
            uInit = LoadFunction<uInit_>(dll, "_uInit@0");
            uCreate = LoadFunction<uCreate_>(dll, "_uCreate@0");
            uRelease = LoadFunction<uRelease_>(dll, "_uRelease@4");
            uGetErrorString = LoadFunction<uGetErrorString_>(dll, "_uGetErrorString@4");
            uLoadFontFace = LoadFunction<uLoadFontFace_>(dll, "_uLoadFontFace@12");
            uLoadFontFaceFromMemory = LoadFunction<uLoadFontFaceFromMemory_>(dll, "_uLoadFontFaceFromMemory@16");
            uFontFace_GetLineMetrics = LoadFunction<uFontFace_GetLineMetrics_>(dll, "_uFontFace_GetLineMetrics@12");
            uFontFace_GetKerning = LoadFunction<uFontFace_GetKerning_>(dll, "_uFontFace_GetKerning@20");
            uFontFace_ContainsGlyph = LoadFunction<uFontFace_ContainsGlyph_>(dll, "_uFontFace_ContainsGlyph@16");
            uFontFace_RenderGlyph = LoadFunction<uFontFace_RenderGlyph_>(dll, "_uFontFace_RenderGlyph@20");
            uFontFace_ReadGlyphBitmap = LoadFunction<uFontFace_ReadGlyphBitmap_>(dll, "_uFontFace_ReadGlyphBitmap@12");
            uLoadTexture = LoadFunction<uLoadTexture_>(dll, "_uLoadTexture@12");
            uLoadTextureFromMemory = LoadFunction<uLoadTextureFromMemory_>(dll, "_uLoadTextureFromMemory@20");
            uTexture_Convert2DToCube = LoadFunction<uTexture_Convert2DToCube_>(dll, "_uTexture_Convert2DToCube@8");
            uTexture_ConvertCubeTo2D = LoadFunction<uTexture_ConvertCubeTo2D_>(dll, "_uTexture_ConvertCubeTo2D@12");
            uTexture_GetDataSize = LoadFunction<uTexture_GetDataSize_>(dll, "_uTexture_GetDataSize@16");
            uTexture_ReadData = LoadFunction<uTexture_ReadData_>(dll, "_uTexture_ReadData@20");
            uTexture_SaveAsPng = LoadFunction<uTexture_SaveAsPng_>(dll, "_uTexture_SaveAsPng@16");
        }

        internal delegate void uInit_();
        internal static uInit_ uInit;

        internal delegate IntPtr uCreate_();
        internal static uCreate_ uCreate;

        internal delegate void uRelease_(IntPtr handle);
        internal static uRelease_ uRelease;

        internal delegate IntPtr uGetErrorString_(IntPtr handle);
        internal static uGetErrorString_ uGetErrorString;

        internal delegate int uLoadFontFace_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename, out FontFaceInfo result);
        internal static uLoadFontFace_ uLoadFontFace;

        internal delegate int uLoadFontFaceFromMemory_(IntPtr handle, IntPtr data, int size, out FontFaceInfo result);
        internal static uLoadFontFaceFromMemory_ uLoadFontFaceFromMemory;

        internal delegate int uFontFace_GetLineMetrics_(IntPtr handle, float pixelSize, out FontLineMetricInfo result);
        internal static uFontFace_GetLineMetrics_ uFontFace_GetLineMetrics;

        internal delegate int uFontFace_GetKerning_(IntPtr handle, float pixelSize, uint left, uint right, out FontKerningInfo result);
        internal static uFontFace_GetKerning_ uFontFace_GetKerning;

        internal delegate int uFontFace_ContainsGlyph_(IntPtr handle, float pixelSize, uint glyph, out int result);
        internal static uFontFace_ContainsGlyph_ uFontFace_ContainsGlyph;

        internal delegate int uFontFace_RenderGlyph_(IntPtr handle, float pixelSize, uint glyph, [MarshalAs(UnmanagedType.I4)] FontRenderMode mode, out FontGlyphInfo result);
        internal static uFontFace_RenderGlyph_ uFontFace_RenderGlyph;

        internal delegate int uFontFace_ReadGlyphBitmap_(IntPtr handle, [MarshalAs(UnmanagedType.LPArray)] byte[] result, int byteCount);
        internal static uFontFace_ReadGlyphBitmap_ uFontFace_ReadGlyphBitmap;

        internal delegate int uLoadTexture_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename, out TextureInfo result);
        internal static uLoadTexture_ uLoadTexture;

        internal delegate int uLoadTextureFromMemory_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename, IntPtr data, int size, out TextureInfo result);
        internal static uLoadTextureFromMemory_ uLoadTextureFromMemory;

        internal delegate int uTexture_Convert2DToCube_(IntPtr handle, out TextureInfo result);
        internal static uTexture_Convert2DToCube_ uTexture_Convert2DToCube;

        internal delegate int uTexture_ConvertCubeTo2D_(IntPtr handle, [MarshalAs(UnmanagedType.I4)] Cube2DMode cube2DMode, out TextureInfo result);
        internal static uTexture_ConvertCubeTo2D_ uTexture_ConvertCubeTo2D;

        internal delegate int uTexture_GetDataSize_(IntPtr handle, int faceIndex, int mipIndex, out int byteCount);
        internal static uTexture_GetDataSize_ uTexture_GetDataSize;

        internal delegate int uTexture_ReadData_(IntPtr handle, int faceIndex, int mipIndex, [MarshalAs(UnmanagedType.LPArray)] byte[] result, int byteCount);
        internal static uTexture_ReadData_ uTexture_ReadData;

        internal delegate int uTexture_SaveAsPng_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string dstFilename, int faceIndex, int mipIndex);
        internal static uTexture_SaveAsPng_ uTexture_SaveAsPng;
    }
}
