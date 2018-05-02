#include <uImage/Texture.h>
#include <uImage/Bitmap.h>
#include <uBase/Memory.h>
using namespace uBase;

namespace uImage
{
    TextureFace::TextureFace()
    {
    }

    TextureFace::TextureFace(Image* img)
    {
        MipLevels.Add(SharePtr(img));
    }

    TextureFace& TextureFace::operator =(const TextureFace& copy)
    {
        MipLevels.Clear();
        MipLevels.AddRange(copy.MipLevels);
        return *this;
    }

    Texture::Texture(TextureType type)
    {
        Type = type;
    }

    int Texture::GetWidth() const
    {
        return Faces[0].MipLevels[0]->GetWidth();
    }

    int Texture::GetHeight() const
    {
        return Faces[0].MipLevels[0]->GetHeight();
    }

    int Texture::GetDepth() const
    {
        return Faces[0].MipLevels[0]->GetDepth();
    }

    int Texture::GetMipCount() const
    {
        return Faces[0].MipLevels.Length();
    }

    Format Texture::GetFormat() const
    {
        return Faces[0].MipLevels[0]->GetFormat();
    }

    Texture* Texture::Create(Image* img)
    {
        Texture* result = new Texture(TextureType2D);
        result->Faces.Add(TextureFace(img));
        return result;
    }

    Texture* Texture::CreateCube(Image* posX, Image* negX, Image* posY, Image* negY, Image* posZ, Image* negZ)
    {
        Texture* result = new Texture(TextureTypeCube);
        result->Faces.Reserve(6);
        result->Faces.Add(TextureFace(posX));
        result->Faces.Add(TextureFace(negX));
        result->Faces.Add(TextureFace(posY));
        result->Faces.Add(TextureFace(negY));
        result->Faces.Add(TextureFace(posZ));
        result->Faces.Add(TextureFace(negZ));
        return result;
    }

    static Cube2DMode DetectCubeFrom2DMode(int w, int h)
    {
        if (w / 6 == h) return Cube2DMode6x1;
        if (2 * w / 3 == h) return Cube2DMode3x2;
        if (h / 6 == w) return Cube2DMode1x6;
        if (2 * h / 3 == w) return Cube2DMode2x3;
        throw Exception("Unable to create cube map: Invalid image dimension");
    }

    Texture* Texture::Convert2DToCube()
    {
        Auto<Bitmap> bmp = Faces[0].MipLevels[0]->ToBitmap();
        Cube2DMode mode = DetectCubeFrom2DMode(bmp->GetWidth(), bmp->GetHeight());

        int size;
        Recti coordsPosX, coordsNegX, coordsPosY, coordsNegY, coordsPosZ, coordsNegZ;

        switch (mode)
        {
        case Cube2DMode1x6:
            size = bmp->GetWidth();
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(0*size, 1*size, 1*size, 2*size);
            coordsPosY = Recti(0*size, 2*size, 1*size, 3*size);
            coordsNegY = Recti(0*size, 3*size, 1*size, 4*size);
            coordsPosZ = Recti(0*size, 4*size, 1*size, 5*size);
            coordsNegZ = Recti(0*size, 5*size, 1*size, 6*size);
            break;

        case Cube2DMode2x3:
            size = bmp->GetWidth() / 2;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(0*size, 1*size, 1*size, 2*size);
            coordsNegY = Recti(1*size, 1*size, 2*size, 2*size);
            coordsPosZ = Recti(0*size, 2*size, 1*size, 3*size);
            coordsNegZ = Recti(1*size, 2*size, 2*size, 3*size);
            break;

        case Cube2DMode3x2:
            size = bmp->GetHeight() / 2;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(2*size, 0*size, 3*size, 1*size);
            coordsNegY = Recti(0*size, 1*size, 1*size, 2*size);
            coordsPosZ = Recti(1*size, 1*size, 2*size, 2*size);
            coordsNegZ = Recti(2*size, 1*size, 3*size, 2*size);
            break;

        case Cube2DMode6x1:
            size = bmp->GetHeight();
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(2*size, 0*size, 3*size, 1*size);
            coordsNegY = Recti(3*size, 0*size, 4*size, 1*size);
            coordsPosZ = Recti(4*size, 0*size, 5*size, 1*size);
            coordsNegZ = Recti(5*size, 0*size, 6*size, 1*size);
            break;
        }

        Auto<Bitmap> posX = new Bitmap(size, size, bmp->GetFormat());
        Auto<Bitmap> negX = new Bitmap(size, size, bmp->GetFormat());
        Auto<Bitmap> posY = new Bitmap(size, size, bmp->GetFormat());
        Auto<Bitmap> negY = new Bitmap(size, size, bmp->GetFormat());
        Auto<Bitmap> posZ = new Bitmap(size, size, bmp->GetFormat());
        Auto<Bitmap> negZ = new Bitmap(size, size, bmp->GetFormat());

        posX->Blit(bmp, coordsPosX, Vector2i(0, 0));
        negX->Blit(bmp, coordsNegX, Vector2i(0, 0));
        posY->Blit(bmp, coordsPosY, Vector2i(0, 0));
        negY->Blit(bmp, coordsNegY, Vector2i(0, 0));
        posZ->Blit(bmp, coordsPosZ, Vector2i(0, 0));
        negZ->Blit(bmp, coordsNegZ, Vector2i(0, 0));

        return CreateCube(posX, negX, posY, negY, posZ, negZ);
    }

