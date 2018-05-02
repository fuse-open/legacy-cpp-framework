#include <uImage/Format.h>
using namespace uBase;

namespace uImage
{
    String FormatInfo::ToString(Format format)
    {
        switch (format)
        {
#define FORMAT(x) case x: return #x

        FORMAT(FormatUnspecified);

        FORMAT(FormatR_8_Int);
        FORMAT(FormatRG_8_8_Int);
        FORMAT(FormatRGB_8_8_8_Int);
        FORMAT(FormatRGBA_8_8_8_8_Int);

        FORMAT(FormatR_8_UInt);
        FORMAT(FormatRG_8_8_UInt);
        FORMAT(FormatRGB_8_8_8_UInt);
        FORMAT(FormatRGBA_8_8_8_8_UInt);

        FORMAT(FormatR_8_Int_Normalize);
        FORMAT(FormatRG_8_8_Int_Normalize);
        FORMAT(FormatRGB_8_8_8_Int_Normalize);
        FORMAT(FormatRGBA_8_8_8_8_Int_Normalize);

        FORMAT(FormatR_8_UInt_Normalize);
        FORMAT(FormatRG_8_8_UInt_Normalize);
        FORMAT(FormatRGB_8_8_8_UInt_Normalize);
        FORMAT(FormatRGBA_8_8_8_8_UInt_Normalize);

        FORMAT(FormatR_8_Typeless);
        FORMAT(FormatRG_8_8_Typeless);
        FORMAT(FormatRGB_8_8_8_Typeless);
        FORMAT(FormatRGBA_8_8_8_8_Typeless);

        FORMAT(FormatA_8_UInt_Normalize);
        FORMAT(FormatL_8_UInt_Normalize);
        FORMAT(FormatLA_8_8_UInt_Normalize);

        FORMAT(FormatR_16_Int);
        FORMAT(FormatRG_16_16_Int);
        FORMAT(FormatRGB_16_16_16_Int);
        FORMAT(FormatRGBA_16_16_16_16_Int);

        FORMAT(FormatR_16_Int_Normalize);
        FORMAT(FormatRG_16_16_Int_Normalize);
        FORMAT(FormatRGB_16_16_16_Int_Normalize);
        FORMAT(FormatRGBA_16_16_16_16_Int_Normalize);

        FORMAT(FormatR_16_UInt);
        FORMAT(FormatRG_16_16_UInt);
        FORMAT(FormatRGB_16_16_16_UInt);
        FORMAT(FormatRGBA_16_16_16_16_UInt);

        FORMAT(FormatR_16_UInt_Normalize);
        FORMAT(FormatRG_16_16_UInt_Normalize);
        FORMAT(FormatRGB_16_16_16_UInt_Normalize);
        FORMAT(FormatRGBA_16_16_16_16_UInt_Normalize);

        FORMAT(FormatR_16_Float);
        FORMAT(FormatRG_16_16_Float);
        FORMAT(FormatRGB_16_16_16_Float);
        FORMAT(FormatRGBA_16_16_16_16_Float);

        FORMAT(FormatR_16_Typeless);
        FORMAT(FormatRG_16_16_Typeless);
        FORMAT(FormatRGB_16_16_16_Typeless);
        FORMAT(FormatRGBA_16_16_16_16_Typeless);

        FORMAT(FormatR_32_Int);
        FORMAT(FormatRG_32_32_Int);
        FORMAT(FormatRGB_32_32_32_Int);
        FORMAT(FormatRGBA_32_32_32_32_Int);

        FORMAT(FormatR_32_UInt);
        FORMAT(FormatRG_32_32_UInt);
        FORMAT(FormatRGB_32_32_32_UInt);
        FORMAT(FormatRGBA_32_32_32_32_UInt);

        FORMAT(FormatR_32_Float);
        FORMAT(FormatRG_32_32_Float);
        FORMAT(FormatRGB_32_32_32_Float);
        FORMAT(FormatRGBA_32_32_32_32_Float);

        FORMAT(FormatR_32_Typeless);
        FORMAT(FormatRG_32_32_Typeless);
        FORMAT(FormatRGB_32_32_32_Typeless);
        FORMAT(FormatRGBA_32_32_32_32_Typeless);

        FORMAT(FormatCompressedRGB_BC1);
        FORMAT(FormatCompressedRGB_BC2);
        FORMAT(FormatCompressedRGB_BC3);
        FORMAT(FormatCompressedRGB_BC4);
        FORMAT(FormatCompressedRGB_BC5);
        FORMAT(FormatCompressedRGB_ETC1);
        FORMAT(FormatCompressedRGB_PVRTC_4BPP);
        FORMAT(FormatCompressedRGB_PVRTC_2BPP);
        FORMAT(FormatCompressedRGBA_PVRTC_4BPP);
        FORMAT(FormatCompressedRGBA_PVRTC_2BPP);

        FORMAT(FormatD_32_Float);
        FORMAT(FormatDSX_32_8_24_Float_UInt_Typeless);
        FORMAT(FormatDS_24_8_UInt_Normalize_UInt);

        FORMAT(FormatD_16_UNorm);
        FORMAT(FormatR_16_UNorm);

#undef FORMAT

        default: return "<unknown>";
        }
    }

