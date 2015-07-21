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

#include "Clipboard.h"
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
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Functions                                                        Clipboard //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Clipboard::Clear (void)
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

bool Clipboard::HasText (void)
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

string Clipboard::GetText (void)
{
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

bool Clipboard::SetText (const char* text)
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

bool Clipboard::HasImage (void)
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

bool Clipboard::GetImage (Image& image)
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

bool Clipboard::SetImage (const Image& image)
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

uint64 Clipboard::GetSequence (void)
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

ROBOT_NS_END
