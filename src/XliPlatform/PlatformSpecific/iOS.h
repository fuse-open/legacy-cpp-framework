#pragma once

namespace Xli
{
    namespace PlatformSpecific
    {
        /** \ingroup XliPlatform */
        class iOS
        {
        public:
            static void Init();

            enum DeviceType
            {
                DeviceTypeUNKNOWN = 0,
                DeviceTypeIPHONE_1G = 1,
                DeviceTypeIPHONE_3G = 2,
                DeviceTypeIPHONE_3GS = 3,
                DeviceTypeIPHONE_4 = 4,
                DeviceTypeVERIZON_IPHONE_4 = 5,
                DeviceTypeIPHONE_4S = 6,
                DeviceTypeIPHONE_5_GSM = 7,
                DeviceTypeIPHONE_5_GSM_CDMA = 8,
                DeviceTypeIPHONE_5C_GSM = 9,
                DeviceTypeIPHONE_5C_GSM_CDMA = 10,
                DeviceTypeIPHONE_5S_GSM = 11,
                DeviceTypeIPHONE_5S_GSM_CDMA = 12,
                DeviceTypeIPOD_TOUCH_1G = 13,
                DeviceTypeIPOD_TOUCH_2G = 14,
                DeviceTypeIPOD_TOUCH_3G = 15,
                DeviceTypeIPOD_TOUCH_4G = 16,
                DeviceTypeIPOD_TOUCH_5G = 17,
                DeviceTypeIPAD = 18,
                DeviceTypeIPAD_2_WIFI = 19,
                DeviceTypeIPAD_2_GSM = 20,
                DeviceTypeIPAD_2_CDMA = 21,
                DeviceTypeIPAD_MINI_WIFI = 22,
                DeviceTypeIPAD_MINI_GSM = 23,
                DeviceTypeIPAD_MINI_GSM_CDMA = 24,
                DeviceTypeIPAD_3_WIFI = 25,
                DeviceTypeIPAD_3_GSM_CDMA = 26,
                DeviceTypeIPAD_3_GSM = 27,
                DeviceTypeIPAD_4_WIFI = 28,
                DeviceTypeIPAD_4_GSM = 29,
                DeviceTypeIPAD_4_GSM_CDMA = 30,
                DeviceTypeIPAD_AIR_WIFI = 31,
                DeviceTypeIPAD_AIR_CELLULAR = 32,
                DeviceTypeIPAD_MINI_2G_WIFI = 33,
                DeviceTypeIPAD_MINI_2G_CELLULAR = 34,
                DeviceTypeSIMULATOR = 35
            };

            static DeviceType GetDeviceType();
            static bool IsIPhone();
            static bool IsIPad();
            static bool IsIPod();
        };
    }
}
