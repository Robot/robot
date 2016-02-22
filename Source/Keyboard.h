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

#pragma once

#include "Types.h"
#include <vector>
#include <unordered_map>
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Types                                                                      //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

enum Key
{
#ifdef ROBOT_OS_LINUX

	KeySpace		= 0x0020,		// XK_space
	KeyEscape		= 0xFF1B,		// XK_Escape

	KeyTab			= 0xFF09,		// XK_Tab
	KeyAlt			= 0xFFE9,		// XK_Alt_L
	KeyLAlt			= 0xFFE9,		// XK_Alt_L
	KeyRAlt			= 0xFFEA,		// XK_Alt_R
	KeyControl		= 0xFFE3,		// XK_Control_L
	KeyLControl		= 0xFFE3,		// XK_Control_L
	KeyRControl		= 0xFFE4,		// XK_Control_R
	KeyShift		= 0xFFE1,		// XK_Shift_L
	KeyLShift		= 0xFFE1,		// XK_Shift_L
	KeyRShift		= 0xFFE2,		// XK_Shift_R
	KeySystem		= 0xFFEB,		// XK_Super_L
	KeyLSystem		= 0xFFEB,		// XK_Super_L
	KeyRSystem		= 0xFFEC,		// XK_Super_R

	KeyF1			= 0xFFBE,		// XK_F1
	KeyF2			= 0xFFBF,		// XK_F2
	KeyF3			= 0xFFC0,		// XK_F3
	KeyF4			= 0xFFC1,		// XK_F4
	KeyF5			= 0xFFC2,		// XK_F5
	KeyF6			= 0xFFC3,		// XK_F6
	KeyF7			= 0xFFC4,		// XK_F7
	KeyF8			= 0xFFC5,		// XK_F8
	KeyF9			= 0xFFC6,		// XK_F9
	KeyF10			= 0xFFC7,		// XK_F10
	KeyF11			= 0xFFC8,		// XK_F11
	KeyF12			= 0xFFC9,		// XK_F12

	Key0			= 0x0030,		// XK_0
	Key1			= 0x0031,		// XK_1
	Key2			= 0x0032,		// XK_2
	Key3			= 0x0033,		// XK_3
	Key4			= 0x0034,		// XK_4
	Key5			= 0x0035,		// XK_5
	Key6			= 0x0036,		// XK_6
	Key7			= 0x0037,		// XK_7
	Key8			= 0x0038,		// XK_8
	Key9			= 0x0039,		// XK_9

	KeyA			= 0x0061,		// XK_a
	KeyB			= 0x0062,		// XK_b
	KeyC			= 0x0063,		// XK_c
	KeyD			= 0x0064,		// XK_d
	KeyE			= 0x0065,		// XK_e
	KeyF			= 0x0066,		// XK_f
	KeyG			= 0x0067,		// XK_g
	KeyH			= 0x0068,		// XK_h
	KeyI			= 0x0069,		// XK_i
	KeyJ			= 0x006A,		// XK_j
	KeyK			= 0x006B,		// XK_k
	KeyL			= 0x006C,		// XK_l
	KeyM			= 0x006D,		// XK_m
	KeyN			= 0x006E,		// XK_n
	KeyO			= 0x006F,		// XK_o
	KeyP			= 0x0070,		// XK_p
	KeyQ			= 0x0071,		// XK_q
	KeyR			= 0x0072,		// XK_r
	KeyS			= 0x0073,		// XK_s
	KeyT			= 0x0074,		// XK_t
	KeyU			= 0x0075,		// XK_u
	KeyV			= 0x0076,		// XK_v
	KeyW			= 0x0077,		// XK_w
	KeyX			= 0x0078,		// XK_x
	KeyY			= 0x0079,		// XK_y
	KeyZ			= 0x007A,		// XK_z

	KeyGrave		= 0x0060,		// XK_grave
	KeyMinus		= 0x002D,		// XK_minus
	KeyEqual		= 0x003D,		// XK_equal
	KeyBackspace	= 0xFF08,		// XK_BackSpace
	KeyLBracket		= 0x005B,		// XK_bracketleft
	KeyRBracket		= 0x005D,		// XK_bracketright
	KeyBackslash	= 0x005C,		// XK_backslash
	KeySemicolon	= 0x003B,		// XK_semicolon
	KeyQuote		= 0x0027,		// XK_apostrophe
	KeyReturn		= 0xFF0D,		// XK_Return
	KeyComma		= 0x002C,		// XK_comma
	KeyPeriod		= 0x002E,		// XK_period
	KeySlash		= 0x002F,		// XK_slash

