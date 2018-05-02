#include <uBase/Console.h>
#include <XliPlatform/Display.h>
#include <XliPlatform/PlatformSpecific/iOS.h>
#include <sys/sysctl.h>
#include <stdlib.h>
#include <uBase/String.h>
#include <uBase/Console.h>
#include <UIKit/UIKit.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        iOS::DeviceType iOS::GetDeviceType()
        {
            size_t size;
            sysctlbyname("hw.machine", NULL, &size, NULL, 0);
            char* machine = (char*)malloc(size);
            sysctlbyname("hw.machine", machine, &size, NULL, 0);
            String platform(machine,size-1);
            free(machine);

            if (platform == "iPhone1,1") return iOS::DeviceTypeIPHONE_1G;
            if (platform == "iPhone1,2") return iOS::DeviceTypeIPHONE_3G;
            if (platform == "iPhone2,1") return iOS::DeviceTypeIPHONE_3GS;
            if (platform == "iPhone3,1") return iOS::DeviceTypeIPHONE_4;
            if (platform == "iPhone3,3") return iOS::DeviceTypeVERIZON_IPHONE_4;
            if (platform == "iPhone4,1") return iOS::DeviceTypeIPHONE_4S;
            if (platform == "iPhone5,1") return iOS::DeviceTypeIPHONE_5_GSM;
            if (platform == "iPhone5,2") return iOS::DeviceTypeIPHONE_5_GSM_CDMA;
            if (platform == "iPhone5,3") return iOS::DeviceTypeIPHONE_5C_GSM;
            if (platform == "iPhone5,4") return iOS::DeviceTypeIPHONE_5C_GSM_CDMA;
            if (platform == "iPhone6,1") return iOS::DeviceTypeIPHONE_5S_GSM;
            if (platform == "iPhone6,2") return iOS::DeviceTypeIPHONE_5S_GSM_CDMA;
            if (platform == "iPod1,1")   return iOS::DeviceTypeIPOD_TOUCH_1G;
            if (platform == "iPod2,1")   return iOS::DeviceTypeIPOD_TOUCH_2G;
            if (platform == "iPod3,1")   return iOS::DeviceTypeIPOD_TOUCH_3G;
            if (platform == "iPod4,1")   return iOS::DeviceTypeIPOD_TOUCH_4G;
            if (platform == "iPod5,1")   return iOS::DeviceTypeIPOD_TOUCH_5G;
            if (platform == "iPad1,1")   return iOS::DeviceTypeIPAD;
            if (platform == "iPad2,1")   return iOS::DeviceTypeIPAD_2_WIFI;
            if (platform == "iPad2,2")   return iOS::DeviceTypeIPAD_2_GSM;
            if (platform == "iPad2,3")   return iOS::DeviceTypeIPAD_2_CDMA;
            if (platform == "iPad2,4")   return iOS::DeviceTypeIPAD_2_WIFI;
            if (platform == "iPad2,5")   return iOS::DeviceTypeIPAD_MINI_WIFI;
            if (platform == "iPad2,6")   return iOS::DeviceTypeIPAD_MINI_GSM;
            if (platform == "iPad2,7")   return iOS::DeviceTypeIPAD_MINI_GSM_CDMA;
            if (platform == "iPad3,1")   return iOS::DeviceTypeIPAD_3_WIFI;
            if (platform == "iPad3,2")   return iOS::DeviceTypeIPAD_3_GSM_CDMA;
            if (platform == "iPad3,3")   return iOS::DeviceTypeIPAD_3_GSM;
            if (platform == "iPad3,4")   return iOS::DeviceTypeIPAD_4_WIFI;
            if (platform == "iPad3,5")   return iOS::DeviceTypeIPAD_4_GSM;
            if (platform == "iPad3,6")   return iOS::DeviceTypeIPAD_4_GSM_CDMA;
            if (platform == "iPad4,1")   return iOS::DeviceTypeIPAD_AIR_WIFI;
            if (platform == "iPad4,2")   return iOS::DeviceTypeIPAD_AIR_CELLULAR;
            if (platform == "iPad4,4")   return iOS::DeviceTypeIPAD_MINI_2G_WIFI;
            if (platform == "iPad4,5")   return iOS::DeviceTypeIPAD_MINI_2G_CELLULAR;
            if (platform == "i386")      return iOS::DeviceTypeSIMULATOR;
            if (platform == "x86_64")    return iOS::DeviceTypeSIMULATOR;
            return iOS::DeviceTypeUNKNOWN;
        }

        bool iOS::IsIPhone()
        {
            iOS::DeviceType type = iOS::GetDeviceType();
            if (type==iOS::DeviceTypeIPHONE_1G || type==iOS::DeviceTypeIPHONE_3G || type==iOS::DeviceTypeIPHONE_3GS || type==iOS::DeviceTypeIPHONE_4 ||
                type==iOS::DeviceTypeVERIZON_IPHONE_4 || type==iOS::DeviceTypeIPHONE_4S || type==iOS::DeviceTypeIPHONE_5_GSM ||
                type==iOS::DeviceTypeIPHONE_5_GSM_CDMA || type==iOS::DeviceTypeIPHONE_5C_GSM || type==iOS::DeviceTypeIPHONE_5C_GSM_CDMA ||
                type==iOS::DeviceTypeIPHONE_5S_GSM || type==iOS::DeviceTypeIPHONE_5S_GSM_CDMA)
                return true;
            return false;
        }
        bool iOS::IsIPod()
        {
            iOS::DeviceType type = iOS::GetDeviceType();
            if (type==iOS::DeviceTypeIPOD_TOUCH_1G || type==iOS::DeviceTypeIPOD_TOUCH_2G || type==iOS::DeviceTypeIPOD_TOUCH_3G ||
                type==iOS::DeviceTypeIPOD_TOUCH_4G || type==iOS::DeviceTypeIPOD_TOUCH_5G)
                return true;
            return false;
        }
        bool iOS::IsIPad()
        {
            iOS::DeviceType type = iOS::GetDeviceType();
            if (type==iOS::DeviceTypeIPAD || type==iOS::DeviceTypeIPAD_2_WIFI || type==iOS::DeviceTypeIPAD_2_GSM ||
                type==iOS::DeviceTypeIPAD_2_CDMA || type==iOS::DeviceTypeIPAD_2_WIFI || type==iOS::DeviceTypeIPAD_MINI_WIFI ||
                type==iOS::DeviceTypeIPAD_MINI_GSM || type==iOS::DeviceTypeIPAD_MINI_GSM_CDMA || type==iOS::DeviceTypeIPAD_3_WIFI ||
                type==iOS::DeviceTypeIPAD_3_GSM_CDMA || type==iOS::DeviceTypeIPAD_3_GSM || type==iOS::DeviceTypeIPAD_4_WIFI ||
                type==iOS::DeviceTypeIPAD_4_GSM || type==iOS::DeviceTypeIPAD_4_GSM_CDMA || type==iOS::DeviceTypeIPAD_AIR_WIFI ||
                type==iOS::DeviceTypeIPAD_AIR_CELLULAR || type==iOS::DeviceTypeIPAD_MINI_2G_WIFI || type==iOS::DeviceTypeIPAD_MINI_2G_CELLULAR)
                return true;
            return false;
        }

        static float GetDeviceDpi(iOS::DeviceType type)
        {
            switch (type)
            {
            case iOS::DeviceTypeIPHONE_1G:
                return 163.0f;
            case iOS::DeviceTypeIPHONE_3G:
                return 163.0f;
            case iOS::DeviceTypeIPHONE_3GS:
                return 163.0f;
            case iOS::DeviceTypeIPHONE_4:
                return 326.0f;
            case iOS::DeviceTypeVERIZON_IPHONE_4:
                return 163.0f;
            case iOS::DeviceTypeIPHONE_4S:
                return 326.0f;
            case iOS::DeviceTypeIPHONE_5_GSM:
                return 326.0f;
            case iOS::DeviceTypeIPHONE_5_GSM_CDMA:
                return 326.0f;
            case iOS::DeviceTypeIPHONE_5C_GSM:
                return 326.0f;
            case iOS::DeviceTypeIPHONE_5C_GSM_CDMA:
                return 326.0f;
            case iOS::DeviceTypeIPHONE_5S_GSM:
                return 326.0f;
            case iOS::DeviceTypeIPHONE_5S_GSM_CDMA:
                return 326.0f;
            case iOS::DeviceTypeIPOD_TOUCH_1G:
                return 163.0f;
            case iOS::DeviceTypeIPOD_TOUCH_2G:
                return 163.0f;
            case iOS::DeviceTypeIPOD_TOUCH_3G:
                return 163.0f;
            case iOS::DeviceTypeIPOD_TOUCH_4G:
                return 326.0f;
            case iOS::DeviceTypeIPOD_TOUCH_5G:
                return 326.0f;
            case iOS::DeviceTypeIPAD:
                return 132.0f;
            case iOS::DeviceTypeIPAD_2_WIFI:
                return 132.0f;
            case iOS::DeviceTypeIPAD_2_GSM:
                return 132.0f;
            case iOS::DeviceTypeIPAD_2_CDMA:
                return 132.0f;
            case iOS::DeviceTypeIPAD_MINI_WIFI:
                return 163.0f;
            case iOS::DeviceTypeIPAD_MINI_GSM:
                return 163.0f;
            case iOS::DeviceTypeIPAD_MINI_GSM_CDMA:
                return 163.0f;
            case iOS::DeviceTypeIPAD_3_WIFI:
                return 264.0f;
            case iOS::DeviceTypeIPAD_3_GSM_CDMA:
                return 264.0f;
            case iOS::DeviceTypeIPAD_3_GSM:
                return 264.0f;
            case iOS::DeviceTypeIPAD_4_WIFI:
                return 264.0f;
            case iOS::DeviceTypeIPAD_4_GSM:
                return 264.0f;
            case iOS::DeviceTypeIPAD_4_GSM_CDMA:
                return 264.0f;
            case iOS::DeviceTypeIPAD_AIR_WIFI:
                return 264.0f;
            case iOS::DeviceTypeIPAD_AIR_CELLULAR:
                return 264.0f;
            case iOS::DeviceTypeIPAD_MINI_2G_WIFI:
                return 326.0f;
            case iOS::DeviceTypeIPAD_MINI_2G_CELLULAR:
                return 326.0f;
            case iOS::DeviceTypeSIMULATOR:
                return 163.0f;
            }

            float scale = [[UIScreen mainScreen] scale];
            return 163.0 * scale;
        }
    }

    float Display::GetDensity(int displayIndex)
    {
        return [[UIScreen mainScreen] scale];
    }

    Vector2 Display::GetDpi(int displayIndex)
    {
        PlatformSpecific::iOS::DeviceType dType = PlatformSpecific::iOS::GetDeviceType();
        float dpi = PlatformSpecific::GetDeviceDpi(dType);
        return Vector2(dpi, dpi);
    }
}
