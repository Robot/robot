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
static Mouse m;



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

static bool TestLive (void)
{
	m.AutoDelay = 1000;
	Point p; Point old;

	cout << "Press enter to begin live test";
	getchar();

	old = Mouse::GetPos();
	Mouse::SetPos ( 50, 200); Timer::Sleep (1000); p = Mouse::GetPos(); VERIFY (p.X ==  50 && p.Y == 200);
	Mouse::SetPos (200,  50); Timer::Sleep (1000); p = Mouse::GetPos(); VERIFY (p.X == 200 && p.Y ==  50);
	Mouse::SetPos (  0,   0); Timer::Sleep (1000); p = Mouse::GetPos(); VERIFY (p.X ==   0 && p.Y ==   0);
	Mouse::SetPos (old     ); Timer::Sleep (1000); p = Mouse::GetPos(); VERIFY (p == old);

	cout << "\nWarning: The next set of tests cannot be automated\n"
		 << "         Please review the following instructions!\n\n";

	cout << "- Live testing will be performed in sets\n"
		 << "- Press enter to begin testing a new set\n"
		 << "- After beginning, focus the testing app\n"
		 << "- Verify results before testing next set\n\n";

	cout << "Scroll";
	getchar();
	Timer::Sleep (2500);
	m.ScrollV (1); m.ScrollV (-1); m.ScrollV (-1); m.ScrollV (1);
	m.ScrollV (3); m.ScrollV (-2); m.ScrollV (-2); m.ScrollV (1);
	m.ScrollH (2); m.ScrollH (-1); m.ScrollH (-2); m.ScrollH (1);

	cout << "Click";
	getchar();
	Timer::Sleep (2500);
	m.Click (ButtonLeft );
	m.Click (ButtonMid  );
	m.Click (ButtonRight);
	m.Click (ButtonX1   );
	m.Click (ButtonX2   );
	// http://unixpapa.com/js/testmouse.html

	m.AutoDelay.Min = 40;
	m.AutoDelay.Max = 90;

	cout << "Fast Double Click";
	getchar();
	Timer::Sleep (2500);
	m.Click (ButtonLeft);
	m.Click (ButtonLeft);

	cout << "Slow Double Click";
	getchar();
	Timer::Sleep (2500);
	m.Click (ButtonLeft);
	Timer::Sleep (1500);
	m.Click (ButtonLeft);

	cout << "Click and Drag L";
	getchar();
	Timer::Sleep (2500);
	m.Press (ButtonLeft);
	Timer::Sleep (1000);

	old = Mouse::GetPos();
	Mouse::SetPos (old - 50);
	Timer::Sleep (1000);

	Mouse::SetPos (old);
	m.Release (ButtonLeft);

	cout << "Click and Drag R";
	getchar();
	Timer::Sleep (2500);
	m.Press (ButtonRight);
	Timer::Sleep (1000);

	old = Mouse::GetPos();
	Mouse::SetPos (old - 50);
	Timer::Sleep (1000);

	Mouse::SetPos (old);
	m.Release (ButtonRight);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestGetState (void)
{
	cout << "\nWarning: The next set of tests cannot be automated\n"
		 << "         Please review the following instructions!\n\n";

	cout << "- Press buttons and verify output\n"
		 << "- Press enter to begin testing\n"
		 << "- Press escape to stop testing\n";

	getchar();

	m.Press (ButtonLeft);
	m.Press (ButtonMid );
	VERIFY (Mouse::GetState (ButtonLeft) &&
			Mouse::GetState (ButtonMid ));

	m.Release (ButtonLeft);
	ButtonState state; VERIFY (Mouse::GetState (state));
	VERIFY (!state[ButtonLeft] && state[ButtonMid]);
	m.Release (ButtonMid);

	// Loop until the escape key is pressed
	while (!Keyboard::GetState (KeyEscape))
	{
		// Get button state of all buttons in one function call
		ButtonState state; VERIFY (Mouse::GetState (state));

		if (state[ButtonLeft ]) cout << "Left\t";
		if (state[ButtonMid  ]) cout << "Mid\t";
		if (state[ButtonRight]) cout << "Right\t";
		if (state[ButtonX1   ]) cout << "X1\t";
		if (state[ButtonX2   ]) cout << "X2\t";

		cout << endl; Timer::Sleep (90);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool TestMouse (void)
{
	cout << "BEGIN MOUSE TESTING\n------------------------------\n";
	if (!TestLive    ()) { cout << ">> Live Test Failed\n\n"; return false; }
	if (!TestGetState()) { cout << ">> Get State Failed\n\n"; return false; }
	cout << ">> Success\n\n"; return true;
}
