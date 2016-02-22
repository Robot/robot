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

#pragma once

#include "Global.h"
ROBOT_NS_BEGIN
	class Color;
	class Size;
	class Point;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Image
{
public:
				 Image			(void);
	virtual		~Image			(void);

				 Image			(const Image&  image);
				 Image			(      Image&& image);

	explicit	 Image			(const  Size&  size);
	explicit	 Image			(uint16 w, uint16 h);

public:
	bool		IsValid			(void) const;

	bool		Create			(const  Size&  size);
	bool		Create			(uint16 w, uint16 h);
	void		Destroy			(void);

	uint16		GetWidth		(void) const { return mWidth;  }
	uint16		GetHeight		(void) const { return mHeight; }
	uint32		GetLength		(void) const { return mLength; }
	uint32*		GetData			(void) const { return mData;   }
	uint32		GetLimit		(void) const { return mLimit;  }

	Color		GetPixel		(const Point& point) const;
	Color		GetPixel		(uint16 x, uint16 y) const;

	void		SetPixel		(const Point& point, Color c);
	void		SetPixel		(uint16 x, uint16 y, Color c);

	bool		Fill			(const Color& color);
	bool		Fill			(uint8 r, uint8 g,
								 uint8 b, uint8 a = 255);

	bool		Swap			(const char* sw);
	bool		Flip			(bool h, bool v);

private:
	void		Flip			(void);
	void		FlipH			(void);
	void		FlipV			(void);

public:
	Image&		operator =		(const Image&  image);
	Image&		operator =		(      Image&& image);

	bool		operator ==		(const Image& image) const;
	bool		operator !=		(const Image& image) const;

private:
	uint16		mWidth;			// Bitmap width
	uint16		mHeight;		// Bitmap height
	uint32		mLength;		// Pixel data length

	uint32*		mData;			// Pixel data (ARGB)
	uint32		mLimit;			// Pixel data limit
};

ROBOT_NS_END
