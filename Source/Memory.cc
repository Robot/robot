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

#include "Process.h"
#include "Memory.h"
#include <assert.h>

#include <algorithm>
using std::search;
using std::vector;

#include <unordered_map>
using std::unordered_map;

#ifdef ROBOT_OS_LINUX

	#include <string>
	using std::string;
	using std::sort;

	#include <cstring>
	#include <fstream>
	using std::ios;
	using std::ifstream;
	using std:: fstream;
	using std:: getline;

	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/mman.h>

	// Path to proc directory
	#define PROC_PATH "/proc/"

#endif
#ifdef ROBOT_OS_MAC

	#include <unistd.h>
	#include <mach/mach_vm.h>

	// Handle shortcut
	#define ProcHandle \
		((task_t) mData->Proc.GetHandle())

#endif
#ifdef ROBOT_OS_WIN

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

	// Handle shortcut
	#define ProcHandle \
		((HANDLE) mData->Proc.GetHandle())

#endif
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define SET_A_FLAG( i, aID, aV )	\
	if (accessFlags[i] == '\0')		\
		goto skip;					\
									\
	if (accessFlags[i] != ' ' &&	\
		accessFlags[i] != '-' &&	\
		accessFlags[i] != aID &&	\
		accessFlags[i] !=(aID|32))	\
		return result;				\
									\
	aV =accessFlags[i] | 32;

////////////////////////////////////////////////////////////////////////////////

#define HEX_TO_INT( hiVal, loVal )	\
	if (high)						\
	{								\
		byte.Data = hiVal;			\
		byte.Wild = false;			\
	}								\
									\
	else							\
	{								\
		byte.Data += loVal;			\
		compiled.push_back (byte);	\
	}								\
									\
	high = !high; break;



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

static uintptr gMinVM    = 0;	// Minimum VM address
static uintptr gMaxVM_32 = 0;	// Maximum VM address (32-Bit)
static uintptr gMaxVM_64 = 0;	// Maximum VM address (64-Bit)
static uintptr gPageSize = 0;	// Size of single page

#ifdef ROBOT_OS_LINUX

	////////////////////////////////////////////////////////////////////////////////

	struct Mapping
	{
		uintptr Start;			// Start address of region
		uintptr Stop;			// Stop  address of region
		char Access[5];			// Region protection value
	};

#endif

////////////////////////////////////////////////////////////////////////////////

struct PatternByte
{
	uint8		Data;			// Single byte of data
	bool		Wild;			// If data is wildcard
};

////////////////////////////////////////////////////////////////////////////////

static bool SearchWild (uint8 c, const PatternByte& b)
{
	// Check for wildcard values
	return b.Wild || c == b.Data;
}

////////////////////////////////////////////////////////////////////////////////

static bool SearchNorm (uint8 c, const PatternByte& b)
{
	// Ignore wildcards
	return c == b.Data;
}



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
// Functions                                                   Memory::Region //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::Contains (uintptr address) const
{
	return Start <= address && address < Stop;
}



