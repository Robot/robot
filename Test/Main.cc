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

#include "Test.h"



//----------------------------------------------------------------------------//
// Main                                                                       //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

int main (int argc, const char* argv[])
{
	cout << "\nROBOT TESTING SUITE\n";

#ifdef ROBOT_OS_LINUX
	#ifdef ROBOT_ARCH_32
		cout << "ver " ROBOT_VERSION_STR " x32 - LINUX";
	#endif
	#ifdef ROBOT_ARCH_64
		cout << "ver " ROBOT_VERSION_STR " x64 - LINUX";
	#endif
#endif

#ifdef ROBOT_OS_MAC
	#ifdef ROBOT_ARCH_32
		cout << "ver " ROBOT_VERSION_STR " x32 - MAC";
	#endif
	#ifdef ROBOT_ARCH_64
		cout << "ver " ROBOT_VERSION_STR " x64 - MAC";
	#endif
#endif

#ifdef ROBOT_OS_WIN
	#ifdef ROBOT_ARCH_32
		cout << "ver " ROBOT_VERSION_STR " x32 - WIN";
	#endif
	#ifdef ROBOT_ARCH_64
		cout << "ver " ROBOT_VERSION_STR " x64 - WIN";
	#endif
#endif

	cout << (Process::IsSys64Bit() ? " 64\n" : " 32\n");

	cout << "------------------------------\n"
		 << "(C) 2010-2016 Robot Developers\n\n";

	// Check args
	if (argc < 2)
	{
		cout << "This program is designed  to test the Robot library for\n"
			 << "compliance and compatibility on the target platform. It\n"
			 << "does this by running a series of test  cases on various\n"
			 << "components of the  library; if an error is detected all\n"
			 << "subsequent tests are cancelled. In order to get started\n"
			 << "please select the tests you wish to run via the command\n"
			 << "line. Possible tests  include: types, timer,  keyboard,\n"
			 << "mouse,  process, window,  memory, screen and clipboard.\n"
			 << "Multiple tests can be run at the same time.\n\n";

		// All the tests have concluded
		cout << "Press enter to exit\n";
		getchar();
		return 1;
	}

	bool types     = false;
	bool timer     = false;
	bool keyboard  = false;
	bool mouse     = false;
	bool process   = false;
	bool window    = false;
	bool memory    = false;
	bool screen    = false;
	bool clipboard = false;

	// Determine which tests to run
	for (int i = 1; i < argc; ++i)
	{
		// Check for a special all keyword
		if (strcmp (argv[i], "all") == 0)
		{
			types     = true;
			timer     = true;
			keyboard  = true;
			mouse     = true;
			process   = true;
			window    = true;
			memory    = true;
			screen    = true;
			clipboard = true;
			break;
		}

		if (strcmp (argv[i], "types"    ) == 0) types     = true; else
		if (strcmp (argv[i], "timer"    ) == 0) timer     = true; else
		if (strcmp (argv[i], "keyboard" ) == 0) keyboard  = true; else
		if (strcmp (argv[i], "mouse"    ) == 0) mouse     = true; else
		if (strcmp (argv[i], "process"  ) == 0) process   = true; else
		if (strcmp (argv[i], "window"   ) == 0) window    = true; else
		if (strcmp (argv[i], "memory"   ) == 0) memory    = true; else
		if (strcmp (argv[i], "screen"   ) == 0) screen    = true; else
		if (strcmp (argv[i], "clipboard") == 0) clipboard = true;
	}

	int res = 2;
	while (res)
	{
		if (types     && !TestTypes    ()) break;
		if (timer     && !TestTimer    ()) break;
		if (keyboard  && !TestKeyboard ()) break;
		if (mouse     && !TestMouse    ()) break;
		if (process   && !TestProcess  ()) break;
		if (window    && !TestWindow   ()) break;
		if (memory    && !TestMemory   ()) break;
		if (screen    && !TestScreen   ()) break;
		if (clipboard && !TestClipboard()) break;
		res = 0;
	}

	// All the tests have concluded
	cout << "Press enter to exit\n";
	getchar();
	return res;
}
