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

bool System::Is64Bit (void)
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

////////////////////////////////////////////////////////////////////////////////

bool System::IsAxEnabled (bool options)
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

////////////////////////////////////////////////////////////////////////////////

uintptr System::GetMinAddress (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

uintptr System::GetMaxAddress32 (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

uintptr System::GetMaxAddress64 (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

uintptr System::GetPageSize (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool System::IsCompositing (void)
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

////////////////////////////////////////////////////////////////////////////////

void System::SetCompositing (bool enabled)
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

uint32 System::GetCpuCount (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

uintptr System::GetRamCount (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return 0;

#endif
}



//----------------------------------------------------------------------------//
// Internal                                                            System //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void System::InitializeMemory (void)
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

ROBOT_NS_END
