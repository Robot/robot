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

#ifndef ROBOT_POINT_H
#define ROBOT_POINT_H

#include "Global.h"
ROBOT_NS_BEGIN
	class Size;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Point
{
public:
	Point						(int32 value  = 0);
	Point						(int32 x, int32 y);

public:
	bool		IsZero			(void) const;
	Size		ToSize			(void) const;

public:
	Point&		operator +=		(const Point& point);
	Point&		operator -=		(const Point& point);

	Point		operator +		(const Point& point) const;
	Point		operator -		(const Point& point) const;

	bool		operator ==		(const Point& point) const;
	bool		operator !=		(const Point& point) const;

	Point		operator -		(void) const;

public:
	int32		X;				// Point X coordinate
	int32		Y;				// Point Y coordinate
};

ROBOT_NS_END
#endif // ROBOT_POINT_H
