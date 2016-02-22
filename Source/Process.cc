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
	#include <sys/stat.h>

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
	#include <mach/task.h>
	#include <mach/mach_vm.h>

	// Apple process API
	#include <libproc.h>
	#include <ApplicationServices/ApplicationServices.h>

	#ifndef EXC_MASK_GUARD
	#define EXC_MASK_GUARD 0
	#endif

#endif
#ifdef ROBOT_OS_WIN

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

	// Win process API
	#include <Psapi.h>
	using std::wstring;

#endif
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

#ifdef ROBOT_OS_WIN

	#ifdef UNICODE

		////////////////////////////////////////////////////////////////////////////////

		string _UTF8Encode (const wstring& value)
		{
			// Do nothing if parameter is empty
			if (value.empty()) return string();

			int size = 0;
			// Compute size of the resulting string
			size = WideCharToMultiByte (CP_UTF8, 0,
				&value[0], (int) value.size(),
				nullptr, 0, nullptr, nullptr);

			string result (size, 0);
			// Convert wide-string to a UTF8 string
			size = WideCharToMultiByte (CP_UTF8, 0,
				&value [0], (int) value.size(),
				&result[0], size, nullptr, nullptr);

			// Return result if encoding succeeded
			return size > 0 ? result : string();
		}

		////////////////////////////////////////////////////////////////////////////////

		wstring _UTF8Decode (const string& value)
		{
			// Do nothing if parameter is empty
			if (value.empty()) return wstring();

			int size = 0;
			// Compute size of resulting string
			size = MultiByteToWideChar (CP_UTF8,
				0, &value[0], (int) value.size(),
				nullptr, 0);

			wstring result (size, 0);
			// Convert UTF8 string to wide-string
			size = MultiByteToWideChar (CP_UTF8,
				0, &value[0], (int) value.size(),
				&result[0], size);

			// Return result if encoding succeeded
			return size > 0 ? result : wstring();
		}

	#else

		////////////////////////////////////////////////////////////////////////////////

		string _UTF8Encode (const string& value)
		{
			return value;
		}

		////////////////////////////////////////////////////////////////////////////////

		string _UTF8Decode (const string& value)
		{
			return value;
		}

	#endif

#endif



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

#ifdef ROBOT_OS_MAC

	////////////////////////////////////////////////////////////////////////////////

	struct AllImageInfo
	{
		uint32 Version;
		uint32 Count;

		union
		{
			uint32 Array32;
			uint64 Array64;
		};

		uint64 Pad[64];
	};

	////////////////////////////////////////////////////////////////////////////////

	struct ImageInfo32
	{
		uint32 Addr;
		uint32 Path;
		uint32 Date;
	};

	////////////////////////////////////////////////////////////////////////////////

	struct ImageInfo64
	{
		uint64 Addr;
		uint64 Path;
		uint64 Date;
	};

