using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;

namespace Uno.Native
{
    public static partial class NativeLib
    {
        internal static bool IsInitialized { get; private set; }

        public static void Initialize(string dllDir = null)
        {
            if (!IsInitialized)
            {
                LoaduPInvoke(LoadLibrary("uPInvoke", GetSearchPaths(dllDir)));
                uInit();
                IsInitialized = true;
            }
        }

        static string AssemblyDirectory
        {
            get
            {
                var codeBase = Assembly.GetExecutingAssembly().CodeBase;
                var uri = new UriBuilder(codeBase);
                var path = Uri.UnescapeDataString(uri.Path);
                return Path.GetDirectoryName(path);
            }
        }

        static string[] GetSearchPaths(string dllDir)
        {
            dllDir = Path.GetFullPath(dllDir ?? AssemblyDirectory);
            var arch = IntPtr.Size == 8 ? "x64" : "x86";

            return new[]
            {
                dllDir,
                Path.Combine(dllDir, arch),
                Path.Combine(AssemblyDirectory, "..", "..", "..", "lib"),
                Path.Combine(AssemblyDirectory, "..", "..", "..", "lib", arch, "Debug"),
                Path.Combine(AssemblyDirectory, "..", "..", "..", "lib", arch, "Release")
            };
        }

        static IntPtr LoadLibrary(string name, string[] searchPaths)
        {
            Exception e = null;
            name = PlatformDetection.IsWindows
                    ? name + ".dll" :
                PlatformDetection.IsMac
                    ? "lib" + name + ".dylib"
                    : "lib" + name + ".so";

            foreach (var dir in searchPaths)
            {
                var filename = Path.Combine(dir, name);

                if (File.Exists(filename))
                {
                    if (PlatformDetection.IsWindows)
                    {
                        Win32.SetDllDirectory(Path.GetDirectoryName(filename));
                        var result = Win32.LoadLibrary(filename);

                        if (result != IntPtr.Zero)
                            return result;
                    }
                    else if (PlatformDetection.IsMac)
                    {
                        var result = Mac.dlopen(filename, 9);

                        if (result != IntPtr.Zero)
                            return result;
                    }

                    e = new NativeException("Failed to load '" + filename + "'", e);
                }
            }

            if (e != null)
                throw e;

            var sb = new StringBuilder("'" + name + "' was not found in any of these locations:");

            foreach (var dir in searchPaths)
                sb.AppendLine("  * " + dir);

            throw new NativeException(sb.ToString());
        }

        static T LoadFunction<T>(IntPtr dll, string name) where T : class
        {
            if (!PlatformDetection.IsWindows || IntPtr.Size != 4)
                name = name.Substring(1, name.IndexOf('@') - 1);

            var address = PlatformDetection.IsWindows ? Win32.GetProcAddress(dll, name)
                        : PlatformDetection.IsMac ? Mac.dlsym(dll, name)
                        : IntPtr.Zero;
            var result = address != IntPtr.Zero
                       ? Marshal.GetDelegateForFunctionPointer(address, typeof(T)) as T
                       : null;

            if (result != null)
                return result;

            throw new NativeException("Failed to get address of '" + name + "'");
        }
    }
}
