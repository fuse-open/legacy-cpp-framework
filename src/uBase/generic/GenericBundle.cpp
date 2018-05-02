#include <uBase/Disk.h>
#include <uBase/Path.h>

namespace uBase
{
    FileSystem* CreateBundleFileSystem()
    {
        String base = Disk->GetBaseDirectory();

        const char* possibleDataSuffixes[] =
        {
            "data",
            "../data",
            "../../data",
            "../../../data",
        };

        for (int i = 0; i < 4; i++)
        {
            String bundle = Path::Combine(base, possibleDataSuffixes[i]);

            if (Disk->IsDirectory(bundle))
                return Disk->CreateSubFileSystem(bundle);
        }

        return Disk->CreateSubFileSystem("data");
    }
}
