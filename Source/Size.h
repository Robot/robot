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
	class Point;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Size
{
public:
	Size						(int32 value  = 0);
	Size						(int32 w, int32 h);

public:
	bool		IsZero			(void) const;
	bool		IsEmpty			(void) const;
	Point		ToPoint			(void) const;

public:
	Size&		operator +=		(const Size& size);
	Size&		operator -=		(const Size& size);

	Size		operator +		(const Size& size) const;
	Size		operator -		(const Size& size) const;

	bool		operator ==		(const Size& size) const;
	bool		operator !=		(const Size& size) const;

public:
	int32		W;				// Width  of the size
	int32		H;				// Height of the size
};

ROBOT_NS_END
