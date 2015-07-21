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

#include "Screen.h"
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
// Cannot be included as part of the Screen class due to the use of std::deque.

static ScreenList gScreens;



//----------------------------------------------------------------------------//
// Fields                                                              Screen //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Bounds Screen::mTotalBounds;
Bounds Screen::mTotalUsable;



//----------------------------------------------------------------------------//
// Constructors                                                        Screen //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Screen::Screen (void)
{
}

////////////////////////////////////////////////////////////////////////////////

Screen::Screen (Bounds bounds, Bounds usable)
{
	mBounds = bounds;
	mUsable = usable;
}



//----------------------------------------------------------------------------//
// Functions                                                           Screen //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Bounds Screen::GetBounds   (void) const { return mBounds;                }
Bounds Screen::GetUsable   (void) const { return mUsable;                }
bool   Screen::IsPortrait  (void) const { return mBounds.W <  mBounds.H; }
bool   Screen::IsLandscape (void) const { return mBounds.W >= mBounds.H; }

////////////////////////////////////////////////////////////////////////////////

bool Screen::Synchronize (void)
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

Screen* Screen::GetMain (void)
{
	// TODO:
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

ScreenList Screen::GetList (void)
{
	return gScreens;
}

////////////////////////////////////////////////////////////////////////////////

Screen* Screen::GetScreen (const Window& window)
{
	// TODO:
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

Screen* Screen::GetScreen (const Point& point)
{
	// TODO:
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

Screen* Screen::GetScreen (int32 px, int32 py)
{
	// TODO:
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

bool Screen::GrabScreen (Image& image, const Bounds&
						 bounds, const Window& window)
{
	return GrabScreen (image,
		  bounds.X, bounds.Y,
		  bounds.W, bounds.H, window);
}

////////////////////////////////////////////////////////////////////////////////

bool Screen::GrabScreen (Image& image,
	int32 x, int32 y, int32 w, int32 h,
	const Window& window)
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

Bounds Screen::GetTotalBounds (void) { return mTotalBounds; }
Bounds Screen::GetTotalUsable (void) { return mTotalUsable; }

////////////////////////////////////////////////////////////////////////////////

bool Screen::IsCompositing (void)
{
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_LINUX)

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return false;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Screen::SetCompositing (bool enabled)
{
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

ROBOT_NS_END
