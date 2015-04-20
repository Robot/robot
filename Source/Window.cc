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

#include "Window.h"
#include "Process.h"
using std::string;

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
// Constructors                                                        Window //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Window::Window (uintptr handle)
{
	// TODO:
}



//----------------------------------------------------------------------------//
// Functions                                                           Window //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Window::IsValid (void) const
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

void Window::Close (void)
{
	// Check window validity
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

bool Window::IsTopMost (void) const
{
	// Check the window validity
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

bool Window::IsBorderless (void) const
{
	// Check the window validity
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

bool Window::IsMinimized (void) const
{
	// Check the window validity
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

bool Window::IsMaximized (void) const
{
	// Check the window validity
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

void Window::SetTopMost (bool state)
{
	// Check window validity
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

void Window::SetBorderless (bool state)
{
	// Check window validity
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

void Window::SetMinimized (bool state)
{
	// Check window validity
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

void Window::SetMaximized (bool state)
{
	// Check window validity
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

Process Window::GetProcess (void) const
{
	// TODO:
	return Process();
}

////////////////////////////////////////////////////////////////////////////////

int32 Window::GetPID (void) const
{
	// Check window validity
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

uintptr Window::GetHandle (void) const
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

bool Window::SetHandle (uintptr handle)
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

uintptr Window::GetHandleAx (void) const
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

string Window::GetTitle (void) const
{
	// Check if the window is valid
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

void Window::SetTitle (const char* title)
{
	// Check window validity
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

Bounds Window::GetBounds (void) const
{
	// Check if the window is valid
	if (!IsValid()) return Bounds();

#ifdef ROBOT_OS_LINUX

	// TODO:
	return Bounds();

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return Bounds();

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return Bounds();

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetBounds (const Bounds& bounds)
{
	// TODO:
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetBounds (int32 x,
	  int32 y, int32 w, int32 h)
{
	// Check window validity
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

Bounds Window::GetClient (void) const
{
	// Check if the window is valid
	if (!IsValid()) return Bounds();

#ifdef ROBOT_OS_LINUX

	// TODO:
	return Bounds();

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return Bounds();

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return Bounds();

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetClient (const Bounds& bounds)
{
	// TODO:
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetClient (int32 x,
	  int32 y, int32 w, int32 h)
{
	// Check window validity
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

Point Window::MapToClient (const Point& point) const
{
	// Check if the window is valid
	if (!IsValid()) return Point();

	// TODO:
	return Point();
}

////////////////////////////////////////////////////////////////////////////////

Point Window::MapToScreen (const Point& point) const
{
	// Check if the window is valid
	if (!IsValid()) return Point();

	// TODO:
	return Point();

}

////////////////////////////////////////////////////////////////////////////////

Bounds Window::MapToClient (const Bounds& bounds) const
{
	// Check if the window is valid
	if (!IsValid()) return Bounds();

	// TODO:
	return Bounds();

}

////////////////////////////////////////////////////////////////////////////////

Bounds Window::MapToScreen (const Bounds& bounds) const
{
	// Check if the window is valid
	if (!IsValid()) return Bounds();

	// TODO:
	return Bounds();
}

////////////////////////////////////////////////////////////////////////////////

WindowList Window::GetList (const char* title)
{
	WindowList result;

	// TODO:
	return result;
}

////////////////////////////////////////////////////////////////////////////////

Window Window::GetActive (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return Window();

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return Window();

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return Window();

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetActive (const Window& window)
{
	// Check if the window is valid
	if (!window.IsValid()) return;

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

bool Window::IsAxEnabled (bool options)
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

WindowList Window::GetList (const char* title, int32 pid)
{
	WindowList result;

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



//----------------------------------------------------------------------------//
// Operators                                                           Window //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Window::operator == (const Window& window) const
{
	return GetHandle() == window.GetHandle();
}

////////////////////////////////////////////////////////////////////////////////

bool Window::operator != (const Window& window) const
{
	return GetHandle() != window.GetHandle();
}

////////////////////////////////////////////////////////////////////////////////

bool Window::operator == (uintptr handle) const
{
	return GetHandle() == handle;
}

////////////////////////////////////////////////////////////////////////////////

bool Window::operator != (uintptr handle) const
{
	return GetHandle() != handle;
}

}
