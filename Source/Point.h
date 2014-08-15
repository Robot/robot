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

#ifndef ROBOT_POINT_H
#define ROBOT_POINT_H

#include "Global.h"
namespace Robot {
	class Size;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Point
{
public:
	// Constructors
	Point						(int32 value  = 0);
	Point						(int32 x, int32 y);

public:
	// Functions
	bool		IsZero			(void) const;
	Size		ToSize			(void) const;

public:
	// Operators
	Point&		operator +=		(const Point& point);
	Point&		operator -=		(const Point& point);

	Point		operator +		(const Point& point) const;
	Point		operator -		(const Point& point) const;

	bool		operator ==		(const Point& point) const;
	bool		operator !=		(const Point& point) const;

	Point		operator -		(void) const;

public:
	// Properties
	int32		X;				// Point X coordinate
	int32		Y;				// Point Y coordinate
};

} // namespace Robot
#endif // ROBOT_POINT_H
