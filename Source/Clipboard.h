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
#include <string>
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Clipboard
{
private:
	 Clipboard							(void);
	~Clipboard							(void);
	 Clipboard							(const Clipboard&  clipboard);
	 Clipboard							(      Clipboard&& clipboard);

public:
	static bool			Clear			(void);

	static bool			HasText			(void);
	static std::string	GetText			(void);
	static bool			SetText			(const char* string);

	static bool			HasImage		(void);
	static bool			GetImage		(      Image& image);
	static bool			SetImage		(const Image& image);

	static uint64		GetSequence		(void);

private:
	Clipboard&			operator =		(const Clipboard&  clipboard);
	Clipboard&			operator =		(      Clipboard&& clipboard);
};

ROBOT_NS_END
