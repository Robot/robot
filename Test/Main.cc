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

	cout << (Process::IsSys64Bit() ? " 64\n" : " 32\n")
		 << "------------------------------\n"
		 << "(C) 2010-2017 Robot Developers\n\n"

		 << "This program is  designed to test the  Robot library for\n"
		 << "compliance and compatibility on the target platform.  It\n"
		 << "does this by running  a series of test cases  on various\n"
		 << "components of the library. To get started, please select\n"
		 << "the test cases you wish to run by using the  menu below.\n"
		 << "Multiple tests can be run at the same time.\n\n";

	while (true)
	{
		cout << "MAIN MENU\n"
			 << "------------------------------\n"
			 << "   1: Types    \n"
			 << "   2: Timer    \n"
			 << "   3: Keyboard \n"
			 << "   4: Mouse    \n"
			 << "   5: Process  \n"
			 << "   6: Memory   \n"
			 << "   7: Window   \n"
			 << "   8: Screen   \n"
			 << "   9: Overlay  \n"
			 << "  10: Clipboard\n\n";

		// Ask the user to make a selection
		cout << "Enter component(s) to test: ";
		string input; getline (cin, input);

		int selection; cout << endl;
		// Tokenize the input value
		stringstream stream (input);
		while (stream >> selection)
		{
			bool result;
			switch (selection)
			{
				case  1: result = TestTypes    (); break;
				case  2: result = TestTimer    (); break;
				case  3: result = TestKeyboard (); break;
				case  4: result = TestMouse    (); break;
				case  5: result = TestProcess  (); break;
				case  7: result = TestWindow   (); break;
				case  8: result = TestScreen   (); break;
				case  9: result = TestOverlay  (); break;
				case 10: result = TestClipboard(); break;

				case 6:
					cout <<   uppercase;
					result = TestMemory();
					cout << nouppercase;
					break;

				default: continue;
			}

			cout << (result ? ">> Success\n" : ">> Failure\n");
			cout << "Press enter to continue...\n"; getchar();
		}
	}

	return 0;
}
