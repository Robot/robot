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

#include "Global.h"
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Hash
{
public:
	explicit	Hash			(void);
	explicit	Hash			(const  char* file);
	explicit	Hash			(const uint8* data,
								 uintptr dataLength);

public:
	bool		Append			(const  char* file);
	void		Append			(const uint8* data,
								 uintptr dataLength);

public:
	bool		operator ==		(uint32 hash) const;
	bool		operator !=		(uint32 hash) const;

	bool		operator ==		(const Hash& hash) const;
	bool		operator !=		(const Hash& hash) const;

public:
	uint32		Result;			// Rolling checksum value
};

ROBOT_NS_END