    Texture* Texture::ConvertCubeTo2D(Cube2DMode mode)
    {
        Auto<Bitmap> posX = Faces[0].MipLevels[0]->ToBitmap();
        Auto<Bitmap> negX = Faces[1].MipLevels[0]->ToBitmap();
        Auto<Bitmap> posY = Faces[2].MipLevels[0]->ToBitmap();
        Auto<Bitmap> negY = Faces[3].MipLevels[0]->ToBitmap();
        Auto<Bitmap> posZ = Faces[4].MipLevels[0]->ToBitmap();
        Auto<Bitmap> negZ = Faces[5].MipLevels[0]->ToBitmap();

        int size = posX->GetWidth();

        int width, height;
        Recti coordsPosX, coordsNegX, coordsPosY, coordsNegY, coordsPosZ, coordsNegZ;

        switch (mode)
        {
        case Cube2DMode1x6:
            width = 1*size;
            height = 6*size;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(0*size, 1*size, 1*size, 2*size);
            coordsPosY = Recti(0*size, 2*size, 1*size, 3*size);
            coordsNegY = Recti(0*size, 3*size, 1*size, 4*size);
            coordsPosZ = Recti(0*size, 4*size, 1*size, 5*size);
            coordsNegZ = Recti(0*size, 5*size, 1*size, 6*size);
            break;

        case Cube2DMode2x3:
            width = 2*size;
            height = 3*size;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(0*size, 1*size, 1*size, 2*size);
            coordsNegY = Recti(1*size, 1*size, 2*size, 2*size);
            coordsPosZ = Recti(0*size, 2*size, 1*size, 3*size);
            coordsNegZ = Recti(1*size, 2*size, 2*size, 3*size);
            break;

        case Cube2DMode3x2:
            width = 3*size;
            height = 2*size;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(2*size, 0*size, 3*size, 1*size);
            coordsNegY = Recti(0*size, 1*size, 1*size, 2*size);
            coordsPosZ = Recti(1*size, 1*size, 2*size, 2*size);
            coordsNegZ = Recti(2*size, 1*size, 3*size, 2*size);
            break;

        case Cube2DMode6x1:
            width = 6*size;
            height = 1*size;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(2*size, 0*size, 3*size, 1*size);
            coordsNegY = Recti(3*size, 0*size, 4*size, 1*size);
            coordsPosZ = Recti(4*size, 0*size, 5*size, 1*size);
            coordsNegZ = Recti(5*size, 0*size, 6*size, 1*size);
            break;
        }

        Auto<Bitmap> bmp = new Bitmap(width, height, posX->GetFormat());

        bmp->Blit(posX, posX->GetRect(), coordsPosX.TopLeft());
        bmp->Blit(negX, negX->GetRect(), coordsNegX.TopLeft());
        bmp->Blit(posY, posY->GetRect(), coordsPosY.TopLeft());
        bmp->Blit(negY, negY->GetRect(), coordsNegY.TopLeft());
        bmp->Blit(posZ, posZ->GetRect(), coordsPosZ.TopLeft());
        bmp->Blit(negZ, negZ->GetRect(), coordsNegZ.TopLeft());

        return Create(bmp);
    }
}
