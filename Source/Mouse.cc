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

#include "Mouse.h"
#include "Timer.h"

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



//----------------------------------------------------------------------------//
// Declarations                                                        Button //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

ROBOT_ENUM (Button)
{
	ROBOT_ENUM_MAP (ButtonLeft,		"LEFT"		);
	ROBOT_ENUM_MAP (ButtonMid,		"MID"		);
	ROBOT_ENUM_MAP (ButtonMiddle,	"MIDDLE"	);
	ROBOT_ENUM_MAP (ButtonRight,	"RIGHT"		);

	ROBOT_ENUM_MAP (ButtonX1,		"X1"		);
	ROBOT_ENUM_MAP (ButtonX2,		"X2"		);
}



//----------------------------------------------------------------------------//
// Constructors                                                         Mouse //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Mouse::Mouse (void)
{
	AutoDelay.Min = 40;
	AutoDelay.Max = 90;
}



//----------------------------------------------------------------------------//
// Functions                                                            Mouse //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void Mouse::Click (Button button) const
{
	Press   (button);
	Release (button);
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::Press (Button button) const
{
#ifdef ROBOT_OS_LINUX

	// Ignore extra buttons
	if (button == ButtonX1 ||
		button == ButtonX2 ||
		!IsXTestAvailable()) return;

	switch (button)
	{
		case ButtonLeft: XTestFakeButtonEvent
			(gDisplay, 1, True, CurrentTime); break;

		case ButtonMid: XTestFakeButtonEvent
			(gDisplay, 2, True, CurrentTime); break;

		case ButtonRight: XTestFakeButtonEvent
			(gDisplay, 3, True, CurrentTime); break;

		default: break;
	}

	// Flush output buffer
	XSync (gDisplay, False);

#endif
#ifdef ROBOT_OS_MAC

	// Double-click timer
	static Timer elapsed;

	// Ignore extra buttons
	if (button == ButtonX1 ||
		button == ButtonX2) return;

	// Create an HID hardware event source
	CGEventSourceRef src = CGEventSourceCreate
		(kCGEventSourceStateHIDSystemState);

	// Retrieve the current mouse position
	CGEventRef pos = CGEventCreate (src);
	CGPoint p = CGEventGetLocation (pos);
	CFRelease (pos);

	// Create a press event
	CGEventRef evt = nullptr;
	switch (button)
	{
		case ButtonLeft:
			evt = CGEventCreateMouseEvent
			(src, kCGEventLeftMouseDown,
			p, kCGMouseButtonLeft); break;

		case ButtonMid:
			evt = CGEventCreateMouseEvent
			(src, kCGEventOtherMouseDown,
			p, kCGMouseButtonCenter); break;

		case ButtonRight:
			evt = CGEventCreateMouseEvent
			(src, kCGEventRightMouseDown,
			p, kCGMouseButtonRight); break;

		default: break;
	}

	// Detect double click version
	if (!elapsed.HasExpired (500))
	{
		// TODO: Use system double click
		// interval instead. See [NSEvent
		// doubleClickInterval] function.

		CGEventSetIntegerValueField (evt,
			kCGMouseEventClickState, 2);
		elapsed.Reset();
	}

	else
	{
		CGEventSetIntegerValueField (evt,
			kCGMouseEventClickState, 1);
		elapsed.Start();
	}

	// Post mouse event and release
	CGEventPost (kCGHIDEventTap, evt);
	CFRelease (evt); CFRelease (src);

#endif
#ifdef ROBOT_OS_WIN

	INPUT input = { 0 };
	// Prepare a mouse event
	input.type = INPUT_MOUSE;

	switch (button)
	{
		case ButtonLeft:
			input.mi.dwFlags = GetSystemMetrics (SM_SWAPBUTTON) ?
				MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_LEFTDOWN; break;

		case ButtonMid:
			input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN; break;

		case ButtonRight:
			input.mi.dwFlags = GetSystemMetrics (SM_SWAPBUTTON) ?
				MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN; break;

		case ButtonX1:
			input.mi.dwFlags = MOUSEEVENTF_XDOWN;
			input.mi.mouseData = XBUTTON1; break;

		case ButtonX2:
			input.mi.dwFlags = MOUSEEVENTF_XDOWN;
			input.mi.mouseData = XBUTTON2; break;
	}

	// Send the prepared mouse input event
	SendInput (1, &input, sizeof (INPUT));

#endif

	Timer::Sleep (AutoDelay);
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::Release (Button button) const
{
#ifdef ROBOT_OS_LINUX

	// Ignore extra buttons
	if (button == ButtonX1 ||
		button == ButtonX2 ||
		!IsXTestAvailable()) return;

	switch (button)
	{
		case ButtonLeft: XTestFakeButtonEvent
			(gDisplay, 1, False, CurrentTime); break;

		case ButtonMid: XTestFakeButtonEvent
			(gDisplay, 2, False, CurrentTime); break;

		case ButtonRight: XTestFakeButtonEvent
			(gDisplay, 3, False, CurrentTime); break;

		default: break;
	}

	// Flush output buffer
	XSync (gDisplay, False);

#endif
#ifdef ROBOT_OS_MAC

	// Double-click timer
	static Timer elapsed;

	// Ignore extra buttons
	if (button == ButtonX1 ||
		button == ButtonX2) return;

	// Create an HID hardware event source
	CGEventSourceRef src = CGEventSourceCreate
		(kCGEventSourceStateHIDSystemState);

	// Retrieve the current mouse position
	CGEventRef pos = CGEventCreate (src);
	CGPoint p = CGEventGetLocation (pos);
	CFRelease (pos);

	// Create a release event
	CGEventRef evt = nullptr;
	switch (button)
	{
		case ButtonLeft:
			evt = CGEventCreateMouseEvent
			(src, kCGEventLeftMouseUp,
			p, kCGMouseButtonLeft); break;

		case ButtonMid:
			evt = CGEventCreateMouseEvent
			(src, kCGEventOtherMouseUp,
			p, kCGMouseButtonCenter); break;

		case ButtonRight:
			evt = CGEventCreateMouseEvent
			(src, kCGEventRightMouseUp,
			p, kCGMouseButtonRight); break;

		default: break;
	}

	// Detect double click version
	if (!elapsed.HasExpired (500))
	{
		// TODO: Use system double click
		// interval instead. See [NSEvent
		// doubleClickInterval] function.

		CGEventSetIntegerValueField (evt,
			kCGMouseEventClickState, 2);
		elapsed.Reset();
	}

	else
	{
		CGEventSetIntegerValueField (evt,
			kCGMouseEventClickState, 1);
		elapsed.Start();
	}

	// Post mouse event and release
	CGEventPost (kCGHIDEventTap, evt);
	CFRelease (evt); CFRelease (src);

#endif
#ifdef ROBOT_OS_WIN

	INPUT input = { 0 };
	// Prepare a mouse event
	input.type = INPUT_MOUSE;

	switch (button)
	{
		case ButtonLeft:
			input.mi.dwFlags = GetSystemMetrics (SM_SWAPBUTTON) ?
				MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_LEFTUP; break;

		case ButtonMid:
			input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP; break;

		case ButtonRight:
			input.mi.dwFlags = GetSystemMetrics (SM_SWAPBUTTON) ?
				MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP; break;

		case ButtonX1:
			input.mi.dwFlags = MOUSEEVENTF_XUP;
			input.mi.mouseData = XBUTTON1; break;

		case ButtonX2:
			input.mi.dwFlags = MOUSEEVENTF_XUP;
			input.mi.mouseData = XBUTTON2; break;
	}

	// Send the prepared mouse input event
	SendInput (1, &input, sizeof (INPUT));

#endif

	Timer::Sleep (AutoDelay);
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::ScrollH (int32 amount) const
{
#ifdef ROBOT_OS_LINUX

	// Check for XTest availability
	if (!IsXTestAvailable()) return;

	// Loop required number of times
	int32 repeat = abs (amount);
	int32 button = amount < 0 ? 6 : 7;
	for (int32 i = 0; i < repeat; ++i)
	{
		XTestFakeButtonEvent (gDisplay, button, True,  CurrentTime);
		XTestFakeButtonEvent (gDisplay, button, False, CurrentTime);
	}

	// Flush output buffer
	XSync (gDisplay, False);

#endif
#ifdef ROBOT_OS_MAC

	// Create an HID hardware event source
	CGEventSourceRef src = CGEventSourceCreate
		(kCGEventSourceStateHIDSystemState);

	// Create a new horizontal scroll event
	CGEventRef evt = CGEventCreateScrollWheelEvent
		(src, kCGScrollEventUnitPixel,
		2, 0, (int32) (amount * -120));

	// Post mouse event and release
	CGEventPost (kCGHIDEventTap, evt);
	CFRelease (evt); CFRelease (src);

#endif
#ifdef ROBOT_OS_WIN

	INPUT input = { 0 };
	// Prepare a mouse event
	input.type = INPUT_MOUSE;

	// Set new event as a horizontal scroll
	input.mi.dwFlags = MOUSEEVENTF_HWHEEL;
	input.mi.mouseData = amount * WHEEL_DELTA;

	// Send the prepared mouse input event
	SendInput (1, &input, sizeof (INPUT));

#endif

	Timer::Sleep (AutoDelay);
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::ScrollV (int32 amount) const
{
#ifdef ROBOT_OS_LINUX

	// Check for XTest availability
	if (!IsXTestAvailable()) return;

	// Loop required number of times
	int32 repeat = abs (amount);
	int32 button = amount < 0 ? 5 : 4;
	for (int32 i = 0; i < repeat; ++i)
	{
		XTestFakeButtonEvent (gDisplay, button, True,  CurrentTime);
		XTestFakeButtonEvent (gDisplay, button, False, CurrentTime);
	}

	// Flush output buffer
	XSync (gDisplay, False);

#endif
#ifdef ROBOT_OS_MAC

	// Create an HID hardware event source
	CGEventSourceRef src = CGEventSourceCreate
		(kCGEventSourceStateHIDSystemState);

	// Create a new vertical scroll event
	CGEventRef evt = CGEventCreateScrollWheelEvent
		(src, kCGScrollEventUnitPixel,
		2, (int32) (amount * 120), 0);

	// Post mouse event and release
	CGEventPost (kCGHIDEventTap, evt);
	CFRelease (evt); CFRelease (src);

#endif
#ifdef ROBOT_OS_WIN

	INPUT input = { 0 };
	// Prepare a mouse event
	input.type = INPUT_MOUSE;

	// Set new event as a vertical scroll
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	input.mi.mouseData = amount * WHEEL_DELTA;

	// Send the prepared mouse input event
	SendInput (1, &input, sizeof (INPUT));

#endif

	Timer::Sleep (AutoDelay);
}

////////////////////////////////////////////////////////////////////////////////

Point Mouse::GetPos (void)
{
#ifdef ROBOT_OS_LINUX

	Window root, child;
	uint32 mask;
	int32 rx, ry;
	int32 wx, wy;

	// Check whether XTest is available
	if (!IsXTestAvailable()) return 0;

	// Count the number of available screens
	int32 screens = XScreenCount (gDisplay);

	// Iterate screens until pos found
	for (int32 i = 0; i < screens; ++i)
	{
		if (XQueryPointer (gDisplay, XRootWindow
			(gDisplay, i), &root, &child, &rx, &ry,
			&wx, &wy, &mask)) return Point (rx, ry);
	}

	return 0;

#endif
#ifdef ROBOT_OS_MAC

	// Create an HID hardware event source
	CGEventSourceRef src = CGEventSourceCreate
		(kCGEventSourceStateHIDSystemState);

	// Retrieve the current mouse position
	CGEventRef pos = CGEventCreate (src);
	CGPoint p = CGEventGetLocation (pos);
	CFRelease (pos); CFRelease (src);

	// Create and return as a point value
	return Point (p.x, p.y);

#endif
#ifdef ROBOT_OS_WIN

	POINT p;
	// Return the current mouse cursor position
	return GetCursorPos (&p) ? Point (p.x, p.y) : 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::SetPos (const Point& point)
{
	SetPos (point.X, point.Y);
}

////////////////////////////////////////////////////////////////////////////////

void Mouse::SetPos (uint32 x, uint32 y)
{
#ifdef ROBOT_OS_LINUX

	// Check for XTest availability
	if (!IsXTestAvailable()) return;

	// Move default mouse pointer
	XWarpPointer (gDisplay, None,
		XDefaultRootWindow (gDisplay),
		0, 0, 0, 0, x, y);

	// Flush output buffer
	XSync (gDisplay, False);

#endif
#ifdef ROBOT_OS_MAC

	CGPoint position = CGPointMake (x, y);
	// Create an HID hardware event source
	CGEventSourceRef src = CGEventSourceCreate
		(kCGEventSourceStateHIDSystemState);

	CGEventRef evt = nullptr;
	if (GetState (ButtonLeft))
	{
		// Create a left button drag
		evt = CGEventCreateMouseEvent
			(src, kCGEventLeftMouseDragged,
			 position, kCGMouseButtonLeft);
	}

	else
	{
		if (GetState (ButtonRight))
		{
			// Create a right button drag
			evt = CGEventCreateMouseEvent
				(src, kCGEventRightMouseDragged,
				 position, kCGMouseButtonLeft);
		}

		else
		{
			// Create a mouse move event
			evt = CGEventCreateMouseEvent
				(src, kCGEventMouseMoved,
				 position, kCGMouseButtonLeft);
		}
	}

	// Post mouse event and release
	CGEventPost (kCGHIDEventTap, evt);
	CFRelease (evt); CFRelease (src);

#endif
#ifdef ROBOT_OS_WIN

	SetCursorPos (x, y);

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Mouse::GetState (Button button)
{
#ifdef ROBOT_OS_LINUX

	Window root, child;
	uint32 mask;
	int32 rx, ry;
	int32 wx, wy;

	// Ignore extra buttons
	if (button == ButtonX1 ||
		button == ButtonX2 ||
		!IsXTestAvailable()) return false;

	// Count the number of available screens
	int32 screens = XScreenCount (gDisplay);

	// Iterate screens until mask found
	for (int32 i = 0; i < screens; ++i)
	{
		if (XQueryPointer (gDisplay, XRootWindow
			(gDisplay, i), &root, &child, &rx, &ry,
			&wx, &wy, &mask)) switch (button)
			{
				case ButtonLeft : if ((mask & Button1Mask) >> 8) return true;
				case ButtonMid  : if ((mask & Button2Mask) >> 8) return true;
				case ButtonRight: if ((mask & Button3Mask) >> 8) return true;
				default: break;
			}
	}

#endif
#ifdef ROBOT_OS_MAC

	switch (button)
	{
		case ButtonLeft : return CGEventSourceButtonState (kCGEventSourceStateHIDSystemState, kCGMouseButtonLeft  );
		case ButtonMid  : return CGEventSourceButtonState (kCGEventSourceStateHIDSystemState, kCGMouseButtonCenter);
		case ButtonRight: return CGEventSourceButtonState (kCGEventSourceStateHIDSystemState, kCGMouseButtonRight );
		default: break;
	}

#endif
#ifdef ROBOT_OS_WIN

	switch (button)
	{
		case ButtonLeft : return GetAsyncKeyState (GetSystemMetrics (SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON) != 0;
		case ButtonMid  : return GetAsyncKeyState (VK_MBUTTON) != 0;
		case ButtonRight: return GetAsyncKeyState (GetSystemMetrics (SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON) != 0;
		case ButtonX1   : return GetAsyncKeyState (VK_XBUTTON1) != 0;
		case ButtonX2   : return GetAsyncKeyState (VK_XBUTTON2) != 0;
	}

#endif

	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Mouse::GetState (ButtonState& result)
{
	// Clear result
	result.clear();

#ifdef ROBOT_OS_LINUX

	Window root, child;
	uint32 mask;
	int32 rx, ry;
	int32 wx, wy;

	// Check whether XTest is available
	if (!IsXTestAvailable()) return false;

	// Count the number of available screens
	int32 screens = XScreenCount (gDisplay);

	// Iterate screens until mask found
	for (int32 i = 0; i < screens; ++i)
	{
		if (XQueryPointer (gDisplay, XRootWindow
			(gDisplay, i), &root, &child, &rx, &ry,
			&wx, &wy, &mask))
		{
			result[ButtonLeft ] = ((mask & Button1Mask) >> 8) != 0;
			result[ButtonMid  ] = ((mask & Button2Mask) >> 8) != 0;
			result[ButtonRight] = ((mask & Button3Mask) >> 8) != 0;
			result[ButtonX1   ] = false;
			result[ButtonX2   ] = false;
			return true;
		}
	}

	return false;

#endif
#ifdef ROBOT_OS_MAC

	result[ButtonLeft ] = CGEventSourceButtonState (kCGEventSourceStateHIDSystemState, kCGMouseButtonLeft  );
	result[ButtonMid  ] = CGEventSourceButtonState (kCGEventSourceStateHIDSystemState, kCGMouseButtonCenter);
	result[ButtonRight] = CGEventSourceButtonState (kCGEventSourceStateHIDSystemState, kCGMouseButtonRight );
	result[ButtonX1   ] = false;
	result[ButtonX2   ] = false;

#endif
#ifdef ROBOT_OS_WIN

	result[ButtonLeft ] = GetAsyncKeyState (GetSystemMetrics (SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON) != 0;
	result[ButtonMid  ] = GetAsyncKeyState (VK_MBUTTON) != 0;
	result[ButtonRight] = GetAsyncKeyState (GetSystemMetrics (SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON) != 0;
	result[ButtonX1   ] = GetAsyncKeyState (VK_XBUTTON1) != 0;
	result[ButtonX2   ] = GetAsyncKeyState (VK_XBUTTON2) != 0;

#endif

	return true;
}

ROBOT_NS_END
