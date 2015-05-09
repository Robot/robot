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
using std::sort;
using std::replace;
using std::unique;
using std::string;

#include <regex>
using std::regex;
using std::regex_match;

#ifdef ROBOT_OS_LINUX

	#include <wait.h>
	#include <dirent.h>
	#include <unistd.h>
	#include <cstring>

	#include <fstream>
	using std::ios;
	using std::ifstream;
	using std:: fstream;
	using std:: getline;

	#include <sys/utsname.h>
	// Path to proc directory
	#define PROC_PATH "/proc/"

#endif
#ifdef ROBOT_OS_MAC

	#include <sys/utsname.h>

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
// Classes                                                            Process //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

struct Process::Data
{
	int32		ProcID;			// The process ID

	string		Name;			// Name of process
	string		Path;			// Path of process

	bool		Is64Bit;		// Process is 64-Bit

#ifdef ROBOT_OS_LINUX

	uint32		Handle;			// Unused handle

#endif
#ifdef ROBOT_OS_MAC

	task_t		Handle;			// The mach task

#endif
#ifdef ROBOT_OS_WIN

	HANDLE		Handle;			// Process handle

#endif
};



//----------------------------------------------------------------------------//
// Constructors                                                       Process //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Process::Process (int32 pid) : mData (new Process::Data(), [](Process::Data* data)
{
#ifdef ROBOT_OS_WIN

	// Close the process handle
	CloseHandle (data->Handle);

#endif

	// Free data
	delete data;
})
{
	mData->ProcID  = 0;
	mData->Handle  = 0;
	mData->Is64Bit = false;
	Open (pid);
}



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
		// Store the ProcID
		mData->ProcID = pid;

	#ifdef ROBOT_ARCH_64
		// Use default setting
		mData->Is64Bit = true;
	#endif

		char exe[32], link[4096];
		// Build path to process's executable link
		snprintf (exe, 32, PROC_PATH "%d/exe", pid);

		// Determine arch by reading ELF
		ifstream file (exe, ios::binary);
		if (file)
		{
			// Arch offset
			file.seekg (4);
			int8 format = 0;

			if (file >> format)
				// 64-Bit if the format is 2
				mData->Is64Bit = format == 2;
		}

		// Read symbolic link of the executable
		auto len = readlink (exe, link, 4096);

		if (len > 0)
		{
			string name, path (link, (size_t) len);

			// Retrieve the file part of the path
			auto last = path.find_last_of ('/');
			if (last == string::npos) name = path;
			else name = path.substr (last + 1);

			// Store both the name and path values
			mData->Name = name; mData->Path = path;
		}

		return true;
	}

	return false;

#endif
#ifdef ROBOT_OS_MAC

	if (pid > 0 && (kill (pid, 0) == 0 || errno != ESRCH))
	{
		// Store the ProcID
		mData->ProcID = pid;

	#ifdef ROBOT_ARCH_64
		// Use default setting
		mData->Is64Bit = true;
	#endif

		proc_bsdshortinfo info;
		// Retrieve BSD info to determine process kind
		if (proc_pidinfo (pid, PROC_PIDT_SHORTBSDINFO, 0,
			&info, PROC_PIDT_SHORTBSDINFO_SIZE)) mData->
			Is64Bit = info.pbsi_flags & PROC_FLAG_LP64;

		// Attempt to retrieve process path
		char link[PROC_PIDPATHINFO_MAXSIZE];
		auto len = proc_pidpath (pid, link,
				  PROC_PIDPATHINFO_MAXSIZE);

		if (len > 0)
		{
			string name, path (link, (size_t) len);

			// Retrieve the file part of the path
			auto last = path.find_last_of ('/');
			if (last == string::npos) name = path;
			else name = path.substr (last + 1);

			// Store both the name and path values
			mData->Name = name; mData->Path = path;
		}

		// Attempt to retrieve the mach task port for the process
		if (task_for_pid (mach_task_self(), pid, &mData->Handle))
			mData->Handle = 0;

		return true;
	}

	return false;

#endif
#ifdef ROBOT_OS_WIN

	mData->Handle = OpenProcess (PROCESS_VM_OPERATION |
		 PROCESS_VM_READ  | PROCESS_QUERY_INFORMATION |
		 PROCESS_VM_WRITE | PROCESS_TERMINATE, FALSE, pid);

	// Check if handle is valid
	if (mData->Handle != nullptr)
	{
		// Store the ProcID
		mData->ProcID = pid;

		// Check if system 64-Bit
		if (Process::IsSys64Bit())
		{
			BOOL is32Bit = TRUE;
			// Set whether process is 64-Bit
			mData->Is64Bit = IsWow64Process
				(mData->Handle, &is32Bit) &&
				is32Bit == FALSE;
		}

		DWORD size = MAX_PATH;
		TCHAR link  [MAX_PATH];
		string name, path;

		if (QueryFullProcessImageName
			// Try and get process path name
			(mData->Handle, 0, link, &size))
		{
		#ifdef UNICODE

			char conv[MAX_PATH * 2];
			size = WideCharToMultiByte (CP_UTF8,
				0, link, size, conv, MAX_PATH*2,
				nullptr, nullptr);

			path.append (conv, size);

		#else

			path.append (link, size);

		#endif

			// Convert any backslashes to normal slashes
			replace (path.begin(), path.end(), '\\', '/');

			// Retrieve the file part of the path
			auto last = path.find_last_of ('/');
			if (last == string::npos) name = path;
			else name = path.substr (last + 1);

			// Store both the name and path values
			mData->Name = name; mData->Path = path;
		}

		return true;
	}

	return false;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Process::Close (void)
{
#ifdef ROBOT_OS_WIN

	// Close the process handle
	CloseHandle (mData->Handle);

#endif

	mData->ProcID  = 0;
	mData->Handle  = 0;
	mData->Is64Bit = false;

	mData->Name.clear();
	mData->Path.clear();
}

