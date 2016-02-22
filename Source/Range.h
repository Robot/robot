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



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Range
{
public:
	Range							(int32 value = 0);
	Range							(int32 minimum,
									 int32 maximum);

public:
	int32			GetRange		(void) const;
	void			SetRange		(int32 value);
	void			SetRange		(int32 minimum,
									 int32 maximum);

	int32			GetRandom		(void) const;
	bool			Contains		(int32 value,
									 bool inclusive = true) const;

public:
	bool			operator ==		(const Range& range) const;
	bool			operator !=		(const Range& range) const;

public:
	int32			Min;			// Minimum range value
	int32			Max;			// Maximum range value

private:
	mutable uint32	mState;			// Random generator state
};

ROBOT_NS_END
