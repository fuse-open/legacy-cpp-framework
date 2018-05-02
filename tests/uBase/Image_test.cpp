#include "../catch.hpp"
#include <uBase/Buffer.h>
#include <uBase/BufferStream.h>
#include <uBase/Memory.h>
#include <uImage/Png.h>
#include <uImage/Jpeg.h>
#include <uImage/Texture.h>
#include <fstream>
#include "ExampleImage.h"

using namespace uBase;

static void TestJpegMemoryLeak(unsigned char *imageRaw, int size)
{
	uBase::Auto<uBase::BufferPtr> bp = new uBase::BufferPtr(imageRaw, size, false);
	uBase::Auto<uBase::BufferStream> bs = new uBase::BufferStream(bp, true, false);
	uBase::Auto<uImage::ImageReader> ir = uImage::Jpeg::CreateReader(bs);
	uBase::Auto<uImage::Bitmap> bmp = ir->ReadBitmap();
	{
		uBase::Auto<uImage::Texture> tex = uImage::Texture::Create(bmp);
	}
	
	CHECK(bmp->GetRefCount() == 1);
}

TEST_CASE("LoadJpegMemLeakTest")
{
	TestJpegMemoryLeak(tux, sizeof(tux));
}