	KeyLeft			= 0xFF51,		// XK_Left
	KeyUp			= 0xFF52,		// XK_Up
	KeyRight		= 0xFF53,		// XK_Right
	KeyDown			= 0xFF54,		// XK_Down

	KeyPrint		= 0xFF61,		// XK_Print
	KeyPause		= 0xFF13,		// XK_Pause
	KeyInsert		= 0xFF63,		// XK_Insert
	KeyDelete		= 0xFFFF,		// XK_Delete
	KeyHome			= 0xFF50,		// XK_Home
	KeyEnd			= 0xFF57,		// XK_End
	KeyPageUp		= 0xFF55,		// XK_Page_Up
	KeyPageDown		= 0xFF56,		// XK_Page_Down

	KeyAdd			= 0xFFAB,		// XK_KP_Add
	KeySubtract		= 0xFFAD,		// XK_KP_Subtract
	KeyMultiply		= 0xFFAA,		// XK_KP_Multiply
	KeyDivide		= 0xFFAF,		// XK_KP_Divide
	KeyDecimal		= 0xFFAE,		// XK_KP_Decimal
	KeyEnter		= 0xFF8D,		// XK_KP_Enter

	KeyNum0			= 0xFFB0,		// XK_KP_0
	KeyNum1			= 0xFFB1,		// XK_KP_1
	KeyNum2			= 0xFFB2,		// XK_KP_2
	KeyNum3			= 0xFFB3,		// XK_KP_3
	KeyNum4			= 0xFFB4,		// XK_KP_4
	KeyNum5			= 0xFFB5,		// XK_KP_5
	KeyNum6			= 0xFFB6,		// XK_KP_6
	KeyNum7			= 0xFFB7,		// XK_KP_7
	KeyNum8			= 0xFFB8,		// XK_KP_8
	KeyNum9			= 0xFFB9,		// XK_KP_9

	KeyCapsLock		= 0xFFE5,		// XK_Caps_Lock
	KeyScrollLock	= 0xFF14,		// XK_Scroll_Lock
	KeyNumLock		= 0xFF7F,		// XK_Num_Lock

#endif
#ifdef ROBOT_OS_MAC

	KeySpace		= 0x31,			// kVK_Space
	KeyEscape		= 0x35,			// kVK_Escape

	KeyTab			= 0x30,			// kVK_Tab
	KeyAlt			= 0x3A,			// kVK_Option
	KeyLAlt			= 0x3A,			// kVK_Option
	KeyRAlt			= 0x3D,			// kVK_RightOption
	KeyControl		= 0x3B,			// kVK_Control
	KeyLControl		= 0x3B,			// kVK_Control
	KeyRControl		= 0x3E,			// kVK_RightControl
	KeyShift		= 0x38,			// kVK_Shift
	KeyLShift		= 0x38,			// kVK_Shift
	KeyRShift		= 0x3C,			// kVK_RightShift
	KeySystem		= 0x37,			// kVK_Command
	KeyLSystem		= 0x37,			// kVK_Command
	KeyRSystem		= 0x36,			// kVK_RightCommand

	KeyF1			= 0x7A,			// kVK_F1
	KeyF2			= 0x78,			// kVK_F2
	KeyF3			= 0x63,			// kVK_F3
	KeyF4			= 0x76,			// kVK_F4
	KeyF5			= 0x60,			// kVK_F5
	KeyF6			= 0x61,			// kVK_F6
	KeyF7			= 0x62,			// kVK_F7
	KeyF8			= 0x64,			// kVK_F8
	KeyF9			= 0x65,			// kVK_F9
	KeyF10			= 0x6D,			// kVK_F10
	KeyF11			= 0x67,			// kVK_F11
	KeyF12			= 0x6F,			// kVK_F12

