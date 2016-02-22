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
#include "Module.h"

#include <cstring>
using std::string;
using std::vector;

#ifdef ROBOT_OS_MAC

	#include <mach/mach_vm.h>
	#include <mach-o/loader.h>

#endif
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Classes                                                             Module //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

struct Module::Data
{
	bool		Valid;			// Module validity

	string		Name;			// Name of module
	string		Path;			// Path of module

	uintptr		Base;			// Base of module
	uintptr		Size;			// Size of module

	Process		Proc;			// Parent process
	SegmentList	Segments;		// Segments list
};



//----------------------------------------------------------------------------//
// Functions                                                  Module::Segment //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::Contains (uintptr address) const
{
	return Base <= address && address < (Base + Size);
}



//----------------------------------------------------------------------------//
// Operators                                                  Module::Segment //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::operator < (uintptr address) const
{
	return Base < address;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::operator > (uintptr address) const
{
	return Base > address;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::operator <= (uintptr address) const
{
	return Base <= address;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::operator >= (uintptr address) const
{
	return Base >= address;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::operator < (const Segment& segment) const
{
	return Base < segment.Base;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::operator > (const Segment& segment) const
{
	return Base > segment.Base;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::operator <= (const Segment& segment) const
{
	return Base <= segment.Base;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::operator >= (const Segment& segment) const
{
	return Base >= segment.Base;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::operator == (const Segment& segment) const
{
	return Valid == segment.Valid &&
		   Base  == segment.Base  &&
		   Size  == segment.Size  &&
		   strcmp (Name, segment.Name) == 0;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::Segment::operator != (const Segment& segment) const
{
	return Valid != segment.Valid ||
		   Base  != segment.Base  ||
		   Size  != segment.Size  ||
		   strcmp (Name, segment.Name) != 0;
}



//----------------------------------------------------------------------------//
// Constructors                                                        Module //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Module::Module (void)
	: mData (new Module::Data())
{
	mData->Valid = false;
	mData->Base  = 0;
	mData->Size  = 0;
}

////////////////////////////////////////////////////////////////////////////////

Module::Module (const Process& process, const string& name,
			const string& path, uintptr base, uintptr size)
			: mData (new Module::Data())
{
	mData->Valid = true;
	mData->Name  = name;
	mData->Path  = path;
	mData->Base  = base;
	mData->Size  = size;
	mData->Proc  = process;
}



//----------------------------------------------------------------------------//
// Functions                                                           Module //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool    Module::IsValid    (void) const { return mData->Valid; }
string  Module::GetName    (void) const { return mData->Name;  }
string  Module::GetPath    (void) const { return mData->Path;  }
uintptr Module::GetBase    (void) const { return mData->Base;  }
uintptr Module::GetSize    (void) const { return mData->Size;  }
Process Module::GetProcess (void) const { return mData->Proc;  }

////////////////////////////////////////////////////////////////////////////////

Module::SegmentList Module::GetSegments (void) const
{
#ifdef ROBOT_OS_MAC

	// Check if segments should be created
	if (mData->Segments.empty() && mData->
		Base != 0 && mData->Proc.IsValid()
		&& mData->Proc.GetHandle() != 0)
	{
		mach_header header; mach_vm_size_t bytesRead = 0;
		static const uint32 HSize32 = sizeof (mach_header   );
		static const uint32 HSize64 = sizeof (mach_header_64);

		// Attempt to read the current module's mach header
		if (mach_vm_read_overwrite ((task_t) mData->Proc.
			GetHandle(), (mach_vm_address_t) mData->Base,
			HSize32, (mach_vm_address_t) &header, &bytesRead)
			|| bytesRead != HSize32) return mData->Segments;

		const uintptr size =
			// Make sure to select the correct header size
			header.magic == MH_MAGIC ? HSize32 : HSize64;

		// Must allocate enough space to store all commands
		uint8* loadCommands = new uint8[header.sizeofcmds];

		// Attempt to read all of the current module's load commands
		if (mach_vm_read_overwrite ((task_t) mData->Proc.GetHandle(),
			(mach_vm_address_t) (mData->Base + size), header.sizeofcmds,
			(mach_vm_address_t) loadCommands, &bytesRead) || bytesRead
			!= header.sizeofcmds)
		{
			delete[] loadCommands;
			return mData->Segments;
		}

		uintptr slide = 0;
		// Iterate through every segment command
		auto* cmd = (load_command*) loadCommands;
		for (uint32 i = 0; i < header.ncmds; ++i)
		{
			// Check for a 64-bit segment
			if (cmd->cmd == LC_SEGMENT_64)
			{
				auto seg = (segment_command_64*) cmd;
				// Keep track of current address difference
				if (seg->fileoff == 0 && seg->filesize != 0)
					slide += mData->Base - seg->vmaddr;

				// Create a new segment
				Segment segment = { 0 };
				segment.Valid   = true;
				segment.Base = seg->vmaddr + slide;
				segment.Size = seg->vmsize + 0x000;
				strncpy (segment.Name,
					seg->segname, 16);
				segment.Name[15]='\0';

				// Append the newly created segment
				mData->Segments.push_back (segment);
			}

			// Check for 32-bit segment
			if (cmd->cmd == LC_SEGMENT)
			{
				auto seg = (segment_command*) cmd;
				// Keep track of current address difference
				if (seg->fileoff == 0 && seg->filesize != 0)
					slide += mData->Base - seg->vmaddr;

				// Create a new segment
				Segment segment = { 0 };
				segment.Valid   = true;
				segment.Base = seg->vmaddr + slide;
				segment.Size = seg->vmsize + 0x000;
				strncpy (segment.Name,
					seg->segname, 16);
				segment.Name[15]='\0';

				// Append the newly created segment
				mData->Segments.push_back (segment);
			}

			// Progress to the next command using command size
			cmd = (load_command*) ((uint8*) cmd + cmd->cmdsize);
		}

		delete[] loadCommands;
		// Sort segments using their base
		std::sort (mData->Segments.begin(),
				   mData->Segments.end());
	}

#endif

	return mData->Segments;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::Contains (uintptr address) const
{
	return mData->Base <= address && address < (mData->Base + mData->Size);
}



//----------------------------------------------------------------------------//
// Operators                                                           Module //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Module::operator < (uintptr address) const
{
	return mData->Base < address;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::operator > (uintptr address) const
{
	return mData->Base > address;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::operator <= (uintptr address) const
{
	return mData->Base <= address;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::operator >= (uintptr address) const
{
	return mData->Base >= address;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::operator < (const Module& module) const
{
	return mData->Base < module.mData->Base;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::operator > (const Module& module) const
{
	return mData->Base > module.mData->Base;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::operator <= (const Module& module) const
{
	return mData->Base <= module.mData->Base;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::operator >= (const Module& module) const
{
	return mData->Base >= module.mData->Base;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::operator == (const Module& module) const
{
	return mData->Valid == module.mData->Valid &&
		   mData->Base  == module.mData->Base  &&
		   mData->Size  == module.mData->Size  &&
		   mData->Proc  == module.mData->Proc;
}

////////////////////////////////////////////////////////////////////////////////

bool Module::operator != (const Module& module) const
{
	return mData->Valid != module.mData->Valid ||
		   mData->Base  != module.mData->Base  ||
		   mData->Size  != module.mData->Size  ||
		   mData->Proc  != module.mData->Proc;
}

ROBOT_NS_END
