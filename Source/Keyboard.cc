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

#include "Keyboard.h"
#include "Timer.h"

#include <string>
using std::make_pair;
using std::string;

#ifdef ROBOT_OS_LINUX

	#include <X11/extensions/XTest.h>

	// Reference default display
	 extern Display* _Robot_Display;
	#define gDisplay _Robot_Display

#endif
#ifdef ROBOT_OS_MAC

	#include <ApplicationServices/ApplicationServices.h>

#endif
#ifdef ROBOT_OS_WIN

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

#endif
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define TOUPPER( c ) static_cast<char> (toupper (static_cast<int> (c)))

////////////////////////////////////////////////////////////////////////////////

#ifdef ROBOT_OS_LINUX

	#define SET_KEY_STATE( keycode )								\
		key = XKeysymToKeycode (gDisplay, keycode);					\
		result[keycode] =											\
			(keys[key / 8] & (1 << (key % 8))) != 0;

#endif
#ifdef ROBOT_OS_MAC

	#define SET_KEY_STATE( keycode )								\
		result[keycode] = CGEventSourceKeyState						\
			(kCGEventSourceStateHIDSystemState,						\
			(CGKeyCode) keycode);

#endif
#ifdef ROBOT_OS_WIN

	#define SET_KEY_STATE( keycode )								\
		result[keycode] = GetAsyncKeyState (keycode) != 0;

#endif



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

#ifdef ROBOT_OS_LINUX

	////////////////////////////////////////////////////////////////////////////////

	static bool IsXTestAvailable (void)
	{
		// Initialize only one time
		static int8 available = -1;

		// If not checked yet
		if (available == -1)
		{
			available = 0;
			// Check for a valid X-Window display
			if (gDisplay == nullptr) return false;

			// Check for XTest extension
			int32 major, minor, evt, error;
			if (!XQueryExtension (gDisplay,
				XTestExtensionName, &major,
				&evt, &error)) return false;

			// Check the XTest version value
			if (!XTestQueryExtension (gDisplay,
				&evt, &error, &major, &minor))
				return false;

			// We require XTest version 2.2 in order to work properly
			if (major < 2 || (major == 2 && minor < 2)) return false;

			// Perform a test grab and finish
			XTestGrabControl (gDisplay, True);
			available = 1;
		}

		return available == 1;
	}

#endif

////////////////////////////////////////////////////////////////////////////////

static void CancelMods (int8* modkeys, int8 group, KeyList& result)
{
	if (modkeys[0] == group)
	{
		// Append single alt modifier release command
		result.push_back (make_pair (false, KeyAlt));
		modkeys[0] = -1;
	}

	if (modkeys[1] == group)
	{
		// Append single control modifier release command
		result.push_back (make_pair (false, KeyControl));
		modkeys[1] = -1;
	}

	if (modkeys[2] == group)
	{
		// Append single shift modifier release command
		result.push_back (make_pair (false, KeyShift));
		modkeys[2] = -1;
	}

	if (modkeys[3] == group)
	{
		// Append single system modifier release command
		result.push_back (make_pair (false, KeySystem));
		modkeys[3] = -1;
	}
}



//----------------------------------------------------------------------------//
// Declarations                                                           Key //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