#endif



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

	#ifdef ROBOT_ARCH_32
		// Don't attach to x64 processes from x86 apps
		if (mData->Is64Bit) { Close(); return false; }
	#endif

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

	#ifdef ROBOT_ARCH_32
		// Don't attach to x64 processes from x86 apps
		if (mData->Is64Bit) { Close(); return false; }
	#endif

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

	#ifdef ROBOT_ARCH_32
		// Don't attach to x64 processes from x86 apps
		if (mData->Is64Bit) { Close(); return false; }
	#endif

		DWORD size = MAX_PATH;
		TCHAR link  [MAX_PATH];
		string name, path;

		if (QueryFullProcessImageName
			// Try and get process path name
			(mData->Handle, 0, link, &size))
		{
			path = _UTF8Encode (link);
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

	char status[32];
	// Build path to the status file
	snprintf (status, 32, PROC_PATH
		"%d/status", mData->ProcID);

	// Attempt to open status file
	fstream file (status, ios::in);
	if (!file) return false;

	string line;
	// Create the mappings list
	while (getline (file, line))
	{
		// Check if reached tracer and return result
		if (line.find ("TracerPid:") != string::npos)
			return atoi (line.data() + 10) != 0;
	}

	return false;

#endif
#ifdef ROBOT_OS_MAC

	// Verify whether mach task is valid
	if (mData->Handle == 0) return false;

	// Based on: stackoverflow.com/questions/2200277
	exception_mask_t      masks    [EXC_TYPES_COUNT];
	mach_port_t           ports    [EXC_TYPES_COUNT];
	exception_behavior_t  behaviors[EXC_TYPES_COUNT];
	thread_state_flavor_t flavors  [EXC_TYPES_COUNT];
	mach_msg_type_number_t count = 0;

	static const exception_mask_t excMask =
		EXC_MASK_ALL & ~(EXC_MASK_RESOURCE | EXC_MASK_GUARD);

	// Get send rights to task's exception ports
	if (!task_get_exception_ports (mData->Handle,
		excMask, masks, &count, ports, behaviors, flavors))
	{
		// A debugger is present if any mach port is valid
		for (mach_msg_type_number_t i = 0; i < count; ++i)
			if (MACH_PORT_VALID (ports[i])) return true;
	}

	return false;

#endif
#ifdef ROBOT_OS_WIN

	BOOL res = FALSE;
	// Is the process being debugged
	return CheckRemoteDebuggerPresent
		(mData->Handle, &res) && res;

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

	// Check if the name is empty
	bool empty = name == nullptr;
	regex regexp; if (!empty) {
		// Attempt to use a case-insensitive regex
		try { regexp = regex (name, regex::icase); }
		catch (...) { return result; }
	}

#ifdef ROBOT_OS_LINUX

	char maps[32];
	// Build path to PID maps file
	snprintf (maps, 32, PROC_PATH
		"%d/maps", mData->ProcID);

	// Attempt to open maps file
	fstream file (maps, ios::in);
	if (!file) return result;

	// Delegate for processing our modules
	auto storeModule = [&] (uintptr start,
		uintptr stop, const string& path)
	{
		string name;
		// Retrieve the file part of the path
		auto last = path.find_last_of ('/');
		if (last == string::npos) name = path;
		else name = path.substr (last + 1);

		// Check if current module name matches
		if (empty || regex_match (name, regexp))
		{
			result.push_back (Module
				(*this, name, path,
				start, stop - start));
		}
	};

	char currentPath[1024];
	currentPath[0] = '\0';
	uintptr currentStart = 0;
	uintptr currentStop  = 0;

	string line;
	// Create the mappings list
	while (getline (file, line))
	{
		size_t start; uint32 offset, inode;
		size_t stop;  uint16 devmaj, devmin;
		char access[5]; char pathname[1024];

		// Parse the current line into a single complete mapping entry
		if (sscanf (line.data(), "%zx-%zx %4s %x %hx:%hx %u %1023s",
			&start, &stop, access, &offset, &devmaj, &devmin, &inode,
			pathname) != 8) continue;

		// Check if module continued from before
		if (strcmp (currentPath, pathname) == 0)
			{ currentStop = stop; continue; }

		if (currentStart != 0)
			storeModule (currentStart, currentStop, currentPath);

		// Check whether the current pathname is valid
		if (pathname[0] == '\0' || pathname[0] == '[')
			{ currentStart = 0; currentPath[0] = '\0'; }

		else
		{
			currentStart = start;
			currentStop  = stop;
			strncpy (currentPath, pathname, 1024);
			currentPath[1023] = '\0';
		}
	}

	if (currentStart != 0)
		// Append any modules that are at the end of the file
		storeModule (currentStart, currentStop, currentPath);

	// Sort modules using their address
	sort (result.begin(), result.end());

#endif
#ifdef ROBOT_OS_MAC

	// Verify whether mach task is valid
	if (mData->Handle == 0) return result;

	task_dyld_info dyldInfo;
	mach_msg_type_number_t count;
	count = TASK_DYLD_INFO_COUNT;
	mach_vm_size_t bytesRead = 0;
	task_t task = mData->Handle;

	// Find the task's dyld-info address
	if (task_info (task, TASK_DYLD_INFO,
		(task_info_t) &dyldInfo, &count))
		return result;

	// This shouldn't happen, but if it does, avoid overflow
	if (dyldInfo.all_image_info_size > sizeof (AllImageInfo))
		return result;

	AllImageInfo allInfo;
	// Read the all-image-info address
	if (mach_vm_read_overwrite (task,
		dyldInfo.all_image_info_addr,
		dyldInfo.all_image_info_size,
		(mach_vm_address_t) &allInfo,
		&bytesRead)) return result;

	if (allInfo.Count == 0 || bytesRead
		!= dyldInfo.all_image_info_size)
		return result;

	// Delegate for processing our modules
	auto storeModule = [&] (uint32 index,
		uint64 imageAddr, uint64 imagePath)
	{
		std::string path; std::string name;
		// Check if module is executable
		if (index == 0) path = mData->Path;

		else
		{
			char mPath[PATH_MAX];
			char rPath[PATH_MAX];
			// Attempt to read the file path of the current image
			if (mach_vm_read_overwrite (task, (mach_vm_address_t)
				imagePath, PATH_MAX, (mach_vm_address_t) mPath,
				&bytesRead) || bytesRead != PATH_MAX) return;

			// Convert path to a canonicalized absolute pathname
			path = realpath (mPath, rPath) == 0 ? mPath : rPath;
		}

		// Retrieve the file part of the path
		auto last = path.find_last_of ('/');
		if (last == string::npos) name = path;
		else name = path.substr (last + 1);

		// Check if current module name matches
		if (empty || regex_match (name, regexp))
		{
			result.push_back (Module
				(*this, name, path,
				(uintptr) imageAddr, 0));
		}
	};

	if (mData->Is64Bit)
	{
		// Ensure array is valid
		if (allInfo.Array64 == 0)
			return result;

		// Allocate enough data to store entire info array
		ImageInfo64* info = new ImageInfo64[allInfo.Count];

		// Compute sizes of info array
		mach_vm_size_t size64 = sizeof
		(ImageInfo64) * allInfo.Count;

		// Read the entire info array as single read operation
		if (mach_vm_read_overwrite (task, (mach_vm_address_t)
			allInfo.Array64, size64, (mach_vm_address_t) info,
			&bytesRead) == KERN_SUCCESS && bytesRead == size64)
		{
			// Iterate through all info array elements
			for (uint32 i = 0; i < allInfo.Count; ++i)
			{
				// Use delegate to process and add our module
				storeModule (i, info[i].Addr, info[i].Path);
			}
		}

		delete[] info;
	}

	else
	{
		// Ensure array is valid
		if (allInfo.Array32 == 0)
			return result;

		// Allocate enough data to store entire info array
		ImageInfo32* info = new ImageInfo32[allInfo.Count];

		// Compute sizes of info array
		mach_vm_size_t size32 = sizeof
		(ImageInfo32) * allInfo.Count;

		// Read the entire info array as single read operation
		if (mach_vm_read_overwrite (task, (mach_vm_address_t)
			allInfo.Array32, size32, (mach_vm_address_t) info,
			&bytesRead) == KERN_SUCCESS && bytesRead == size32)
		{
			// Iterate through all info array elements
			for (uint32 i = 0; i < allInfo.Count; ++i)
			{
				// Use delegate to process and add our module
				storeModule (i, info[i].Addr, info[i].Path);
			}
		}

		delete[] info;
	}

	// Sort modules using their address
	sort (result.begin(), result.end());

	auto iterator =
		// Remove duplicates in strange cases
		unique (result.begin(), result.end());
		result.erase (iterator, result.end());

#endif
#ifdef ROBOT_OS_WIN

	// Save current process handle
	HANDLE handle = mData->Handle;

	// Retrieve current process modules
	HMODULE list[2048]; DWORD size = 0;
	if (!EnumProcessModulesEx (handle,
		list, sizeof (list), &size,
		LIST_MODULES_ALL)) return result;

	// Loop through every process module
	DWORD count = size / sizeof (HMODULE);
	for (DWORD i = 0; i < count; ++i)
	{
		std::string path, name;
		TCHAR modPath[MAX_PATH];

		// Get the current modules path
		if (GetModuleFileNameEx (handle,
			list[i], modPath, MAX_PATH))
		{
			path = _UTF8Encode (modPath);
			// Convert any backslashes to normal slashes
			replace (path.begin(), path.end(), '\\', '/');

			// Retrieve the file part of the path
			auto last = path.find_last_of ('/');
			if (last == string::npos) name = path;
			else name = path.substr (last + 1);
		}

		// Check if current module name matches
		if (empty || regex_match (name, regexp))
		{
			MODULEINFO info = { 0 };
			// Attempt to retrieve module info
			if (GetModuleInformation (handle,
				list[i], &info, sizeof (info)))
			{
				result.push_back
					(Module (*this, name, path,
					(uintptr) info.lpBaseOfDll,
					(uintptr) info.SizeOfImage));
			}
		}
	}

	// Sort modules using their address
	sort (result.begin(), result.end());

#endif

	return result;
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

ROBOT_NS_END
