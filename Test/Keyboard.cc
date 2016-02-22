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
static Keyboard k;



//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define ADD_KEY_C( key )						\
	list2.push_back (make_pair ( true, key));	\
	list2.push_back (make_pair (false, key));

////////////////////////////////////////////////////////////////////////////////

#define ADD_KEY_P( key )						\
	list2.push_back (make_pair ( true, key));

////////////////////////////////////////////////////////////////////////////////

#define ADD_KEY_R( key )						\
	list2.push_back (make_pair (false, key));



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

static bool TestCompiler (void)
{
	KeyList list1;
	KeyList list2;

	VERIFY (Keyboard::Compile ("{SPACE}{SPC}{ESCAPE}{ESC}", list1));
	ADD_KEY_C (KeySpace);
	ADD_KEY_C (KeySpace);
	ADD_KEY_C (KeyEscape);
	ADD_KEY_C (KeyEscape);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{TAB}{ALT}{LALT}{RALT}{CONTROL}{LCONTROL}{RCONTROL}{CTRL}{LCTRL}{RCTRL}{SHIFT}{LSHIFT}{RSHIFT}{SYSTEM}{LSYSTEM}{RSYSTEM}", list1));
	ADD_KEY_C (KeyTab);
	ADD_KEY_C (KeyAlt);
	ADD_KEY_C (KeyLAlt);
	ADD_KEY_C (KeyRAlt);
	ADD_KEY_C (KeyControl);
	ADD_KEY_C (KeyLControl);
	ADD_KEY_C (KeyRControl);
	ADD_KEY_C (KeyControl);
	ADD_KEY_C (KeyLControl);
	ADD_KEY_C (KeyRControl);
	ADD_KEY_C (KeyShift);
	ADD_KEY_C (KeyLShift);
	ADD_KEY_C (KeyRShift);
	ADD_KEY_C (KeySystem);
	ADD_KEY_C (KeyLSystem);
	ADD_KEY_C (KeyRSystem);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{F1}{F2}{F3}{F4}{F5}{F6}{F7}{F8}{F9}{F10}{F11}{F12}", list1));
	ADD_KEY_C (KeyF1);
	ADD_KEY_C (KeyF2);
	ADD_KEY_C (KeyF3);
	ADD_KEY_C (KeyF4);
	ADD_KEY_C (KeyF5);
	ADD_KEY_C (KeyF6);
	ADD_KEY_C (KeyF7);
	ADD_KEY_C (KeyF8);
	ADD_KEY_C (KeyF9);
	ADD_KEY_C (KeyF10);
	ADD_KEY_C (KeyF11);
	ADD_KEY_C (KeyF12);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{0}{1}{2}{3}{4}{5}{6}{7}{8}{9}", list1));
	ADD_KEY_C (Key0);
	ADD_KEY_C (Key1);
	ADD_KEY_C (Key2);
	ADD_KEY_C (Key3);
	ADD_KEY_C (Key4);
	ADD_KEY_C (Key5);
	ADD_KEY_C (Key6);
	ADD_KEY_C (Key7);
	ADD_KEY_C (Key8);
	ADD_KEY_C (Key9);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{A}{B}{C}{D}{E}{F}{G}{H}{I}{J}{K}{L}{M}{N}{O}{P}{Q}{R}{S}{T}{U}{V}{W}{X}{Y}{Z}", list1));
	ADD_KEY_C (KeyA);
	ADD_KEY_C (KeyB);
	ADD_KEY_C (KeyC);
	ADD_KEY_C (KeyD);
	ADD_KEY_C (KeyE);
	ADD_KEY_C (KeyF);
	ADD_KEY_C (KeyG);
	ADD_KEY_C (KeyH);
	ADD_KEY_C (KeyI);
	ADD_KEY_C (KeyJ);
	ADD_KEY_C (KeyK);
	ADD_KEY_C (KeyL);
	ADD_KEY_C (KeyM);
	ADD_KEY_C (KeyN);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyP);
	ADD_KEY_C (KeyQ);
	ADD_KEY_C (KeyR);
	ADD_KEY_C (KeyS);
	ADD_KEY_C (KeyT);
	ADD_KEY_C (KeyU);
	ADD_KEY_C (KeyV);
	ADD_KEY_C (KeyW);
	ADD_KEY_C (KeyX);
	ADD_KEY_C (KeyY);
	ADD_KEY_C (KeyZ);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{`}{-}{=}{<}{[}{]}{\\}{;}{'}{~}{,}{.}{/}", list1));
	ADD_KEY_C (KeyGrave);
	ADD_KEY_C (KeyMinus);
	ADD_KEY_C (KeyEqual);
	ADD_KEY_C (KeyBackspace);
	ADD_KEY_C (KeyLBracket);
	ADD_KEY_C (KeyRBracket);
	ADD_KEY_C (KeyBackslash);
	ADD_KEY_C (KeySemicolon);
	ADD_KEY_C (KeyQuote);
	ADD_KEY_C (KeyReturn);
	ADD_KEY_C (KeyComma);
	ADD_KEY_C (KeyPeriod);
	ADD_KEY_C (KeySlash);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{GRAVE}{MINUS}{EQUAL}{BACKSPACE}{BS}{LBRACKET}{RBRACKET}{BACKSLASH}{SEMICOLON}{QUOTE}{RETURN}{COMMA}{PERIOD}{SLASH}", list1));
	ADD_KEY_C (KeyGrave);
	ADD_KEY_C (KeyMinus);
	ADD_KEY_C (KeyEqual);
	ADD_KEY_C (KeyBackspace);
	ADD_KEY_C (KeyBackspace);
	ADD_KEY_C (KeyLBracket);
	ADD_KEY_C (KeyRBracket);
	ADD_KEY_C (KeyBackslash);
	ADD_KEY_C (KeySemicolon);
	ADD_KEY_C (KeyQuote);
	ADD_KEY_C (KeyReturn);
	ADD_KEY_C (KeyComma);
	ADD_KEY_C (KeyPeriod);
	ADD_KEY_C (KeySlash);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{LEFT}{UP}{RIGHT}{DOWN}", list1));
	ADD_KEY_C (KeyLeft);
	ADD_KEY_C (KeyUp);
	ADD_KEY_C (KeyRight);
	ADD_KEY_C (KeyDown);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{PRINT}{PAUSE}{BREAK}{INSERT}{INS}{DELETE}{DEL}{HOME}{END}{PAGEUP}{PGUP}{PAGEDOWN}{PGDN}", list1));
	ADD_KEY_C (KeyPrint);
	ADD_KEY_C (KeyPause);
	ADD_KEY_C (KeyPause);
	ADD_KEY_C (KeyInsert);
	ADD_KEY_C (KeyInsert);
	ADD_KEY_C (KeyDelete);
	ADD_KEY_C (KeyDelete);
	ADD_KEY_C (KeyHome);
	ADD_KEY_C (KeyEnd);
	ADD_KEY_C (KeyPageUp);
	ADD_KEY_C (KeyPageUp);
	ADD_KEY_C (KeyPageDown);
	ADD_KEY_C (KeyPageDown);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{NUM+}{NUM-}{NUM*}{NUM/}{NUM.}{NUM~}", list1));
	ADD_KEY_C (KeyAdd);
	ADD_KEY_C (KeySubtract);
	ADD_KEY_C (KeyMultiply);
	ADD_KEY_C (KeyDivide);
	ADD_KEY_C (KeyDecimal);
	ADD_KEY_C (KeyEnter);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{ADD}{SUBTRACT}{MULTIPLY}{DIVIDE}{DECIMAL}{ENTER}", list1));
	ADD_KEY_C (KeyAdd);
	ADD_KEY_C (KeySubtract);
	ADD_KEY_C (KeyMultiply);
	ADD_KEY_C (KeyDivide);
	ADD_KEY_C (KeyDecimal);
	ADD_KEY_C (KeyEnter);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{NUM0}{NUM1}{NUM2}{NUM3}{NUM4}{NUM5}{NUM6}{NUM7}{NUM8}{NUM9}", list1));
	ADD_KEY_C (KeyNum0);
	ADD_KEY_C (KeyNum1);
	ADD_KEY_C (KeyNum2);
	ADD_KEY_C (KeyNum3);
	ADD_KEY_C (KeyNum4);
	ADD_KEY_C (KeyNum5);
	ADD_KEY_C (KeyNum6);
	ADD_KEY_C (KeyNum7);
	ADD_KEY_C (KeyNum8);
	ADD_KEY_C (KeyNum9);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{CAPSLOCK}{SCROLLLOCK}{NUMLOCK}", list1));
	ADD_KEY_C (KeyCapsLock);
	ADD_KEY_C (KeyScrollLock);
	ADD_KEY_C (KeyNumLock);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile (" 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ`-=[]\\;'~,./", list1));
	ADD_KEY_C (KeySpace);
	ADD_KEY_C (Key0);
	ADD_KEY_C (Key1);
	ADD_KEY_C (Key2);
	ADD_KEY_C (Key3);
	ADD_KEY_C (Key4);
	ADD_KEY_C (Key5);
	ADD_KEY_C (Key6);
	ADD_KEY_C (Key7);
	ADD_KEY_C (Key8);
	ADD_KEY_C (Key9);
	ADD_KEY_C (KeyA);
	ADD_KEY_C (KeyB);
	ADD_KEY_C (KeyC);
	ADD_KEY_C (KeyD);
	ADD_KEY_C (KeyE);
	ADD_KEY_C (KeyF);
	ADD_KEY_C (KeyG);
	ADD_KEY_C (KeyH);
	ADD_KEY_C (KeyI);
	ADD_KEY_C (KeyJ);
	ADD_KEY_C (KeyK);
	ADD_KEY_C (KeyL);
	ADD_KEY_C (KeyM);
	ADD_KEY_C (KeyN);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyP);
	ADD_KEY_C (KeyQ);
	ADD_KEY_C (KeyR);
	ADD_KEY_C (KeyS);
	ADD_KEY_C (KeyT);
	ADD_KEY_C (KeyU);
	ADD_KEY_C (KeyV);
	ADD_KEY_C (KeyW);
	ADD_KEY_C (KeyX);
	ADD_KEY_C (KeyY);
	ADD_KEY_C (KeyZ);
	ADD_KEY_C (KeyGrave);
	ADD_KEY_C (KeyMinus);
	ADD_KEY_C (KeyEqual);
	ADD_KEY_C (KeyLBracket);
	ADD_KEY_C (KeyRBracket);
	ADD_KEY_C (KeyBackslash);
	ADD_KEY_C (KeySemicolon);
	ADD_KEY_C (KeyQuote);
	ADD_KEY_C (KeyReturn);
	ADD_KEY_C (KeyComma);
	ADD_KEY_C (KeyPeriod);
	ADD_KEY_C (KeySlash);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (!Keyboard::Compile (nullptr, list1));
	VERIFY (!Keyboard::Compile ("!o", list1));
	VERIFY (!Keyboard::Compile ("{! 2}", list1));
	VERIFY (!Keyboard::Compile ("{oooooooooooooooo}", list1));
	VERIFY (!Keyboard::Compile ("{ooooooooooooooo}", list1));
	VERIFY (!Keyboard::Compile ("{ooooooooooooooooo}", list1));
	VERIFY (!Keyboard::Compile ("}", list1));
	VERIFY (!Keyboard::Compile ("{o}{bs}o}", list1));
	VERIFY (!Keyboard::Compile ("{bs -1}", list1));
	VERIFY (!Keyboard::Compile ("{bs 100}", list1));
	VERIFY (!Keyboard::Compile ("{bs 9999}", list1));
	VERIFY (!Keyboard::Compile ("{bs 2 6 }", list1));
	VERIFY (!Keyboard::Compile ("$$o", list1));
	VERIFY (!Keyboard::Compile ("(o(o(o(o)))", list1));
	VERIFY (!Keyboard::Compile ("(((((o)))))", list1));
	VERIFY (!Keyboard::Compile ("+(o+(ooo))", list1));

	VERIFY (Keyboard::Compile ("", list1));
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("oO{o}Oo", list1));
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("ooo{< 3}", list1));
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyBackspace);
	ADD_KEY_C (KeyBackspace);
	ADD_KEY_C (KeyBackspace);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{bs 0}", list1));
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{bs 1}", list1));
	ADD_KEY_C (KeyBackspace);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{bs 0xa}", list1));
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{bs ooo}", list1));
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{< 2 6}", list1));
	ADD_KEY_C (KeyBackspace);
	ADD_KEY_C (KeyBackspace);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("{bs 2 a}", list1));
	ADD_KEY_C (KeyBackspace);
	ADD_KEY_C (KeyBackspace);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("+(oo)+%o^()", list1));
	ADD_KEY_P (KeyShift);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeyShift);
	ADD_KEY_P (KeyShift);
	ADD_KEY_P (KeyAlt);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeyAlt);
	ADD_KEY_R (KeyShift);
	ADD_KEY_P (KeyControl);
	ADD_KEY_R (KeyControl);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("$o", list1));
	ADD_KEY_P (KeySystem);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeySystem);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("o(o)o((o))", list1));
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("o(O(o(O)o)O)o", list1));
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("((((o))))", list1));
	ADD_KEY_C (KeyO);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("%^+$o", list1));
	ADD_KEY_P (KeyAlt);
	ADD_KEY_P (KeyControl);
	ADD_KEY_P (KeyShift);
	ADD_KEY_P (KeySystem);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeyAlt);
	ADD_KEY_R (KeyControl);
	ADD_KEY_R (KeyShift);
	ADD_KEY_R (KeySystem);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("^$%+o", list1));
	ADD_KEY_P (KeyControl);
	ADD_KEY_P (KeySystem);
	ADD_KEY_P (KeyAlt);
	ADD_KEY_P (KeyShift);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeyAlt);
	ADD_KEY_R (KeyControl);
	ADD_KEY_R (KeyShift);
	ADD_KEY_R (KeySystem);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("+(o^({o}%(o$({o}))))", list1));
	ADD_KEY_P (KeyShift);
	ADD_KEY_C (KeyO);
	ADD_KEY_P (KeyControl);
	ADD_KEY_C (KeyO);
	ADD_KEY_P (KeyAlt);
	ADD_KEY_C (KeyO);
	ADD_KEY_P (KeySystem);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeySystem);
	ADD_KEY_R (KeyAlt);
	ADD_KEY_R (KeyControl);
	ADD_KEY_R (KeyShift);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("+{o}(%o(^{o}($o)))", list1));
	ADD_KEY_P (KeyShift);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeyShift);
	ADD_KEY_P (KeyAlt);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeyAlt);
	ADD_KEY_P (KeyControl);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeyControl);
	ADD_KEY_P (KeySystem);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeySystem);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("()o()o()o()o()", list1));
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("(oOo)", list1));
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("(+((%o)))", list1));
	ADD_KEY_P (KeyShift);
	ADD_KEY_P (KeyAlt);
	ADD_KEY_C (KeyO);
	ADD_KEY_R (KeyAlt);
	ADD_KEY_R (KeyShift);
	VERIFY (list1 == list2);
	list2.clear();

	VERIFY (Keyboard::Compile ("(o)(o)(o)(o)(o)", list1));
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	ADD_KEY_C (KeyO);
	VERIFY (list1 == list2);
	list2.clear();

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static void LiveThread (void)
{
	Timer::Sleep (500);
	k.Click (Key0);
	k.Click (Key1);
	k.Click (Key2);
	k.Click (Key3);
	k.Click (Key4);
	k.Click (Key5);
	k.Click (Key6);
	k.Click (Key7);
	k.Click (Key8);
	k.Click (Key9);
	k.Click (KeyReturn);

	Timer::Sleep (500);
	k.Click (KeyA);
	k.Click (KeyB);
	k.Click (KeyC);
	k.Click (KeyD);
	k.Click (KeyE);
	k.Click (KeyF);
	k.Click (KeyG);
	k.Click (KeyH);
	k.Click (KeyI);
	k.Click (KeyJ);
	k.Click (KeyK);
	k.Click (KeyL);
	k.Click (KeyM);
	k.Click (KeyN);
	k.Click (KeyO);
	k.Click (KeyP);
	k.Click (KeyQ);
	k.Click (KeyR);
	k.Click (KeyS);
	k.Click (KeyT);
	k.Click (KeyU);
	k.Click (KeyV);
	k.Click (KeyW);
	k.Click (KeyX);
	k.Click (KeyY);
	k.Click (KeyZ);
	k.Click (KeyReturn);

	Timer::Sleep (500);
	k.Press (KeyShift);
	k.Click (KeyA);
	k.Click (KeyB);
	k.Click (KeyC);
	k.Click (KeyD);
	k.Click (KeyE);
	k.Click (KeyF);
	k.Click (KeyG);
	k.Click (KeyH);
	k.Click (KeyI);
	k.Click (KeyJ);
	k.Click (KeyK);
	k.Click (KeyL);
	k.Click (KeyM);
	k.Click (KeyN);
	k.Click (KeyO);
	k.Click (KeyP);
	k.Click (KeyQ);
	k.Click (KeyR);
	k.Click (KeyS);
	k.Click (KeyT);
	k.Click (KeyU);
	k.Click (KeyV);
	k.Click (KeyW);
	k.Click (KeyX);
	k.Click (KeyY);
	k.Click (KeyZ);
	k.Release (KeyShift);
	k.Click (KeyReturn);

	Timer::Sleep (500);
	k.Click (KeyAdd);
	k.Click (KeySubtract);
	k.Click (KeyMultiply);
	k.Click (KeyDivide);
	k.Click (KeyDecimal);
	k.Click (KeyNum0);
	k.Click (KeyNum1);
	k.Click (KeyNum2);
	k.Click (KeyNum3);
	k.Click (KeyNum4);
	k.Click (KeyNum5);
	k.Click (KeyNum6);
	k.Click (KeyNum7);
	k.Click (KeyNum8);
	k.Click (KeyNum9);
	k.Click (KeyEnter);

	Timer::Sleep (500);
	k.Click (KeyMinus);
	k.Click (KeyEqual);
	k.Click (KeyBackspace);
	k.Click (KeyBackspace);
	k.Click (KeyGrave);
	k.Click (KeyMinus);
	k.Click (KeyEqual);
	k.Click (KeyLBracket);
	k.Click (KeySpace);
	k.Click (KeyRBracket);
	k.Click (KeyBackslash);
	k.Click (KeySemicolon);
	k.Click (KeyQuote);
	k.Click (KeyComma);
	k.Click (KeyPeriod);
	k.Click (KeySlash);
	k.Click (KeyReturn);

	Timer::Sleep (500);
	k.Press (KeyShift);
	k.Click (KeyGrave);
	k.Click (KeyMinus);
	k.Click (KeyEqual);
	k.Click (KeyLBracket);
	k.Click (KeySpace);
	k.Click (KeyRBracket);
	k.Click (KeyBackslash);
	k.Click (KeySemicolon);
	k.Click (KeyQuote);
	k.Click (KeyComma);
	k.Click (KeyPeriod);
	k.Click (KeySlash);
	k.Release (KeyShift);
	k.Click (KeyReturn);

	Timer::Sleep (500);
	k.Click ("+Hello +Robo<<<+(obot)+1~");
}

