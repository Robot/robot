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

#include "Keyboard.h"
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
// Constructors                                                      Keyboard //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Keyboard::Keyboard (void)
{
	AutoDelay.Min = 40;
	AutoDelay.Max = 90;
}



//----------------------------------------------------------------------------//
// Functions                                                         Keyboard //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void Keyboard::Click (Key keycode) const
{
	Press   (keycode);
	Release (keycode);
}

////////////////////////////////////////////////////////////////////////////////

void Keyboard::Press (Key keycode) const
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

void Keyboard::Release (Key keycode) const
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

bool Keyboard::Click (const char* keys) const
{
	// TODO:
	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Keyboard::Compile (const char* keys, KeyList& result)
{
	// TODO:
	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Keyboard::GetState (Key keycode)
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

bool Keyboard::GetState (KeyState& result)
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

void Keyboard::Delay (const Range& delay)
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
