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

#ifndef ROBOT_IMAGE_H
#define ROBOT_IMAGE_H

#include "Global.h"
namespace Robot {
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
				~Image			(void);

				 Image			(const Image&  image);
				 Image			(      Image&& image);

	explicit	 Image			(const  Size&  size);
	explicit	 Image			(uint16 w, uint16 h);

public:
	bool		IsValid			(void) const;

	void		Create			(const  Size&  size);
	void		Create			(uint16 w, uint16 h);
	void		Destroy			(void);

	uint16		GetWidth		(void) const;
	uint16		GetHeight		(void) const;
	uint32		GetLength		(void) const;
	uint32*		GetData			(void) const;

	Color		GetPixel		(const Point& point) const;
	Color		GetPixel		(uint16 x, uint16 y) const;

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
};

}
#endif // ROBOT_IMAGE_H
