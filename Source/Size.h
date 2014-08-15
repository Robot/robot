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

#ifndef ROBOT_SIZE_H
#define ROBOT_SIZE_H

#include "Global.h"
namespace Robot {
	class Point;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Size
{
public:
	// Constructors
	Size						(int32 value  = 0);
	Size						(int32 w, int32 h);

public:
	// Functions
	bool		IsEmpty			(void) const;
	Point		ToPoint			(void) const;

public:
	// Operators
	Size&		operator +=		(const Size& size);
	Size&		operator -=		(const Size& size);

	Size		operator +		(const Size& size) const;
	Size		operator -		(const Size& size) const;

	bool		operator ==		(const Size& size) const;
	bool		operator !=		(const Size& size) const;

public:
	// Properties
	int32		W;				// Width  of the size
	int32		H;				// Height of the size
};

} // namespace Robot
#endif // ROBOT_SIZE_H
