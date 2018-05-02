#!/bin/bash
SELF=`echo $0 | sed 's/\\\\/\\//g'`
cd "`dirname "$SELF"`"

FUNCTIONS=$(cat <<EOF
void:_uInit@0
IntPtr:_uCreate@0
void:_uRelease@4:IntPtr handle
IntPtr:_uGetErrorString@4:IntPtr handle
int:_uLoadFontFace@12:IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename, out FontFaceInfo result
int:_uLoadFontFaceFromMemory@16:IntPtr handle, IntPtr data, int size, out FontFaceInfo result
int:_uFontFace_GetLineMetrics@12:IntPtr handle, float pixelSize, out FontLineMetricInfo result
int:_uFontFace_GetKerning@20:IntPtr handle, float pixelSize, uint left, uint right, out FontKerningInfo result
int:_uFontFace_ContainsGlyph@16:IntPtr handle, float pixelSize, uint glyph, out int result
int:_uFontFace_RenderGlyph@20:IntPtr handle, float pixelSize, uint glyph, [MarshalAs(UnmanagedType.I4)] FontRenderMode mode, out FontGlyphInfo result
int:_uFontFace_ReadGlyphBitmap@12:IntPtr handle, [MarshalAs(UnmanagedType.LPArray)] byte[] result, int byteCount
int:_uLoadTexture@12:IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename, out TextureInfo result
int:_uLoadTextureFromMemory@20:IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename, IntPtr data, int size, out TextureInfo result
int:_uTexture_Convert2DToCube@8:IntPtr handle, out TextureInfo result
int:_uTexture_ConvertCubeTo2D@12:IntPtr handle, [MarshalAs(UnmanagedType.I4)] TextureCube2DMode cube2DMode, out TextureInfo result
int:_uTexture_GetDataSize@16:IntPtr handle, int faceIndex, int mipIndex, out int byteCount
int:_uTexture_ReadData@20:IntPtr handle, int faceIndex, int mipIndex, [MarshalAs(UnmanagedType.LPArray)] byte[] result, int byteCount
int:_uTexture_SaveAsPng@16:IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string dstFilename, int faceIndex, int mipIndex
EOF)

function generate {

    LOADER=""
    DECLARATIONS=""

    while IFS=':' read -ra func; do
        ret=${func[0]}
        signature=${func[1]}
        IFS='@' name=($signature)
        name=${name[0]}
        name=${name:1}
        params=${func[2]}

        LOADER=$(cat <<EOF
$LOADER
            $name = LoadFunction<${name}_>(dll, "$signature");
EOF)

        DECLARATIONS=$(cat <<EOF
$DECLARATIONS

        internal delegate $ret ${name}_($params);
        internal static ${name}_ ${name};
EOF)
    done <<< "$1"

    cat <<EOF > Uno.Native/NativeLib.${2}.cs
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
        static void Load${2}(IntPtr dll)
        {$LOADER
        }$DECLARATIONS
    }
}
EOF
}

generate "$FUNCTIONS" uPInvoke