	Key0			= 0x1D,			// kVK_ANSI_0
	Key1			= 0x12,			// kVK_ANSI_1
	Key2			= 0x13,			// kVK_ANSI_2
	Key3			= 0x14,			// kVK_ANSI_3
	Key4			= 0x15,			// kVK_ANSI_4
	Key5			= 0x17,			// kVK_ANSI_5
	Key6			= 0x16,			// kVK_ANSI_6
	Key7			= 0x1A,			// kVK_ANSI_7
	Key8			= 0x1C,			// kVK_ANSI_8
	Key9			= 0x19,			// kVK_ANSI_9

	KeyA			= 0x00,			// kVK_ANSI_A
	KeyB			= 0x0B,			// kVK_ANSI_B
	KeyC			= 0x08,			// kVK_ANSI_C
	KeyD			= 0x02,			// kVK_ANSI_D
	KeyE			= 0x0E,			// kVK_ANSI_E
	KeyF			= 0x03,			// kVK_ANSI_F
	KeyG			= 0x05,			// kVK_ANSI_G
	KeyH			= 0x04,			// kVK_ANSI_H
	KeyI			= 0x22,			// kVK_ANSI_I
	KeyJ			= 0x26,			// kVK_ANSI_J
	KeyK			= 0x28,			// kVK_ANSI_K
	KeyL			= 0x25,			// kVK_ANSI_L
	KeyM			= 0x2E,			// kVK_ANSI_M
	KeyN			= 0x2D,			// kVK_ANSI_N
	KeyO			= 0x1F,			// kVK_ANSI_O
	KeyP			= 0x23,			// kVK_ANSI_P
	KeyQ			= 0x0C,			// kVK_ANSI_Q
	KeyR			= 0x0F,			// kVK_ANSI_R
	KeyS			= 0x01,			// kVK_ANSI_S
	KeyT			= 0x11,			// kVK_ANSI_T
	KeyU			= 0x20,			// kVK_ANSI_U
	KeyV			= 0x09,			// kVK_ANSI_V
	KeyW			= 0x0D,			// kVK_ANSI_W
	KeyX			= 0x07,			// kVK_ANSI_X
	KeyY			= 0x10,			// kVK_ANSI_Y
	KeyZ			= 0x06,			// kVK_ANSI_Z

	KeyGrave		= 0x32,			// kVK_ANSI_Grave
	KeyMinus		= 0x1B,			// kVK_ANSI_Minus
	KeyEqual		= 0x18,			// kVK_ANSI_Equal
	KeyBackspace	= 0x33,			// kVK_Delete
	KeyLBracket		= 0x21,			// kVK_ANSI_LeftBracket
	KeyRBracket		= 0x1E,			// kVK_ANSI_RightBracket
	KeyBackslash	= 0x2A,			// kVK_ANSI_Backslash
	KeySemicolon	= 0x29,			// kVK_ANSI_Semicolon
	KeyQuote		= 0x27,			// kVK_ANSI_Quote
	KeyReturn		= 0x24,			// kVK_Return
	KeyComma		= 0x2B,			// kVK_ANSI_Comma
	KeyPeriod		= 0x2F,			// kVK_ANSI_Period
	KeySlash		= 0x2C,			// kVK_ANSI_Slash

	KeyLeft			= 0x7B,			// kVK_LeftArrow
	KeyUp			= 0x7E,			// kVK_UpArrow
	KeyRight		= 0x7C,			// kVK_RightArrow
	KeyDown			= 0x7D,			// kVK_DownArrow

	KeyPrint		= 0x69,			// kVK_F13
	KeyPause		= 0x71,			// kVK_F15
	KeyInsert		= 0x72,			// kVK_Help
	KeyDelete		= 0x75,			// kVK_ForwardDelete
	KeyHome			= 0x73,			// kVK_Home
	KeyEnd			= 0x77,			// kVK_End
	KeyPageUp		= 0x74,			// kVK_PageUp
	KeyPageDown		= 0x79,			// kVK_PageDown

	KeyAdd			= 0x45,			// kVK_ANSI_KeypadPlus
	KeySubtract		= 0x4E,			// kVK_ANSI_KeypadMinus
	KeyMultiply		= 0x43,			// kVK_ANSI_KeypadMultiply
	KeyDivide		= 0x4B,			// kVK_ANSI_KeypadDivide
	KeyDecimal		= 0x41,			// kVK_ANSI_KeypadDecimal
	KeyEnter		= 0x4C,			// kVK_ANSI_KeypadEnter

