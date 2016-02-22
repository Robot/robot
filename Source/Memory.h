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
#include <memory>
#include <vector>

ROBOT_NS_BEGIN
	class Process;

#ifdef ROBOT_OS_WIN
	#pragma warning (push)
	// Ignore the VS C4251 warning
	#pragma warning (disable:4251)
#endif



//----------------------------------------------------------------------------//
// Types                                                                      //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

typedef std::vector<uintptr> AddressList;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Memory
{
public:
	////////////////////////////////////////////////////////////////////////////////

	struct ROBOT_EXPORT Stats
	{
		uint32			SystemReads;	// Total system reads
		uint32			CachedReads;	// Total cached reads
		uint32			SystemWrites;	// Total system writes
		uint32			AccessWrites;	// Total access writes

		uint32			 ReadErrors;	// Total  read errors
		uint32			WriteErrors;	// Total write errors

		bool			operator ==		(const Stats& stats) const;
		bool			operator !=		(const Stats& stats) const;
	};

	////////////////////////////////////////////////////////////////////////////////

	struct ROBOT_EXPORT Region
	{
		bool			Valid;			// If this structure is valid
		bool			Bound;			// If memory has been committed

		uintptr			Start;			// Start address of the region
		uintptr			Stop;			// Stop  address of the region
		uintptr			Size;			// Total size of the region

		bool			Readable;		// If the region is readable
		bool			Writable;		// If the region is writable
		bool			Executable;		// If the region is executable
		uint32			Access;			// Native protection attribute

		bool			Private;		// If the region is private
		bool			Guarded;		// If the region is guarded

		bool			Contains		(uintptr address) const;

		bool			operator <		(uintptr address) const;
		bool			operator >		(uintptr address) const;
		bool			operator <=		(uintptr address) const;
		bool			operator >=		(uintptr address) const;

		bool			operator <		(const Region& region) const;
		bool			operator >		(const Region& region) const;
		bool			operator <=		(const Region& region) const;
		bool			operator >=		(const Region& region) const;

		bool			operator ==		(const Region& region) const;
		bool			operator !=		(const Region& region) const;
	};

	////////////////////////////////////////////////////////////////////////////////

	enum Flags
	{
		Default			= 0x0,			// Use default system call
		SkipErrors		= 0x1,			// Zero fill erroneous regions
		AutoAccess		= 0x2,			// Attempt to patch access or
										// zero fill erroneous regions
	};

	////////////////////////////////////////////////////////////////////////////////

	typedef std::vector<Region> RegionList;

public:
	explicit			Memory			(const Process&
										 p = Process());

public:
	bool				IsValid			(void) const;
	Process				GetProcess		(void) const;

	Stats				GetStats		(void) const;
	Stats				GetStats		(bool reset);

	Region				GetRegion		(uintptr address) const;

	RegionList			GetRegions		(uintptr start =  0,
										 uintptr stop  = -1) const;

	bool				SetAccess		(const Region& region,
										 uint32 nativeAccess);
	bool				SetAccess		(const Region& region,
										 bool r, bool w, bool x);

	AddressList			Find			(const char* pattern,
										 uintptr start =  0,
										 uintptr stop  = -1,
										 uintptr limit =  0,
										 const char* f =  0);

	bool				CreateCache		(uintptr blockLength,
										 uintptr blockBuffer,
										 uintptr initialSize,
										 uintptr enlargeSize = 0,
										 uintptr maximumSize = 0);

	void				 ClearCache		(void);
	void				DeleteCache		(void);

	bool				IsCaching		(void) const;
	uintptr				GetCacheSize	(void) const;

	uintptr				GetPtrSize		(void) const;
	uintptr				GetMinAddress	(void) const;
	uintptr				GetMaxAddress	(void) const;
	uintptr				GetPageSize		(void) const;

	uintptr				ReadData		(uintptr address, void* dataOutput,
										 uintptr length, Flags f = Default);

	uintptr				WriteData		(uintptr address, const void* data,
										 uintptr length, Flags f = Default);

private:
	uintptr				MemRead			(uintptr address, void* dataOutput,
										 uintptr length, Flags readFlags);

	uintptr				MemWrite		(uintptr address, const void* data,
										 uintptr length, Flags writeFlags);

	bool				SysRead			(uintptr address, void* dataOutput,
										 uintptr length, uintptr* bytes);

	bool				SysWrite		(uintptr address, const void* data,
										 uintptr length, uintptr* bytes);

	static void			InitializeVM	(void);

private:
	struct Data;
	std::shared_ptr<Data> mData;		// Shared information
};

#ifdef ROBOT_OS_WIN
	#pragma warning (pop)
#endif

ROBOT_NS_END
