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
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool TestTimer (void)
{
	cout << "BEGIN TIMER TESTING\n------------------------------\n";

	Timer t1;   VERIFY (!t1.HasStarted());
	Timer t2;   VERIFY (!t2.HasStarted());
	t1.Reset(); VERIFY (!t1.HasStarted());

	VERIFY (t1.HasExpired (  0));
	VERIFY (t1.HasExpired (500));

	VERIFY (t1.GetElapsed() ==    0);
	VERIFY (t1.GetElapsed() == t1());

	VERIFY ( (t1 == t1)); VERIFY ( (t2 == t2));
	VERIFY (!(t1 != t1)); VERIFY (!(t2 != t2));
	VERIFY ( (t1 == t2)); VERIFY ( (t2 == t1));
	VERIFY (!(t1 != t2)); VERIFY (!(t2 != t1));

	VERIFY (!(t1 <  t2)); VERIFY (!(t2 >  t1));
	VERIFY (!(t1 >  t2)); VERIFY (!(t2 <  t1));
	VERIFY ( (t1 <= t2)); VERIFY ( (t2 >= t1));
	VERIFY ( (t1 >= t2)); VERIFY ( (t2 <= t1));

	VERIFY ((t1 <  t2) == (t1() <  t2())); VERIFY ((t2 >  t1) == (t2() >  t1()));
	VERIFY ((t1 >  t2) == (t1() >  t2())); VERIFY ((t2 <  t1) == (t2() <  t1()));
	VERIFY ((t1 <= t2) == (t1() <= t2())); VERIFY ((t2 >= t1) == (t2() >= t1()));
	VERIFY ((t1 >= t2) == (t1() >= t2())); VERIFY ((t2 <= t1) == (t2() <= t1()));

	VERIFY (t1() == t1.Reset  ()); VERIFY (t1() == t1.Restart());
	VERIFY (t2() == t2.Restart()); VERIFY (t2() == t2.Reset  ());

	//----------------------------------------------------------------------------//

	cout << "Warning: The next set of tests cannot be automated\n"
		 << "         Please verify the following results below\n\n";

	t1.Start();
	VERIFY ( t1.HasStarted());
	VERIFY (!t2.HasStarted());

	cout << "CPU Time: " << Timer::GetCpuTime() << "\n";
	cout << setfill ('0') << setw (3) << t1.GetElapsed() << " = "
		 << setfill ('0') << setw (3) << t1()            << " =   0\n\n";

	VERIFY (!t1.HasExpired (100));
	Timer::Sleep (200);
	VERIFY ( t1.HasExpired (100));

	cout << "CPU Time: " << Timer::GetCpuTime() << "\n";
	cout << setfill ('0') << setw (3) << t1.GetElapsed() << " = "
		 << setfill ('0') << setw (3) << t1()            << " = 200\n\n";

	VERIFY ( (t1 == t1)); VERIFY ( (t2 == t2));
	VERIFY (!(t1 != t1)); VERIFY (!(t2 != t2));
	VERIFY (!(t1 == t2)); VERIFY (!(t2 == t1));
	VERIFY ( (t1 != t2)); VERIFY ( (t2 != t1));

	VERIFY (!(t1 <  t2)); VERIFY (!(t2 >  t1));
	VERIFY ( (t1 >  t2)); VERIFY ( (t2 <  t1));
	VERIFY (!(t1 <= t2)); VERIFY (!(t2 >= t1));
	VERIFY ( (t1 >= t2)); VERIFY ( (t2 <= t1));

	VERIFY ((t1 <  t2) == (t1() <  t2())); VERIFY ((t2 >  t1) == (t2() >  t1()));
	VERIFY ((t1 >  t2) == (t1() >  t2())); VERIFY ((t2 <  t1) == (t2() <  t1()));
	VERIFY ((t1 <= t2) == (t1() <= t2())); VERIFY ((t2 >= t1) == (t2() >= t1()));
	VERIFY ((t1 >= t2) == (t1() >= t2())); VERIFY ((t2 <= t1) == (t2() <= t1()));

	//----------------------------------------------------------------------------//

	t2.Start();
	VERIFY (t1.HasStarted());
	VERIFY (t2.HasStarted());

	VERIFY ( t1.HasExpired (100));
	VERIFY (!t2.HasExpired (100));
	Timer::Sleep (200);
	VERIFY ( t1.HasExpired (100));
	VERIFY ( t2.HasExpired (100));

	VERIFY ( (t1 == t1)); VERIFY ( (t2 == t2));
	VERIFY (!(t1 != t1)); VERIFY (!(t2 != t2));
	VERIFY (!(t1 == t2)); VERIFY (!(t2 == t1));
	VERIFY ( (t1 != t2)); VERIFY ( (t2 != t1));

	VERIFY (!(t1 <  t2)); VERIFY (!(t2 >  t1));
	VERIFY ( (t1 >  t2)); VERIFY ( (t2 <  t1));
	VERIFY (!(t1 <= t2)); VERIFY (!(t2 >= t1));
	VERIFY ( (t1 >= t2)); VERIFY ( (t2 <= t1));

	VERIFY ((t1 <  t2) == (t1() <  t2())); VERIFY ((t2 >  t1) == (t2() >  t1()));
	VERIFY ((t1 >  t2) == (t1() >  t2())); VERIFY ((t2 <  t1) == (t2() <  t1()));
	VERIFY ((t1 <= t2) == (t1() <= t2())); VERIFY ((t2 >= t1) == (t2() >= t1()));
	VERIFY ((t1 >= t2) == (t1() >= t2())); VERIFY ((t2 <= t1) == (t2() <= t1()));

	cout << setfill ('0') << setw (3) << t1.Restart() << " = 400 : "
		 << setfill ('0') << setw (3) << t2.Restart() << " = 200\n";

	VERIFY (t1.HasStarted());
	VERIFY (t2.HasStarted());

	cout << setfill ('0') << setw (3) << t1() << " =   0 : "
		 << setfill ('0') << setw (3) << t2() << " =   0\n\n";

	VERIFY (!t1.HasExpired (100));
	VERIFY (!t2.HasExpired (100));
	Timer::Sleep (200, 400);
	VERIFY ( t1.HasExpired (100));
	VERIFY ( t2.HasExpired (100));

	VERIFY (200 <= t1() && t1() <= 400);
	VERIFY (200 <= t2() && t2() <= 400);
	cout << setfill ('0') << setw (3) << t1() << " = (200 - 400) : "
		 << setfill ('0') << setw (3) << t2() << " = (200 - 400)\n";

	//----------------------------------------------------------------------------//

	t1.Start();
	VERIFY (t1.HasStarted());
	VERIFY (t2.HasStarted());

	VERIFY (!t1.HasExpired (100));
	VERIFY ( t2.HasExpired (100));
	Timer::Sleep (200, 400);
	VERIFY ( t1.HasExpired (100));
	VERIFY ( t2.HasExpired (100));

	VERIFY (200 <= t1() && t1() <= 400);
	VERIFY (400 <= t2() && t2() <= 800);
	cout << setfill ('0') << setw (3) << t1() << " = (200 - 400) : "
		 << setfill ('0') << setw (3) << t2() << " = (400 - 800)\n\n";

	VERIFY ( (t1 == t1)); VERIFY ( (t2 == t2));
	VERIFY (!(t1 != t1)); VERIFY (!(t2 != t2));
	VERIFY (!(t1 == t2)); VERIFY (!(t2 == t1));
	VERIFY ( (t1 != t2)); VERIFY ( (t2 != t1));

	VERIFY ( (t1 <  t2)); VERIFY ( (t2 >  t1));
	VERIFY (!(t1 >  t2)); VERIFY (!(t2 <  t1));
	VERIFY ( (t1 <= t2)); VERIFY ( (t2 >= t1));
	VERIFY (!(t1 >= t2)); VERIFY (!(t2 <= t1));

	VERIFY ((t1 <  t2) == (t1() <  t2())); VERIFY ((t2 >  t1) == (t2() >  t1()));
	VERIFY ((t1 >  t2) == (t1() >  t2())); VERIFY ((t2 <  t1) == (t2() <  t1()));
	VERIFY ((t1 <= t2) == (t1() <= t2())); VERIFY ((t2 >= t1) == (t2() >= t1()));
	VERIFY ((t1 >= t2) == (t1() >= t2())); VERIFY ((t2 <= t1) == (t2() <= t1()));

	t1.Reset(); VERIFY (!t1.HasStarted());
	t2.Reset(); VERIFY (!t2.HasStarted());
	cout << ">> Success\n\n"; return true;
}