////////////////////////////////////////////////////////////////////////////////

bool Process::IsValid (void) const
{
#ifdef ROBOT_OS_LINUX

	return mData->ProcID > 0 &&
		(kill (mData->ProcID, 0)
		== 0 || errno != ESRCH);

#endif
#ifdef ROBOT_OS_MAC

	return mData->ProcID > 0 &&
		(kill (mData->ProcID, 0)
		== 0 || errno != ESRCH);

#endif
#ifdef ROBOT_OS_WIN

	return mData->Handle != 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Process::Is64Bit (void) const
{
	return mData->Is64Bit;
}

////////////////////////////////////////////////////////////////////////////////

bool Process::IsDebugged (void) const
{
	// Check the process validity
	if (!IsValid()) return false;

#ifdef ROBOT_OS_LINUX

	// NYI: Not yet implemented
	return false;

#endif
#ifdef ROBOT_OS_MAC

	// NYI: Not yet implemented
	return false;

#endif
#ifdef ROBOT_OS_WIN

	// NYI: Not yet implemented
	return false;

#endif
}

////////////////////////////////////////////////////////////////////////////////

int32 Process::GetPID (void) const
{
	return mData->ProcID;
}

////////////////////////////////////////////////////////////////////////////////

uintptr Process::GetHandle (void) const
{
	return (uintptr)
		mData->Handle;
}

////////////////////////////////////////////////////////////////////////////////

Memory Process::GetMemory (void) const
{
	// NYI: Not yet implemented
	return Memory();
}

////////////////////////////////////////////////////////////////////////////////

string Process::GetName (void) const
{
	return mData->Name;
}

////////////////////////////////////////////////////////////////////////////////

string Process::GetPath (void) const
{
	return mData->Path;
}

////////////////////////////////////////////////////////////////////////////////

void Process::Exit (void)
{
	// Check process validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	kill (mData->ProcID, SIGTERM);

#endif
#ifdef ROBOT_OS_MAC

	kill (mData->ProcID, SIGTERM);

#endif
#ifdef ROBOT_OS_WIN

	// Get every process window
	auto list = Window::GetList
		(nullptr, mData->ProcID);

	// Close every open window in this process
	for (uintptr i = 0; i < list.size(); ++i)
		list[i].Close();

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Process::Kill (void)
{
	// Check process validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	kill (mData->ProcID, SIGKILL);

#endif
#ifdef ROBOT_OS_MAC

	kill (mData->ProcID, SIGKILL);

#endif
#ifdef ROBOT_OS_WIN

	TerminateProcess (mData->Handle, -1);

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
	return !GetExitCodeProcess
		(mData->Handle, &code)
		|| code != STILL_ACTIVE;

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

	// Get results using the private window call
	return Window::GetList (title, mData->ProcID);
}

////////////////////////////////////////////////////////////////////////////////

ProcessList Process::GetList (const char* name)
{
	ProcessList result;
	// Check if the name is empty
	bool empty = name == nullptr;
	regex regexp; if (!empty) {
		// Attempt to use a case-insensitive regex
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
			regex_match (process.mData->Name, regexp)))
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
			regex_match (process.mData->Name, regexp)))
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
			regex_match (process.mData->Name, regexp)))
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

////////////////////////////////////////////////////////////////////////////////

bool Process::IsSys64Bit (void)
{
	// Initialize only once
	static int8 is64Bit = -1;

	if (is64Bit == -1)
	{
	#if defined (ROBOT_OS_MAC) || \
		defined (ROBOT_OS_LINUX)

		utsname unameData;
		uname (&unameData);

		is64Bit = !strcmp (unameData.
			machine, "x86_64") ? 1:0;

	#endif
	#ifdef ROBOT_OS_WIN

		SYSTEM_INFO info;
		// Retrieve the system info
		GetNativeSystemInfo (&info);

		is64Bit =
			info.wProcessorArchitecture ==
			PROCESSOR_ARCHITECTURE_AMD64 ?
			1 : 0;

	#endif
	}

	return is64Bit == 1;
}



//----------------------------------------------------------------------------//
// Operators                                                          Process //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Process::operator == (const Process& process) const
{
	return mData->ProcID == process.mData->ProcID;
}

////////////////////////////////////////////////////////////////////////////////

bool Process::operator != (const Process& process) const
{
	return mData->ProcID != process.mData->ProcID;
}

////////////////////////////////////////////////////////////////////////////////

bool Process::operator == (int32 pid) const
{
	return mData->ProcID == pid;
}

////////////////////////////////////////////////////////////////////////////////

bool Process::operator != (int32 pid) const
{
	return mData->ProcID != pid;
}

}
