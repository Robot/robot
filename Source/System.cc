////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2017 Robot Developers                       //
//                       See LICENSE for licensing info                       //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
// Prefaces                                                                   //
//----------------------------------------------------------------------------//

#include <cassert>
#include "System.h"

#ifdef ROBOT_OS_LINUX

	#include <unistd.h>
	#include <sys/utsname.h>

	#include <cstring>
	#include <fstream>
	using std::ifstream;

#endif
#ifdef ROBOT_OS_MAC

	#include <unistd.h>
	#include <ApplicationServices/ApplicationServices.h>
	#include <sys/utsname.h>

#endif
#ifdef ROBOT_OS_WIN

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

	// Compositing API
	#include <Dwmapi.h>

	using std:: string;
	using std::wstring;

	#pragma warning (push)
	// Ignore deprecation warning
	#pragma warning (disable:4995)

#endif
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

#ifdef ROBOT_OS_WIN

	////////////////////////////////////////////////////////////////////////////////

	#ifdef UNICODE
		extern  string _UTF8Encode (const wstring& value);
		extern wstring _UTF8Decode (const  string& value);
	#else
		extern  string _UTF8Encode (const  string& value);
		extern  string _UTF8Decode (const  string& value);
	#endif

#endif



//----------------------------------------------------------------------------//
// Fields                                                              System //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool System::mInitialized = false;
bool System::mIs64Bit     = true;

////////////////////////////////////////////////////////////////////////////////

uintptr System::mMinAddress   = 0;
uintptr System::mMaxAddress32 = 0;
uintptr System::mMaxAddress64 = 0;
uintptr System::mPageSize     = 0;



//----------------------------------------------------------------------------//
// Functions                                                           System //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool System::Is64Bit (void)
{
	if (!mInitialized)
		Initialize();

	return mIs64Bit;
}

////////////////////////////////////////////////////////////////////////////////

