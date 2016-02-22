////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2016 Robot Developers                       //
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

ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define RESET( target )			\
	(target).mWidth  = 0;		\
	(target).mHeight = 0;		\
	(target).mLength = 0;		\
	(target).mData   = nullptr;	\
	(target).mLimit  = 0;



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
	mLimit  = image.mLimit;
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

bool Image::Create (const Size& size)
{
	return Create (size.W, size.H);
}

////////////////////////////////////////////////////////////////////////////////

bool Image::Create (uint16 w, uint16 h)
{
	// Verify dimensions
	if (w == 0 || h == 0)
		return false;

	mWidth  = w;
	mHeight = h;
	mLength = (uint32) w * h;

	// Can we reuse memory
	if (mLimit < mLength)
	{
		if (mData != nullptr)
			delete[] mData;

		// Allocate memory
		mData = new uint32
			[mLimit = mLength];
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

void Image::Destroy (void)
{
	if (mData != nullptr)
		delete[] mData;

	RESET (*this);
}

////////////////////////////////////////////////////////////////////////////////

Color Image::GetPixel (const Point& point) const
{
	return GetPixel (point.X, point.Y);
}

////////////////////////////////////////////////////////////////////////////////

Color Image::GetPixel (uint16 x, uint16 y) const
{
	// Perform simple boundary check
	if (x >= mWidth || y >= mHeight)
		return Color();

	// Return color at the specified coordinate
	return ((Color*) mData) [x + (y * mWidth)];
}

////////////////////////////////////////////////////////////////////////////////

void Image::SetPixel (const Point& point, Color c)
{
	return SetPixel (point.X, point.Y, c);
}

////////////////////////////////////////////////////////////////////////////////

void Image::SetPixel (uint16 x, uint16 y, Color c)
{
	// Perform simple boundary check
	if (x >= mWidth || y >= mHeight)
		return;

	// Set color at the specified coordinate
	((Color*) mData) [x + (y * mWidth)] = c;
}

////////////////////////////////////////////////////////////////////////////////

bool Image::Fill (const Color& color)
{
	// Check the validity of this image
	if (mData == nullptr) return false;

	// Loop data and fill contents
	uint32 argb = color.GetARGB();
	for (uint32 i = 0; i < mLength; ++i)
		mData[i] = argb; return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Image::Fill (uint8 r, uint8 g, uint8 b, uint8 a)
{
	return Fill (Color (r, g, b, a));
}

////////////////////////////////////////////////////////////////////////////////

bool Image::Swap (const char* sw)
{
	// Verify parameters
	if (mData == nullptr ||
		sw    == nullptr)
		return false;

	int8 a = -1; int8 r = -1;
	int8 g = -1; int8 b = -1;
	int8 count;

	// Parse and validate the switch parameter
	for (count = 0; sw[count] != '\0'; ++count)
	{
		char c = sw[count] | 32; // Lower case

			 if (c == 'a' && a == -1) a = (3-count) << 3;
		else if (c == 'r' && r == -1) r = (3-count) << 3;
		else if (c == 'g' && g == -1) g = (3-count) << 3;
		else if (c == 'b' && b == -1) b = (3-count) << 3;
		else return false;
	}

	// Check for missing channels
	if (count != 4) return false;
	Color* data = (Color*) mData;
	// Loop data and perform the switch
	for (uint32 i = 0; i < mLength; ++i)
	{
		mData[i] = (data[i].A << a) |
				   (data[i].R << r) |
				   (data[i].G << g) |
				   (data[i].B << b);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Image::Flip (bool h, bool v)
{
	// Check the validity of this image
	if (mData == nullptr) return false;
	if ( h &&  v) Flip ();
	if ( h && !v) FlipH();
	if (!h &&  v) FlipV();
	return true;
}



//----------------------------------------------------------------------------//
// Internal                                                             Image //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void Image::Flip (void)
{
	uint32 length = mLength / 2;
	// Loop data and perform mirroring
	for (uint32 i = 0; i < length; ++i)
	{
		uint32 f = mLength - 1 - i;
		uint32 c = mData[i];
		mData[i] = mData[f];
		mData[f] = c;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Image::FlipH (void)
{
	uint16 width = mWidth / 2;
	// Loop data and perform mirroring
	for (uint32 y = 0; y < mHeight; ++y)
	{
		for (uint32 x = 0; x < width; ++x)
		{
			uint32 f = mWidth - 1 - x;
			uint32 a = mData[x + (y * mWidth)];
			uint32 b = mData[f + (y * mWidth)];
			mData[x + (y * mWidth)] = b;
			mData[f + (y * mWidth)] = a;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void Image::FlipV (void)
{
	uint16 height = mHeight / 2;
	// Loop data and perform the mirror
	for (uint32 y = 0; y < height; ++y)
	{
		uint32 f = mHeight - 1 - y;
		for (uint32 x = 0; x < mWidth; ++x)
		{
			uint32 a = mData[x + (y * mWidth)];
			uint32 b = mData[x + (f * mWidth)];
			mData[x + (y * mWidth)] = b;
			mData[x + (f * mWidth)] = a;
		}
	}
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
		mLimit  = image.mLimit;
		RESET (image);
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

bool Image::operator == (const Image& image) const
{
	return mWidth  == image.mWidth  &&
		   mHeight == image.mHeight &&
		   memcmp (mData, image.mData, mLength * sizeof (uint32)) == 0;
}

////////////////////////////////////////////////////////////////////////////////

bool Image::operator != (const Image& image) const
{
	return mWidth  != image.mWidth  ||
		   mHeight != image.mHeight ||
		   memcmp (mData, image.mData, mLength * sizeof (uint32)) != 0;
}

ROBOT_NS_END
