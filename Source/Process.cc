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

#include "Process.h"
#include "Memory.h"
#include "Module.h"
#include "Window.h"

#include <algorithm>
using std::string;
using std::replace;

#include <regex>
using std::regex;
using std::regex_match;

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
// Constructors                                                       Process //
//----------------------------------------------------------------------------//

Process::Process (int32 pid)
{
	// TODO:
}



//----------------------------------------------------------------------------//
// Functions                                                          Process //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Process::Open (int32 pid)
{
	// Close opened process
	if (IsValid()) Close();

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

void Process::Close (void)
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

bool Process::IsValid (void) const
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

bool Process::Is64Bit (void) const
{
	// Check the process validity
	if (!IsValid()) return false;

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

int32 Process::GetPID (void) const
{
	// Check process validity
	if (!IsValid()) return 0;

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

uintptr Process::GetHandle (void) const
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

Memory Process::GetMemory (void) const
{
	return Memory();

	// NYI: Not yet implemented
}

////////////////////////////////////////////////////////////////////////////////

string Process::GetName (void) const
{
	// TODO:
	return string();
}

////////////////////////////////////////////////////////////////////////////////

string Process::GetPath (void) const
{
	// Check if the process is valid
	if (!IsValid()) return string();

#ifdef ROBOT_OS_LINUX

	// TODO:
	return string();

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return string();

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return string();

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Process::Exit (void)
{
	// Check process validity
	if (!IsValid()) return;

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

void Process::Kill (void)
{
	// Check process validity
	if (!IsValid()) return;

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

bool Process::HasExited (void) const
{
	// Check the process validity
	if (!IsValid()) return true;

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

ModuleList Process::GetModules (const char* name) const
{
	ModuleList result;
	// Check the process validity
	if (!IsValid()) return result;

#ifdef ROBOT_OS_LINUX

	// NYI: Not yet implemented
	return result;

#endif
#ifdef ROBOT_OS_MAC

	// NYI: Not yet implemented
	return result;

#endif
#ifdef ROBOT_OS_WIN

	// NYI: Not yet implemented
	return result;

#endif
}

////////////////////////////////////////////////////////////////////////////////

WindowList Process::GetWindows (const char* title) const
{
	WindowList result;
	// Check the process validity
	if (!IsValid()) return result;

#ifdef ROBOT_OS_LINUX

	// NYI: Not yet implemented
	return result;

#endif
#ifdef ROBOT_OS_MAC

	// NYI: Not yet implemented
	return result;

#endif
#ifdef ROBOT_OS_WIN

	// NYI: Not yet implemented
	return result;

#endif
}

////////////////////////////////////////////////////////////////////////////////

ProcessList Process::GetList (const char* name)
{
	ProcessList result;

#ifdef ROBOT_OS_LINUX

	// TODO:
	return result;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return result;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return result;

#endif
}

////////////////////////////////////////////////////////////////////////////////

Process Process::GetCurrent (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return Process();

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return Process();

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return Process();

#endif
}



//----------------------------------------------------------------------------//
// Operators                                                          Process //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Process::operator == (const Process& process) const
{
	return GetPID() == process.GetPID();
}

////////////////////////////////////////////////////////////////////////////////

bool Process::operator != (const Process& process) const
{
	return GetPID() != process.GetPID();
}

////////////////////////////////////////////////////////////////////////////////

bool Process::operator == (int32 pid) const
{
	return GetPID() == pid;
}

////////////////////////////////////////////////////////////////////////////////

bool Process::operator != (int32 pid) const
{
	return GetPID() != pid;
}

} // namespace Robot
