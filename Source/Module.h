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
#include <string>
#include <vector>

ROBOT_NS_BEGIN
	class Process;

#ifdef ROBOT_OS_WIN
	#pragma warning (push)
	// Ignore the VS C4251 warning
	#pragma warning (disable:4251)
#endif



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Module
{
public:
	////////////////////////////////////////////////////////////////////////////////

	struct ROBOT_EXPORT Segment
	{
		bool			Valid;			// If this structure is valid
		char			Name[16];		// Short name of the segment

		uintptr			Base;			// Base address of the segment
		uintptr			Size;			// Virtual size of the segment

		bool			Contains		(uintptr address) const;

		bool			operator <		(uintptr address) const;
		bool			operator >		(uintptr address) const;
		bool			operator <=		(uintptr address) const;
		bool			operator >=		(uintptr address) const;

		bool			operator <		(const Segment& segment) const;
		bool			operator >		(const Segment& segment) const;
		bool			operator <=		(const Segment& segment) const;
		bool			operator >=		(const Segment& segment) const;

		bool			operator ==		(const Segment& segment) const;
		bool			operator !=		(const Segment& segment) const;
	};

	////////////////////////////////////////////////////////////////////////////////

	typedef std::vector<Segment> SegmentList;

public:
	Module								(void);
	Module								(const Process& process,
										 const std::string& name,
										 const std::string& path,
										 uintptr base, uintptr size);

public:
	bool				IsValid			(void) const;

	std::string			GetName			(void) const;
	std::string			GetPath			(void) const;

	uintptr				GetBase			(void) const;
	uintptr				GetSize			(void) const;

	Process				GetProcess		(void) const;
	SegmentList			GetSegments		(void) const;

	bool				Contains		(uintptr address) const;

public:
	bool				operator <		(uintptr address) const;
	bool				operator >		(uintptr address) const;
	bool				operator <=		(uintptr address) const;
	bool				operator >=		(uintptr address) const;

	bool				operator <		(const Module& module) const;
	bool				operator >		(const Module& module) const;
	bool				operator <=		(const Module& module) const;
	bool				operator >=		(const Module& module) const;

	bool				operator ==		(const Module& module) const;
	bool				operator !=		(const Module& module) const;

private:
	struct Data;
	std::shared_ptr<Data> mData;		// Shared information
};

#ifdef ROBOT_OS_WIN
	#pragma warning (pop)
#endif

ROBOT_NS_END
