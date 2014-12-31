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

#ifndef ROBOT_HASH_H
#define ROBOT_HASH_H

#include "Global.h"
namespace Robot {



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Hash
{
public:
	// Constructors
	explicit	Hash			(void);
	explicit	Hash			(const char* file);
	explicit	Hash			(const uint8* data,
								 uintptr dataLength);

public:
	// Functions
	bool		Append			(const char* file);
	void		Append			(const uint8* data,
								 uintptr dataLength);

public:
	// Operators
	bool		operator ==		(uint32 hash) const;
	bool		operator !=		(uint32 hash) const;

	bool		operator ==		(const Hash& hash) const;
	bool		operator !=		(const Hash& hash) const;

public:
	// Properties
	uint32		Result;			// Rolling checksum value
};

} // namespace Robot
#endif // ROBOT_HASH_H
