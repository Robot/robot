////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2015 Robot Developers                       //
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

void Image::Create (const Size& size)
{
	Create (size.W, size.H);
}

////////////////////////////////////////////////////////////////////////////////

void Image::Create (uint16 w, uint16 h)
{
	// Don't accept empty values
	if (w == 0 || h == 0) return;

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

void Image::Fill (const Color& color)
{
	Fill (color.R, color.G,
		  color.B, color.A);
}

////////////////////////////////////////////////////////////////////////////////

void Image::Fill (uint8 r, uint8 g, uint8 b, uint8 a)
{
	// Check if the image is valid
	if (mData == nullptr) return;
	Color* data = (Color*) mData;

	// Loop data and perform fill function
	for (uint32 i = 0; i < mLength; ++i)
	{
		data[i].R = r;
		data[i].G = g;
		data[i].B = b;
		data[i].A = a;
	}
}

////////////////////////////////////////////////////////////////////////////////

bool Image::Switch (const char* sw, Image* result) const
{
	// Check all parameters
	if (mData  == nullptr ||
		sw     == nullptr ||
		result == nullptr)
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

	// Allocate memory on new image
	result->Create (mWidth, mHeight);

	 Color* tData = (Color*) mData;
	uint32* rData =  result->mData;

	// Loop data and perform switch copy
	for (uint32 i = 0; i < mLength; ++i)
	{
		rData[i] = (tData[i].A << a) |
				   (tData[i].R << r) |
				   (tData[i].G << g) |
				   (tData[i].B << b);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Image::Mirror (bool h, bool v, Image* result) const
{
	// Check whether all parameters are valid
	if (mData == nullptr || result == nullptr)
		return false;

	// Allocate memory on new image
	result->Create (mWidth, mHeight);

	uint32* tData =   this->mData;
	uint32* rData = result->mData;

	// Loop data and perform mirror copy
	for (uint32 y = 0; y < mHeight; ++y)
	{
		uint32 yy = v ? mHeight - 1 - y : y;
		for (uint32 x = 0; x < mWidth; ++x)
		{
			uint32 xx = h ? mWidth - 1 - x : x;
			rData[x  + (y  * mWidth)] =
			tData[xx + (yy * mWidth)];
		}
	}

	return true;
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
