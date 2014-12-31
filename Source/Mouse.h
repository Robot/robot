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

#ifndef ROBOT_MOUSE_H
#define ROBOT_MOUSE_H

#include "Types.h"
#include <unordered_map>
namespace Robot {



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

#if defined (BUILDING_ROBOT_SHARED) || defined (USING_ROBOT_SHARED)
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
	// Constructors
	Mouse							(void);

public:
	// Functions
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

	static void		Delay			(const Range& delay);
	static void		Delay			(uint32 minimum,
									 uint32 maximum);

public:
	// Properties
	Range			AutoDelay;		// Auto delay range
};

} // namespace Robot
#endif // ROBOT_MOUSE_H
