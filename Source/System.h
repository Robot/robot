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

#pragma once

#include "Types.h"
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT System
{
private:
	 System									(void);
	~System									(void);
	 System									(const System&  system);
	 System									(      System&& system);

public:
	static bool			Is64Bit				(void);
	static bool			IsUserAdmin			(void);
	static bool			IsAxEnabled			(bool options = false);

	static uintptr		GetMinAddress		(void);
	static uintptr		GetMaxAddress32		(void);
	static uintptr		GetMaxAddress64		(void);
	static uintptr		GetPageSize			(void);

	static bool			 IsCompositing		(void);
	static void			SetCompositing		(bool enabled);

	static uint32		GetCpuCount			(void);
	static uintptr		GetRamCount			(void);

private:
	static void			InitializeMemory	(void);

private:
	System&				operator =			(const System&  system);
	System&				operator =			(      System&& system);
};

ROBOT_NS_END