	KeyNum0			= 0x52,			// kVK_ANSI_Keypad0
	KeyNum1			= 0x53,			// kVK_ANSI_Keypad1
	KeyNum2			= 0x54,			// kVK_ANSI_Keypad2
	KeyNum3			= 0x55,			// kVK_ANSI_Keypad3
	KeyNum4			= 0x56,			// kVK_ANSI_Keypad4
	KeyNum5			= 0x57,			// kVK_ANSI_Keypad5
	KeyNum6			= 0x58,			// kVK_ANSI_Keypad6
	KeyNum7			= 0x59,			// kVK_ANSI_Keypad7
	KeyNum8			= 0x5B,			// kVK_ANSI_Keypad8
	KeyNum9			= 0x5C,			// kVK_ANSI_Keypad9

	KeyCapsLock		= 0x39,			// kVK_CapsLock
	KeyScrollLock	= 0x6B,			// kVK_F14
	KeyNumLock		= 0x47,			// kVK_ANSI_KeypadClear

#endif
#ifdef ROBOT_OS_WIN

	KeySpace		= 0x20,			// VK_SPACE
	KeyEscape		= 0x1B,			// VK_ESCAPE

	KeyTab			= 0x09,			// VK_TAB
	KeyAlt			= 0x12,			// VK_MENU
	KeyLAlt			= 0xA4,			// VK_LMENU
	KeyRAlt			= 0xA5,			// VK_RMENU
	KeyControl		= 0x11,			// VK_CONTROL
	KeyLControl		= 0xA2,			// VK_LCONTROL
	KeyRControl		= 0xA3,			// VK_RCONTROL
	KeyShift		= 0x10,			// VK_SHIFT
	KeyLShift		= 0xA0,			// VK_LSHIFT
	KeyRShift		= 0xA1,			// VK_RSHIFT
	KeySystem		= 0x5B,			// VK_LWIN
	KeyLSystem		= 0x5B,			// VK_LWIN
	KeyRSystem		= 0x5C,			// VK_RWIN

	KeyF1			= 0x70,			// VK_F1
	KeyF2			= 0x71,			// VK_F2
	KeyF3			= 0x72,			// VK_F3
	KeyF4			= 0x73,			// VK_F4
	KeyF5			= 0x74,			// VK_F5
	KeyF6			= 0x75,			// VK_F6
	KeyF7			= 0x76,			// VK_F7
	KeyF8			= 0x77,			// VK_F8
	KeyF9			= 0x78,			// VK_F9
	KeyF10			= 0x79,			// VK_F10
	KeyF11			= 0x7A,			// VK_F11
	KeyF12			= 0x7B,			// VK_F12

	Key0			= 0x30,			// VK_0
	Key1			= 0x31,			// VK_1
	Key2			= 0x32,			// VK_2
	Key3			= 0x33,			// VK_3
	Key4			= 0x34,			// VK_4
	Key5			= 0x35,			// VK_5
	Key6			= 0x36,			// VK_6
	Key7			= 0x37,			// VK_7
	Key8			= 0x38,			// VK_8
	Key9			= 0x39,			// VK_9

	KeyA			= 0x41,			// VK_A
	KeyB			= 0x42,			// VK_B
	KeyC			= 0x43,			// VK_C
	KeyD			= 0x44,			// VK_D
	KeyE			= 0x45,			// VK_E
	KeyF			= 0x46,			// VK_F
	KeyG			= 0x47,			// VK_G
	KeyH			= 0x48,			// VK_H
	KeyI			= 0x49,			// VK_I
	KeyJ			= 0x4A,			// VK_J
	KeyK			= 0x4B,			// VK_K
	KeyL			= 0x4C,			// VK_L
	KeyM			= 0x4D,			// VK_M
	KeyN			= 0x4E,			// VK_N
	KeyO			= 0x4F,			// VK_O
	KeyP			= 0x50,			// VK_P
	KeyQ			= 0x51,			// VK_Q
	KeyR			= 0x52,			// VK_R
	KeyS			= 0x53,			// VK_S
	KeyT			= 0x54,			// VK_T
	KeyU			= 0x55,			// VK_U
	KeyV			= 0x56,			// VK_V
	KeyW			= 0x57,			// VK_W
	KeyX			= 0x58,			// VK_X
	KeyY			= 0x59,			// VK_Y
	KeyZ			= 0x5A,			// VK_Z

