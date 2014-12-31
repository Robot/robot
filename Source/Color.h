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

#ifndef ROBOT_COLOR_H
#define ROBOT_COLOR_H

#include "Global.h"
namespace Robot {



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Color
{
public:
	// Constructors
	Color						(uint32 argb = 0);
	Color						(uint8 r, uint8 g,
								 uint8 b, uint8 a = 255);

public:
	// Functions
	uint32		GetARGB			(void) const;
	void		SetARGB			(uint32 argb);

public:
	// Operators
	bool		operator ==		(const Color& color) const;
	bool		operator !=		(const Color& color) const;

public:
	// Properties
	uint8		A;				// Color alpha channel
	uint8		R;				// Color red   channel
	uint8		G;				// Color green channel
	uint8		B;				// Color blue  channel
};

} // namespace Robot
#endif // ROBOT_COLOR_H
