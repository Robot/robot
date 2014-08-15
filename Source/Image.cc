////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2014 Robot Developers                       //
//                       See LICENSE for licensing info                       //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
// Prefaces                                                                   //
//----------------------------------------------------------------------------//

#include "Image.h"
#include "Color.h"
#include "Size.h"
#include "Point.h"

#include <cstring>
using std::memcpy;
using std::memcmp;

namespace Robot {



//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define RESET( target )				\
	(target).mWidth  = 0;			\
	(target).mHeight = 0;			\
	(target).mLength = 0;			\
	(target).mData   = nullptr;



//----------------------------------------------------------------------------//
// Constructors                                                         Image //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Image::Image (void)
{
	RESET (*this);
}

////////////////////////////////////////////////////////////////////////////////

Image::~Image (void)
{
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////

Image::Image (const Image& image)
{
	RESET (*this);
	Create (image.mWidth, image.mHeight);
	if (mLength != 0) memcpy (mData, image.
		mData, mLength * sizeof (uint32));
}

////////////////////////////////////////////////////////////////////////////////

Image::Image (Image&& image)
{
	mWidth  = image.mWidth;
	mHeight = image.mHeight;
	mLength = image.mLength;
	mData   = image.mData;
	RESET (image);
}

////////////////////////////////////////////////////////////////////////////////

Image::Image (const Size& size)
{
	RESET (*this);
	Create (size);
}

////////////////////////////////////////////////////////////////////////////////

Image::Image (uint16 w, uint16 h)
{
	RESET (*this);
	Create (w, h);
}



//----------------------------------------------------------------------------//
// Functions                                                            Image //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Image::IsValid (void) const
{
	return mData != nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void Image::Create (const Size& size)
{
	Create (size.W, size.H);
}

////////////////////////////////////////////////////////////////////////////////

void Image::Create (uint16 w, uint16 h)
{
	// Don't accept empty values
	if (w == 0 || h == 0) return;

	// Attempt to reuse the memory
	if (mLength < (uint32) w * h)
		Destroy();

	mWidth  = w;
	mHeight = h;

	// Check if allocated
	if (mData == nullptr)
	{
		mLength = w * h;
		mData = new uint32[mLength];
	}
}

////////////////////////////////////////////////////////////////////////////////

void Image::Destroy (void)
{
	mWidth  = 0;
	mHeight = 0;
	mLength = 0;

	if (mData != nullptr)
	{
		delete[] mData;
		mData = nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////

uint16 Image::GetWidth (void) const
{
	return mWidth;
}

////////////////////////////////////////////////////////////////////////////////

uint16 Image::GetHeight (void) const
{
	return mHeight;
}

////////////////////////////////////////////////////////////////////////////////

uint32 Image::GetLength (void) const
{
	return mWidth * mHeight;
}

////////////////////////////////////////////////////////////////////////////////

uint32* Image::GetData (void) const
{
	return mData;
}

////////////////////////////////////////////////////////////////////////////////

Color Image::GetPixel (const Point& point) const
{
	return GetPixel (point.X, point.Y);
}

////////////////////////////////////////////////////////////////////////////////

Color Image::GetPixel (uint16 x, uint16 y) const
{
	// Boundary check
	if (x >= mWidth ||
		y >= mHeight)
		return Color();

	// Return color at specified coordinate
	return Color (mData[x + (y * mWidth)]);
}



//----------------------------------------------------------------------------//
// Operators                                                            Image //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Image& Image::operator = (const Image& image)
{
	if (this != &image)
	{
		Destroy();
		Create (image.mWidth, image.mHeight);
		if (mLength != 0) memcpy (mData, image.
			mData, mLength * sizeof (uint32));
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

Image& Image::operator = (Image&& image)
{
	if (this != &image)
	{
		Destroy();
		mWidth  = image.mWidth;
		mHeight = image.mHeight;
		mLength = image.mLength;
		mData   = image.mData;
		RESET (image);
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

bool Image::operator == (const Image& image) const
{
	return mWidth  == image.mWidth  &&
		   mHeight == image.mHeight &&
		   memcmp (mData, image.mData, mWidth * mHeight * sizeof (uint32)) == 0;
}

////////////////////////////////////////////////////////////////////////////////

bool Image::operator != (const Image& image) const
{
	return mWidth  != image.mWidth  ||
		   mHeight != image.mHeight ||
		   memcmp (mData, image.mData, mWidth * mHeight * sizeof (uint32)) != 0;
}

} // namespace Robot