	KeyGrave		= 0xC0,			// VK_OEM_3
	KeyMinus		= 0xBD,			// VK_OEM_MINUS
	KeyEqual		= 0xBB,			// VK_OEM_PLUS
	KeyBackspace	= 0x08,			// VK_BACK
	KeyLBracket		= 0xDB,			// VK_OEM_4
	KeyRBracket		= 0xDD,			// VK_OEM_6
	KeyBackslash	= 0xDC,			// VK_OEM_5
	KeySemicolon	= 0xBA,			// VK_OEM_1
	KeyQuote		= 0xDE,			// VK_OEM_7
	KeyReturn		= 0x0D,			// VK_RETURN
	KeyComma		= 0xBC,			// VK_OEM_COMMA
	KeyPeriod		= 0xBE,			// VK_OEM_PERIOD
	KeySlash		= 0xBF,			// VK_OEM_2

	KeyLeft			= 0x25,			// VK_LEFT
	KeyUp			= 0x26,			// VK_UP
	KeyRight		= 0x27,			// VK_RIGHT
	KeyDown			= 0x28,			// VK_DOWN

	KeyPrint		= 0x2C,			// VK_SNAPSHOT
	KeyPause		= 0x13,			// VK_PAUSE
	KeyInsert		= 0x2D,			// VK_INSERT
	KeyDelete		= 0x2E,			// VK_DELETE
	KeyHome			= 0x24,			// VK_HOME
	KeyEnd			= 0x23,			// VK_END
	KeyPageUp		= 0x21,			// VK_PRIOR
	KeyPageDown		= 0x22,			// VK_NEXT

	KeyAdd			= 0x6B,			// VK_ADD
	KeySubtract		= 0x6D,			// VK_SUBTRACT
	KeyMultiply		= 0x6A,			// VK_MULTIPLY
	KeyDivide		= 0x6F,			// VK_DIVIDE
	KeyDecimal		= 0x6E,			// VK_DECIMAL
	KeyEnter		= 0x0D,			// VK_RETURN

	KeyNum0			= 0x60,			// VK_NUMPAD0
	KeyNum1			= 0x61,			// VK_NUMPAD1
	KeyNum2			= 0x62,			// VK_NUMPAD2
	KeyNum3			= 0x63,			// VK_NUMPAD3
	KeyNum4			= 0x64,			// VK_NUMPAD4
	KeyNum5			= 0x65,			// VK_NUMPAD5
	KeyNum6			= 0x66,			// VK_NUMPAD6
	KeyNum7			= 0x67,			// VK_NUMPAD7
	KeyNum8			= 0x68,			// VK_NUMPAD8
	KeyNum9			= 0x69,			// VK_NUMPAD9

	KeyCapsLock		= 0x14,			// VK_CAPITAL
	KeyScrollLock	= 0x91,			// VK_SCROLL
	KeyNumLock		= 0x90,			// VK_NUMLOCK

#endif
};

////////////////////////////////////////////////////////////////////////////////

#if defined (BUILDING_ROBOT_SHARED) || \
	defined (   USING_ROBOT_SHARED)
	template class ROBOT_EXPORT Enum<Key>;
#endif

////////////////////////////////////////////////////////////////////////////////

typedef std::vector<std::pair<bool, Key>> KeyList;

////////////////////////////////////////////////////////////////////////////////

typedef std::unordered_map<Key, bool, std::hash<int>> KeyState;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Keyboard
{
public:
	Keyboard						(void);

public:
	void			Click			(Key keycode) const;
	void			Press			(Key keycode) const;
	void			Release			(Key keycode) const;

	bool			Click			(const char* keys) const;
	static bool		Compile			(const char* keys,
									 KeyList& result);

	static bool		GetState		(Key       keycode);
	static bool		GetState		(KeyState& result );

public:
	Range			AutoDelay;		// Auto delay range
};

ROBOT_NS_END