static bool TestLive (void)
{
#if (_MSC_VER == 1600)
	cout << "Warning: This test section is unavailable in VS10!\n";
#else

	cout << "Press enter to begin live test";
	getchar();

	char input[32] = { 0 };
	// Create live testing thread
	std::thread live (LiveThread);

	cout << "Numbers: ";
	cin.getline (input, 32);
	VERIFY (strcmp (input, "0123456789") == 0);

	cout << "Alphabet: ";
	cin.getline (input, 32);
	VERIFY (strcmp (input, "abcdefghijklmnopqrstuvwxyz") == 0);

	cout << "Alphabet: ";
	cin.getline (input, 32);
	VERIFY (strcmp (input, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0);

	cout << "Keypad: ";
	cin.getline (input, 32);
	VERIFY (strcmp (input, "+-*/.0123456789") == 0);

	cout << "Punctuation: ";
	cin.getline (input, 32);
	VERIFY (strcmp (input, "`-=[ ]\\;',./") == 0);

	cout << "Punctuation: ";
	cin.getline (input, 32);
	VERIFY (strcmp (input, "~_+{ }|:\"<>?") == 0);

	cout << "Hello Robot: ";
	cin.getline (input, 32);
	VERIFY (strcmp (input, "Hello ROBOT!") == 0);

	// End thread
	live.join();

#endif

	cout << "\nWarning: The next set of tests cannot be automated\n"
		 << "         Please review the following instructions!\n\n";

	cout << "- Live testing will be performed in sets\n"
		 << "- Press enter to begin testing a new set\n"
		 << "- After beginning, focus the testing app\n"
		 << "- Verify results before testing next set\n\n";

	cout << "Function Keys";
	getchar();
	Timer::Sleep (2500);
	k.Click (KeyF1);
	k.Click (KeyF2);
	k.Click (KeyF3);
	k.Click (KeyF4);
	k.Click (KeyF5);
	k.Click (KeyF6);
	k.Click (KeyF7);
	k.Click (KeyF8);
	k.Click (KeyF9);
	k.Click (KeyF10);
	k.Click (KeyF11);
	k.Click (KeyF12);

	cout << "Miscellaneous";
	getchar();
	Timer::Sleep (2500);
	k.Click (KeyPause);
	k.Click (KeyInsert);
	k.Click (KeyDelete);
	k.Click (KeyHome);
	k.Click (KeyEnd);
	k.Click (KeyPageUp);
	k.Click (KeyPageDown);
	k.Click (KeyEscape);
	k.Click (KeyPrint);

	cout << "Modifiers";
	getchar();
	Timer::Sleep (2500);
	k.Click (KeyTab);
	k.Click (KeyAlt);
	k.Click (KeyLAlt);
	k.Click (KeyRAlt);
	k.Click (KeyControl);
	k.Click (KeyLControl);
	k.Click (KeyRControl);
	k.Click (KeyShift);
	k.Click (KeyLShift);
	k.Click (KeyRShift);
	k.Click (KeySystem);
	k.Click (KeySystem);
	k.Click (KeyLSystem);
	k.Click (KeyLSystem);
	k.Click (KeyRSystem);
	k.Click (KeyRSystem);

	cout << "Arrow Keys";
	getchar();
	Timer::Sleep (2500);
	k.Click (KeyLeft);
	k.Click (KeyUp);
	k.Click (KeyRight);
	k.Click (KeyDown);

	cout << "Light Show";
	getchar();
	Timer::Sleep (2500);
	k.Click (KeyCapsLock);
	k.Click (KeyCapsLock);
	k.Click (KeyScrollLock);
	k.Click (KeyScrollLock);
	k.Click (KeyNumLock);
	k.Click (KeyNumLock);

	cout << endl;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestGetState (void)
{
	cout << "Warning: The next set of tests cannot be automated\n"
		 << "         Please review the following instructions!\n\n";

	cout << "- Press keys and verify output\n"
		 << "- Press enter to begin testing\n"
		 << "- Press escape to stop testing\n";

	getchar();

#ifndef ROBOT_OS_MAC

	// OSX doesn't properly handle automated key presses
	k.Press (KeyA);
	k.Press (KeyB);
	k.Press (KeyC);
	VERIFY (Keyboard::GetState (KeyA) &&
			Keyboard::GetState (KeyB) &&
			Keyboard::GetState (KeyC));

	k.Release (KeyA);
	KeyState state; VERIFY (Keyboard::GetState (state));
	VERIFY (!state[KeyA] && state[KeyB] && state[KeyC]);
	k.Release (KeyB);
	k.Release (KeyC);

#endif

	// Loop until the escape key is pressed
	while (!Keyboard::GetState (KeyEscape))
	{
		// Get the key state of all keys in one function call
		KeyState state; VERIFY (Keyboard::GetState (state));

		if (state[KeySpace		]) cout << "Space\t";

		if (state[KeyTab		]) cout << "Tab\t";
		if (state[KeyLAlt		] ||
			state[KeyRAlt		]) cout << "Alt\t";
		if (state[KeyLControl	] ||
			state[KeyRControl	]) cout << "Ctrl\t";
		if (state[KeyLShift		] ||
			state[KeyRShift		]) cout << "Shift\t";
		if (state[KeyLSystem	] ||
			state[KeyRSystem	]) cout << "System\t";

		if (state[KeyF1			]) cout << "F1\t";
		if (state[KeyF2			]) cout << "F2\t";
		if (state[KeyF3			]) cout << "F3\t";
		if (state[KeyF4			]) cout << "F4\t";
		if (state[KeyF5			]) cout << "F5\t";
		if (state[KeyF6			]) cout << "F6\t";
		if (state[KeyF7			]) cout << "F7\t";
		if (state[KeyF8			]) cout << "F8\t";
		if (state[KeyF9			]) cout << "F9\t";
		if (state[KeyF10		]) cout << "F10\t";
		if (state[KeyF11		]) cout << "F11\t";
		if (state[KeyF12		]) cout << "F12\t";

		if (state[Key0			]) cout << "0\t";
		if (state[Key1			]) cout << "1\t";
		if (state[Key2			]) cout << "2\t";
		if (state[Key3			]) cout << "3\t";
		if (state[Key4			]) cout << "4\t";
		if (state[Key5			]) cout << "5\t";
		if (state[Key6			]) cout << "6\t";
		if (state[Key7			]) cout << "7\t";
		if (state[Key8			]) cout << "8\t";
		if (state[Key9			]) cout << "9\t";

		if (state[KeyA			]) cout << "A\t";
		if (state[KeyB			]) cout << "B\t";
		if (state[KeyC			]) cout << "C\t";
		if (state[KeyD			]) cout << "D\t";
		if (state[KeyE			]) cout << "E\t";
		if (state[KeyF			]) cout << "F\t";
		if (state[KeyG			]) cout << "G\t";
		if (state[KeyH			]) cout << "H\t";
		if (state[KeyI			]) cout << "I\t";
		if (state[KeyJ			]) cout << "J\t";
		if (state[KeyK			]) cout << "K\t";
		if (state[KeyL			]) cout << "L\t";
		if (state[KeyM			]) cout << "M\t";
		if (state[KeyN			]) cout << "N\t";
		if (state[KeyO			]) cout << "O\t";
		if (state[KeyP			]) cout << "P\t";
		if (state[KeyQ			]) cout << "Q\t";
		if (state[KeyR			]) cout << "R\t";
		if (state[KeyS			]) cout << "S\t";
		if (state[KeyT			]) cout << "T\t";
		if (state[KeyU			]) cout << "U\t";
		if (state[KeyV			]) cout << "V\t";
		if (state[KeyW			]) cout << "W\t";
		if (state[KeyX			]) cout << "X\t";
		if (state[KeyY			]) cout << "Y\t";
		if (state[KeyZ			]) cout << "Z\t";

		if (state[KeyGrave		]) cout << "`\t";
		if (state[KeyMinus		]) cout << "-\t";
		if (state[KeyEqual		]) cout << "=\t";
		if (state[KeyBackspace	]) cout << "<\t";
		if (state[KeyLBracket	]) cout << "[\t";
		if (state[KeyRBracket	]) cout << "]\t";
		if (state[KeyBackslash	]) cout << "\\\t";
		if (state[KeySemicolon	]) cout << ";\t";
		if (state[KeyQuote		]) cout << "'\t";
		if (state[KeyReturn		]) cout << "~\t";
		if (state[KeyComma		]) cout << ",\t";
		if (state[KeyPeriod		]) cout << ".\t";
		if (state[KeySlash		]) cout << "/\t";

		if (state[KeyLeft		]) cout << "Left\t";
		if (state[KeyUp			]) cout << "Up\t";
		if (state[KeyRight		]) cout << "Right\t";
		if (state[KeyDown		]) cout << "Down\t";

		if (state[KeyPrint		]) cout << "Print\t";
		if (state[KeyPause		]) cout << "Pause\t";
		if (state[KeyInsert		]) cout << "Ins\t";
		if (state[KeyDelete		]) cout << "Del\t";
		if (state[KeyHome		]) cout << "Home\t";
		if (state[KeyEnd		]) cout << "End\t";
		if (state[KeyPageUp		]) cout << "PgUp\t";
		if (state[KeyPageDown	]) cout << "PgDn\t";

		if (state[KeyAdd		]) cout << "N+\t";
		if (state[KeySubtract	]) cout << "N-\t";
		if (state[KeyMultiply	]) cout << "N*\t";
		if (state[KeyDivide		]) cout << "N/\t";
		if (state[KeyDecimal	]) cout << "N.\t";
		if (state[KeyEnter		]) cout << "N~\t";

		if (state[KeyNum0		]) cout << "N0\t";
		if (state[KeyNum1		]) cout << "N1\t";
		if (state[KeyNum2		]) cout << "N2\t";
		if (state[KeyNum3		]) cout << "N3\t";
		if (state[KeyNum4		]) cout << "N4\t";
		if (state[KeyNum5		]) cout << "N5\t";
		if (state[KeyNum6		]) cout << "N6\t";
		if (state[KeyNum7		]) cout << "N7\t";
		if (state[KeyNum8		]) cout << "N8\t";
		if (state[KeyNum9		]) cout << "N9\t";

		if (state[KeyCapsLock	]) cout << "Caps\t";
		if (state[KeyScrollLock	]) cout << "Scroll\t";
		if (state[KeyNumLock	]) cout << "Num\t";

		cout << endl; Timer::Sleep (90);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool TestKeyboard (void)
{
	cout << "BEGIN KEYBOARD TESTING\n------------------------------\n";
	if (!TestCompiler()) { cout << ">> Compiler Failed \n\n"; return false; }
	if (!TestLive    ()) { cout << ">> Live Test Failed\n\n"; return false; }
	if (!TestGetState()) { cout << ">> Get State Failed\n\n"; return false; }
	cout << ">> Success\n\n"; return true;
}