ROBOT_ENUM (Key)
{
	ROBOT_ENUM_MAP (KeySpace,		" "			);
	ROBOT_ENUM_MAP (KeySpace,		"SPACE"		);
	ROBOT_ENUM_MAP (KeySpace,		"SPC"		);
	ROBOT_ENUM_MAP (KeyEscape,		"ESCAPE"	);
	ROBOT_ENUM_MAP (KeyEscape,		"ESC"		);

	ROBOT_ENUM_MAP (KeyTab,			"TAB"		);
	ROBOT_ENUM_MAP (KeyAlt,			"ALT"		);
	ROBOT_ENUM_MAP (KeyLAlt,		"LALT"		);
	ROBOT_ENUM_MAP (KeyRAlt,		"RALT"		);
	ROBOT_ENUM_MAP (KeyControl,		"CONTROL"	);
	ROBOT_ENUM_MAP (KeyLControl,	"LCONTROL"	);
	ROBOT_ENUM_MAP (KeyRControl,	"RCONTROL"	);
	ROBOT_ENUM_MAP (KeyControl,		"CTRL"		);
	ROBOT_ENUM_MAP (KeyLControl,	"LCTRL"		);
	ROBOT_ENUM_MAP (KeyRControl,	"RCTRL"		);
	ROBOT_ENUM_MAP (KeyShift,		"SHIFT"		);
	ROBOT_ENUM_MAP (KeyLShift,		"LSHIFT"	);
	ROBOT_ENUM_MAP (KeyRShift,		"RSHIFT"	);
	ROBOT_ENUM_MAP (KeySystem,		"SYSTEM"	);
	ROBOT_ENUM_MAP (KeyLSystem,		"LSYSTEM"	);
	ROBOT_ENUM_MAP (KeyRSystem,		"RSYSTEM"	);

	ROBOT_ENUM_MAP (KeyF1,			"F1"		);
	ROBOT_ENUM_MAP (KeyF2,			"F2"		);
	ROBOT_ENUM_MAP (KeyF3,			"F3"		);
	ROBOT_ENUM_MAP (KeyF4,			"F4"		);
	ROBOT_ENUM_MAP (KeyF5,			"F5"		);
	ROBOT_ENUM_MAP (KeyF6,			"F6"		);
	ROBOT_ENUM_MAP (KeyF7,			"F7"		);
	ROBOT_ENUM_MAP (KeyF8,			"F8"		);
	ROBOT_ENUM_MAP (KeyF9,			"F9"		);
	ROBOT_ENUM_MAP (KeyF10,			"F10"		);
	ROBOT_ENUM_MAP (KeyF11,			"F11"		);
	ROBOT_ENUM_MAP (KeyF12,			"F12"		);

	ROBOT_ENUM_MAP (Key0,			"0"			);
	ROBOT_ENUM_MAP (Key1,			"1"			);
	ROBOT_ENUM_MAP (Key2,			"2"			);
	ROBOT_ENUM_MAP (Key3,			"3"			);
	ROBOT_ENUM_MAP (Key4,			"4"			);
	ROBOT_ENUM_MAP (Key5,			"5"			);
	ROBOT_ENUM_MAP (Key6,			"6"			);
	ROBOT_ENUM_MAP (Key7,			"7"			);
	ROBOT_ENUM_MAP (Key8,			"8"			);
	ROBOT_ENUM_MAP (Key9,			"9"			);

	ROBOT_ENUM_MAP (KeyA,			"A"			);
	ROBOT_ENUM_MAP (KeyB,			"B"			);
	ROBOT_ENUM_MAP (KeyC,			"C"			);
	ROBOT_ENUM_MAP (KeyD,			"D"			);
	ROBOT_ENUM_MAP (KeyE,			"E"			);
	ROBOT_ENUM_MAP (KeyF,			"F"			);
	ROBOT_ENUM_MAP (KeyG,			"G"			);
	ROBOT_ENUM_MAP (KeyH,			"H"			);
	ROBOT_ENUM_MAP (KeyI,			"I"			);
	ROBOT_ENUM_MAP (KeyJ,			"J"			);
	ROBOT_ENUM_MAP (KeyK,			"K"			);
	ROBOT_ENUM_MAP (KeyL,			"L"			);
	ROBOT_ENUM_MAP (KeyM,			"M"			);
	ROBOT_ENUM_MAP (KeyN,			"N"			);
	ROBOT_ENUM_MAP (KeyO,			"O"			);
	ROBOT_ENUM_MAP (KeyP,			"P"			);
	ROBOT_ENUM_MAP (KeyQ,			"Q"			);
	ROBOT_ENUM_MAP (KeyR,			"R"			);
	ROBOT_ENUM_MAP (KeyS,			"S"			);
	ROBOT_ENUM_MAP (KeyT,			"T"			);
	ROBOT_ENUM_MAP (KeyU,			"U"			);
	ROBOT_ENUM_MAP (KeyV,			"V"			);
	ROBOT_ENUM_MAP (KeyW,			"W"			);
	ROBOT_ENUM_MAP (KeyX,			"X"			);
	ROBOT_ENUM_MAP (KeyY,			"Y"			);
	ROBOT_ENUM_MAP (KeyZ,			"Z"			);

	ROBOT_ENUM_MAP (KeyGrave,		"`"			);
	ROBOT_ENUM_MAP (KeyMinus,		"-"			);
	ROBOT_ENUM_MAP (KeyEqual,		"="			);
	ROBOT_ENUM_MAP (KeyBackspace,	"<"			);
	ROBOT_ENUM_MAP (KeyLBracket,	"["			);
	ROBOT_ENUM_MAP (KeyRBracket,	"]"			);
	ROBOT_ENUM_MAP (KeyBackslash,	"\\"		);
	ROBOT_ENUM_MAP (KeySemicolon,	";"			);
	ROBOT_ENUM_MAP (KeyQuote,		"'"			);
	ROBOT_ENUM_MAP (KeyReturn,		"~"			);
	ROBOT_ENUM_MAP (KeyComma,		","			);
	ROBOT_ENUM_MAP (KeyPeriod,		"."			);
	ROBOT_ENUM_MAP (KeySlash,		"/"			);

	ROBOT_ENUM_MAP (KeyGrave,		"GRAVE"		);
	ROBOT_ENUM_MAP (KeyMinus,		"MINUS"		);
	ROBOT_ENUM_MAP (KeyEqual,		"EQUAL"		);
	ROBOT_ENUM_MAP (KeyBackspace,	"BACKSPACE"	);
	ROBOT_ENUM_MAP (KeyBackspace,	"BS"		);
	ROBOT_ENUM_MAP (KeyLBracket,	"LBRACKET"	);
	ROBOT_ENUM_MAP (KeyRBracket,	"RBRACKET"	);
	ROBOT_ENUM_MAP (KeyBackslash,	"BACKSLASH"	);
	ROBOT_ENUM_MAP (KeySemicolon,	"SEMICOLON"	);
	ROBOT_ENUM_MAP (KeyQuote,		"QUOTE"		);
	ROBOT_ENUM_MAP (KeyReturn,		"RETURN"	);
	ROBOT_ENUM_MAP (KeyComma,		"COMMA"		);
	ROBOT_ENUM_MAP (KeyPeriod,		"PERIOD"	);
	ROBOT_ENUM_MAP (KeySlash,		"SLASH"		);

	ROBOT_ENUM_MAP (KeyLeft,		"LEFT"		);
	ROBOT_ENUM_MAP (KeyUp,			"UP"		);
	ROBOT_ENUM_MAP (KeyRight,		"RIGHT"		);
	ROBOT_ENUM_MAP (KeyDown,		"DOWN"		);

	ROBOT_ENUM_MAP (KeyPrint,		"PRINT"		);
	ROBOT_ENUM_MAP (KeyPause,		"PAUSE"		);
	ROBOT_ENUM_MAP (KeyPause,		"BREAK"		);
	ROBOT_ENUM_MAP (KeyInsert,		"INSERT"	);
	ROBOT_ENUM_MAP (KeyInsert,		"INS"		);
	ROBOT_ENUM_MAP (KeyDelete,		"DELETE"	);
	ROBOT_ENUM_MAP (KeyDelete,		"DEL"		);
	ROBOT_ENUM_MAP (KeyHome,		"HOME"		);
	ROBOT_ENUM_MAP (KeyEnd,			"END"		);
	ROBOT_ENUM_MAP (KeyPageUp,		"PAGEUP"	);
	ROBOT_ENUM_MAP (KeyPageUp,		"PGUP"		);
	ROBOT_ENUM_MAP (KeyPageDown,	"PAGEDOWN"	);
	ROBOT_ENUM_MAP (KeyPageDown,	"PGDN"		);

	ROBOT_ENUM_MAP (KeyAdd,			"NUM+"		);
	ROBOT_ENUM_MAP (KeySubtract,	"NUM-"		);
	ROBOT_ENUM_MAP (KeyMultiply,	"NUM*"		);
	ROBOT_ENUM_MAP (KeyDivide,		"NUM/"		);
	ROBOT_ENUM_MAP (KeyDecimal,		"NUM."		);
	ROBOT_ENUM_MAP (KeyEnter,		"NUM~"		);

	ROBOT_ENUM_MAP (KeyAdd,			"ADD"		);
	ROBOT_ENUM_MAP (KeySubtract,	"SUBTRACT"	);
	ROBOT_ENUM_MAP (KeyMultiply,	"MULTIPLY"	);
	ROBOT_ENUM_MAP (KeyDivide,		"DIVIDE"	);
	ROBOT_ENUM_MAP (KeyDecimal,		"DECIMAL"	);
	ROBOT_ENUM_MAP (KeyEnter,		"ENTER"		);

	ROBOT_ENUM_MAP (KeyNum0,		"NUM0"		);
	ROBOT_ENUM_MAP (KeyNum1,		"NUM1"		);
	ROBOT_ENUM_MAP (KeyNum2,		"NUM2"		);
	ROBOT_ENUM_MAP (KeyNum3,		"NUM3"		);
	ROBOT_ENUM_MAP (KeyNum4,		"NUM4"		);
	ROBOT_ENUM_MAP (KeyNum5,		"NUM5"		);
	ROBOT_ENUM_MAP (KeyNum6,		"NUM6"		);
	ROBOT_ENUM_MAP (KeyNum7,		"NUM7"		);
	ROBOT_ENUM_MAP (KeyNum8,		"NUM8"		);
	ROBOT_ENUM_MAP (KeyNum9,		"NUM9"		);

	ROBOT_ENUM_MAP (KeyCapsLock,	"CAPSLOCK"	);
	ROBOT_ENUM_MAP (KeyScrollLock,	"SCROLLLOCK");
	ROBOT_ENUM_MAP (KeyNumLock,		"NUMLOCK"	);
}



