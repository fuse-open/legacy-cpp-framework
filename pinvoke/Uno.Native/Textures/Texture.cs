using System;
using System.IO;
using System.Runtime.InteropServices;

namespace Uno.Native.Textures
{
    public class Texture : NativeObject
    {
        TextureInfo _info;

        public int Width
        {
            get { return _info.Width; }
        }

        public int Height
        {
            get { return _info.Height; }
        }

        public int Depth
        {
            get { return _info.Depth; }
        }

        public int FaceCount
        {
            get { return _info.FaceCount; }
        }

        public int MipCount
        {
            get { return _info.MipCount; }
        }

        public TextureType TextureType
        {
            get { return _info.TextureType; }
        }

        public PixelFormat PixelFormat
        {
            get { return _info.PixelFormat; }
        }

        public string Filename
        {
            get;
            private set;
        }

        public Texture(string filename)
        {
            Filename = filename;
            CheckError(NativeLib.uLoadTexture(Handle, filename, out _info));
        }

        public Texture(string filename, byte[] bytes)
        {
            Filename = filename;
            var ptr = GCHandle.Alloc(bytes, GCHandleType.Pinned);

            try
            {
                CheckError(NativeLib.uLoadTextureFromMemory(Handle, filename, ptr.AddrOfPinnedObject(), bytes.Length, out _info));
            }
            finally
            {
                ptr.Free();
            }
        }

        public Texture(string filename, Stream stream)
            : this(filename, stream.ReadAllBytes())
        {
        }

        public void Convert2DToCube()
        {
            CheckError(NativeLib.uTexture_Convert2DToCube(Handle, out _info));
        }

        public void ConvertCubeTo2D(Cube2DMode mode)
        {
            CheckError(NativeLib.uTexture_ConvertCubeTo2D(Handle, mode, out _info));
        }

        public byte[] ReadData(int faceIndex = 0, int mipIndex = 0)
        {
            int byteCount;
            CheckError(NativeLib.uTexture_GetDataSize(Handle, faceIndex, mipIndex, out byteCount));

            var result = new byte[byteCount];
            CheckError(NativeLib.uTexture_ReadData(Handle, faceIndex, mipIndex, result, result.Length));

            return result;
        }

        public void SaveAsPng(string dstFilename, int faceIndex = 0, int mipIndex = 0)
        {
            IOHelper.CreateContainingDirectory(dstFilename);
            CheckError(NativeLib.uTexture_SaveAsPng(Handle, dstFilename, faceIndex, mipIndex));
        }
    }
}
