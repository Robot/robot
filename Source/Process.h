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

#ifndef ROBOT_PROCESS_H
#define ROBOT_PROCESS_H

#include "Types.h"
#include <string>
#include <vector>

#ifdef ROBOT_OS_WIN
	#include <memory>
#endif

namespace Robot {
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

	int32				GetPID			(void) const;
	uintptr				GetHandle		(void) const;
	Memory				GetMemory		(void) const;

	std::string			GetName			(void) const;
	std::string			GetPath			(void) const;

	void				Exit			(void);
	void				Kill			(void);
	bool				HasExited		(void) const;

	ModuleList			GetModules		(const char* name  = nullptr) const;
	WindowList			GetWindows		(const char* title = nullptr) const;
	static ProcessList	GetList			(const char* name  = nullptr);

	static Process		GetCurrent		(void);

public:
	bool				operator ==		(const Process& process) const;
	bool				operator !=		(const Process& process) const;

	bool				operator ==		(int32 pid) const;
	bool				operator !=		(int32 pid) const;

private:
#ifdef ROBOT_OS_LINUX

	int32 mPID;							// The process ID

#endif
#ifdef ROBOT_OS_MAC

	int32 mPID;							// The process ID

#endif
#ifdef ROBOT_OS_WIN

	std::shared_ptr<uintptr> mHandle;	// Process handle

#endif
};

#ifdef ROBOT_OS_WIN
	#pragma warning (pop)
#endif

}
#endif // ROBOT_PROCESS_H
