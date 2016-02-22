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
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define TEST_PROCESS									\
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
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

static bool TestInvalid (void)
{
	Process p1;
	Process p2; VERIFY (!p2.Open ( 0));
	Process p3; VERIFY (!p3.Open (-1));
	Process p4 (8888);

	VERIFY (!p1.IsValid()); VERIFY (!p1.Is64Bit()); VERIFY (p1.HasExited());
	VERIFY (!p2.IsValid()); VERIFY (!p2.Is64Bit()); VERIFY (p2.HasExited());
	VERIFY (!p3.IsValid()); VERIFY (!p3.Is64Bit()); VERIFY (p3.HasExited());
	VERIFY (!p4.IsValid()); VERIFY (!p4.Is64Bit()); VERIFY (p4.HasExited());

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
	cout << "Warning: The next set of tests cannot be automated\n"
		 << "         Please execute the following instructions\n\n";

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

	int32 pid1 = atoi (input1); VERIFY (pid1 > 0); VERIFY (p1.Open (pid1));
	int32 pid2 = atoi (input2); VERIFY (pid2 > 0); VERIFY (p2.Open (pid2));

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
	p1.Exit();
	p2.Kill();
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
	cout << "Warning: The next set of tests cannot be automated\n"
		 << "         Please execute the following instructions\n\n";

	cout << "Input this application's PID: ";
	char input[32] = { 0 };
	cin.getline (input, 32);
	int32 pid = atoi (input);

	VERIFY (pid > 0);
	Process p1 (pid);
	Process p2 = Process::GetCurrent();

	VERIFY (p1.IsValid()); VERIFY (!p1.HasExited());
	VERIFY (p2.IsValid()); VERIFY (!p2.HasExited());

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
	cout << "Warning: The next set of tests cannot be automated\n"
		 << "         Please verify the following process lists\n\n";

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

////////////////////////////////////////////////////////////////////////////////

bool TestProcess (void)
{
	cout << "BEGIN PROCESS TESTING\n------------------------------\n";
	if (!TestInvalid()) { cout << ">> Invalid Failed\n\n"; return false; }
	if (!TestSelect ()) { cout << ">> Select Failed \n\n"; return false; }
	if (!TestCurrent()) { cout << ">> Current Failed\n\n"; return false; }
	if (!TestGetList()) { cout << ">> GetList Failed\n\n"; return false; }
	cout << ">> Success\n\n"; return true;
}
