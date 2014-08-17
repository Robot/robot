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

#include "Taskbar.h"
#ifdef ROBOT_OS_WIN

	#define NOMINMAX
	#include <Windows.h>

#endif
namespace Robot {



//----------------------------------------------------------------------------//
// Fields                                                             Taskbar //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Bounds Taskbar::mBounds;

////////////////////////////////////////////////////////////////////////////////

Taskbar::Location Taskbar::mLocation = Bottom;

////////////////////////////////////////////////////////////////////////////////

bool Taskbar::mAutoHide = false;



//----------------------------------------------------------------------------//
// Functions                                                          Taskbar //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Taskbar::Synchronize (void)
{
#ifdef ROBOT_OS_LINUX

	return false;

#endif
#ifdef ROBOT_OS_MAC

	return false;

#endif
#ifdef ROBOT_OS_WIN

	HWND handle = FindWindow (TEXT ("Shell_TrayWnd"), 0);
	if (handle == nullptr) return false;

	APPBARDATA data = { 0 };
	data.cbSize = sizeof (APPBARDATA); data.hWnd = handle;
	if (SHAppBarMessage (ABM_GETTASKBARPOS, &data) == 0)
		return false;

	mLocation = (Taskbar::Location) data.uEdge;
	mBounds.SetLTRB (data.rc.left, data.rc.top,
				data.rc.right, data.rc.bottom);

	UINT_PTR state = SHAppBarMessage (ABM_GETSTATE, &data);
	mAutoHide = (state & ABS_AUTOHIDE) != 0; return true;

#endif
}

} // namespace Robot
