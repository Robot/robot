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
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define TEST_PROCESS								\
{													\
	VERIFY ( list1[i].IsValid  ());					\
	VERIFY ( list2[i].IsValid  ());					\
	VERIFY (!list1[i].HasExited());					\
													\
	VERIFY ( (list1[i] == list2[i]));				\
	VERIFY (!(list1[i] != list2[i]));				\
													\
	cout << setw (6) << list1[i].GetPID() << " "	\
		 << (list1[i].Is64Bit() ? "x64 " : "x32 ")	\
		 <<  list1[i].GetName() << endl;			\
}



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

static bool TestInvalid (void)
{
	Process p1;
	Process p2; VERIFY (!p2.Open ( 0));
	Process p3; VERIFY (!p3.Open (-1));
	Process p4 (8888);

	VERIFY (!p1.IsValid()); VERIFY (!p1.Is64Bit());
	VERIFY (!p2.IsValid()); VERIFY (!p2.Is64Bit());
	VERIFY (!p3.IsValid()); VERIFY (!p3.Is64Bit());
	VERIFY (!p4.IsValid()); VERIFY (!p4.Is64Bit());

	VERIFY (!p1.Exit()); VERIFY (!p1.Kill()); VERIFY (p1.HasExited());
	VERIFY (!p2.Exit()); VERIFY (!p2.Kill()); VERIFY (p2.HasExited());
	VERIFY (!p3.Exit()); VERIFY (!p3.Kill()); VERIFY (p3.HasExited());
	VERIFY (!p4.Exit()); VERIFY (!p4.Kill()); VERIFY (p4.HasExited());

	VERIFY (p1.GetPID() == 0); VERIFY (p1.GetHandle() == 0);
	VERIFY (p2.GetPID() == 0); VERIFY (p2.GetHandle() == 0);
	VERIFY (p3.GetPID() == 0); VERIFY (p3.GetHandle() == 0);
	VERIFY (p4.GetPID() == 0); VERIFY (p4.GetHandle() == 0);

	VERIFY (p1.GetName().empty()); VERIFY (p1.GetPath().empty());
	VERIFY (p2.GetName().empty()); VERIFY (p2.GetPath().empty());
	VERIFY (p3.GetName().empty()); VERIFY (p3.GetPath().empty());
	VERIFY (p4.GetName().empty()); VERIFY (p4.GetPath().empty());

	VERIFY ( (p1 == p2)); VERIFY ( (p2 == p1));
	VERIFY (!(p1 != p2)); VERIFY (!(p2 != p1));
	VERIFY ( (p3 == p4)); VERIFY ( (p4 == p3));
	VERIFY (!(p3 != p4)); VERIFY (!(p4 != p3));

	VERIFY (p1 == 0); VERIFY (p1 != 8888);
	VERIFY (p2 == 0); VERIFY (p2 != 8888);
	VERIFY (p3 == 0); VERIFY (p3 != 8888);
	VERIFY (p4 == 0); VERIFY (p4 != 8888);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestSelect (void)
{
	Process p1, p2;
	char input1[32] = { 0 };
	char input2[32] = { 0 };

#ifdef ROBOT_OS_LINUX

	cout << "Open Leafpad and input PID: "; cin.getline (input1, 32);
	cout << "Open gedit   and input PID: "; cin.getline (input2, 32);

#endif
#ifdef ROBOT_OS_MAC

	cout << "Open TextEdit and input PID: "; cin.getline (input1, 32);
	cout << "Open Notes    and input PID: "; cin.getline (input2, 32);

#endif
#ifdef ROBOT_OS_WIN

	cout << "Open Notepad and input PID: "; cin.getline (input1, 32);
	cout << "Open Wordpad and input PID: "; cin.getline (input2, 32);

#endif

	auto pid1 = atoi (input1); VERIFY (pid1 > 0); VERIFY (p1.Open (pid1));
	auto pid2 = atoi (input2); VERIFY (pid2 > 0); VERIFY (p2.Open (pid2));

	VERIFY (p1.IsValid()); VERIFY (!p1.HasExited());
	VERIFY (p2.IsValid()); VERIFY (!p2.HasExited());
	VERIFY (p1.Is64Bit() == p2.Is64Bit());

	VERIFY (p1.GetPID() == pid1);
	VERIFY (p2.GetPID() == pid2);

#ifdef ROBOT_OS_LINUX

	VERIFY (p1.GetHandle() == 0);
	VERIFY (p2.GetHandle() == 0);
	VERIFY (p1.GetHandle() == p2.GetHandle());

	VERIFY (p1.GetName() == "leafpad");
	VERIFY (p2.GetName() == "gedit"  );

	VERIFY (p1.GetPath() == "/usr/bin/leafpad");
	VERIFY (p2.GetPath() == "/usr/bin/gedit"  );

#endif
#ifdef ROBOT_OS_MAC

	VERIFY (p1.GetName() == "TextEdit");
	VERIFY (p2.GetName() == "Notes"   );

	VERIFY (p1.GetPath() == "/Applications/TextEdit.app/Contents/MacOS/TextEdit");
	VERIFY (p2.GetPath() == "/Applications/Notes" ".app/Contents/MacOS/Notes"   );

#endif
#ifdef ROBOT_OS_WIN

	VERIFY (p1.GetHandle() != 0);
	VERIFY (p2.GetHandle() != 0);
	VERIFY (p1.GetHandle() != p2.GetHandle());

	VERIFY (p1.GetName() == "notepad.exe");
	VERIFY (p2.GetName() == "wordpad.exe");

	VERIFY (p1.GetPath() == "C:/Windows/System32/notepad.exe"                    );
	VERIFY (p2.GetPath() == "C:/Program Files/Windows NT/Accessories/wordpad.exe");

#endif

	VERIFY (!(p1 == p2)); VERIFY (!(p2 == p1));
	VERIFY ( (p1 != p2)); VERIFY ( (p2 != p1));

	VERIFY (p1 == pid1); VERIFY (p1 != 8888);
	VERIFY (p2 == pid2); VERIFY (p2 != 8888);

	VERIFY (!(p1 != pid1)); VERIFY (!(p1 == 8888));
	VERIFY (!(p2 != pid2)); VERIFY (!(p2 == 8888));

	cout << "Type something in both apps then press enter";
	getchar();
	VERIFY (p1.Exit());
	VERIFY (p2.Kill());
	cout << "Close both applications and then press enter";
	getchar();

#ifdef ROBOT_OS_WIN

	VERIFY (p1.IsValid());
	VERIFY (p2.IsValid());

#endif

	VERIFY (p1.HasExited());
	VERIFY (p2.HasExited());

	cout << endl;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestCurrent (void)
{
	cout << "Input this application's PID: ";
	char input[32] = { 0 };
	cin.getline (input, 32);
	auto pid = atoi (input);

	VERIFY (pid > 0);
	Process p1 (pid);
	Process p2 = Process::GetCurrent();

	VERIFY (p1.IsValid()); VERIFY (!p1.HasExited());
	VERIFY (p2.IsValid()); VERIFY (!p2.HasExited());

	VERIFY (        pid == Process::GetCurrentPID());
	VERIFY (p1.GetPID() == Process::GetCurrentPID());
	VERIFY (p2.GetPID() == Process::GetCurrentPID());

	VERIFY (p1.GetPID() == pid);
	VERIFY (p2.GetPID() == pid);

#ifdef ROBOT_ARCH_32

	VERIFY (!p1.Is64Bit());
	VERIFY (!p2.Is64Bit());

#endif
#ifdef ROBOT_ARCH_64

	VERIFY (p1.Is64Bit());
	VERIFY (p2.Is64Bit());

#endif

#ifdef ROBOT_OS_LINUX

	VERIFY (p1.GetHandle() == 0);
	VERIFY (p2.GetHandle() == 0);
	VERIFY (p1.GetHandle() == p2.GetHandle());

#endif
#ifdef ROBOT_OS_MAC

	VERIFY (p1.GetHandle() != 0);
	VERIFY (p2.GetHandle() != 0);
	VERIFY (p1.GetHandle() == p2.GetHandle());

#endif
#ifdef ROBOT_OS_WIN

	VERIFY (p1.GetHandle() != 0);
	VERIFY (p2.GetHandle() != 0);
	VERIFY (p1.GetHandle() != p2.GetHandle());

#endif

	VERIFY ( (p1 == p2)); VERIFY ( (p2 == p1));
	VERIFY (!(p1 != p2)); VERIFY (!(p2 != p1));

	VERIFY (p1 == pid); VERIFY (p1 != 8888);
	VERIFY (p2 == pid); VERIFY (p2 != 8888);

	cout << "Verify the following information\n"
		 << "Path: " << p2.GetPath() << "\n\n";

	p1.Close(); VERIFY (!p1.IsValid());
	p2.Close(); VERIFY (!p2.IsValid());

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestGetList (void)
{
#ifdef ROBOT_OS_LINUX

	cout << "Open a couple Leafpads & gedits and press enter\n";

#endif
#ifdef ROBOT_OS_MAC

	cout << "Open a couple TextEdits & Notes and press enter\n";

#endif
#ifdef ROBOT_OS_WIN

	cout << "Open a couple Notepads & Wordpads and press enter\n";

#endif

	getchar();

#if defined (ROBOT_OS_LINUX) || \
	defined (ROBOT_OS_WIN  )

	// This result is unreliable on OSX
	VERIFY (Process::GetList ("*").empty());
	VERIFY (Process::GetList (")").empty());

#endif

	ProcessList list1 = Process::GetList (    );
	ProcessList list2 = Process::GetList (".*");
	cout << "List all - " << list1.size() << endl;

	VERIFY (list1.size() !=            0);
	VERIFY (list1.size() == list2.size());

	for (uintptr i = 0; i < list1.size(); ++i)
		TEST_PROCESS; cout << endl;

	list1 = Process::GetList (".*a.*");
	list2 = Process::GetList (".*A.*");
	cout << "List *a* - " << list1.size() << endl;
	VERIFY (list1.size() == list2.size());

	for (uintptr i = 0; i < list1.size(); ++i)
	{
		auto name = list1[i].GetName();
		VERIFY (name.find ('a') != string::npos ||
				name.find ('A') != string::npos);
	}

	cout << "Verified\n\n";

#ifdef ROBOT_OS_LINUX

	list1 = Process::GetList (".*leafpad.*|.*gedit.*");
	list2 = Process::GetList (".*gEdit.*|.*Leafpad.*");

#endif
#ifdef ROBOT_OS_MAC

	list1 = Process::GetList (".*textedit.*|.*notes.*");
	list2 = Process::GetList (".*Notes.*|.*TextEdit.*");

#endif
#ifdef ROBOT_OS_WIN

	list1 = Process::GetList (".*notepad.*|.*wordpad.*");
	list2 = Process::GetList (".*WordPad.*|.*NotePad.*");

#endif

	cout << "List apps - " << list1.size() << endl;

	VERIFY (list1.size() !=            0);
	VERIFY (list1.size() == list2.size());

	for (uintptr i = 0; i < list1.size(); ++i)
		TEST_PROCESS; cout << endl;

	return true;
}



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool TestProcess (void)
{
	cout << "TEST PROCESS\n"
		 << "------------------------------\n"
		 << "  0: All    \n"
		 << "  1: Invalid\n"
		 << "  2: Select \n"
		 << "  3: Current\n"
		 << "  4: GetList\n\n";

	// Ask the user to make a selection
	cout << "Enter component(s) to test: ";
	string input; getline (cin, input);

	int selection; cout << endl;
	// Tokenize the input value
	stringstream stream (input);
	while (stream >> selection)
	{
		// Test everything
		if (selection == 0)
		{
			return TestInvalid()
				&& TestSelect ()
				&& TestCurrent()
				&& TestGetList();
		}

		switch (selection)
		{
			case 1: if (!TestInvalid()) return false; break;
			case 2: if (!TestSelect ()) return false; break;
			case 3: if (!TestCurrent()) return false; break;
			case 4: if (!TestGetList()) return false; break;
		}
	}

	return true;
}