//----------------------------------------------------------------------------//
// Operators                                                   Memory::Region //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::operator < (uintptr address) const
{
	return Start < address;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::operator > (uintptr address) const
{
	return Start > address;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::operator <= (uintptr address) const
{
	return Start <= address;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::Region::operator >= (uintptr address) const
{
	return Start >= address;
}

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

Memory::Memory (const Process& p)
	  : mData (new Memory::Data(),
	  [] (Memory::Data* data)
{
	// Delete the memory cache
	if (data->Cache != nullptr)
		delete[] data->Cache;

#ifdef ROBOT_OS_LINUX

	// Close memory file if open
	if (data->MemoryFile != -1)
		close (data->MemoryFile);

#endif

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

#ifdef ROBOT_OS_LINUX

	char mem[32];
	// Build a path to mem file
	snprintf (mem, 32, PROC_PATH
			"%d/mem", p.GetPID());

	// Attempt to open the proc memory file
	mData->MemoryFile = open (mem, O_RDWR);

#endif
}



//----------------------------------------------------------------------------//
// Functions                                                           Memory //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Memory::IsValid (void) const
{
#ifdef ROBOT_OS_LINUX

	// Verify that process is valid
	return mData->Proc.IsValid();

#endif
#ifdef ROBOT_OS_MAC

	// Verify that process is valid
	// and a mach task is available
	return mData->Proc.IsValid  () &&
		   mData->Proc.GetHandle() != 0;

#endif
#ifdef ROBOT_OS_WIN

	// Verify that process is valid
	return mData->Proc.IsValid();

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
	Memory::Stats s;
	s.SystemReads  = mData->SystemReads;
	s.CachedReads  = mData->CachedReads;
	s.SystemWrites = mData->SystemWrites;
	s.AccessWrites = mData->AccessWrites;

	s. ReadErrors  = mData-> ReadErrors;
	s.WriteErrors  = mData->WriteErrors;
	return s;
}

////////////////////////////////////////////////////////////////////////////////

Memory::Stats Memory::GetStats (bool reset)
{
	Memory::Stats s;
	s.SystemReads  = mData->SystemReads;
	s.CachedReads  = mData->CachedReads;
	s.SystemWrites = mData->SystemWrites;
	s.AccessWrites = mData->AccessWrites;

	s. ReadErrors  = mData-> ReadErrors;
	s.WriteErrors  = mData->WriteErrors;

	if (reset)
	{
		mData->SystemReads  = 0;
		mData->CachedReads  = 0;
		mData->SystemWrites = 0;
		mData->AccessWrites = 0;

		mData-> ReadErrors  = 0;
		mData->WriteErrors  = 0;
	}

	return s;
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

	// Align address to page boundary
	address &= ~(GetPageSize() - 1);

#ifdef ROBOT_OS_LINUX

	RegionList list =
		// Get the region at the address
		GetRegions (address, address + 1);

	// If list isn't empty, return front
	return list.empty() ? region : list[0];

#endif
#ifdef ROBOT_OS_MAC

	mach_port_t port = MACH_PORT_NULL;
	mach_msg_type_number_t count =
		VM_REGION_BASIC_INFO_COUNT_64;

	uint64 base = address, size;
	vm_region_basic_info_64 info;
	vm_region_flavor_t flavor =
		VM_REGION_BASIC_INFO_64;

	// Query region info using specified address
	if (mach_vm_region (ProcHandle, &base, &size,
		flavor, (vm_region_info_t) &info, &count,
		&port)) return region;

	// Determine the start and stop
	uintptr start = (uintptr) base;
	uintptr stop  = (uintptr) base +
					(uintptr) size;

	// Avoid returning invalid memory regions
	if (stop > GetMaxAddress()) return region;

	region.Start = address;
	// Check if address is within the result
	if (start <= address && address < stop)
	{
		region.Bound = true;
		region.Stop  = stop;
		region.Size  = stop - address;

		// Determine the access of the region
		region.Access     = (info.protection);
		region.Executable = (info.protection & VM_PROT_EXECUTE) != 0;
		region.Readable   = (info.protection & VM_PROT_READ   ) != 0;
		region.Writable   = (info.protection & VM_PROT_WRITE  ) != 0;

		// Is the region private or shared
		region.Private = info.shared == 0;
	}

	else
	{
		// Region is unbound
		region.Stop = start;
		region.Size = start - address;
	}

	assert (region.Start % GetPageSize() == 0);
	assert (region.Stop  % GetPageSize() == 0);
	assert (region.Size  % GetPageSize() == 0);
	assert (region.Size                  != 0);

	region.Valid = true; return region;

#endif
#ifdef ROBOT_OS_WIN

	MEMORY_BASIC_INFORMATION mbInfo;
	// Determine region info using specified address
	if (VirtualQueryEx (ProcHandle, (LPCVOID) address,
		&mbInfo, sizeof (mbInfo)) != sizeof (mbInfo))
		return region;

	// Ignore invalid memory regions
	if ((uintptr) mbInfo.BaseAddress +
		(uintptr) mbInfo.RegionSize >
		GetMaxAddress()) return region;

	// Set the size and base address of region
	region.Start = (uintptr) mbInfo.BaseAddress;
	region.Stop  = (uintptr) mbInfo.BaseAddress +
				   (uintptr) mbInfo.RegionSize;
	region.Size  = (uintptr) mbInfo.RegionSize;

	assert (region.Start % GetPageSize() == 0);
	assert (region.Stop  % GetPageSize() == 0);
	assert (region.Size  % GetPageSize() == 0);
	assert (region.Size                  != 0);

	region.Access = PAGE_NOACCESS;
	// Check if storage is assigned
	if (mbInfo.State == MEM_COMMIT)
	{
		region.Bound = true;
		// Define constants encompassing various access rights
		static const DWORD R = PAGE_READONLY | PAGE_READWRITE |
			PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ |
			PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

		static const DWORD W = PAGE_READWRITE | PAGE_WRITECOPY |
			PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

		static const DWORD X = PAGE_EXECUTE | PAGE_EXECUTE_READ |
			PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

		region.Readable   = (mbInfo.Protect & R) != 0;
		region.Writable   = (mbInfo.Protect & W) != 0;
		region.Executable = (mbInfo.Protect & X) != 0;
		region.Access     =  mbInfo.Protect;

		region.Private    = (mbInfo.Type    & MEM_PRIVATE) != 0;
		region.Guarded    = (mbInfo.Protect & PAGE_GUARD ) != 0;
	}

	region.Valid = true; return region;

#endif
}

////////////////////////////////////////////////////////////////////////////////

Memory::RegionList Memory::GetRegions
	(uintptr start, uintptr stop) const
{
	RegionList result;
	// Verify the memory validity
	if (!IsValid()) return result;

	// Ensure start is in range
	if (start < GetMinAddress())
		start = GetMinAddress();

	// Ensure stop is in range
	if (stop > GetMaxAddress())
		stop = GetMaxAddress();

	uintptr oldStop = stop;
	// Align range to the page size
	start &= ~(GetPageSize() - 1);
	 stop &= ~(GetPageSize() - 1);
	if (oldStop != stop)
		stop += GetPageSize();

	// Make sure the range is correct
	if (start >= stop) return result;

#ifdef ROBOT_OS_LINUX

	char maps[32];
	// Build path to proc PID maps
	snprintf (maps, 32, PROC_PATH
		"%d/maps", mData->Proc.GetPID());

	// Attempt to open maps file
	fstream file (maps, ios::in);
	if (!file) return result;

	Mapping mapping; string line;
	std::vector<Mapping> mappings;
	// Create the mappings list
	while (getline (file, line))
	{
		size_t begin, end;
		// Parse current line into single mapping entry
		if (sscanf (line.data(), "%zx-%zx %4s", &begin,
			&end, mapping.Access) != 3) return result;

		// Ignore any invalid memory regions
		if (end > GetMaxAddress()) continue;

		// Avoid GCC warning
		mapping.Start = begin;
		mapping.Stop  = end;
		mappings.push_back (mapping);
	}

	// Sort the mappings based on their address
	std::sort (mappings.begin(), mappings.end(),
		[](const Mapping& a, const Mapping& b)
		-> bool { return a.Start < b.Start; });

	Region region = { 0 };
	region.Valid  = true;
	region.Start  = start;

	// Iterate through every mapping in the list
	for (uintptr i = 0; i < mappings.size(); ++i)
	{
		// Avoid dereferencing each iteration
		const Mapping& current = mappings[i];

		// Position at the start address
		if (region.Start >= current.Stop)
			continue;

		// Check if region is unbounded
		if (region.Start < current.Start)
		{
			region.Bound = false;
			region.Stop  = current.Start;
			region.Size  = current.Start - region.Start;

			region.Readable   = false;
			region.Writable   = false;
			region.Executable = false;
			region.Access     = PROT_NONE;

			region.Private    = false;
			result.push_back (region);

			assert (region.Start % GetPageSize() == 0);
			assert (region.Stop  % GetPageSize() == 0);
			assert (region.Size  % GetPageSize() == 0);
			assert (region.Size                  != 0);

			// Increase the base address
			region.Start = current.Start;
			if (region.Start >= stop) break;
		}

		region.Bound = true;
		region.Stop  = current.Stop;
		region.Size  = current.Stop - region.Start;

		region.Readable   = current.Access[0] == 'r';
		region.Writable   = current.Access[1] == 'w';
		region.Executable = current.Access[2] == 'x';
		region.Private    = current.Access[3] == 'p';

		region.Access = PROT_NONE;
		if (region.Executable) region.Access |= PROT_EXEC;
		if (region.Readable  ) region.Access |= PROT_READ;
		if (region.Writable  ) region.Access |= PROT_WRITE;
		result.push_back (region);

		assert (region.Start % GetPageSize() == 0);
		assert (region.Stop  % GetPageSize() == 0);
		assert (region.Size  % GetPageSize() == 0);
		assert (region.Size                  != 0);

		// Increase the base address
		region.Start = current.Stop;
		if (region.Start >= stop) break;
	}

	return result;

#endif
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_WIN)

	Region region;
	// Read memory, region by region
	for (uintptr addr = start; addr
		 < stop; addr = region.Stop)
	{
		// Get region info and verify validity
		if (!(region = GetRegion (addr)).Valid)
			break;

		// Append current region
		result.push_back (region);
	}

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

	// NOTE: Not able to be implemented safely
	return false;

#endif
#ifdef ROBOT_OS_MAC

	// Modify access value of the specified region
	if (mach_vm_protect (ProcHandle, region.Start,
		region.Size, 0, access)) return false;

	// Update stats value
	++mData->AccessWrites;
	return true;

#endif
#ifdef ROBOT_OS_WIN

	DWORD old = 0;
	// Modify access value of specified region
	if (!VirtualProtectEx (ProcHandle, (LPVOID)
		region.Start, region.Size, access, &old))
		return false;

	// Update stats value
	++mData->AccessWrites;
	return true;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::SetAccess (const Region& region,
			bool read, bool write, bool exec)
{
#ifdef ROBOT_OS_LINUX

	uint32 access = PROT_NONE;
	// Compute new protection value
	if (exec ) access |= PROT_EXEC;
	if (read ) access |= PROT_READ;
	if (write) access |= PROT_WRITE;

	return SetAccess (region, access);

#endif
#ifdef ROBOT_OS_MAC

	uint32 access = VM_PROT_NONE;
	// Compute the new protection value
	if (exec ) access |= VM_PROT_EXECUTE;
	if (read ) access |= VM_PROT_READ;
	if (write) access |= VM_PROT_WRITE;

	return SetAccess (region, access);

#endif
#ifdef ROBOT_OS_WIN

	uint32 access;

	if (exec)
	{
		if (write)
			access = PAGE_EXECUTE_READWRITE;

		else
		{
			if (read)
				access = PAGE_EXECUTE_READ;
			else
				access = PAGE_EXECUTE;
		}
	}

	else
	{
		if (write)
			access = PAGE_READWRITE;

		else
		{
			if (read)
				access = PAGE_READONLY;
			else
				access = PAGE_NOACCESS;
		}
	}

	return SetAccess (region, access);

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

	char aw = ' '; char ax = ' ';
	char ap = ' '; char ag = ' ';

	// Check if flags are valid
	if (accessFlags != nullptr)
	{
		SET_A_FLAG (0, 'W', aw);
		SET_A_FLAG (1, 'X', ax);
		SET_A_FLAG (2, 'P', ap);
		SET_A_FLAG (3, 'G', ag);

	skip:
		assert (aw == ' ' || aw == '-' || aw == 'w');
		assert (ax == ' ' || ax == '-' || ax == 'x');
		assert (ap == ' ' || ap == '-' || ap == 'p');
		assert (ag == ' ' || ag == '-' || ag == 'g');
	}

	bool wild = false;
	bool high = true;
	PatternByte byte;
	vector<PatternByte> compiled;

	// Compile the pattern into an array format
	for (uintptr i = 0; pattern[i] != '\0'; ++i)
	{
		switch (pattern[i])
		{
			case '0': HEX_TO_INT (  0,  0);
			case '1': HEX_TO_INT ( 16,  1);
			case '2': HEX_TO_INT ( 32,  2);
			case '3': HEX_TO_INT ( 48,  3);
			case '4': HEX_TO_INT ( 64,  4);
			case '5': HEX_TO_INT ( 80,  5);
			case '6': HEX_TO_INT ( 96,  6);
			case '7': HEX_TO_INT (112,  7);
			case '8': HEX_TO_INT (128,  8);
			case '9': HEX_TO_INT (144,  9);
			case 'A': HEX_TO_INT (160, 10);
			case 'a': HEX_TO_INT (160, 10);
			case 'B': HEX_TO_INT (176, 11);
			case 'b': HEX_TO_INT (176, 11);
			case 'C': HEX_TO_INT (192, 12);
			case 'c': HEX_TO_INT (192, 12);
			case 'D': HEX_TO_INT (208, 13);
			case 'd': HEX_TO_INT (208, 13);
			case 'E': HEX_TO_INT (224, 14);
			case 'e': HEX_TO_INT (224, 14);
			case 'F': HEX_TO_INT (240, 15);
			case 'f': HEX_TO_INT (240, 15);

			case ' ':
				if (!high) return result;
				break;

			case '?':
				if (!high) return result;
				byte.Wild = wild = true;
				compiled.push_back (byte);
				break;

			default:
				return result;
		}
	}

	// Do the last compiler checks
	if (!high || compiled.empty())
		return result;

	RegionList list =
		// Get a list of regions
		GetRegions (start, stop);

	// Check for at least one region
	if (list.empty()) return result;

	uintptr size = 1048576;
	// Initially allocate single MB
	uint8*  data = new uint8[size];

	// Loop through memory region list
	for (uintptr a = start, i = 0; i <
		list.size(); a = list[i++].Stop)
	{
		// Avoid dereferencing each time
		const Region& current = list[i];

	#ifndef NDEBUG
		// Paranoid bounds check
		if (current.Stop >= stop)
			assert (i+1 == list.size());
	#endif

		// Skip if region is unreadable
		if (!current.Readable) continue;

		// Don't check if defaults
		if (accessFlags != nullptr)
		{
			// Filter regions not matching the access flags
			if (aw == '-' &&  current.Writable  ) continue;
			if (aw == 'w' && !current.Writable  ) continue;
			if (ax == '-' &&  current.Executable) continue;
			if (ax == 'x' && !current.Executable) continue;
			if (ap == '-' &&  current.Private   ) continue;
			if (ap == 'p' && !current.Private   ) continue;
			if (ag == '-' &&  current.Guarded   ) continue;
			if (ag == 'g' && !current.Guarded   ) continue;
		}

		// Compute the current value of length parameter
		uintptr len = std::min (current.Stop, stop) - a;

		if (size < len)
		{
			// Multiply by two until
			// the length is reached
			while ((size *= 2) < len);

			delete[] data;
			// Reallocate the buffer
			data = new uint8[size];
		}

		// Attempt to read the memory region
		if (ReadData (a, data, len) != len)
			continue;

		// Compute the range
		uint8* begin = data;
		uint8* end   = data + len;

		while (true)
		{
			// Search for the pattern in memory
			uint8* res = std::search (begin, end,
				compiled.begin(), compiled.end(),
				wild ? SearchWild : SearchNorm);

			// We're done searching
			if (res >= end) break;

			// Rebase the result from search
			result.push_back (res - data + a);

			if (limit != 0 &&
				// Limit has been hit
				limit <= result.size())
			{
				delete[] data;
				return result;
			}

			// Progress the start pointer
			begin = res + compiled.size();

			// We're done searching
			if (begin >= end) break;
		}
	}

	delete[] data;
	return result;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::CreateCache (uintptr blockLength,
	 uintptr blockBuffer, uintptr initialSize,
	 uintptr enlargeSize, uintptr maximumSize)
{
	// Check for any zeroes
	if (blockLength == 0 ||
		blockBuffer == 0 ||
		initialSize == 0)
		return false;

	// Retrieve the system page size
	uintptr pageSize = GetPageSize();
	if (pageSize == 0) return false;

	// Ensure alignment to page size
	if (blockLength % pageSize != 0 ||
		blockBuffer % pageSize != 0 ||
		initialSize % pageSize != 0 ||
		enlargeSize % pageSize != 0 ||
		maximumSize % pageSize != 0)
		return false;

	// Avoid any unoptimized alignment
	if (blockLength & (blockLength-1))
		return false;

	// Ensure block can store data
	if (blockLength < blockBuffer)
		return false;

	// Validate the initial size
	if (initialSize < blockLength +
		blockBuffer) return false;

	// Validate enlarge size
	if (enlargeSize != 0 &&
		enlargeSize < blockLength +
		blockBuffer) return false;

	// Validate maximum size
	if (maximumSize != 0 &&
		maximumSize < initialSize)
		return false;

	// Set the memory cache properties
	mData->BlockLength = blockLength;
	mData->BlockBuffer = blockBuffer;

	mData->  CacheSize = initialSize;
	mData->EnlargeSize = enlargeSize;
	mData->MaximumSize = maximumSize;
	mData->Next = 0;

	// Clear cached pages
	mData->Pages.clear();

	// Clean up the memory cache
	if (mData->Cache != nullptr)
		delete[] mData->Cache;

	mData->Cache =
		// Create memory cache
		new uint8[initialSize];

	return true;
}

////////////////////////////////////////////////////////////////////////////////

void Memory::ClearCache (void)
{
	mData->Next = 0;
	// Clear cached pages
	mData->Pages.clear();
}

////////////////////////////////////////////////////////////////////////////////

void Memory::DeleteCache (void)
{
	// Reset memory cache
	mData->BlockLength = 0;
	mData->BlockBuffer = 0;

	mData->  CacheSize = 0;
	mData->EnlargeSize = 0;
	mData->MaximumSize = 0;
	mData->Next = 0;

	// Clear cached pages
	mData->Pages.clear();

	// Clean up the memory cache
	if (mData->Cache != nullptr)
	{
		delete[] mData->Cache;
		mData->Cache = nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::IsCaching (void) const
{
	// Return the state of the cache
	return mData->Cache != nullptr;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::GetCacheSize (void) const
{
	// Return the cache size
	return mData->CacheSize;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::GetPtrSize (void) const
{
	return mData->Proc.Is64Bit() ? 8 : 4;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::GetMinAddress (void) const
{
	InitializeVM();
	return gMinVM;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::GetMaxAddress (void) const
{
	InitializeVM();
	// Depends on architecture
	return mData->Proc.Is64Bit() ?
		   gMaxVM_64 : gMaxVM_32;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::GetPageSize (void) const
{
	InitializeVM();
	return gPageSize;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::ReadData (uintptr address,
	void* result, uintptr length, Flags f)
{
	// Check the parameter values and status
	if (!result || length == 0 || !IsValid())
		return 0;

	// Check if the address is within bounds
	if (GetMaxAddress() < address + length ||
		GetMinAddress() > address) return 0;

	// Check if caching can be used
	if (length > mData->BlockBuffer)
		return MemRead (address, result, length, f);

	const uintptr pageSize =
		// Calculate size of single cached page
		mData->BlockLength + mData->BlockBuffer;

	const uintptr aligned =
		// Compute alignment to block length
		address & ~(mData->BlockLength - 1);

	// Check if new address is within bounds
	if (GetMaxAddress() < aligned + pageSize ||
		GetMinAddress() > aligned) return 0;

	// Check if the memory has already been cached before
	if (mData->Pages.find (aligned) == mData->Pages.end())
	{
		// Increase size of the cache if out of space
		if (mData->CacheSize - mData->Next < pageSize)
		{
			if (mData->EnlargeSize == 0)
				// We can't enlarge the cache any further
				return MemRead (address, result, length, f);

			// Compute the new size of the cache
			uintptr newSize = mData->CacheSize +
							  mData->EnlargeSize;

			if (mData->MaximumSize != 0 &&
				mData->MaximumSize < newSize)
				// We have reached the maximum cache size
				return MemRead (address, result, length, f);

			// Allocate a cache of the new size
			uint8* newData = new uint8[newSize];

			// Copy existing cache data and free memory
			memcpy (newData, mData->Cache, mData->Next);
			delete[] mData->Cache;

			// Set new size and cache
			mData->CacheSize = newSize;
			mData->Cache     = newData;
		}

		// Read data into next available cache position
		if (MemRead (aligned, mData->Cache + mData->Next,
			pageSize, f) != pageSize)
		{
			// Perform a system call in case of an error
			return MemRead (address, result, length, f);
		}

		// Store the location of this page
		mData->Pages[aligned] = mData->Next;

		// Update next free page
		mData->Next += pageSize;
	}

	// Update statistics
	++mData->CachedReads;

	uintptr offset =
		// Find the memory offset in cached memory
		mData->Pages[aligned] + address - aligned;

	// Read the cached memory and copy the result
	memcpy (result, mData->Cache + offset, length);
	return length;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::WriteData (uintptr address,
	const void* data, uintptr length, Flags f)
{
	// Check the parameter values and status
	if (!data || length == 0 || !IsValid())
		return 0;

	// Check if the address is within bounds
	if (GetMaxAddress() < address + length ||
		GetMinAddress() > address) return 0;

	// Nothing much to do, perform system call
	return MemWrite (address, data, length, f);
}



//----------------------------------------------------------------------------//
// Internal                                                            Memory //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::MemRead (uintptr address,
	void* result, uintptr length, Flags f)
{
	assert (address != 0);
	assert (result  != 0);
	assert (length  != 0);

	uintptr bytes = 0;
	// Use default system call
	if (f == Memory::Default)
	{
		SysRead (address, result, length, &bytes);
		return bytes;
	}

	// Compute the stop address value
	uintptr stop = address + length;

	RegionList list =
		// Get the list of regions
		GetRegions (address, stop);

	// Read the memory region by region
	for (uintptr a = address, i = 0; i <
		list.size(); a = list[i++].Stop)
	{
		// Avoid dereferencing each time
		const Region& current = list[i];

	#ifndef NDEBUG
		// Paranoid bounds check
		if (current.Stop >= stop)
			assert (i+1 == list.size());
	#endif

		// Memory region access status
		bool access = current.Readable;

		// Compute result offset
		uint8* offset = (uint8*)
			result + a - address;

		// Compute the current value of length parameter
		uintptr len = std::min (current.Stop, stop) - a;

		// Check whether the region can be made readable
		if (!current.Readable && f == Memory::AutoAccess)
			access = SetAccess (current, true, false, false);

		// Attempt to read memory region or fill with zero
		if (!access || !SysRead (a, offset, len, nullptr))
			memset (offset, 0, len);

		// Check whether region access should be restored
		if (!current.Readable && f == Memory::AutoAccess)
			if (access) SetAccess (current, current.Access);

		bytes += len; // Update total bytes currently read
	}

	return bytes;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Memory::MemWrite (uintptr address,
	const void* data, uintptr length, Flags f)
{
	assert (address != 0);
	assert (  data  != 0);
	assert (length  != 0);

	uintptr bytes = 0;
	// Use default system call
	if (f == Memory::Default)
	{
		SysWrite (address, data, length, &bytes);
		return bytes;
	}

	// Compute the stop address value
	uintptr stop = address + length;

	RegionList list =
		// Get the list of regions
		GetRegions (address, stop);

	// Read the memory region by region
	for (uintptr a = address, i = 0; i <
		list.size(); a = list[i++].Stop)
	{
		// Avoid dereferencing each time
		const Region& current = list[i];

	#ifndef NDEBUG
		// Paranoid bounds check
		if (current.Stop >= stop)
			assert (i+1 == list.size());
	#endif

		// Memory region access status
		bool access = current.Writable;

		// Compute result offset
		uint8* offset = (uint8*)
			data + a - address;

		// Compute the current value of length parameter
		uintptr len = std::min (current.Stop, stop) - a;

		// Check whether the region can be made writable
		if (!current.Writable && f == Memory::AutoAccess)
			access = SetAccess (current, true, true, current.Executable);

		// Attempt to write to the current memory region
		if (access) SysWrite (a, offset, len, nullptr);

		// Check whether region access should be restored
		if (!current.Writable && f == Memory::AutoAccess)
			if (access) SetAccess (current, current.Access);

		bytes += len; // Update total bytes written
	}

	return bytes;
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::SysRead (uintptr address, void*
	result, uintptr length, uintptr* bytes)
{
	assert (address != 0);
	assert (result  != 0);
	assert (length  != 0);

	// Update statistics
	++mData->SystemReads;

#ifdef ROBOT_OS_LINUX

	// NOTE: We assume that the Linux Kernel
	// allows us to read and write to the mem
	// file. This is not the case across some
	// distributions which means we will, at
	// some point, have to implement a ptrace
	// version. Perhaps if we add a debugger.

	// Check file and attempt seek
	if (mData->MemoryFile == -1 ||
		lseek (mData->MemoryFile,
		address, SEEK_SET) == -1)
	{
		// Attempt to reset the bytes read
		if (bytes != nullptr) *bytes = 0;
		++mData->ReadErrors; return false;
	}

	ssize_t count = read
		// Attempt to read from the mem file
		(mData->MemoryFile, result, length);

	if (count == -1)
	{
		// Attempt to reset the bytes read
		if (bytes != nullptr) *bytes = 0;
		++mData->ReadErrors; return false;
	}

	// Attempt to set bytes read result
	if (bytes != nullptr) *bytes = count;
	return true;

#endif
#ifdef ROBOT_OS_MAC

	mach_vm_size_t bytesRead = 0;
	// Read memory at the specified address
	if (mach_vm_read_overwrite (ProcHandle,
		address, length, (mach_vm_address_t)
		result, &bytesRead))
	{
		++mData->ReadErrors;
		if (bytes != nullptr)
			*bytes = bytesRead;

		return false;
	}

	else
	{
		if (bytes != nullptr)
			*bytes = bytesRead;

		return true;
	}

#endif
#ifdef ROBOT_OS_WIN

	bool status = true; SIZE_T bytesRead = 0;
	// Read memory at the specified address
	if (!ReadProcessMemory (ProcHandle, (LPCVOID)
		address, result, length, &bytesRead))
		{ ++mData->ReadErrors; status = false; }

	// Attempt to set the bytes read result
	if (bytes != nullptr) *bytes = bytesRead;
	return status;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Memory::SysWrite (uintptr address, const
	void* data, uintptr length, uintptr* bytes)
{
	assert (address != 0);
	assert (  data  != 0);
	assert (length  != 0);

	// Update statistics
	++mData->SystemWrites;

#ifdef ROBOT_OS_LINUX

	// NOTE: We assume that the Linux Kernel
	// allows us to read and write to the mem
	// file. This is not the case across some
	// distributions which means we will, at
	// some point, have to implement a ptrace
	// version. Perhaps if we add a debugger.

	// Check file and attempt seek
	if (mData->MemoryFile == -1 ||
		lseek (mData->MemoryFile,
		address, SEEK_SET) == -1)
	{
		// Attempt to reset bytes written
		if (bytes != nullptr) *bytes = 0;
		++mData->WriteErrors; return false;
	}

	ssize_t count = write
		// Attempt to write to the mem file
		(mData->MemoryFile, data, length);

	if (count == -1)
	{
		// Attempt to reset bytes written
		if (bytes != nullptr) *bytes = 0;
		++mData->WriteErrors; return false;
	}

	// Attempt to set bytes written result
	if (bytes != nullptr) *bytes = count;
	return true;

#endif
#ifdef ROBOT_OS_MAC

	// Write memory at a specified address
	if (mach_vm_write (ProcHandle, address,
		(uintptr) data, (natural_t) length))
	{
		++mData->WriteErrors;
		if (bytes != nullptr)
			*bytes = 0;

		return false;
	}

	else
	{
		if (bytes != nullptr)
			*bytes = length;

		return true;
	}

#endif
#ifdef ROBOT_OS_WIN

	bool status = true; SIZE_T bytesWritten = 0;
	// Write memory at the specified address
	if (!WriteProcessMemory (ProcHandle, (LPVOID)
		address, data, length, &bytesWritten))
		{ ++mData->WriteErrors; status = false; }

	// Attempt to set the bytes written result
	if (bytes != nullptr) *bytes = bytesWritten;
	return status;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Memory::InitializeVM (void)
{
	// Initialize system info once
	static bool initialized = false;

	if (initialized == false)
	{
		initialized = true;

	#if defined (ROBOT_OS_MAC) || \
		defined (ROBOT_OS_LINUX)

		gMinVM    = 0x000000001000;
		gMaxVM_32 = 0x0000C0000000; // 3G

		#ifdef ROBOT_ARCH_64
			gMaxVM_64 = 0x7FFFFFFF0000;
		#else
			// This shouldn't be used
			gMaxVM_64 = 0x0000C0000000;
		#endif

		#ifdef ROBOT_OS_LINUX

			// Adjust minimum VM value
			ifstream file (PROC_PATH
				"sys/vm/mmap_min_addr");
			if (file) file >> gMinVM;

		#endif

		gPageSize = sysconf (_SC_PAGESIZE);

	#endif
	#ifdef ROBOT_OS_WIN

		SYSTEM_INFO info;
		// Retrieve the system info
		GetNativeSystemInfo (&info);

		gMinVM = (uintptr)
			info.lpMinimumApplicationAddress + 0;

		gMaxVM_64 = (uintptr)
			info.lpMaximumApplicationAddress + 1;

		// Always use full low 32-Bit value
		gMaxVM_32 = gMaxVM_64 & 0xFFFFFFFF;

		gPageSize = (uintptr) info.dwPageSize;

	#endif

		assert ((gMinVM    % gPageSize) == 0);
		assert ((gMaxVM_32 % gPageSize) == 0);
		assert ((gMaxVM_64 % gPageSize) == 0);
	}
}

ROBOT_NS_END
