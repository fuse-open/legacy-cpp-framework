using System;
using System.Diagnostics;
using System.IO;
using Uno.Native.Fonts;
using Uno.Native.Textures;

namespace Uno.Native.Tests
{
    class Program
    {
        static void LoadFontFace(string filename, float size)
        {
            Console.WriteLine("--------------------------------------------------------------------");
            Console.WriteLine("Loading Font Face: '" + filename + "', size = " + size);
            Console.WriteLine("--------------------------------------------------------------------");

            using (var font = new FontFace(filename))
            {
                Console.WriteLine("FamilyName: " + font.FamilyName);
                Console.WriteLine("StyleName: " + font.StyleName);

                Console.WriteLine("Ascender: " + font.GetAscender(size));
                Console.WriteLine("Descender: " + font.GetDescender(size));
                Console.WriteLine("LineHeight: " + font.GetLineHeight(size));

                for (char c = ' '; c < 127; c++)
                {
                    if (font.ContainsGlyph(size, c))
                    {
                        var g = font.RenderGlyph(size, c, FontRenderMode.Normal);
                        Console.WriteLine((int)c + " (" + c + "): " + g.AdvanceX + ", " + g.AdvanceY + "; " + g.BearingX + ", " + g.BearingY + "; " + g.Width + "x" + g.Height);
                    }
                }
            }

            using (var s = File.OpenRead(filename))
                using (var font = new FontFace(s))
                    font.ToString();
        }

        static void LoadTexture(string filename)
        {
            Console.WriteLine("--------------------------------------------------------------------");
            Console.WriteLine("Loading Texture: '" + filename + "'");
            Console.WriteLine("--------------------------------------------------------------------");

            var sw = new Stopwatch();
            sw.Start();

            using (var tex = new Texture(filename))
            {
                Console.WriteLine("Depth: " + tex.Depth);
                Console.WriteLine("FaceCount: " + tex.FaceCount);
                Console.WriteLine("Filename: " + tex.Filename);
                Console.WriteLine("Height: " + tex.Height);
                Console.WriteLine("MipCount: " + tex.MipCount);
                Console.WriteLine("PixelFormat: " + tex.PixelFormat);
                Console.WriteLine("TextureType: " + tex.TextureType);
                Console.WriteLine("Width: " + tex.Width);

                tex.SaveAsPng("foo.png");
            }

            sw.Stop();
            Console.WriteLine("Time used loading: " + sw.ElapsedMilliseconds);
        }

        static void LoadTextureFromStream(string filename)
        {
            Console.WriteLine("--------------------------------------------------------------------");
            Console.WriteLine("Loading Texture: '" + filename + "'");
            Console.WriteLine("--------------------------------------------------------------------");

            var sw = new Stopwatch();
            sw.Start();

            using (var stream = File.OpenRead(filename))
            {
                using (var tex = new Texture(filename, stream))
                {
                    Console.WriteLine("Depth: " + tex.Depth);
                    Console.WriteLine("FaceCount: " + tex.FaceCount);
                    Console.WriteLine("Filename: " + tex.Filename);
                    Console.WriteLine("Height: " + tex.Height);
                    Console.WriteLine("MipCount: " + tex.MipCount);
                    Console.WriteLine("PixelFormat: " + tex.PixelFormat);
                    Console.WriteLine("TextureType: " + tex.TextureType);
                    Console.WriteLine("Width: " + tex.Width);

                    tex.SaveAsPng("foo.png");
                }
            }

            sw.Stop();
            Console.WriteLine("Time used loading: " + sw.ElapsedMilliseconds);
        }

        static void Main(string[] args)
        {
            NativeLib.Initialize();
            LoadFontFace(Path.Combine("fonts", "arial.ttf"), 100);
            LoadTexture(Path.Combine("textures", "foo.jpg"));
            LoadTextureFromStream(Path.Combine("textures", "foo.jpg"));
            LoadTexture(Path.Combine("textures", "earth.bmp"));
            LoadTextureFromStream(Path.Combine("textures", "earth.bmp"));
            LoadTexture(Path.Combine("textures", "like_button.png"));
            LoadTextureFromStream(Path.Combine("textures", "like_button.png"));
        }
    }
}