//----------------------------------------------------------------------------//
// Constructors                                                      Keyboard //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Keyboard::Keyboard (void)
{
	AutoDelay.Min = 40;
	AutoDelay.Max = 90;
}



//----------------------------------------------------------------------------//
// Functions                                                         Keyboard //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void Keyboard::Click (Key keycode) const
{
	Press   (keycode);
	Release (keycode);
}

////////////////////////////////////////////////////////////////////////////////

void Keyboard::Press (Key keycode) const
{
#ifdef ROBOT_OS_LINUX

	// Check for XTest availability
	if (!IsXTestAvailable()) return;

	// Convert keysym and perform the key event
	XTestFakeKeyEvent (gDisplay, XKeysymToKeycode
		(gDisplay, keycode), True, CurrentTime);

	// Flush output buffer
	XSync (gDisplay, False);

#endif
#ifdef ROBOT_OS_MAC

	// Create an HID hardware event source
	CGEventSourceRef src = CGEventSourceCreate
		(kCGEventSourceStateHIDSystemState);

	// Create a new keyboard key press event
	CGEventRef evt = CGEventCreateKeyboardEvent
			(src, (CGKeyCode) keycode, true);

	// Post keyboard event and release
	CGEventPost (kCGHIDEventTap, evt);
	CFRelease (evt); CFRelease (src);

#endif
#ifdef ROBOT_OS_WIN

	INPUT input = { 0 };
	// Prepare a keyboard event
	input.type = INPUT_KEYBOARD;

	// Attach the keycode
	input.ki.wVk = keycode;
	input.ki.dwFlags = 0;

	// Send the prepared key input event
	SendInput (1, &input, sizeof (INPUT));

#endif

	Timer::Sleep (AutoDelay);
}

