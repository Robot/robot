////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2017 Robot Developers                       //
//                       See LICENSE for licensing info                       //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
// Prefaces                                                                   //
//----------------------------------------------------------------------------//

#include "System.h"
#ifdef ROBOT_OS_LINUX

	#include <unistd.h>

#endif
#ifdef ROBOT_OS_MAC

	#include <unistd.h>

#endif
#ifdef ROBOT_OS_WIN

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

#endif
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Functions                                                           System //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool System::IsUserAdmin (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return false;

#endif
}

ROBOT_NS_END