    int FormatInfo::SizeOf(Format format)
    {
        switch (format)
        {
        case FormatR_8_Int: return 1;
        case FormatRG_8_8_Int: return 2;
        case FormatRGB_8_8_8_Int: return 3;
        case FormatRGBA_8_8_8_8_Int: return 4;

        case FormatR_8_UInt: return 1;
        case FormatRG_8_8_UInt: return 2;
        case FormatRGB_8_8_8_UInt: return 3;
        case FormatRGBA_8_8_8_8_UInt: return 4;

        case FormatR_8_Int_Normalize: return 1;
        case FormatRG_8_8_Int_Normalize: return 2;
        case FormatRGB_8_8_8_Int_Normalize: return 3;
        case FormatRGBA_8_8_8_8_Int_Normalize: return 4;

        case FormatR_8_UInt_Normalize: return 1;
        case FormatRG_8_8_UInt_Normalize: return 2;
        case FormatRGB_8_8_8_UInt_Normalize: return 3;
        case FormatRGBA_8_8_8_8_UInt_Normalize: return 4;

        case FormatA_8_UInt_Normalize: return 1;
        case FormatL_8_UInt_Normalize: return 1;
        case FormatLA_8_8_UInt_Normalize: return 2;

        case FormatR_16_Int: return 2;
        case FormatRG_16_16_Int: return 4;
        case FormatRGB_16_16_16_Int: return 6;
        case FormatRGBA_16_16_16_16_Int: return 8;

        case FormatR_16_Int_Normalize: return 2;
        case FormatRG_16_16_Int_Normalize: return 4;
        case FormatRGB_16_16_16_Int_Normalize: return 6;
        case FormatRGBA_16_16_16_16_Int_Normalize: return 8;

        case FormatR_16_UInt: return 2;
        case FormatRG_16_16_UInt: return 4;
        case FormatRGB_16_16_16_UInt: return 6;
        case FormatRGBA_16_16_16_16_UInt: return 8;

        case FormatR_16_UInt_Normalize: return 2;
        case FormatRG_16_16_UInt_Normalize: return 4;
        case FormatRGB_16_16_16_UInt_Normalize: return 6;
        case FormatRGBA_16_16_16_16_UInt_Normalize: return 8;

        case FormatR_16_Float: return 2;
        case FormatRG_16_16_Float: return 4;
        case FormatRGB_16_16_16_Float: return 6;
        case FormatRGBA_16_16_16_16_Float: return 8;

        case FormatR_32_Int: return 4;
        case FormatRG_32_32_Int: return 8;
        case FormatRGB_32_32_32_Int: return 12;
        case FormatRGBA_32_32_32_32_Int: return 16;

        case FormatR_32_UInt: return 4;
        case FormatRG_32_32_UInt: return 8;
        case FormatRGB_32_32_32_UInt: return 12;
        case FormatRGBA_32_32_32_32_UInt: return 16;

        case FormatR_32_Float: return 4;
        case FormatRG_32_32_Float: return 8;
        case FormatRGB_32_32_32_Float: return 12;
        case FormatRGBA_32_32_32_32_Float: return 16;

        case FormatD_32_Float: return 4;
        case FormatDSX_32_8_24_Float_UInt_Typeless: return 8;
        case FormatDS_24_8_UInt_Normalize_UInt: return 4;

        case FormatD_16_UNorm: return 2;
        case FormatR_16_UNorm: return 2;

        default: throw Exception("Unknown size of " + ToString(format));
        }
    }

