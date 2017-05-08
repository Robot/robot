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

	// Based on the following implementation:
	// http://support.microsoft.com/kb/118626

	//----------------------------------------------------------------------------//

	#define ACCESS_READ  1
	#define ACCESS_WRITE 2

	GENERIC_MAPPING genericMapping =
	{
		ACCESS_READ,  ACCESS_WRITE, 0,
		ACCESS_READ | ACCESS_WRITE
	};

	BOOL result = FALSE;

	HANDLE token    = nullptr;
	HANDLE tokenImp = nullptr;
	PSID   adminSID = nullptr;

	DWORD status;
	SID_IDENTIFIER_AUTHORITY
		systemAuthoritySID =
		SECURITY_NT_AUTHORITY;

	SECURITY_DESCRIPTOR adminSD;
	PACL  adminACL     = nullptr;
	DWORD adminACLSize = 0;

	PRIVILEGE_SET ps;
	DWORD psSize = sizeof (PRIVILEGE_SET);
	HANDLE processHeap = GetProcessHeap();

	//----------------------------------------------------------------------------//

	// Attempt to open the thread access token
	if (!OpenThreadToken (GetCurrentThread(),
		TOKEN_DUPLICATE | TOKEN_QUERY, TRUE, &token))
	{
		// Check for errors obtaining a token
		if (GetLastError() != ERROR_NO_TOKEN)
			return false;

		// Attempt to open the process access token
		if (!OpenProcessToken (GetCurrentProcess(),
			TOKEN_DUPLICATE | TOKEN_QUERY, &token))
			return false;
	}

	// AccessCheck requires impersonation-level token
	if (!DuplicateToken (token, SecurityImpersonation,
		&tokenImp)) goto exit;

	//----------------------------------------------------------------------------//

	// Create well-known SID that represents the admin group
	if (!AllocateAndInitializeSid (&systemAuthoritySID, 2,
		SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0, &adminSID)) goto exit;

	// Initialize the admin security descriptor
	if (!InitializeSecurityDescriptor (&adminSD,
		SECURITY_DESCRIPTOR_REVISION)) goto exit;

	adminACLSize =
		// Compute size needed for the ACL struct
		sizeof (ACL) + sizeof (ACCESS_ALLOWED_ACE)
		+ GetLengthSid (adminSID) - sizeof (DWORD);

	adminACL = (PACL) HeapAlloc
		// Allocate mem for ACL struct
		(processHeap, 0, adminACLSize);
	if (adminACL == nullptr) goto exit;

	// Initialize the ACL struct
	if (!InitializeAcl (adminACL,
		adminACLSize, ACL_REVISION))
		goto exit;

	// Add the ACE to the previously initialized ACL
	if (!AddAccessAllowedAce (adminACL, ACL_REVISION,
		ACCESS_READ | ACCESS_WRITE, adminSID))
		goto exit;

	// Attach ACL to our security descriptor
	if (!SetSecurityDescriptorDacl (&adminSD,
		TRUE, adminACL, FALSE)) goto exit;

	//----------------------------------------------------------------------------//

	// Setup security descriptor so it passes validation
	SetSecurityDescriptorGroup (&adminSD, adminSID, FALSE);
	SetSecurityDescriptorOwner (&adminSD, adminSID, FALSE);
	if (!IsValidSecurityDescriptor (&adminSD)) goto exit;

	// Perform an access check to determine the result
	if (!AccessCheck (&adminSD, tokenImp, ACCESS_READ,
		&genericMapping, &ps, &psSize, &status, &result))
		{ result = FALSE; goto exit; }

	//----------------------------------------------------------------------------//

exit:
	if (adminACL) HeapFree    (processHeap, 0, adminACL);
	if (adminSID) FreeSid     (adminSID);
	if (tokenImp) CloseHandle (tokenImp);
	if (token   ) CloseHandle (token   );

	// Return check result
	return result != FALSE;

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