////////////////////////////////////////////////////////////////////////////////

void Keyboard::Release (Key keycode) const
{
#ifdef ROBOT_OS_LINUX

	// Check for XTest availability
	if (!IsXTestAvailable()) return;

	// Convert keysym and perform the key event
	XTestFakeKeyEvent (gDisplay, XKeysymToKeycode
		(gDisplay, keycode), False, CurrentTime);

	// Flush output buffer
	XSync (gDisplay, False);

#endif
#ifdef ROBOT_OS_MAC

	// Create an HID hardware event source
	CGEventSourceRef src = CGEventSourceCreate
		(kCGEventSourceStateHIDSystemState);

	// Create a new keyboard key release event
	CGEventRef evt = CGEventCreateKeyboardEvent
			(src, (CGKeyCode) keycode, false);

	// Post keyboard event and release
	CGEventPost (kCGHIDEventTap, evt);
	CFRelease (evt); CFRelease (src);

#endif
#ifdef ROBOT_OS_WIN

	INPUT input = { 0 };
	// Prepare a keyboard event
	input.type = INPUT_KEYBOARD;

	// Attach the keycode
	input.ki.wVk = keycode;
	input.ki.dwFlags = KEYEVENTF_KEYUP;

	// Send the prepared key input event
	SendInput (1, &input, sizeof (INPUT));

#endif

	Timer::Sleep (AutoDelay);
}

