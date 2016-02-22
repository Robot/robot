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
#include "Targa.h"



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

static bool TestInvalid (void)
{
	Bounds b1;
	Bounds b2 ( 1,  2, 3, 4);
	Bounds b3 (-1, -2, 4, 3);
	Bounds b4 ( 0 , 0, 2, 2);

	VERIFY (Screen::GetTotalBounds() == b1);
	VERIFY (Screen::GetTotalUsable() == b1);
	VERIFY (Screen::GetMain()            == nullptr);
	VERIFY (Screen::GetList().size()     == 0      );
	VERIFY (Screen::GetScreen (Window()) == nullptr);
	VERIFY (Screen::GetScreen (Point ()) == nullptr);
	VERIFY (Screen::GetScreen (  0,   0) == nullptr);

	Screen s1, s2;
	VERIFY ( s1.GetBounds  () == b1);
	VERIFY ( s1.GetUsable  () == b1);
	VERIFY (!s1.IsPortrait ());
	VERIFY ( s1.IsLandscape());

	s1 = Screen (b2, b1);
	s2 = Screen (b1, b2);
	VERIFY ( s1.GetBounds  () == b2);
	VERIFY ( s1.GetUsable  () == b1);
	VERIFY ( s1.IsPortrait ());
	VERIFY (!s1.IsLandscape());
	VERIFY ( s2.GetBounds  () == b1);
	VERIFY ( s2.GetUsable  () == b2);
	VERIFY (!s2.IsPortrait ());
	VERIFY ( s2.IsLandscape());

	s1 = Screen (b3, b1);
	s2 = Screen (b1, b3);
	VERIFY ( s1.GetBounds  () == b3);
	VERIFY ( s1.GetUsable  () == b1);
	VERIFY (!s1.IsPortrait ());
	VERIFY ( s1.IsLandscape());
	VERIFY ( s2.GetBounds  () == b1);
	VERIFY ( s2.GetUsable  () == b3);
	VERIFY (!s2.IsPortrait ());
	VERIFY ( s2.IsLandscape());

	s1 = Screen (b4, b1);
	s2 = Screen (b1, b4);
	VERIFY ( s1.GetBounds  () == b4);
	VERIFY ( s1.GetUsable  () == b1);
	VERIFY (!s1.IsPortrait ());
	VERIFY ( s1.IsLandscape());
	VERIFY ( s2.GetBounds  () == b1);
	VERIFY ( s2.GetUsable  () == b4);
	VERIFY (!s2.IsPortrait ());
	VERIFY ( s2.IsLandscape());

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestAero (void)
{
#if defined (ROBOT_OS_LINUX) || \
	defined (ROBOT_OS_MAC  )

	VERIFY (Screen::IsCompositing());

	Screen::SetCompositing (false);
	VERIFY (Screen::IsCompositing());

	Screen::SetCompositing (true );
	VERIFY (Screen::IsCompositing());

#endif
#ifdef ROBOT_OS_WIN

	cout << "Composition status: "
		 << (Screen::IsCompositing() ?
			 "enabled" : "disabled");
	getchar();

	Screen::SetCompositing (false);

	cout << "Composition status: "
		 << (Screen::IsCompositing() ?
			 "enabled" : "disabled");
	getchar();

	Screen::SetCompositing (true );

	cout << "Composition status: "
		 << (Screen::IsCompositing() ?
			 "enabled" : "disabled");
	getchar();

	cout << endl;

#endif

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestSync (void)
{
	VERIFY (Screen::Synchronize());

	auto main = Screen::GetMain();
	auto list = Screen::GetList();
	VERIFY (!list.empty());
	VERIFY (main != nullptr);

	VERIFY (Screen::GetTotalBounds().IsValid());
	VERIFY (Screen::GetTotalUsable().IsValid());
	cout << "Total bounds: " << Screen::GetTotalBounds() << endl
		 << "Total usable: " << Screen::GetTotalUsable() << "\n\n";

	// Iterate through every available screen
	for (uintptr i = 0; i < list.size(); ++i)
	{
		VERIFY (list[i]->GetBounds().IsValid());
		VERIFY (list[i]->GetUsable().IsValid());
		VERIFY (list[i]->IsPortrait () ||
				list[i]->IsLandscape());

		if (i == 0) { VERIFY (list[i] == main); }
		       else { VERIFY (list[i] != main); }

		const char* sPort =  list[i]->IsPortrait () ? "true" : "false";
		const char* sLand =  list[i]->IsLandscape() ? "true" : "false";
		const char* sMain = (list[i] == main      ) ? "true" : "false";

		cout << "Screen "  << i << endl
			 << "Bounds: " << list[i]->GetBounds() << endl
			 << "Usable: " << list[i]->GetUsable() << endl
			 << "  Port: " << sPort << endl
			 << "  Land: " << sLand << endl
			 << "  Main: " << sMain << endl << endl;
	}

	cout << "Please verify these results\n"; getchar();

	//----------------------------------------------------------------------------//

	cout << "Press enter  to begin mouse tracking\n"
		 << "Press escape to continue other tests";
	getchar();

	int32 last = -1; bool usePoint = false;
	// Loop until the escape key is pressed
	while (!Keyboard::GetState (KeyEscape))
	{
		auto p = Mouse::GetPos();
		Screen* s = (usePoint = !usePoint) ?
					 Screen::GetScreen (p) :
					 Screen::GetScreen (p.X, p.Y);

		VERIFY (s != nullptr);

		int32 curr = -1;
		// Iterate through every available screen
		for (uintptr i = 0; i < list.size(); ++i)
			if (list[i] == s) { curr = (int32) i; break; }

		VERIFY (curr >= 0);
		if (last != curr)
			cout << (last = curr) << endl;

		Timer::Sleep (90);
	}

	cout << endl;

	//----------------------------------------------------------------------------//

#ifdef ROBOT_OS_LINUX

	cout << "Open Leafpad";

#endif
#ifdef ROBOT_OS_MAC

	cout << "Open TextEdit";

#endif
#ifdef ROBOT_OS_WIN

	cout << "Open Notepad";

#endif

	getchar(); Window w, wx =
		Window::GetActive();

	cout << "Focus the application to begin mouse tracking\n";
	while (true)
	{
		w = Window::GetActive();
		if (w != wx) break;
		Timer::Sleep (250);
	}

	last = -1;
	while (Window::GetActive() == w)
	{
		auto s = Screen::GetScreen (w);
		VERIFY (s != nullptr);

		int32 curr = -1;
		// Iterate through every available screen
		for (uintptr i = 0; i < list.size(); ++i)
			if (list[i] == s) { curr = (int32) i; break; }

		VERIFY (curr >= 0);
		if (last != curr)
			cout << (last = curr) << endl;

		Timer::Sleep (90);
	}

	cout << endl;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestGrab (void)
{
	Image image1;
	Image image2;
	Window window;
	VERIFY (Screen::Synchronize());

	auto test = [&] (const char* name, int32 x, int32 y, int32 w, int32 h) -> bool
	{
		Bounds bounds (x, y, w, h);
		VERIFY (Screen::GrabScreen (image1, x,y,w,h, window)); VERIFY (image1.IsValid());
		VERIFY (Screen::GrabScreen (image2,  bounds, window)); VERIFY (image2.IsValid());

		tga_write_bgr (name, (uint8*) image1.GetData(),
			image1.GetWidth(), image1.GetHeight(), 32);

		return true;
	};

	//----------------------------------------------------------------------------//

	test ("0-01.tga",    0,    0,    0,    0);
	test ("0-02.tga",    0,    0,   50,  150);
	test ("0-03.tga",  -50, -150,   -1,   -1);
	test ("0-04.tga",  -50, -150,  150,  250);
	test ("0-05.tga",   50,  150,   -1,   -1);
	test ("0-06.tga",   50,  150,   50,  150);
	test ("0-07.tga", 4000, 2000,   -1,   -1);
	test ("0-08.tga", 4000, 2000,   50,  150);
	test ("0-09.tga",    0,    0, 4000, 2000);
	test ("0-10.tga",  -50, -150, 4000, 2000);
	test ("0-11.tga",   50,  150, 4000, 2000);
	test ("0-12.tga", 4000, 2000, 4000, 2000);

	cout << "Please verify saved images\n"; getchar();

	//----------------------------------------------------------------------------//

#ifdef ROBOT_OS_LINUX

	cout << "Open Leafpad";

#endif
#ifdef ROBOT_OS_MAC

	cout << "Open TextEdit";

#endif
#ifdef ROBOT_OS_WIN

	cout << "Open Notepad";

#endif

	getchar(); Window w, wx =
		Window::GetActive();

	cout << "Focus the application to begin screen tests\n";
	while (true)
	{
		w = Window::GetActive();
		if (w != wx) break;
		Timer::Sleep (250);
	}

	window = w;
	test ("1-01.tga",    0,    0,   -1,   -1);
	test ("1-02.tga",    0,    0,   50,  150);
	test ("1-03.tga",  -50, -150,   -1,   -1);
	test ("1-04.tga",  -50, -150,  150,  250);
	test ("1-05.tga",   50,  150,   -1,   -1);
	test ("1-06.tga",   50,  150,   50,  150);
	test ("1-07.tga", 4000, 2000,   -1,   -1);
	test ("1-08.tga", 4000, 2000,   50,  150);
	test ("1-09.tga",    0,    0, 4000, 2000);
	test ("1-10.tga",  -50, -150, 4000, 2000);
	test ("1-11.tga",   50,  150, 4000, 2000);
	test ("1-12.tga", 4000, 2000, 4000, 2000);

	cout << "Please verify saved images\n"; getchar();
	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestSpeed (void)
{
	Timer timer;
	Image image (1920, 1080);
	VERIFY (Screen::Synchronize());

	//----------------------------------------------------------------------------//

	cout << "Small  image (ms): ";
	for (uint32 i = 0; i < 5; ++i)
	{
		timer.Start();
		VERIFY (Screen::GrabScreen
			(image, 0, 0, 16, 16));

		cout << timer() << " ";
	}

	cout << endl;

	//----------------------------------------------------------------------------//

	cout << "Medium image (ms): ";
	for (uint32 i = 0; i < 5; ++i)
	{
		timer.Start();
		VERIFY (Screen::GrabScreen
			(image, 0, 0, 512, 512));

		cout << timer() << " ";
	}

	cout << endl;

	//----------------------------------------------------------------------------//

	cout << "Large  image (ms): ";
	for (uint32 i = 0; i < 5; ++i)
	{
		timer.Start();
		VERIFY (Screen::GrabScreen
			(image, 0, 0, 1920, 1080));

		cout << timer() << " ";
	}

	cout << endl;
	return true;
}



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool TestScreen (void)
{
	cout << "BEGIN SCREEN TESTING\n------------------------------\n";

	cout << "Warning: Some set of tests cannot be automated\n"
		 << "         Please execute the following commands\n\n";

	if (!TestInvalid()) { cout << ">> Invalid Failed\n\n"; return false; }
	if (!TestAero   ()) { cout << ">> Aero Failed   \n\n"; return false; }
	if (!TestSync   ()) { cout << ">> Sync Failed   \n\n"; return false; }
	if (!TestGrab   ()) { cout << ">> Grab Failed   \n\n"; return false; }
	if (!TestSpeed  ()) { cout << ">> Speed Failed  \n\n"; return false; }
	cout << ">> Success\n\n"; return true;
}
