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

#ifndef ROBOT_TASKBAR_H
#define ROBOT_TASKBAR_H

#include "Types.h"
namespace Robot {



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Taskbar
{
public:
	// Types
	enum Location { Left, Top, Right, Bottom };

private:
	// Constructors
	 Taskbar						(void);
	~Taskbar						(void);
	 Taskbar						(const Taskbar&  taskbar);
	 Taskbar						(      Taskbar&& taskbar);

public:
	// Functions
	static bool		Synchronize		(void);

	static Bounds	GetBounds		(void) { return mBounds;   }
	static Location	GetLocation		(void) { return mLocation; }
	static bool		IsAutoHide		(void) { return mAutoHide; }

private:
	// Operators
	Taskbar&		operator =		(const Taskbar&  taskbar);
	Taskbar&		operator =		(      Taskbar&& taskbar);

private:
	// Fields
	static Bounds	mBounds;		// Position and size
	static Location	mLocation;		// Taskbar location
	static bool		mAutoHide;		// Taskbar auto hides
};

} // namespace Robot
#endif // ROBOT_TASKBAR_H