////////////////////////////////////////////////////////////////////////////////

bool Keyboard::Click (const char* keys) const
{
	// Attempt to compile keys
	KeyList list = KeyList();
	if (Compile (keys, list))
	{
		// Iterate keys and execute
		for (auto i = list.begin();
			 i != list.end(); ++i)
		{
			i->first ? Press (i->second) :
					 Release (i->second);
		}

		// All done
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Keyboard::Compile (const char* keys, KeyList& result)
{
	// Exit if the keys value is empty
	if (keys == nullptr) return false;

	// Group states (Alt Ctrl Shift Sys)
	int8 modkeys[] = { -1, -1, -1, -1 };
	int8 group = 0;

	// Clear result
	result.clear();

	// Iterate through all characters in keys
	for (uintptr i = 0; keys[i] != '\0'; ++i)
	{
		switch (keys[i])
		{
			case '}':
			{
				// If these appear at this point, they
				// are out of context, so return error
				return false;
			}

			case '{':
			{
				char token[16];
				char count[ 4]; ++i;
				bool cFlag = false;

				// Begin assembling potential token
				for (uintptr j = 0; true; ++j, ++i)
				{
					// No closing brace
					if (keys[i] == '\0')
						return false;

					if (cFlag)
					{
						// Token too long
						if (j >= 4)
							return false;

						// Token finished
						if (keys[i] == '}')
						{
							count[j] = '\0';
							break;
						}

						// Add to the count
						count[j] = keys[i];
					}

					else
					{
						// Token too long
						if (j >= 16)
							return false;

						// Token finished
						if (keys[i] == '}')
						{
							token[j] = '\0';
							break;
						}

						// Begin the count
						if (keys[i] == ' ')
						{
							cFlag = true;
							token[j] = '\0';
							j = -1;
							continue;
						}

						// Add character to the token
						token[j] = TOUPPER (keys[i]);
					}
				}

				// Parse and convert the current key
				Key key = Enum<Key>::Parse (token);
				if ((int32) key == -1) return false;
				int32 keyCount = 1; if (cFlag)
				{
					keyCount = (int32) strtol
						(count, nullptr, 10);

					// Ensure count value is within range
					if (keyCount < 0 || keyCount > 99)
						return false;
				}

				// Append a count number of commands
				for (int32 j = 0; j < keyCount; ++j)
				{
					result.push_back (make_pair ( true, key));
					result.push_back (make_pair (false, key));
				}

				// Cancel all current modifiers
				CancelMods (modkeys, 0, result);
				break;
			}

			case '%':
			{
				// Check for multiple definitions
				if (modkeys[0] != -1) return false;

				// Append single alt modifier press command
				result.push_back (make_pair (true, KeyAlt));
				modkeys[0] = 0; break;
			}

			case '^':
			{
				// Check for multiple definitions
				if (modkeys[1] != -1) return false;

				// Append single control modifier press command
				result.push_back (make_pair (true, KeyControl));
				modkeys[1] = 0; break;
			}

			case '+':
			{
				// Check for multiple definitions
				if (modkeys[2] != -1) return false;

				// Append single shift modifier press command
				result.push_back (make_pair (true, KeyShift));
				modkeys[2] = 0; break;
			}

			case '$':
			{
				// Check for multiple definitions
				if (modkeys[3] != -1) return false;

				// Append single system modifier press command
				result.push_back (make_pair (true, KeySystem));
				modkeys[3] = 0; break;
			}

			case '(':
			{
				// Increment the group value
				if (++group > 4) return false;

				// Adjust current modifier parent value
				if (modkeys[0] == 0) modkeys[0] = group;
				if (modkeys[1] == 0) modkeys[1] = group;
				if (modkeys[2] == 0) modkeys[2] = group;
				if (modkeys[3] == 0) modkeys[3] = group;
				break;
			}

			case ')':
			{
				// Check current group value
				if (group < 1) return false;

				// Decrement and cancel the modifiers
				CancelMods (modkeys, group--, result);
				break;
			}

			// Ignore control characters
			case '\t': case '\n': case '\v':
			case '\f': case '\r': break;

			default:
			{
				// Parse and convert the current key
				char temp[2] = { TOUPPER (keys[i]), '\0' };
				Key key = Enum<Key>::Parse (temp);
				if ((int32) key == -1) return false;

				// Append a single key value command
				result.push_back (make_pair ( true, key));
				result.push_back (make_pair (false, key));

				// Cancel all current modifiers
				CancelMods (modkeys, 0, result);
				break;
			}
		}
	}

	// Check group ID
	return group == 0;
}

////////////////////////////////////////////////////////////////////////////////

bool Keyboard::GetState (Key keycode)
{
#ifdef ROBOT_OS_LINUX

	// Check whether XTest is available
	if (!IsXTestAvailable()) return false;

	// Get the current key status of the entire keyboard
	char keys[32] = {0}; XQueryKeymap (gDisplay, keys);
	KeyCode key = XKeysymToKeycode (gDisplay, keycode);

	// Lookup the specified keycode in the keymap
	return (keys[key / 8] & (1 << (key % 8))) != 0;

#endif
#ifdef ROBOT_OS_MAC

	// Check if keycode pressed
	return CGEventSourceKeyState
		(kCGEventSourceStateHIDSystemState, (CGKeyCode) keycode);

#endif
#ifdef ROBOT_OS_WIN

	// Check if keycode is currently pressed
	return GetAsyncKeyState (keycode) != 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Keyboard::GetState (KeyState& result)
{
	// Clear result
	result.clear();

#ifdef ROBOT_OS_LINUX

	// Check whether XTest is available
	if (!IsXTestAvailable()) return false;

	// Get the current key status of the entire keyboard
	char keys[32] = {0}; XQueryKeymap (gDisplay, keys);
	KeyCode key;

#endif

	SET_KEY_STATE (KeySpace		);
	SET_KEY_STATE (KeyEscape	);

	SET_KEY_STATE (KeyTab		);
	SET_KEY_STATE (KeyAlt		);
	SET_KEY_STATE (KeyLAlt		);
	SET_KEY_STATE (KeyRAlt		);
	SET_KEY_STATE (KeyControl	);
	SET_KEY_STATE (KeyLControl	);
	SET_KEY_STATE (KeyRControl	);
	SET_KEY_STATE (KeyShift		);
	SET_KEY_STATE (KeyLShift	);
	SET_KEY_STATE (KeyRShift	);
	SET_KEY_STATE (KeySystem	);
	SET_KEY_STATE (KeyLSystem	);
	SET_KEY_STATE (KeyRSystem	);

	SET_KEY_STATE (KeyF1		);
	SET_KEY_STATE (KeyF2		);
	SET_KEY_STATE (KeyF3		);
	SET_KEY_STATE (KeyF4		);
	SET_KEY_STATE (KeyF5		);
	SET_KEY_STATE (KeyF6		);
	SET_KEY_STATE (KeyF7		);
	SET_KEY_STATE (KeyF8		);
	SET_KEY_STATE (KeyF9		);
	SET_KEY_STATE (KeyF10		);
	SET_KEY_STATE (KeyF11		);
	SET_KEY_STATE (KeyF12		);

	SET_KEY_STATE (Key0			);
	SET_KEY_STATE (Key1			);
	SET_KEY_STATE (Key2			);
	SET_KEY_STATE (Key3			);
	SET_KEY_STATE (Key4			);
	SET_KEY_STATE (Key5			);
	SET_KEY_STATE (Key6			);
	SET_KEY_STATE (Key7			);
	SET_KEY_STATE (Key8			);
	SET_KEY_STATE (Key9			);

	SET_KEY_STATE (KeyA			);
	SET_KEY_STATE (KeyB			);
	SET_KEY_STATE (KeyC			);
	SET_KEY_STATE (KeyD			);
	SET_KEY_STATE (KeyE			);
	SET_KEY_STATE (KeyF			);
	SET_KEY_STATE (KeyG			);
	SET_KEY_STATE (KeyH			);
	SET_KEY_STATE (KeyI			);
	SET_KEY_STATE (KeyJ			);
	SET_KEY_STATE (KeyK			);
	SET_KEY_STATE (KeyL			);
	SET_KEY_STATE (KeyM			);
	SET_KEY_STATE (KeyN			);
	SET_KEY_STATE (KeyO			);
	SET_KEY_STATE (KeyP			);
	SET_KEY_STATE (KeyQ			);
	SET_KEY_STATE (KeyR			);
	SET_KEY_STATE (KeyS			);
	SET_KEY_STATE (KeyT			);
	SET_KEY_STATE (KeyU			);
	SET_KEY_STATE (KeyV			);
	SET_KEY_STATE (KeyW			);
	SET_KEY_STATE (KeyX			);
	SET_KEY_STATE (KeyY			);
	SET_KEY_STATE (KeyZ			);

	SET_KEY_STATE (KeyGrave		);
	SET_KEY_STATE (KeyMinus		);
	SET_KEY_STATE (KeyEqual		);
	SET_KEY_STATE (KeyBackspace	);
	SET_KEY_STATE (KeyLBracket	);
	SET_KEY_STATE (KeyRBracket	);
	SET_KEY_STATE (KeyBackslash	);
	SET_KEY_STATE (KeySemicolon	);
	SET_KEY_STATE (KeyQuote		);
	SET_KEY_STATE (KeyReturn	);
	SET_KEY_STATE (KeyComma		);
	SET_KEY_STATE (KeyPeriod	);
	SET_KEY_STATE (KeySlash		);

	SET_KEY_STATE (KeyLeft		);
	SET_KEY_STATE (KeyUp		);
	SET_KEY_STATE (KeyRight		);
	SET_KEY_STATE (KeyDown		);

	SET_KEY_STATE (KeyPrint		);
	SET_KEY_STATE (KeyPause		);
	SET_KEY_STATE (KeyInsert	);
	SET_KEY_STATE (KeyDelete	);
	SET_KEY_STATE (KeyHome		);
	SET_KEY_STATE (KeyEnd		);
	SET_KEY_STATE (KeyPageUp	);
	SET_KEY_STATE (KeyPageDown	);

	SET_KEY_STATE (KeyAdd		);
	SET_KEY_STATE (KeySubtract	);
	SET_KEY_STATE (KeyMultiply	);
	SET_KEY_STATE (KeyDivide	);
	SET_KEY_STATE (KeyDecimal	);
	SET_KEY_STATE (KeyEnter		);

	SET_KEY_STATE (KeyNum0		);
	SET_KEY_STATE (KeyNum1		);
	SET_KEY_STATE (KeyNum2		);
	SET_KEY_STATE (KeyNum3		);
	SET_KEY_STATE (KeyNum4		);
	SET_KEY_STATE (KeyNum5		);
	SET_KEY_STATE (KeyNum6		);
	SET_KEY_STATE (KeyNum7		);
	SET_KEY_STATE (KeyNum8		);
	SET_KEY_STATE (KeyNum9		);

	SET_KEY_STATE (KeyCapsLock	);
	SET_KEY_STATE (KeyScrollLock);
	SET_KEY_STATE (KeyNumLock	);

	return true;
}

ROBOT_NS_END
