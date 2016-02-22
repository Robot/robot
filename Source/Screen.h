////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2016 Robot Developers                       //
//                       See LICENSE for licensing info                       //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
// Prefaces                                                                   //
//----------------------------------------------------------------------------//

#pragma once

#include "Types.h"
#include "Window.h"
#include <deque>

ROBOT_NS_BEGIN
	class Screen;

#ifdef ROBOT_OS_WIN
	#pragma warning (push)
	// Ignore the VS C4251 warning
	#pragma warning (disable:4251)
#endif



//----------------------------------------------------------------------------//
// Types                                                                      //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

typedef std::deque<Screen*> ScreenList;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Screen
{
public:
	Screen								(void);
	Screen								(const Bounds& bounds,
										 const Bounds& usable);

public:
	Bounds				GetBounds		(void) const;
	Bounds				GetUsable		(void) const;
	bool				IsPortrait		(void) const;
	bool				IsLandscape		(void) const;

	static bool			Synchronize		(void);

	static Screen*		GetMain			(void);
	static ScreenList	GetList			(void);
	static Screen*		GetScreen		(const Window& window);
	static Screen*		GetScreen		(const Point & point );
	static Screen*		GetScreen		(int32  px, int32  py);

	static bool			GrabScreen		(Image& image, const Bounds& bounds,
										 const Window& window = Window());

	static bool			GrabScreen		(Image& image,
										 int32 x = 0, int32 y = 0,
										 int32 w = 0, int32 h = 0,
										 const Window& window = Window());

	static Bounds		GetTotalBounds	(void);
	static Bounds		GetTotalUsable	(void);

	static bool			 IsCompositing	(void);
	static void			SetCompositing	(bool enabled);

private:
	Bounds				mBounds;		// Screen total  bounds
	Bounds				mUsable;		// Screen usable bounds

	static Bounds		mTotalBounds;	// System total  bounds
	static Bounds		mTotalUsable;	// System usable bounds

	static ScreenList	mScreens;		// System screens deque
};

#ifdef ROBOT_OS_WIN
	#pragma warning (pop)
#endif

ROBOT_NS_END
