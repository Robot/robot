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

#include "Mouse.h"
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
namespace Robot {



//----------------------------------------------------------------------------//
// Constructors                                                         Mouse //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Mouse::Mouse (void)
{
	AutoDelay.Min = 40;
	AutoDelay.Max = 90;
}



//----------------------------------------------------------------------------//
// Functions                                                            Mouse //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void Mouse::Click (Button button) const
{
	Press   (button);
	Release (button);
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::Press (Button button) const
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif

	Delay (AutoDelay);
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::Release (Button button) const
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif

	Delay (AutoDelay);
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::ScrollH (int32 amount) const
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif

	Delay (AutoDelay);
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::ScrollV (int32 amount) const
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif

	Delay (AutoDelay);
}

////////////////////////////////////////////////////////////////////////////////

Point Mouse::GetPos (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return Point();

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return Point();

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return Point();

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::SetPos (const Point& point)
{
	SetPos (point.X, point.Y);
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::SetPos (uint32 x, uint32 y)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Mouse::GetState (Button button)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif

	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Mouse::GetState (ButtonState& result)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif

	return false;
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::Delay (const Range& delay)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

} // namespace Robot
