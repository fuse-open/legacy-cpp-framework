#include <XliPlatform/CommonDialogs.h>
#include <uBase/Disk.h>
#include <uBase/StringBuilder.h>
#include <uBase/Path.h>
#include <XliPlatform/MessageBox.h>
#include <XliPlatform/PlatformSpecific/Win32.h>
#include <XliPlatform/Window.h>
#include <uBase/Unicode.h>
#include <CommDlg.h>

namespace Xli
{
    static String FixExtension(const String& str)
    {
        return str.Trim('.').Trim('*');
    }

    static void InitOptions(Window* parent, const FileDialogOptions& options, bool mustExist, OPENFILENAMEW& ofn, WCHAR fnbufW[4096], StringU16& filterW, StringU16& defW, StringU16& dirW, StringU16& captionW)
    {
        fnbufW[0] = '\0';

        ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
        ofn.lStructSize = sizeof(OPENFILENAMEW);

        defW = Unicode::Utf8To16(FixExtension(options.DefaultExtension));
        dirW = Unicode::Utf8To16(options.DefaultFolder);
        captionW = Unicode::Utf8To16(options.Caption);

        for (int i = 0; i < dirW.Length(); i++)
            if (dirW[i] == '/')
                dirW[i] = '\\';

        if (options.FileExtensions.Length())
        {
            ofn.nFilterIndex = 1;

            StringBuilder fb;
            for (int i = 0; i < options.FileExtensions.Length(); i++)
            {
                String ext = FixExtension(options.FileExtensions[i].Extension);

                if (ext.Length())
                    ext = "*." + ext;
                else
                    ext = "*.*";

                fb.Append(options.FileExtensions[i].Description);
                fb.Append(" (" + ext + ")");
                fb.Append('\0');
                fb.Append(ext);
                fb.Append('\0');

                if (options.FileExtensions[i].Extension == options.DefaultExtension)
                    ofn.nFilterIndex = i + 1;
            }

            fb.Append('\0');
            filterW = Unicode::Utf8To16(fb.ToString());
        }
        else
        {
            ofn.nFilterIndex = 1;
            filterW = Unicode::Utf8To16("All files (*.*)\0*.*\0\0");
        }

        if (options.DefaultFile.Length())
        {
            StringU16 defaultFileW = Unicode::Utf8To16(options.DefaultFile);
            memcpy(fnbufW, defaultFileW.Ptr(), defaultFileW.Length() * 2 + 2);
        }

        ofn.hwndOwner = PlatformSpecific::Win32::GetWindowHandle(parent);
        ofn.hInstance = GetModuleHandle(NULL);
        ofn.lpstrFilter = filterW.Ptr();
        ofn.lpstrFile = fnbufW;
        ofn.nMaxFile = 4096;
        ofn.lpstrInitialDir = dirW.Ptr();
        ofn.lpstrTitle = captionW.Ptr();

        if (mustExist)
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
        else
            ofn.Flags = OFN_OVERWRITEPROMPT | OFN_ENABLESIZING;

        ofn.lpstrDefExt = defW.Ptr();
    }

    static bool EndFileDialog(bool ret, WCHAR* fnbufW, const String& cd, String& result)
    {
        if (ret)
        {
            result = Unicode::Utf16To8(fnbufW);

            for (int i = 0; i < result.Length(); i++)
                if (result[i] == '\\')
                    result[i] = '/';
        }

        Disk->SetCurrentDirectory(cd);
        return ret;
    }

    bool CommonDialogs::OpenFile(Window* parent, const FileDialogOptions& options, String& result)
    {
        WCHAR fnbufW[4096];
        OPENFILENAMEW ofn;
        StringU16 filterW, defW, dirW, captionW;
        String cd = Disk->GetCurrentDirectory();
        InitOptions(parent, options, true, ofn, fnbufW, filterW, defW, dirW, captionW);
        return EndFileDialog(GetOpenFileNameW(&ofn) == TRUE, fnbufW, cd, result);
    }

    bool CommonDialogs::SaveFile(Window* parent, const FileDialogOptions& options, String& result)
    {
        WCHAR fnbufW[4096];
        OPENFILENAMEW ofn;
        StringU16 filterW, defW, dirW, captionW;
        String cd = Disk->GetCurrentDirectory();
        InitOptions(parent, options, false, ofn, fnbufW, filterW, defW, dirW, captionW);
        return EndFileDialog(GetSaveFileNameW(&ofn) == TRUE, fnbufW, cd, result);
    }
}
