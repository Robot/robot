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

#include "Process.h"
#include "Memory.h"
#include <assert.h>

#include <algorithm>
using std::search;
using std::vector;

#include <unordered_map>
using std::unordered_map;

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
// Classes                                                             Memory //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

typedef unordered_map<uintptr, uintptr> PageMap;

////////////////////////////////////////////////////////////////////////////////

struct Memory::Data
{
	Process		Proc;			// The process object

	uint8*		Cache;			// Cached memory data
	PageMap		Pages;			// Page pointers (R->C)
	uintptr		Next;			// The next free page

	uintptr		BlockLength;	// Single block size
	uintptr		BlockBuffer;	// Maximum read size

	uintptr		  CacheSize;	// Current cache size
	uintptr		EnlargeSize;	// Cache enlarge size
	uintptr		MaximumSize;	// Maximum cache size

	uint32		SystemReads;	// Total system reads
	uint32		CachedReads;	// Total cached reads
	uint32		SystemWrites;	// Total system writes
	uint32		AccessWrites;	// Total access writes

	uint32		 ReadErrors;	// Total  read errors
	uint32		WriteErrors;	// Total write errors

	int32		MemoryFile;		// Linux proc mem file
};



//----------------------------------------------------------------------------//
// Operators                                                    Memory::Stats //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Memory::Stats::operator == (const Stats& stats) const
{
	return SystemReads  == stats.SystemReads  &&
		   CachedReads  == stats.CachedReads  &&
		   SystemWrites == stats.SystemWrites &&
		   AccessWrites == stats.AccessWrites &&

		    ReadErrors  == stats. ReadErrors  &&
		   WriteErrors  == stats.WriteErrors;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::Stats::operator != (const Stats& stats) const
{
	return SystemReads  != stats.SystemReads  ||
		   CachedReads  != stats.CachedReads  ||
		   SystemWrites != stats.SystemWrites ||
		   AccessWrites != stats.AccessWrites ||

		    ReadErrors  != stats. ReadErrors  ||
		   WriteErrors  != stats.WriteErrors;
}



//----------------------------------------------------------------------------//
// Operators                                                   Memory::Region //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::operator < (const Region& region) const
{
	return Start < region.Start;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::operator > (const Region& region) const
{
	return Start > region.Start;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::operator <= (const Region& region) const
{
	return Start <= region.Start;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::operator >= (const Region& region) const
{
	return Start >= region.Start;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::operator == (const Region& region) const
{
	return Valid      == region.Valid      &&
		   Bound      == region.Bound      &&

		   Start      == region.Start      &&
		   Stop       == region.Stop       &&
		   Size       == region.Size       &&

		   Readable   == region.Readable   &&
		   Writable   == region.Writable   &&
		   Executable == region.Executable &&
		   Access     == region.Access     &&

		   Private    == region.Private    &&
		   Guarded    == region.Guarded;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::operator != (const Region& region) const
{
	return Valid      != region.Valid      ||
		   Bound      != region.Bound      ||

		   Start      != region.Start      ||
		   Stop       != region.Stop       ||
		   Size       != region.Size       ||

		   Readable   != region.Readable   ||
		   Writable   != region.Writable   ||
		   Executable != region.Executable ||
		   Access     != region.Access     ||

		   Private    != region.Private    ||
		   Guarded    != region.Guarded;
}



//----------------------------------------------------------------------------//
// Constructors                                                        Memory //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Memory::Memory (void) : mData (new Memory::Data(), [](Memory::Data* data)
{
	// Delete the memory cache
	if (data->Cache != nullptr)
		delete[] data->Cache;

	// Free data
	delete data;
})
{
	mData->Cache        = nullptr;
	mData->Next         =  0;

	mData->BlockLength  =  0;
	mData->BlockBuffer  =  0;

	mData->  CacheSize  =  0;
	mData->EnlargeSize  =  0;
	mData->MaximumSize  =  0;

	mData->SystemReads  =  0;
	mData->CachedReads  =  0;
	mData->SystemWrites =  0;
	mData->AccessWrites =  0;

	mData-> ReadErrors  =  0;
	mData->WriteErrors  =  0;

	mData->MemoryFile   = -1;
}

////////////////////////////////////////////////////////////////////////////////

Memory::Memory (Process p) : mData (new Memory::Data(), [](Memory::Data* data)
{
	// Delete the memory cache
	if (data->Cache != nullptr)
		delete[] data->Cache;

	// Free data
	delete data;
})
{
	mData->Proc         = p;
	mData->Cache        = nullptr;
	mData->Next         =  0;

	mData->BlockLength  =  0;
	mData->BlockBuffer  =  0;

	mData->  CacheSize  =  0;
	mData->EnlargeSize  =  0;
	mData->MaximumSize  =  0;

	mData->SystemReads  =  0;
	mData->CachedReads  =  0;
	mData->SystemWrites =  0;
	mData->AccessWrites =  0;

	mData-> ReadErrors  =  0;
	mData->WriteErrors  =  0;

	mData->MemoryFile   = -1;
}



//----------------------------------------------------------------------------//
// Functions                                                           Memory //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Memory::IsValid (void) const
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

Process Memory::GetProcess (void) const
{
	return mData->Proc;
}

////////////////////////////////////////////////////////////////////////////////

Memory::Stats Memory::GetStats (void) const
{
	// TODO:
	return Stats();
}

////////////////////////////////////////////////////////////////////////////////

Memory::Stats Memory::GetStats (bool reset)
{
	// TODO:
	return Stats();
}

////////////////////////////////////////////////////////////////////////////////

Memory::Region Memory::GetRegion (uintptr address) const
{
	Region region = { 0 };
	region.Valid  = false;

	// Check whether address is valid
	if (address <  GetMinAddress() ||
		address >= GetMaxAddress() ||
		!IsValid()) return region;

#ifdef ROBOT_OS_LINUX

	// TODO:
	return region;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return region;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return region;

#endif
}

////////////////////////////////////////////////////////////////////////////////

Memory::RegionList Memory::GetRegions
	(uintptr start, uintptr stop) const
{
	RegionList result;
	// Verify the memory validity
	if (!IsValid()) return result;

#ifdef ROBOT_OS_LINUX

	// TODO:
	return result;

#endif
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_WIN)

	// TODO:
	return result;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::SetAccess (const Region& region, uint32 access)
{
	// Verify the parameters and status
	if (!region.Valid || !region.Bound ||
		IsValid() == false) return false;

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

bool Memory::SetAccess (const Region& region,
			bool read, bool write, bool exec)
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

AddressList Memory::Find (const char* pattern, uintptr start,
		uintptr stop, uintptr limit, const char* accessFlags)
{
	AddressList result;
	// Check the parameters and the status
	if (pattern == nullptr || !IsValid())
		return result;

	// TODO:
	return result;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::CreateCache (uintptr blockLength,
	 uintptr blockBuffer, uintptr initialSize,
	 uintptr enlargeSize, uintptr maximumSize)
{
	// TODO:
	return false;
}

////////////////////////////////////////////////////////////////////////////////

void Memory::ClearCache (void)
{
	// TODO:
}

////////////////////////////////////////////////////////////////////////////////

void Memory::DeleteCache (void)
{
	// TODO:
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::IsCaching (void) const
{
	// TODO:
	return false;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::GetCacheSize (void) const
{
	// TODO:
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::GetMinAddress (void) const
{
	// TODO:
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::GetMaxAddress (void) const
{
	// TODO:
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::GetPageSize (void) const
{
	// TODO:
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::ReadData (uintptr address,
	void* result, uintptr length, Flags f)
{
	// TODO:
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::WriteData (uintptr address,
	const void* data, uintptr length, Flags f)
{
	// TODO:
	return 0;
}



//----------------------------------------------------------------------------//
// Internal                                                            Memory //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::MemRead (uintptr address,
	void* result, uintptr length, Flags f)
{
	// TODO:
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::MemWrite (uintptr address,
	const void* data, uintptr length, Flags f)
{
	// TODO:
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::SysRead (uintptr address, void*
	result, uintptr length, uintptr* bytes)
{
	// TODO:
	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::SysWrite (uintptr address, const
	void* data, uintptr length, uintptr* bytes)
{
	// TODO:
	return false;
}

////////////////////////////////////////////////////////////////////////////////

void Memory::InitializeVM (void)
{
	// TODO:
}

ROBOT_NS_END
