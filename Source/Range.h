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

#ifndef ROBOT_RANGE_H
#define ROBOT_RANGE_H

#include "Global.h"
namespace Robot {



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Range
{
public:
	// Constructors
	Range						(int32 value = 0);
	Range						(int32 minimum,
								 int32 maximum);

public:
	// Functions
	int32		GetRange		(void) const;
	void		SetRange		(int32 value);
	void		SetRange		(int32 minimum,
								 int32 maximum);

	int32		GetRandom		(void) const;
	bool		Contains		(int32 value,
								 bool inclusive = true) const;

public:
	// Operators
	bool		operator ==		(const Range& range) const;
	bool		operator !=		(const Range& range) const;

public:
	// Properties
	int32		Min;			// Minimum range value
	int32		Max;			// Maximum range value

private:
	// Fields
	uint32		mState;			// Random generator state
};

} // namespace Robot
#endif // ROBOT_RANGE_H