bool System::IsUserAdmin (void)
{
#ifdef ROBOT_OS_LINUX

	return geteuid() == 0;

#endif
#ifdef ROBOT_OS_MAC

	return geteuid() == 0;

#endif
#ifdef ROBOT_OS_WIN

	// NYI: Not yet implemented
	return false;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool System::IsAxEnabled (bool options)
{
#ifdef ROBOT_OS_LINUX

	return true;

#endif
#ifdef ROBOT_OS_MAC

	// Load functions just once
	static dispatch_once_t once;
	dispatch_once (&once, ^{
		// Open the framework
		void* handle = dlopen
			("/System/Library/Frameworks/Application"
			 "Services.framework/ApplicationServices", RTLD_LAZY);

		// Validate the handle
		if (handle != nullptr)
		{
			*(void**) (&gAXIsProcessTrustedWithOptions) =
				dlsym (handle, "AXIsProcessTrustedWithOptions");

			gkAXTrustedCheckOptionPrompt = (CFStringRef*)
				dlsym (handle, "kAXTrustedCheckOptionPrompt");
		}
	});

	// Check for new OSX 10.9 function
	if (gAXIsProcessTrustedWithOptions)
	{
		// Check whether to show prompt
		auto displayPrompt = options ?
			kCFBooleanTrue : kCFBooleanFalse;

		// Convert display prompt value into a dictionary
		const void* k[] = { *gkAXTrustedCheckOptionPrompt };
		const void* v[] = { displayPrompt };
		CFDictionaryRef o = CFDictionaryCreate
			(nullptr, k, v, 1, nullptr, nullptr);

		// Determine whether the process is actually trusted
		bool result = (*gAXIsProcessTrustedWithOptions) (o);

		// Free memory
		CFRelease (o);
		return result;
	}

	else
	{
		// Ignore deprecated warnings
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wdeprecated-declarations"

		// Check whether we have accessibility access
		return AXAPIEnabled() || AXIsProcessTrusted();
		#pragma clang diagnostic pop
	}

#endif
#ifdef ROBOT_OS_WIN

	return true;

#endif
}

////////////////////////////////////////////////////////////////////////////////

uintptr System::GetMinAddress (void)
{
	if (!mInitialized)
		Initialize();

	return mMinAddress;
}

////////////////////////////////////////////////////////////////////////////////

uintptr System::GetMaxAddress32 (void)
{
	if (!mInitialized)
		Initialize();

	return mMaxAddress32;
}

////////////////////////////////////////////////////////////////////////////////

uintptr System::GetMaxAddress64 (void)
{
	if (!mInitialized)
		Initialize();

	return mMaxAddress64;
}

////////////////////////////////////////////////////////////////////////////////

uintptr System::GetPageSize (void)
{
	if (!mInitialized)
		Initialize();

	return mPageSize;
}

////////////////////////////////////////////////////////////////////////////////

bool System::IsCompositing (void)
{
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_LINUX)

	return true;

#endif
#ifdef ROBOT_OS_WIN

	BOOL enabled = FALSE;
	DwmIsCompositionEnabled (&enabled);
	return enabled != FALSE;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void System::SetCompositing (bool enabled)
{
#ifdef ROBOT_OS_WIN

	DwmEnableComposition (enabled ?
		DWM_EC_ENABLECOMPOSITION :
		DWM_EC_DISABLECOMPOSITION);

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool System::AdjustPrivilege (const char* privilege, bool enable)
{
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_LINUX)

	return true;

#endif
#ifdef ROBOT_OS_WIN

	HANDLE token;
	TOKEN_PRIVILEGES tp;
	bool result = false;

	// Attempt to open the thread access token
	if (!OpenThreadToken (GetCurrentThread(),
		TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
		FALSE, &token))
	{
		// Check for errors obtaining a token
		if (GetLastError() != ERROR_NO_TOKEN)
			return false;

		// Attempt to open the process access token
		if (!OpenProcessToken (GetCurrentProcess(),
			TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
			&token)) return false;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = enable
			 ? SE_PRIVILEGE_ENABLED : 0;

	// Attempt to lookup specified privilege value
	if (!LookupPrivilegeValue (nullptr, _UTF8Decode
		(privilege).data(), &tp.Privileges[0].Luid))
		goto exit;

	// Attempt to adjust requested privilege
	if (!AdjustTokenPrivileges (token, FALSE,
		&tp, 0, nullptr, nullptr)) goto exit;

	// Ensure the privilege was correctly adjusted
	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		goto exit;

	result = true;

exit:
	// Cleanup if needed
	CloseHandle (token);
	return result;

#endif
}



//----------------------------------------------------------------------------//
// Internal                                                            System //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void System::Initialize (void)
{
	mInitialized = true;

#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_LINUX)

	#ifdef ROBOT_ARCH_32

		utsname unameData;
		uname (&unameData);

		// If system is 64-Bit
		if (!strcmp (unameData.
			machine, "x86_64"))
			mIs64Bit = true;

	#endif

	mMinAddress   = 0x000000001000;
	mMaxAddress32 = 0x0000C0000000; // 3G

	#ifdef ROBOT_ARCH_64
		mMaxAddress64 = 0x7FFFFFFF0000;
	#else
		mMaxAddress64 = 0x0000C0000000;
	#endif

	#ifdef ROBOT_OS_LINUX

		// Adjust minimum VM value
		ifstream file (PROC_PATH
			"sys/vm/mmap_min_addr");
		if (file) file >> mMinAddress;

	#endif

	mPageSize = sysconf (_SC_PAGESIZE);

#endif
#ifdef ROBOT_OS_WIN

	SYSTEM_INFO info;
	// Retrieve the system info
	GetNativeSystemInfo (&info);

	#ifdef ROBOT_ARCH_32

		mIs64Bit =
			info.wProcessorArchitecture ==
			PROCESSOR_ARCHITECTURE_AMD64;

	#endif

	mMinAddress = (uintptr)
		info.lpMinimumApplicationAddress + 0;

	mMaxAddress64 = (uintptr)
		info.lpMaximumApplicationAddress + 1;

	mMaxAddress32 = mMaxAddress64 & 0xFFFFFFFF;

	mPageSize = (uintptr) info.dwPageSize;

#endif

	assert ((mMinAddress   % mPageSize) == 0);
	assert ((mMaxAddress32 % mPageSize) == 0);
	assert ((mMaxAddress64 % mPageSize) == 0);
}

ROBOT_NS_END

#ifdef ROBOT_OS_WIN
	#pragma warning (pop)
#endif
