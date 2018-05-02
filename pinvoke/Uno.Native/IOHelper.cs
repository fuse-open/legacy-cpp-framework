using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace Uno.Native
{
    static class IOHelper
    {
        public static string ReadAllText(this Stream stream)
        {
            return new StreamReader(stream).ReadToEnd();
        }

        public static byte[] ReadAllBytes(this Stream stream)
        {
            var result = new byte[(int)stream.Length - (int)stream.Position];
            stream.Read(result, 0, result.Length);
            return result;
        }

        public static void CreateContainingDirectory(string filename)
        {
            var dir = Path.GetDirectoryName(filename);

            if (!string.IsNullOrEmpty(dir))
                Directory.CreateDirectory(dir);
        }
    }
}
