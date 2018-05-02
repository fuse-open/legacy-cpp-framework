#pragma once
#include <uBase/Array.h>
#include <uBase/Memory.h>
#include <uImage/Image.h>

namespace uImage
{
    /** \ingroup uImage */
    enum TextureType
    {
        TextureType2D,
        TextureType3D,
        TextureTypeCube,
    };

    enum CubeIndex
    {
        CubeIndexPositiveX = 0,
        CubeIndexNegativeX = 1,
        CubeIndexPositiveY = 2,
        CubeIndexNegativeY = 3,
        CubeIndexPositiveZ = 4,
        CubeIndexNegativeZ = 5,
    };

    struct U_DLLEXPORT TextureFace
    {
        uBase::Array<uBase::Auto<Image> > MipLevels;

        TextureFace();
        TextureFace(Image* image);

        TextureFace& operator =(const TextureFace& copy);
    };

    enum Cube2DMode
    {
        Cube2DMode1x6,
        Cube2DMode2x3,
        Cube2DMode3x2,
        Cube2DMode6x1,
    };

    /** \ingroup uImage */
    class Texture: public uBase::Object
    {
    public:
        TextureType Type;
        uBase::Array<TextureFace> Faces;

        int GetWidth() const;
        int GetHeight() const;
        int GetDepth() const;
        int GetMipCount() const;
        Format GetFormat() const;

        Texture(TextureType type);

        static Texture* Create(Image* img);
        static Texture* CreateCube(Image* posX, Image* negX, Image* posY, Image* negY, Image* posZ, Image* negZ);

        Texture* Convert2DToCube();
        Texture* ConvertCubeTo2D(Cube2DMode mode);
    };
}
