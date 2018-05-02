#pragma once
#include <uBase/Object.h>
#include <uBase/File.h>
#include <uBase/Vector2.h>
#include <uBase/String.h>

namespace uImage
{
    class Bitmap;

    /** \ingroup uImage */
    enum FontRenderMode
    {
        FontRenderModeNone,
        FontRenderModeNormal,
        FontRenderModeMonochrome,
    };

    /** \ingroup uImage */
    class FontFace: public uBase::Object
    {
    public:
        static void Init();
        static FontFace* Load(uBase::Stream* fontFile);
        static FontFace* Load(const uBase::String& filename) {
            uBase::File f(filename, uBase::FileModeRead);
            return Load(&f);
        }

        virtual uBase::String GetFamilyName() = 0;
        virtual uBase::String GetStyleName() = 0;

        virtual float GetAscender(float pixelSize) = 0;
        virtual float GetDescender(float pixelSize) = 0;
        virtual float GetLineHeight(float pixelSize) = 0;

        virtual bool ContainsGlyph(float pixelSize, int c) = 0;
        virtual Bitmap* RenderGlyph(float pixelSize, int c, FontRenderMode mode, uBase::Vector2* outAdvance, uBase::Vector2* outBearing) = 0;

        virtual bool TryGetKerning(float pixelSize, int left, int right, uBase::Vector2* outKerning) = 0;
    };
}
