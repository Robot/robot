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
// Locals                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

static bool TestGenericInfo (void)
{
	cout << "Is64Bit     : " << (System::Is64Bit    () ? "True" : "False") << "\n"
		 << "IsUserAdmin : " << (System::IsUserAdmin() ? "True" : "False") << "\n"
		 << "IsAxEnabled : " << (System::IsAxEnabled() ? "True" : "False") << "\n\n";

	cout <<   uppercase << hex
		 << "MinAddress  : 0x" << setw (12) << setfill ('0') << System::GetMinAddress  () << "\n"
		 << "MaxAddress32: 0x" << setw (12) << setfill ('0') << System::GetMaxAddress32() << "\n"
		 << "MaxAddress64: 0x" << setw (12) << setfill ('0') << System::GetMaxAddress64() << "\n"
		 << "PageSize    : 0x" << setw (12) << setfill ('0') << System::GetPageSize    () << "\n\n"
		 << nouppercase << dec;

	cout << "Please verify these results\n"; getchar();
	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestCompositing (void)
{
#if defined (ROBOT_OS_LINUX) || \
	defined (ROBOT_OS_MAC  )

	VERIFY (System::IsCompositing());

	System::SetCompositing (false);
	VERIFY (System::IsCompositing());

	System::SetCompositing (true );
	VERIFY (System::IsCompositing());

#endif
#ifdef ROBOT_OS_WIN

	cout << "Composition status: "
		 << (System::IsCompositing() ?
			 "enabled" : "disabled");
	getchar();

	System::SetCompositing (false);

	cout << "Composition status: "
		 << (System::IsCompositing() ?
			 "enabled" : "disabled");
	getchar();

	System::SetCompositing (true );

	cout << "Composition status: "
		 << (System::IsCompositing() ?
			 "enabled" : "disabled");
	getchar();

	cout << endl;

#endif

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestPrivileges (void)
{
#if defined (ROBOT_OS_LINUX) || \
	defined (ROBOT_OS_MAC  )

	VERIFY (System::AdjustPrivilege (""));

#endif
#ifdef ROBOT_OS_WIN

	cout << "Warning: This test must be ran as a standalone app!\n";
	cout << "Enter PID of an application on another user account: ";
	char input[32] = { 0 };
	cin.getline (input, 32);
	auto pid = atoi (input);

	cout << endl;
	Process process;
	VERIFY (pid > 0);

	VERIFY (!process.Open (pid)); process.Close();

	VERIFY (System::AdjustPrivilege ("SeDebugPrivilege", true ));
	VERIFY ( process.Open (pid)); process.Close();

	VERIFY (System::AdjustPrivilege ("SeDebugPrivilege", false));
	VERIFY (!process.Open (pid)); process.Close();

#endif

	return true;
}



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool TestSystem (void)
{
	cout << "TEST SYSTEM\n"
		 << "------------------------------\n"
		 << "  0: All        \n"
		 << "  1: GenericInfo\n"
		 << "  2: Compositing\n"
		 << "  3: Privileges \n\n";

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
			return TestGenericInfo()
				&& TestCompositing()
				&& TestPrivileges ();
		}

		switch (selection)
		{
			case 1: if (!TestGenericInfo()) return false; break;
			case 2: if (!TestCompositing()) return false; break;
			case 3: if (!TestPrivileges ()) return false; break;
		}
	}

	return true;
}
