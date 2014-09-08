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

#include "Test.h"



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool TestTaskbar (void)
{
	cout << "BEGIN TASKBAR TESTING\n------------------------------\n";

#if defined (ROBOT_OS_LINUX) || defined (ROBOT_OS_MAC)

	// This should always return false
	VERIFY (!Taskbar::Synchronize());

	// Check for default Taskbar values
	VERIFY ( Taskbar::GetBounds  () == Bounds(0,0,0,0));
	VERIFY ( Taskbar::GetLocation() == Taskbar::Bottom);
	VERIFY (!Taskbar::IsAutoHide ());

#endif
#ifdef ROBOT_OS_WIN

	VERIFY (Taskbar::Synchronize());

	auto b = Taskbar::GetBounds  ();
	auto l = Taskbar::GetLocation();
	auto h = Taskbar::IsAutoHide ();

	VERIFY (Taskbar::Synchronize());

	VERIFY (Taskbar::GetBounds  () == b);
	VERIFY (Taskbar::GetLocation() == l);
	VERIFY (Taskbar::IsAutoHide () == h);

	cout << "Warning: Taskbar testing cannot be automated\n"
		 << "         Please review the following results\n";

	cout << "Taskbar @ ";

	switch (l)
	{
		case Taskbar::Left   : cout << "Left"  ; break;
		case Taskbar::Top    : cout << "Top"   ; break;
		case Taskbar::Right  : cout << "Right" ; break;
		case Taskbar::Bottom : cout << "Bottom"; break;
	}

	cout << " ("
		 << b.X << ", " << b.Y << ", "
		 << b.W << ", " << b.H << ") "
		 << (h ? "Auto-Hides\n\n" : "\n");
#endif

	cout << ">> Success\n\n"; return true;
}
