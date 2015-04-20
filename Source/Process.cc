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
#include "Module.h"
#include "Window.h"

#include <algorithm>
using std::string;
using std::replace;

#include <regex>
using std::regex;
using std::regex_match;

#ifdef ROBOT_OS_LINUX

	#include <wait.h>
	#include <dirent.h>
	#include <unistd.h>
	#include <cstring>
	#include <X11/Xlib.h>

	// Reference default display
	 extern Display* Robot_Display;
	#define gDisplay Robot_Display

	// Path to proc directory
	#define PROC_PATH "/proc/"

#endif
#ifdef ROBOT_OS_MAC

	// Apple process API
	#include <libproc.h>
	#include <ApplicationServices/ApplicationServices.h>

#endif
#ifdef ROBOT_OS_WIN

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

	// Win process API
	#include <Psapi.h>

#endif
namespace Robot {



//----------------------------------------------------------------------------//
// Constructors                                                       Process //
//----------------------------------------------------------------------------//

#ifdef ROBOT_OS_LINUX

	////////////////////////////////////////////////////////////////////////////////

	Process::Process (int32 pid) :
		mPID (0) { Open (pid); }

#endif
#ifdef ROBOT_OS_MAC

	////////////////////////////////////////////////////////////////////////////////

	Process::Process (int32 pid) :
		mPID (0) { Open (pid); }

#endif
#ifdef ROBOT_OS_WIN

	////////////////////////////////////////////////////////////////////////////////

