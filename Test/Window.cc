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

#define TEST_WINDOW										\
	{													\
		VERIFY (list1[i].IsValid());					\
		VERIFY (list2[i].IsValid());					\
														\
		VERIFY ( (list1[i] == list2[i]));				\
		VERIFY (!(list1[i] != list2[i]));				\
														\
		cout << setw (6) << list1[i].GetPID  ()			\
			 << ": "     << list1[i].GetTitle()			\
			 << "\n";									\
	}

////////////////////////////////////////////////////////////////////////////////

#define MINMAX_TEST( action )							\
	Timer::Sleep (500);									\
	cout << " - " action " = "							\
		 << (w.IsMinimized() ? " Min " : "!Min ")		\
		 << (w.IsMaximized() ? " Max " : "!Max ");		\
	getchar();



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

static bool TestInvalid (void)
{
	Window w1;
	Window w2; VERIFY ( w2.SetHandle (   0));
	Window w3; VERIFY (!w3.SetHandle (8888));
	Window w4 (8888);

	VERIFY (!w1.IsValid()); VERIFY ( w1.SetHandle (   0));
	VERIFY (!w2.IsValid()); VERIFY ( w2.SetHandle (   0));
	VERIFY (!w3.IsValid()); VERIFY (!w3.SetHandle (8888));
	VERIFY (!w4.IsValid()); VERIFY (!w4.SetHandle (8888));

	VERIFY (!w1.IsTopMost()); VERIFY (!w1.IsBorderless());
	VERIFY (!w2.IsTopMost()); VERIFY (!w2.IsBorderless());
	VERIFY (!w3.IsTopMost()); VERIFY (!w3.IsBorderless());
	VERIFY (!w4.IsTopMost()); VERIFY (!w4.IsBorderless());

	VERIFY (!w1.IsMinimized()); VERIFY (!w1.IsMaximized());
	VERIFY (!w2.IsMinimized()); VERIFY (!w2.IsMaximized());
	VERIFY (!w3.IsMinimized()); VERIFY (!w3.IsMaximized());
	VERIFY (!w4.IsMinimized()); VERIFY (!w4.IsMaximized());

	VERIFY (!w1.GetProcess().IsValid()); VERIFY (w1.GetPID() == 0);
	VERIFY (!w2.GetProcess().IsValid()); VERIFY (w2.GetPID() == 0);
	VERIFY (!w3.GetProcess().IsValid()); VERIFY (w3.GetPID() == 0);
	VERIFY (!w4.GetProcess().IsValid()); VERIFY (w4.GetPID() == 0);

	VERIFY (w1.GetHandle() == 0); VERIFY (w1.GetHandleAx() == 0);
	VERIFY (w2.GetHandle() == 0); VERIFY (w2.GetHandleAx() == 0);
	VERIFY (w3.GetHandle() == 0); VERIFY (w3.GetHandleAx() == 0);
	VERIFY (w4.GetHandle() == 0); VERIFY (w4.GetHandleAx() == 0);

	VERIFY (w1.GetTitle().empty());
	VERIFY (w2.GetTitle().empty());
	VERIFY (w3.GetTitle().empty());
	VERIFY (w4.GetTitle().empty());

	VERIFY (w1.GetBounds() == 0); VERIFY (w1.GetClient() == 0);
	VERIFY (w2.GetBounds() == 0); VERIFY (w2.GetClient() == 0);
	VERIFY (w3.GetBounds() == 0); VERIFY (w3.GetClient() == 0);
	VERIFY (w4.GetBounds() == 0); VERIFY (w4.GetClient() == 0);

	VERIFY (w1.MapToClient ((Point)  20) == 0); VERIFY (w1.MapToScreen ((Point)  20) == 0);
	VERIFY (w2.MapToClient ((Point)  20) == 0); VERIFY (w2.MapToScreen ((Point)  20) == 0);
	VERIFY (w3.MapToClient ((Point)  20) == 0); VERIFY (w3.MapToScreen ((Point)  20) == 0);
	VERIFY (w4.MapToClient ((Point)  20) == 0); VERIFY (w4.MapToScreen ((Point)  20) == 0);

	VERIFY (w1.MapToClient ((Bounds) 20) == 0); VERIFY (w1.MapToScreen ((Bounds) 20) == 0);
	VERIFY (w2.MapToClient ((Bounds) 20) == 0); VERIFY (w2.MapToScreen ((Bounds) 20) == 0);
	VERIFY (w3.MapToClient ((Bounds) 20) == 0); VERIFY (w3.MapToScreen ((Bounds) 20) == 0);
	VERIFY (w4.MapToClient ((Bounds) 20) == 0); VERIFY (w4.MapToScreen ((Bounds) 20) == 0);

	VERIFY ( (w1 == w2)); VERIFY ( (w2 == w1));
	VERIFY (!(w1 != w2)); VERIFY (!(w2 != w1));
	VERIFY ( (w3 == w4)); VERIFY ( (w4 == w3));
	VERIFY (!(w3 != w4)); VERIFY (!(w4 != w3));

	VERIFY (w1 == 0); VERIFY (w1 != 8888);
	VERIFY (w2 == 0); VERIFY (w2 != 8888);
	VERIFY (w3 == 0); VERIFY (w3 != 8888);
	VERIFY (w4 == 0); VERIFY (w4 != 8888);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestSelect1 (void)
{
	cout << "Warning: The next set of tests cannot be automated\n"
		 << "         Please execute the following instructions\n\n";

	//----------------------------------------------------------------------------//

#ifdef ROBOT_OS_LINUX

	cout << "Open Leafpad and gedit";

#endif
#ifdef ROBOT_OS_MAC

	cout << "Open TextEdit and Notes";

#endif
#ifdef ROBOT_OS_WIN

	cout << "Open Notepad and Wordpad";

#endif

	getchar(); Window w1, w2,
		wx = Window::GetActive();

	cout << "Focus 1st app...\n";
	while (true)
	{
		w1 = Window::GetActive();
		if (w1 != wx) break;
		Timer::Sleep (250);
	}

	cout << "Focus 2nd app...\n\n";
	while (true)
	{
		w2 = Window::GetActive();
		if (w2 != w1 &&
			w2 != wx) break;
		Timer::Sleep (250);
	}

	//----------------------------------------------------------------------------//

	VERIFY (w1.IsValid());
	VERIFY (w2.IsValid());

	Window w3 (w1.GetHandle());
	Window w4;
	VERIFY (w4.SetHandle
		(w2.GetHandle()));

	VERIFY (w3.IsValid());
	VERIFY (w4.IsValid());

#ifdef ROBOT_OS_LINUX

	VERIFY (w1.GetProcess().GetName() == "leafpad");
	VERIFY (w2.GetProcess().GetName() == "gedit"  );

#endif
#ifdef ROBOT_OS_MAC

	VERIFY (w1.GetProcess().GetName() == "TextEdit");
	VERIFY (w2.GetProcess().GetName() == "Notes"   );

	VERIFY (w1.GetHandle() != 0); VERIFY (w1.GetHandleAx() != 0);
	VERIFY (w2.GetHandle() != 0); VERIFY (w2.GetHandleAx() != 0);
	VERIFY (w3.GetHandle() != 0); VERIFY (w3.GetHandleAx() != 0);
	VERIFY (w4.GetHandle() != 0); VERIFY (w4.GetHandleAx() != 0);

	VERIFY (w3.GetHandle  () == w1.GetHandle  ()); VERIFY (w3 == w1);
	VERIFY (w3.GetHandle  () != w2.GetHandle  ()); VERIFY (w3 != w2);
	VERIFY (w3.GetHandleAx() != w1.GetHandleAx()); VERIFY (w3 == w1.GetHandle());
	VERIFY (w3.GetHandleAx() != w2.GetHandleAx()); VERIFY (w3 != w2.GetHandle());

	VERIFY (w4.GetHandle  () != w1.GetHandle  ()); VERIFY (w4 != w1);
	VERIFY (w4.GetHandle  () == w2.GetHandle  ()); VERIFY (w4 == w2);
	VERIFY (w4.GetHandleAx() != w1.GetHandleAx()); VERIFY (w4 != w1.GetHandle());
	VERIFY (w4.GetHandleAx() != w2.GetHandleAx()); VERIFY (w4 == w2.GetHandle());

#endif
#ifdef ROBOT_OS_WIN

	VERIFY (w1.GetProcess().GetName() == "notepad.exe");
	VERIFY (w2.GetProcess().GetName() == "wordpad.exe");

#endif

#if defined (ROBOT_OS_LINUX) || \
	defined (ROBOT_OS_WIN  )

	VERIFY (w1.GetHandle() != 0); VERIFY (w1.GetHandleAx() == 0);
	VERIFY (w2.GetHandle() != 0); VERIFY (w2.GetHandleAx() == 0);
	VERIFY (w3.GetHandle() != 0); VERIFY (w3.GetHandleAx() == 0);
	VERIFY (w4.GetHandle() != 0); VERIFY (w4.GetHandleAx() == 0);

	VERIFY (w3.GetHandle  () == w1.GetHandle  ()); VERIFY (w3 == w1);
	VERIFY (w3.GetHandle  () != w2.GetHandle  ()); VERIFY (w3 != w2);
	VERIFY (w3.GetHandleAx() == w1.GetHandleAx()); VERIFY (w3 == w1.GetHandle());
	VERIFY (w3.GetHandleAx() == w2.GetHandleAx()); VERIFY (w3 != w2.GetHandle());

	VERIFY (w4.GetHandle  () != w1.GetHandle  ()); VERIFY (w4 != w1);
	VERIFY (w4.GetHandle  () == w2.GetHandle  ()); VERIFY (w4 == w2);
	VERIFY (w4.GetHandleAx() == w1.GetHandleAx()); VERIFY (w4 != w1.GetHandle());
	VERIFY (w4.GetHandleAx() == w2.GetHandleAx()); VERIFY (w4 == w2.GetHandle());

	VERIFY (!w1.IsTopMost()); VERIFY (!w1.IsBorderless());
	VERIFY (!w2.IsTopMost()); VERIFY (!w2.IsBorderless());

	w1.SetTopMost    (true);
	w2.SetBorderless (true);
	Timer::Sleep (500);

	VERIFY ( w1.IsTopMost()); VERIFY (!w1.IsBorderless());
	VERIFY (!w2.IsTopMost()); VERIFY ( w2.IsBorderless());

	cout << "1st app is topmost, 2nd app is borderless";
	getchar();

	w1.SetTopMost    (false);
	w2.SetTopMost    (true );
	w1.SetBorderless (true );
	w2.SetBorderless (false);
	Timer::Sleep (500);

	VERIFY (!w1.IsTopMost()); VERIFY ( w1.IsBorderless());
	VERIFY ( w2.IsTopMost()); VERIFY (!w2.IsBorderless());

	cout << "2nd app is topmost, 1st app is borderless";
	getchar();

	w1.SetBorderless (false);
	w2.SetTopMost    (false);
	Timer::Sleep (500);

	VERIFY (!w1.IsTopMost()); VERIFY (!w1.IsBorderless());
	VERIFY (!w2.IsTopMost()); VERIFY (!w2.IsBorderless());

	cout << "Both applications are now back to normal\n";
	getchar();

#endif

	//----------------------------------------------------------------------------//

	cout << "Press enter to activate 1st app"; getchar();
	Window::SetActive (w1); Timer::Sleep (500); VERIFY (Window::GetActive() == w1);

	cout << "Press enter to activate 2nd app\n"; getchar();
	Window::SetActive (w2); Timer::Sleep (500); VERIFY (Window::GetActive() == w2);

	//----------------------------------------------------------------------------//

	string t1 = w1.GetTitle();
	string t2 = w2.GetTitle();

	Bounds b1 = w1.GetBounds();
	Bounds b2 = w2.GetBounds();

	w1.SetBounds (100, 100, 250, 300); w1.SetTitle ("Hello");
	w2.SetBounds (350, 400, 550, 400); w2.SetTitle ("World");
	Timer::Sleep (500);
	VERIFY (w1.GetBounds() == Bounds (100, 100, 250, 300));
	VERIFY (w2.GetBounds() == Bounds (350, 400, 550, 400));

#ifdef ROBOT_OS_MAC

	cout << "w1 = " << w1.GetTitle() << " | "
		 << "w2 = " << w2.GetTitle() << endl;

#else

	VERIFY (w1.GetTitle() == "Hello");
	VERIFY (w2.GetTitle() == "World");

#endif

	cout << "Verify window title & arrangement";
	getchar();

	w1.SetBounds (100, 400, 250, 300); w1.SetTitle (nullptr);
	w2.SetBounds (350, 100, 550, 400); w2.SetTitle (nullptr);
	Timer::Sleep (500);
	VERIFY (w1.GetBounds() == Bounds (100, 400, 250, 300));
	VERIFY (w2.GetBounds() == Bounds (350, 100, 550, 400));

#ifndef ROBOT_OS_MAC

	VERIFY (w1.GetTitle() == "Hello");
	VERIFY (w2.GetTitle() == "World");

#endif

	cout << "Verify window title & arrangement";
	getchar();

#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_WIN)

	w1.SetBounds (-50, 400, 250, 300);
	w2.SetBounds (200, 100, 550, 600);
	Timer::Sleep (500);
	VERIFY (w1.GetBounds() == Bounds (-50, 400, 250, 300));
	VERIFY (w2.GetBounds() == Bounds (200, 100, 550, 600));
	cout << "Verify window title & arrangement";
	getchar();

#endif

#if defined (ROBOT_OS_LINUX) || \
	defined (ROBOT_OS_WIN  )

	w1.SetClient (100, 100, 250, 300); w1.SetTitle ("");
	w2.SetBounds (350, 400, 550, 300); w2.SetTitle ("");
	Timer::Sleep (500);
	VERIFY (w1.GetClient() == Bounds (100, 100, 250, 300));
	VERIFY (w2.GetBounds() == Bounds (350, 400, 550, 300));
	VERIFY (w1.GetTitle().empty());
	VERIFY (w2.GetTitle().empty());
	cout << "Verify window title & arrangement";
	getchar();

#endif

	cout << endl;
	w1.SetBounds (b1); w1.SetTitle (t1.data());
	w2.SetBounds (b2); w2.SetTitle (t2.data());
	Timer::Sleep (500);
	VERIFY (w1.GetBounds() == b1);
	VERIFY (w2.GetBounds() == b2);

#ifndef ROBOT_OS_MAC

	VERIFY (w1.GetTitle() == t1);
	VERIFY (w2.GetTitle() == t2);

#endif

	//----------------------------------------------------------------------------//

	w1.Close();
	w2.Close();
	Timer::Sleep (1000);
	VERIFY (!w1.IsValid());
	VERIFY (!w2.IsValid());

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestSelect2 (void)
{
	cout << "Warning: The next set of tests cannot be automated\n"
		 << "         Please execute the following instructions\n\n";

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

	cout << "Focus the application to begin mapping test...\n";
	while (true)
	{
		w = Window::GetActive();
		if (w != wx) break;
		Timer::Sleep (250);
	}

#ifdef ROBOT_OS_LINUX

	VERIFY (w.GetProcess().GetName() == "leafpad");

#endif
#ifdef ROBOT_OS_MAC

	VERIFY (w.GetProcess().GetName() == "TextEdit");

#endif
#ifdef ROBOT_OS_WIN

	VERIFY (w.GetProcess().GetName() == "notepad.exe");

#endif

	//----------------------------------------------------------------------------//

	Bounds b = w.GetBounds();
	w.SetBounds (Bounds (250, 250));
	while (Window::GetActive() == w)
	{
		Point mp = Mouse::GetPos();

		Point p1 = w.MapToClient (mp);
		Point p2 = w.MapToScreen (p1);
		VERIFY (p2 == mp);

		Bounds b1 = w.MapToClient (Bounds (mp, 20));
		Bounds b2 = w.MapToScreen (Bounds (b1    ));
		VERIFY (b1.GetPoint() == p1);
		VERIFY (b2.GetPoint() == p2);
		VERIFY (b2 == Bounds (mp, 20));

		cout << p1.X << " " << p1.Y << endl;
		Timer::Sleep (30);
	}

	cout << endl;
	w.SetBounds (b);

	//----------------------------------------------------------------------------//

	cout << "Press enter to begin min max test";
	VERIFY (!w.IsMinimized());
	VERIFY (!w.IsMaximized());

	cout << "\nKeep window !min !max"; getchar();
	w.SetMinimized (false); MINMAX_TEST ("!Min");
	w.SetMinimized (true ); MINMAX_TEST (" Min");
	w.SetMaximized (false); MINMAX_TEST ("!Max");
	w.SetMaximized (true ); MINMAX_TEST (" Max");

	cout << "\nKeep window  min !max"; getchar();
	w.SetMinimized (false); MINMAX_TEST ("!Min");
	w.SetMinimized (true ); MINMAX_TEST (" Min");
	w.SetMaximized (false); MINMAX_TEST ("!Max");
	w.SetMaximized (true ); MINMAX_TEST (" Max");

	cout << "\nKeep window !min  max"; getchar();
	w.SetMinimized (false); MINMAX_TEST ("!Min");
	w.SetMinimized (true ); MINMAX_TEST (" Min");
	w.SetMaximized (false); MINMAX_TEST ("!Max");
	w.SetMaximized (true ); MINMAX_TEST (" Max");

	cout << "\nKeep window  min  max"; getchar();
	w.SetMinimized (false); MINMAX_TEST ("!Min");
	w.SetMinimized (true ); MINMAX_TEST (" Min");
	w.SetMaximized (false); MINMAX_TEST ("!Max");
	w.SetMaximized (true ); MINMAX_TEST (" Max");

	cout << "\nKeep window  min !max"; getchar();
	Window::SetActive (w); MINMAX_TEST ("Active");

	cout << "\nPress enter to continue\n"; getchar();
	VERIFY (!w.IsMinimized());
	VERIFY (!w.IsMaximized());

	//----------------------------------------------------------------------------//

	w.Close(); Timer::Sleep (750); VERIFY (!w.IsValid());

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestGetList1 (void)
{
	cout << "Warning: The next set of tests cannot be automated\n"
		 << "         Please verify the following window lists\n\n";

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
	VERIFY (Window::GetList ("*").empty());
	VERIFY (Window::GetList (")").empty());

#endif

	WindowList list1 = Window::GetList (    );
	WindowList list2 = Window::GetList (".*");
	cout << "List all - " << list1.size() << endl;

	VERIFY (list1.size() !=            0);
	VERIFY (list1.size() == list2.size());

	for (uintptr i = 0; i < list1.size(); ++i)
		TEST_WINDOW; cout << endl;

	list1 = Window::GetList ("");
	list2 = Window::GetList ("");
	cout << "List \"\" - " << list1.size() << endl;
	VERIFY (list1.size() == list2.size());

	for (uintptr i = 0; i < list1.size(); ++i)
		TEST_WINDOW; cout << endl;

	list1 = Window::GetList (".*a.*");
	list2 = Window::GetList (".*A.*");
	cout << "List *a* - " << list1.size() << endl;
	VERIFY (list1.size() == list2.size());

	for (uintptr i = 0; i < list1.size(); ++i)
	{
		auto title = list1[i].GetTitle();
		VERIFY (title.find ('a') != string::npos ||
				title.find ('A') != string::npos);
	}

	cout << "Verified\n\n";

#ifdef ROBOT_OS_LINUX

	list1 = Window::GetList (".*(untitled).*|.*gedit.*");
	list2 = Window::GetList (".*gEdit.*|.*(Untitled).*");

#endif
#ifdef ROBOT_OS_MAC

	list1 = Window::GetList (".*untitled.*|.*notes.*");
	list2 = Window::GetList (".*Notes.*|.*Untitled.*");

#endif
#ifdef ROBOT_OS_WIN

	list1 = Window::GetList (".*notepad.*|.*wordpad.*");
	list2 = Window::GetList (".*WordPad.*|.*NotePad.*");

#endif

	cout << "List apps - " << list1.size() << endl;

	VERIFY (list1.size() !=            0);
	VERIFY (list1.size() == list2.size());

	for (uintptr i = 0; i < list1.size(); ++i)
		TEST_WINDOW; cout << endl;

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestGetList2 (void)
{
	cout << "Warning: The next set of tests cannot be automated\n"
		 << "         Please verify the following window lists\n\n";

	cout << "Open a multi-window testing application";
	getchar(); Window w, wx =
		Window::GetActive();

	cout << "Focus the application to begin find test...\n";
	while (true)
	{
		w = Window::GetActive();
		if (w != wx) break;
		Timer::Sleep (250);
	}

	WindowList list1 = w.GetProcess().GetWindows (    );
	WindowList list2 = w.GetProcess().GetWindows (".*");
	cout << "List all - " << list1.size() << endl;

	VERIFY (list1.size() !=            0);
	VERIFY (list1.size() == list2.size());

	for (uintptr i = 0; i < list1.size(); ++i)
		TEST_WINDOW; cout << endl;

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool TestWindow (void)
{
	cout << "BEGIN WINDOW TESTING\n------------------------------\n";
	VERIFY (Window::IsAxEnabled());
	if (!TestInvalid ()) { cout << ">> Invalid Failed \n\n"; return false; }
	if (!TestSelect1 ()) { cout << ">> Select1 Failed \n\n"; return false; }
	if (!TestSelect2 ()) { cout << ">> Select2 Failed \n\n"; return false; }
	if (!TestGetList1()) { cout << ">> GetList1 Failed\n\n"; return false; }
	if (!TestGetList2()) { cout << ">> GetList2 Failed\n\n"; return false; }
	cout << ">> Success\n\n"; return true;
}
