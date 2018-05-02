using System;

namespace Uno.Native
{
    public class NativeException : Exception
    {
        internal NativeException(string msg, Exception innerException = null)
            : base(msg, innerException)
        {
        }
    }
}