	Process::Process (int32 pid) : mHandle
	(new uintptr (0), [](uintptr* handle)
	{
		// Manually close process handle
		CloseHandle ((HANDLE) (*handle));

		// Free memory
		delete handle;

	}) { Open (pid); }

#endif



//----------------------------------------------------------------------------//
// Functions                                                          Process //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Process::Open (int32 pid)
{
	// Close opened process
	if (IsValid()) Close();

#ifdef ROBOT_OS_LINUX

	if (pid > 0 && (kill (pid, 0) == 0 || errno != ESRCH))
	{
		mPID = pid;
		return true;
	}

	return false;

#endif
#ifdef ROBOT_OS_MAC

	if (pid > 0 && (kill (pid, 0) == 0 || errno != ESRCH))
	{
		mPID = pid;
		return true;
	}

	return false;

#endif
#ifdef ROBOT_OS_WIN

	*mHandle = (uintptr) OpenProcess (PROCESS_VM_READ |
		PROCESS_VM_WRITE  | PROCESS_QUERY_INFORMATION |
		PROCESS_TERMINATE | PROCESS_VM_OPERATION, FALSE, pid);

	return *mHandle != 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Process::Close (void)
{
#ifdef ROBOT_OS_LINUX

	mPID = 0;

#endif
#ifdef ROBOT_OS_MAC

	mPID = 0;

#endif
#ifdef ROBOT_OS_WIN

	// Manually close process handle
	CloseHandle ((HANDLE) *mHandle);

	// Reset process handle
	*mHandle = (uintptr) 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Process::IsValid (void) const
{
#ifdef ROBOT_OS_LINUX

	return mPID > 0 &&
		(kill (mPID, 0) == 0 || errno != ESRCH);

#endif
#ifdef ROBOT_OS_MAC

	return mPID > 0 &&
		(kill (mPID, 0) == 0 || errno != ESRCH);

#endif
#ifdef ROBOT_OS_WIN

	return *mHandle != 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Process::Is64Bit (void) const
{
	// Check the process validity
	if (!IsValid()) return false;

#ifdef ROBOT_OS_LINUX

	#ifdef ROBOT_ARCH_64
		return true;
	#else
		return false;
	#endif

	// TODO: Better implementation

#endif
#ifdef ROBOT_OS_MAC

	#ifdef ROBOT_ARCH_64
		return true;
	#else
		return false;
	#endif

	// TODO: Better implementation

#endif
#ifdef ROBOT_OS_WIN

	SYSTEM_INFO info = { 0 };
	// Retrieve the system info
	GetNativeSystemInfo (&info);

	// Check whether the OS is 64-bit
	if (info.wProcessorArchitecture ==
		PROCESSOR_ARCHITECTURE_AMD64)
	{
		BOOL result = FALSE;
		return IsWow64Process ((HANDLE)
			*mHandle, &result) != FALSE
			&& result == FALSE;
	}

	return false;

#endif
}

////////////////////////////////////////////////////////////////////////////////

int32 Process::GetPID (void) const
{
	// Check process validity
	if (!IsValid()) return 0;

#ifdef ROBOT_OS_LINUX

	return mPID;

#endif
#ifdef ROBOT_OS_MAC

	return mPID;

#endif
#ifdef ROBOT_OS_WIN

	return GetProcessId ((HANDLE) *mHandle);

#endif
}

////////////////////////////////////////////////////////////////////////////////

uintptr Process::GetHandle (void) const
{
#ifdef ROBOT_OS_LINUX

	return 0;

#endif
#ifdef ROBOT_OS_MAC

	return 0;

#endif
#ifdef ROBOT_OS_WIN

	return *mHandle;

#endif
}

////////////////////////////////////////////////////////////////////////////////

Memory Process::GetMemory (void) const
{
	return Memory();

	// NYI: Not yet implemented
}

////////////////////////////////////////////////////////////////////////////////

string Process::GetName (void) const
{
	// Retrieve process path
	string path = GetPath();

	// Retrieve the file part of the path
	auto last = path.find_last_of ('/');
	if (last == string::npos) return path;
	else return path.substr (last + 1);
}

////////////////////////////////////////////////////////////////////////////////

string Process::GetPath (void) const
{
	// Check if the process is valid
	if (!IsValid()) return string();

#ifdef ROBOT_OS_LINUX

	char exe[  32];
	char lnk[4096];

	// Build path to the process executable (exe)
	snprintf (exe, 32, PROC_PATH "%d/exe", mPID);

	// Read symbolic link of the executable
	ssize_t len = readlink (exe, lnk, 4095);

	if (len > 0)
	{
		// Null terminate
		lnk[len] = '\0';
		return string (lnk);
	}

	return string();

#endif
#ifdef ROBOT_OS_MAC

	char path[PROC_PIDPATHINFO_MAXSIZE];

	if (proc_pidpath (mPID, path,
		PROC_PIDPATHINFO_MAXSIZE) <= 0)
		return string();

	return string (path);

#endif
#ifdef ROBOT_OS_WIN

	DWORD size = MAX_PATH;
	TCHAR path[MAX_PATH];

	if (!QueryFullProcessImageName
		((HANDLE) *mHandle, 0, path,
		&size)) return string();

	#ifdef UNICODE

		char conv[MAX_PATH];
		if (!WideCharToMultiByte (CP_UTF8, 0,
			path, size + 1, conv, MAX_PATH,
			nullptr, nullptr)) return string();

		string res (conv);

	#else

		string res (path);

	#endif

	// Convert any backslashes to normal slashes
	replace (res.begin(), res.end(), '\\', '/');
	return res;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Process::Exit (void)
{
	// Check process validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	kill (mPID, SIGTERM);

#endif
#ifdef ROBOT_OS_MAC

	kill (mPID, SIGTERM);

#endif
#ifdef ROBOT_OS_WIN

	WindowList result = GetWindows();

	// Close every window
	for (uintptr i = 0; i <
		result.size(); ++i)
		result[i].Close();

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Process::Kill (void)
{
	// Check process validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	kill (mPID, SIGKILL);

#endif
#ifdef ROBOT_OS_MAC

	kill (mPID, SIGKILL);

#endif
#ifdef ROBOT_OS_WIN

	TerminateProcess
		((HANDLE) *mHandle, -1);

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Process::HasExited (void) const
{
	// Check the process validity
	if (!IsValid()) return true;

#ifdef ROBOT_OS_LINUX

	return false;

#endif
#ifdef ROBOT_OS_MAC

	return false;

#endif
#ifdef ROBOT_OS_WIN

	DWORD code = -1;
	return GetExitCodeProcess ((HANDLE) *mHandle,
		&code) == FALSE || code != STILL_ACTIVE;

#endif
}

////////////////////////////////////////////////////////////////////////////////

ModuleList Process::GetModules (const char* name) const
{
	ModuleList result;
	// Check the process validity
	if (!IsValid()) return result;

#ifdef ROBOT_OS_LINUX

	// NYI: Not yet implemented
	return result;

#endif
#ifdef ROBOT_OS_MAC

	// NYI: Not yet implemented
	return result;

#endif
#ifdef ROBOT_OS_WIN

	// NYI: Not yet implemented
	return result;

#endif
}

////////////////////////////////////////////////////////////////////////////////

WindowList Process::GetWindows (const char* title) const
{
	// Check whether process is valid
	if (!IsValid()) return WindowList();

	// Get results via a private window call
	return Window::GetList (title, GetPID());
}

////////////////////////////////////////////////////////////////////////////////

ProcessList Process::GetList (const char* name)
{
	ProcessList result;
	// Check if the name is empty
	bool empty = name == nullptr;
	regex regexp; if (!empty) {
		// Attempt to set a case-insensitive regex
		try { regexp = regex (name, regex::icase); }
		catch (...) { return result; }
	}

#ifdef ROBOT_OS_LINUX

	DIR* dir = opendir (PROC_PATH);
	struct dirent* entry = nullptr;

	// Iterate through the entire proc directory
	while ((entry = readdir (dir)) != nullptr)
	{
		// Check if this directory is all numbers
		if (strspn (entry->d_name, "0123456789")
			!= strlen (entry->d_name)) continue;

		// Get the process id as integer
		int32 pid = atoi (entry->d_name);

		Process process;
		// Attempt to open and match current process
		if (process.Open (pid) && (empty == true ||
			regex_match (process.GetName(), regexp)))
		{
			// Append process to result
			result.push_back (process);
		}
	}

	closedir (dir);
	return result;

#endif
#ifdef ROBOT_OS_MAC

	pid_t list[4096];
	// Get current process list
	int32 size  = proc_listpids
		(PROC_ALL_PIDS, 0, list, sizeof (list));
	if (size <= 0) return result;

	// Loop through all system processes
	int32 count = size / sizeof (int32);
	for (uintptr i = 0; i < count; ++i)
	{
		Process process;
		// Attempt to open and match current process
		if (process.Open (list[i]) && (empty == true ||
			regex_match (process.GetName(), regexp)))
		{
			// Append process to result
			result.push_back (process);
		}
	}

	return result;

#endif
#ifdef ROBOT_OS_WIN

	// Get current process list
	DWORD list[4096], size = 0;
	if (!EnumProcesses (list, sizeof (list), &size))
		return result;

	// Loop through all system processes
	DWORD count = size / sizeof (DWORD);
	for (uintptr i = 0; i < count; ++i)
	{
		Process process;
		// Attempt to open and match current process
		if (process.Open (list[i]) && (empty == true ||
			regex_match (process.GetName(), regexp)))
		{
			// Append process to result
			result.push_back (process);
		}
	}

	return result;

#endif
}

////////////////////////////////////////////////////////////////////////////////

Process Process::GetCurrent (void)
{
#ifdef ROBOT_OS_LINUX

	return Process (getpid());

#endif
#ifdef ROBOT_OS_MAC

	return Process (getpid());

#endif
#ifdef ROBOT_OS_WIN

	return Process
		(GetCurrentProcessId());

#endif
}



//----------------------------------------------------------------------------//
// Operators                                                          Process //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Process::operator == (const Process& process) const
{
	return GetPID() == process.GetPID();
}

////////////////////////////////////////////////////////////////////////////////

bool Process::operator != (const Process& process) const
{
	return GetPID() != process.GetPID();
}

////////////////////////////////////////////////////////////////////////////////

bool Process::operator == (int32 pid) const
{
	return GetPID() == pid;
}

////////////////////////////////////////////////////////////////////////////////

bool Process::operator != (int32 pid) const
{
	return GetPID() != pid;
}

}
