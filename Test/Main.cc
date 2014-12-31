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

#include "Test.h"



//----------------------------------------------------------------------------//
// Main                                                                       //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

int main (int argc, const char** argv)
{
	cout << "\nROBOT TESTING SUITE\n";

#ifdef ROBOT_OS_LINUX
	#ifdef ROBOT_ARCH_32
		cout << "ver " ROBOT_VERSION_STR " - LINUX x32\n";
	#endif
	#ifdef ROBOT_ARCH_64
		cout << "ver " ROBOT_VERSION_STR " - LINUX x64\n";
	#endif
#endif

#ifdef ROBOT_OS_MAC
	#ifdef ROBOT_ARCH_32
		cout << "ver " ROBOT_VERSION_STR " - MAC x32\n";
	#endif
	#ifdef ROBOT_ARCH_64
		cout << "ver " ROBOT_VERSION_STR " - MAC x64\n";
	#endif
#endif

#ifdef ROBOT_OS_WIN
	#ifdef ROBOT_ARCH_32
		cout << "ver " ROBOT_VERSION_STR " - WIN x32\n";
	#endif
	#ifdef ROBOT_ARCH_64
		cout << "ver " ROBOT_VERSION_STR " - WIN x64\n";
	#endif
#endif

	cout << "------------------------------\n" \
			"(C) 2010-2015 Robot Developers\n\n";

	// Check args
	if (argc < 2)
	{
		cout << "This application is designed to test the Robot library for\n" \
				"any  errors on the target system to ensure  compliance and\n" \
				"compatibility. It does  this by running  a series  of test\n" \
				"cases on various components of the library; if an error is\n" \
				"detected all  subsequent  tests are cancelled. In order to\n" \
				"get started, please select the test(s) you wish to run via\n" \
				"the command line. Possible tests include: types, keyboard,\n" \
				"mouse, process, module,  memory, screen, window, clipboard\n" \
				"and taskbar. Multiple tests can be run at the same time.\n\n";

		// All the tests have concluded
		cout << "Press enter to exit\n";
		getchar();
		return 1;
	}

	bool types     = false;
	bool keyboard  = false;
	bool mouse     = false;
	bool process   = false;
	bool module    = false;
	bool memory    = false;
	bool screen    = false;
	bool window    = false;
	bool clipboard = false;
	bool taskbar   = false;

	// Determine which tests to run
	for (int i = 1; i < argc; ++i)
	{
		// Check for a special all keyword
		if (strcmp (argv[i], "all") == 0)
		{
			types     = true;
			keyboard  = true;
			mouse     = true;
			process   = true;
			module    = true;
			memory    = true;
			screen    = true;
			window    = true;
			clipboard = true;
			taskbar   = true;
			break;
		}

		if (strcmp (argv[i], "types"    ) == 0) types     = true; else
		if (strcmp (argv[i], "keyboard" ) == 0) keyboard  = true; else
		if (strcmp (argv[i], "mouse"    ) == 0) mouse     = true; else
		if (strcmp (argv[i], "process"  ) == 0) process   = true; else
		if (strcmp (argv[i], "module"   ) == 0) module    = true; else
		if (strcmp (argv[i], "memory"   ) == 0) memory    = true; else
		if (strcmp (argv[i], "screen"   ) == 0) screen    = true; else
		if (strcmp (argv[i], "window"   ) == 0) window    = true; else
		if (strcmp (argv[i], "clipboard") == 0) clipboard = true; else
		if (strcmp (argv[i], "taskbar"  ) == 0) taskbar   = true;
	}

	int res = 2;
	while (res)
	{
		if (types     && !TestTypes    ()) break;
		if (keyboard  && !TestKeyboard ()) break;
		if (mouse     && !TestMouse    ()) break;
		if (process   && !TestProcess  ()) break;
		if (module    && !TestModule   ()) break;
		if (memory    && !TestMemory   ()) break;
		if (screen    && !TestScreen   ()) break;
		if (window    && !TestWindow   ()) break;
		if (clipboard && !TestClipboard()) break;
		if (taskbar   && !TestTaskbar  ()) break;
		res = 0;
	}

	// All the tests have concluded
	cout << "Press enter to exit\n";
	getchar();
	return res;
}
