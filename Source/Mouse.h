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

#include "Types.h"
#include <unordered_map>
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Types                                                                      //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

enum Button
{
	ButtonLeft		= 0,
	ButtonMid		= 1,
	ButtonMiddle	= 1,
	ButtonRight		= 2,
	ButtonX1		= 3,
	ButtonX2		= 4,
};

////////////////////////////////////////////////////////////////////////////////

#if defined (BUILDING_ROBOT_SHARED) || \
	defined (   USING_ROBOT_SHARED)
	template class ROBOT_EXPORT Enum<Button>;
#endif

////////////////////////////////////////////////////////////////////////////////

typedef std::unordered_map<Button, bool, std::hash<int>> ButtonState;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Mouse
{
public:
	Mouse							(void);

public:
	void			Click			(Button button) const;
	void			Press			(Button button) const;
	void			Release			(Button button) const;

	void			ScrollH			(int32 amount) const;
	void			ScrollV			(int32 amount) const;

	static Point	GetPos			(void);
	static void		SetPos			(const Point& point);
	static void		SetPos			(uint32 x, uint32 y);

	static bool		GetState		(Button       button);
	static bool		GetState		(ButtonState& result);

public:
	Range			AutoDelay;		// Auto delay range
};

ROBOT_NS_END
