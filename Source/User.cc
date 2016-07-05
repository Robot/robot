////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2016 Robot Developers                       //
//                       (C) 2016 Chris Gregory czipperz@gmail.com            //
//                       See LICENSE for licensing info                       //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
// Prefaces                                                                   //
//----------------------------------------------------------------------------//

#include "User.h"

#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_LINUX)
#include <unistd.h>
#include <sys/types.h>
#endif

#if defined (ROBOT_OS_WIN)
#include <Winbase.h>
#endif

ROBOT_NS_BEGIN

bool User::IsAdmin(void)
{
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_LINUX)
	return getuid() == 0;
#endif
#ifdef ROBOT_OS_WIN
	BOOL b;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	b = AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&AdministratorsGroup);
	if(b)
	{
		if (!CheckTokenMembership (NULL, AdministratorsGroup, &b))
		{
			b = FALSE;
		}
		FreeSid(AdministratorsGroup);
	}

	return b == TRUE;
#endif
}

ROBOT_NS_END
