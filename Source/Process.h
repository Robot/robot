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
	class Memory;
	class Module;
	class Window;

#ifdef ROBOT_OS_WIN
	#pragma warning (push)
	// Ignore the VS C4251 warning
	#pragma warning (disable:4251)
#endif



//----------------------------------------------------------------------------//
// Types                                                                      //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

typedef std::vector<Module> ModuleList;

////////////////////////////////////////////////////////////////////////////////

typedef std::vector<Window> WindowList;

////////////////////////////////////////////////////////////////////////////////

typedef std::vector<Process> ProcessList;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Process
{
public:
	explicit			Process			(int32 pid = 0);

public:
	bool				 Open			(int32 pid);
	void				Close			(void);

	bool				IsValid			(void) const;
	bool				Is64Bit			(void) const;
	bool				IsDebugged		(void) const;

	int32				GetPID			(void) const;
	uintptr				GetHandle		(void) const;

	std::string			GetName			(void) const;
	std::string			GetPath			(void) const;

	void				Exit			(void);
	void				Kill			(void);
	bool				HasExited		(void) const;

	ModuleList			GetModules		(const char* name  = nullptr) const;
	WindowList			GetWindows		(const char* title = nullptr) const;
	static ProcessList	GetList			(const char* name  = nullptr);

	static Process		GetCurrent		(void);
	static bool			IsSys64Bit		(void);

public:
	bool				operator ==		(const Process& process) const;
	bool				operator !=		(const Process& process) const;

	bool				operator ==		(int32 pid) const;
	bool				operator !=		(int32 pid) const;

private:
	struct Data;
	std::shared_ptr<Data> mData;		// Shared information
};

#ifdef ROBOT_OS_WIN
	#pragma warning (pop)
#endif

ROBOT_NS_END
