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
#include "Module.h"

#include <cstring>
using std::string;
using std::vector;

#ifdef ROBOT_OS_MAC

	// TODO:

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
// Operators                                                  Module::Segment //
//----------------------------------------------------------------------------//

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

Module::Module (Process process,
	string  name, string  path,
	uintptr base, uintptr size)
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

	// TODO:

#endif

	return mData->Segments;
}



//----------------------------------------------------------------------------//
// Operators                                                           Module //
//----------------------------------------------------------------------------//

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