    int FormatInfo::ComponentCount(Format format)
    {
        switch (format)
        {
        case FormatR_8_Int: return 1;
        case FormatRG_8_8_Int: return 2;
        case FormatRGB_8_8_8_Int: return 3;
        case FormatRGBA_8_8_8_8_Int: return 4;

        case FormatR_8_UInt: return 1;
        case FormatRG_8_8_UInt: return 2;
        case FormatRGB_8_8_8_UInt: return 3;
        case FormatRGBA_8_8_8_8_UInt: return 4;

        case FormatR_8_Int_Normalize: return 1;
        case FormatRG_8_8_Int_Normalize: return 2;
        case FormatRGB_8_8_8_Int_Normalize: return 3;
        case FormatRGBA_8_8_8_8_Int_Normalize: return 4;

        case FormatR_8_UInt_Normalize: return 1;
        case FormatRG_8_8_UInt_Normalize: return 2;
        case FormatRGB_8_8_8_UInt_Normalize: return 3;
        case FormatRGBA_8_8_8_8_UInt_Normalize: return 4;

        case FormatA_8_UInt_Normalize: return 1;
        case FormatL_8_UInt_Normalize: return 1;
        case FormatLA_8_8_UInt_Normalize: return 2;

        case FormatR_16_Int: return 1;
        case FormatRG_16_16_Int: return 2;
        case FormatRGB_16_16_16_Int: return 3;
        case FormatRGBA_16_16_16_16_Int: return 4;

        case FormatR_16_Int_Normalize: return 1;
        case FormatRG_16_16_Int_Normalize: return 2;
        case FormatRGB_16_16_16_Int_Normalize: return 3;
        case FormatRGBA_16_16_16_16_Int_Normalize: return 4;

        case FormatR_16_UInt: return 1;
        case FormatRG_16_16_UInt: return 2;
        case FormatRGB_16_16_16_UInt: return 3;
        case FormatRGBA_16_16_16_16_UInt: return 4;

        case FormatR_16_UInt_Normalize: return 1;
        case FormatRG_16_16_UInt_Normalize: return 2;
        case FormatRGB_16_16_16_UInt_Normalize: return 3;
        case FormatRGBA_16_16_16_16_UInt_Normalize: return 4;

        case FormatR_16_Float: return 1;
        case FormatRG_16_16_Float: return 2;
        case FormatRGB_16_16_16_Float: return 3;
        case FormatRGBA_16_16_16_16_Float: return 4;

        case FormatR_32_Int: return 1;
        case FormatRG_32_32_Int: return 2;
        case FormatRGB_32_32_32_Int: return 3;
        case FormatRGBA_32_32_32_32_Int: return 4;

        case FormatR_32_UInt: return 1;
        case FormatRG_32_32_UInt: return 2;
        case FormatRGB_32_32_32_UInt: return 3;
        case FormatRGBA_32_32_32_32_UInt: return 4;

        case FormatR_32_Float: return 1;
        case FormatRG_32_32_Float: return 2;
        case FormatRGB_32_32_32_Float: return 3;
        case FormatRGBA_32_32_32_32_Float: return 4;

        default: throw Exception("Unknown number of components in " + ToString(format));
        }
    }
}
