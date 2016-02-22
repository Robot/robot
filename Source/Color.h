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

class ROBOT_EXPORT Color
{
public:
	Color						(uint32 argb = 0);
	Color						(uint8 r, uint8 g,
								 uint8 b, uint8 a = 255);

public:
	uint32		GetARGB			(void) const;
	void		SetARGB			(uint32 argb);

public:
	bool		operator ==		(const Color& color) const;
	bool		operator !=		(const Color& color) const;

public:
	uint8		B;				// Color blue  channel
	uint8		G;				// Color green channel
	uint8		R;				// Color red   channel
	uint8		A;				// Color alpha channel
};

ROBOT_NS_END
