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

#include "Window.h"
#include "Process.h"
using std::string;

#include <regex>
using std::regex;
using std::regex_match;

#ifdef ROBOT_OS_LINUX

	#include <X11/Xlib.h>
	#include <X11/Xatom.h>
	#include <cstring>

	// Open default display
	Display* _Robot_Display =
		XOpenDisplay (NULL);
	#define gDisplay _Robot_Display

	#ifndef X_HAVE_UTF8_STRING
		#error It appears that X_HAVE_UTF8_STRING is not defined - \
			   please verify that your version of XLib is supported
	#endif

#endif
#ifdef ROBOT_OS_MAC

	#include <dlfcn.h>
	#include <ApplicationServices/ApplicationServices.h>

	#ifdef MAC_OS_X_VERSION_10_11
		#define kAXValueCGPointType kAXValueTypeCGPoint
		#define kAXValueCGSizeType  kAXValueTypeCGSize
	#endif

#endif
#ifdef ROBOT_OS_WIN

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	using std::wstring;

#endif
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

#ifdef ROBOT_OS_LINUX

	//----------------------------------------------------------------------------//
	// Error Handling                                                             //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////

	typedef int (*XErrorHandler) (Display*, XErrorEvent*);

	////////////////////////////////////////////////////////////////////////////////

	static int XHandleError (Display* dp, XErrorEvent* e) { return 0; }

	////////////////////////////////////////////////////////////////////////////////

	class XDismissErrors
	{
	public:
		////////////////////////////////////////////////////////////////////////////////

		XDismissErrors (void)
		{
			// Save old handler and dismiss errors
			mOld = XSetErrorHandler (XHandleError);
		}

		////////////////////////////////////////////////////////////////////////////////

		~XDismissErrors (void)
		{
			// Flush output buffer
			XSync (gDisplay, False);

			// Reinstate old handler
			XSetErrorHandler (mOld);
		}

	private:
		XErrorHandler mOld;
	};



	//----------------------------------------------------------------------------//
	// Definitions                                                                //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////

	struct Hints
	{
		unsigned long Flags;
		unsigned long Funcs;
		unsigned long Decorations;
		  signed long Mode;
		unsigned long Stat;
	};

	////////////////////////////////////////////////////////////////////////////////

	static Atom WM_STATE	= None;
	static Atom WM_ABOVE	= None;
	static Atom WM_HIDDEN	= None;
	static Atom WM_HMAX		= None;
	static Atom WM_VMAX		= None;

	static Atom WM_DESKTOP	= None;
	static Atom WM_CURDESK	= None;

	static Atom WM_NAME		= None;
	static Atom WM_UTF8		= None;
	static Atom WM_PID		= None;
	static Atom WM_ACTIVE	= None;
	static Atom WM_HINTS	= None;
	static Atom WM_EXTENTS	= None;

	////////////////////////////////////////////////////////////////////////////////

	static void LoadAtoms (void)
	{
		WM_STATE   = XInternAtom (gDisplay, "_NET_WM_STATE",                True);
		WM_ABOVE   = XInternAtom (gDisplay, "_NET_WM_STATE_ABOVE",          True);
		WM_HIDDEN  = XInternAtom (gDisplay, "_NET_WM_STATE_HIDDEN",         True);
		WM_HMAX    = XInternAtom (gDisplay, "_NET_WM_STATE_MAXIMIZED_HORZ", True);
		WM_VMAX    = XInternAtom (gDisplay, "_NET_WM_STATE_MAXIMIZED_VERT", True);

		WM_DESKTOP = XInternAtom (gDisplay, "_NET_WM_DESKTOP",              True);
		WM_CURDESK = XInternAtom (gDisplay, "_NET_CURRENT_DESKTOP",         True);

		WM_NAME    = XInternAtom (gDisplay, "_NET_WM_NAME",                 True);
		WM_UTF8    = XInternAtom (gDisplay, "UTF8_STRING",                  True);
		WM_PID     = XInternAtom (gDisplay, "_NET_WM_PID",                  True);
		WM_ACTIVE  = XInternAtom (gDisplay, "_NET_ACTIVE_WINDOW",           True);
		WM_HINTS   = XInternAtom (gDisplay, "_MOTIF_WM_HINTS",              True);
		WM_EXTENTS = XInternAtom (gDisplay, "_NET_FRAME_EXTENTS",           True);
	}



	//----------------------------------------------------------------------------//
	// Functions                                                                  //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////

	static void* GetWindowProperty (::Window win, Atom atom, uint32* items = nullptr)
	{
		// Property variables
		Atom type; int format;
		unsigned long  nItems;
		unsigned long  bAfter;
		unsigned char* result = nullptr;

		// Check the atom
		if (atom != None)
		{
			// Retrieve and validate the specified property
			if (!XGetWindowProperty (gDisplay, win, atom, 0,
				BUFSIZ, False, AnyPropertyType, &type, &format,
				&nItems, &bAfter, &result) && result && nItems)
			{
				// Copy items result
				if (items != nullptr)
					*items = (uint32) nItems;

				return result;
			}
		}

		// Reset the items result if valid
		if (items != nullptr) *items = 0;

		// Free the result if it got allocated
		if (result != nullptr) XFree (result);
		return nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////

	#define STATE_TOPMOST  0
	#define STATE_MINIMIZE 1
	#define STATE_MAXIMIZE 2

	////////////////////////////////////////////////////////////////////////////////

	static bool GetState (::Window win, uint8 setting)
	{
		// Validate every atom that we want to use
		if (WM_STATE  != None && WM_HMAX != None &&
			WM_ABOVE  != None && WM_VMAX != None &&
			WM_HIDDEN != None)
		{
			Atom* atoms; uint32 nItems = 0;
			bool test1 = false, test2 = false;

			// Get the window state property
			atoms = (Atom*) GetWindowProperty
					(win, WM_STATE, &nItems);

			// Check result and iterate items
			if (atoms == nullptr) return false;
			for (uint32 i = 0; i < nItems; ++i)
			{
				switch (setting)
				{
					case STATE_TOPMOST:
						if (atoms[i] == WM_ABOVE)
							test1 = test2 = true;
						break;

					case STATE_MINIMIZE:
						if (atoms[i] == WM_HIDDEN)
							test1 = test2 = true;
						break;

					case STATE_MAXIMIZE:
						if (atoms[i] == WM_HMAX)
							test1 = true;
						if (atoms[i] == WM_VMAX)
							test2 = true;
						break;

					// Unknown setting
					default: break;
				}

				// We're finished iterating
				if (test1 && test2) break;
			}

			XFree (atoms);
			return test1 && test2;
		}

		return false;
	}

	////////////////////////////////////////////////////////////////////////////////

	static void SetState (::Window win, uint8 setting, bool state)
	{
		// Retrieve the screen number
		XWindowAttributes attr = { 0 };
		XGetWindowAttributes (gDisplay, win, &attr);
		int s = XScreenNumberOfScreen (attr.screen);

		// Perform minimize functions
		if (setting == STATE_MINIMIZE)
		{
			if (state) XIconifyWindow (gDisplay, win, s);
			else Window::SetActive (Window ((uintptr) win));
		}

		// Validate every atom that we want to use
		else if (WM_STATE != None && WM_HMAX != None &&
				 WM_ABOVE != None && WM_VMAX != None)
		{
			// Prepare an event
			XClientMessageEvent e = { 0 };
			e.window = win; e.format = 32;
			e.message_type = WM_STATE;
			e.display = gDisplay;
			e.type = ClientMessage;

			switch (setting)
			{
				case STATE_TOPMOST:
					e.data.l[0] = state ? 1 : 0;
					e.data.l[1] = WM_ABOVE; break;

				case STATE_MAXIMIZE:
					e.data.l[0] = state ? 1 : 0;
					e.data.l[1] = WM_HMAX;
					e.data.l[2] = WM_VMAX; break;

				// Unknown setting
				default: return;
			}

			// Send the message
			XSendEvent (gDisplay, XRootWindow (gDisplay, s), False,
				SubstructureNotifyMask | SubstructureRedirectMask,
				(XEvent*) &e);
		}
	}

	////////////////////////////////////////////////////////////////////////////////

	static Bounds GetFrame (::Window win)
	{
		Bounds frame;
		// Retrieve frame bounds
		if (WM_EXTENTS != None)
		{
			long* result; uint32 nItems = 0;
			// Get the window extents property
			result = (long*) GetWindowProperty
					 (win, WM_EXTENTS, &nItems);

			// Verify the results
			if (result != nullptr)
			{
				if (nItems == 4)
				{
					frame = Bounds
						((int32) result[0],  (int32) result[2],
						 (int32) result[0] + (int32) result[1],
						 (int32) result[2] + (int32) result[3]);
				}

				XFree (result);
			}
		}

		return frame;
	}

	////////////////////////////////////////////////////////////////////////////////

	static void SetDesktopForWindow (::Window win)
	{
		// Validate every atom that we want to use
		if (WM_DESKTOP != None && WM_CURDESK != None)
		{
			// Get desktop property
			long* desktop = (long*)
				GetWindowProperty (win, WM_DESKTOP);

			// Check result value
			if (desktop != nullptr)
			{
				// Retrieve the screen number
				XWindowAttributes attr = { 0 };
				XGetWindowAttributes (gDisplay, win, &attr);
				int s = XScreenNumberOfScreen (attr.screen);
				::Window root = XRootWindow (gDisplay, s);

				// Prepare an event
				XClientMessageEvent e = { 0 };
				e.window = root; e.format = 32;
				e.message_type = WM_CURDESK;
				e.display = gDisplay;
				e.type = ClientMessage;
				e.data.l[0] = *desktop;
				e.data.l[1] = CurrentTime;

				// Send the message
				XSendEvent (gDisplay,
					root, False, SubstructureNotifyMask |
					SubstructureRedirectMask, (XEvent*) &e);

				XFree (desktop);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////

	struct EnumWindowsData
	{
		regex		Title;	// Window title to look for
		bool		Empty;	// Whether title is empty
		int32		ProcID;	// Window PID to look for
		WindowList*	Result;	// Pointer to the results
	};

	////////////////////////////////////////////////////////////////////////////////

	static void EnumWindows (::Window win, EnumWindowsData* data)
	{
		// Check the window visibility
		Window window; XWindowAttributes attr = { 0 };
		XGetWindowAttributes (gDisplay, win, &attr);
		if (attr.map_state != IsViewable) goto skip;

		// Attempt to associate current window
		if (!window.SetHandle ((uintptr) win))
			goto skip;

		if (data->ProcID != 0 &&
			// Perform process ID filtering
			data->ProcID != window.GetPID())
			goto skip;

		if (data->Empty ||
			// Try and match current window using regex
			regex_match (window.GetTitle(), data->Title))
		{
			// Add current window to result
			data->Result->push_back (window);
		}

	skip:
		::Window root, parent;
		::Window* children;
		unsigned int count;

		// Retrieve the child list
		Status status = XQueryTree
			(gDisplay, win, &root,
			&parent, &children, &count);

		// Check the results of the query
		if (!status || children == nullptr)
			return;

		// Recursively iterate child list
		for (uint32 i = 0; i < count; ++i)
			EnumWindows (children[i], data);

		// Free children
		XFree (children);
	}

#endif
#ifdef ROBOT_OS_MAC

	////////////////////////////////////////////////////////////////////////////////

	static Boolean (*gAXIsProcessTrustedWithOptions) (CFDictionaryRef);

	////////////////////////////////////////////////////////////////////////////////

	static CFStringRef* gkAXTrustedCheckOptionPrompt;

	////////////////////////////////////////////////////////////////////////////////

	// WARNING: Undocumented API, use with caution as it could one day go away
	extern "C" AXError _AXUIElementGetWindow (AXUIElementRef, CGWindowID* out);

	////////////////////////////////////////////////////////////////////////////////

	static AXUIElementRef GetUIElement (CGWindowID win)
	{
		// Window PID
		int32 pid = 0;

		// Create array storing window
		CGWindowID window[1] = { win };
		CFArrayRef wlist = CFArrayCreate (nullptr,
				(const void**) window, 1, nullptr);

		// Get window info
		CFArrayRef info =
			CGWindowListCreateDescriptionFromArray (wlist);
		CFRelease (wlist);

		// Check whether the resulting array is populated
		if (info != nullptr && CFArrayGetCount (info) > 0)
		{
			// Retrieve description from info array
			CFDictionaryRef desc = (CFDictionaryRef)
					CFArrayGetValueAtIndex (info, 0);

			// Get window PID
			CFNumberRef data =
				(CFNumberRef)
				CFDictionaryGetValue (desc, kCGWindowOwnerPID);

			if (data != nullptr)
				CFNumberGetValue (data, kCFNumberIntType, &pid);

			// Return result
			CFRelease (info);
		}

		// Check if PID was retrieved
		if (pid <= 0) return nullptr;

		// Create an accessibility object using retrieved PID
		auto application = AXUIElementCreateApplication (pid);
		if (application == nullptr) return nullptr;

		CFArrayRef windows = nullptr;
		// Get all windows associated with the app
		AXUIElementCopyAttributeValues (application,
			kAXWindowsAttribute, 0, 1024, &windows);

		// Reference to resulting value
		AXUIElementRef result = nullptr;

		if (windows != nullptr)
		{
			auto count = CFArrayGetCount (windows);
			// Loop all windows in the process
			for (CFIndex i = 0; i < count; ++i)
			{
				// Get the element at the index
				auto element = (AXUIElementRef)
					CFArrayGetValueAtIndex (windows, i);

				CGWindowID temp = 0;
				// Use undocumented API to get WindowID
				_AXUIElementGetWindow (element, &temp);

				// Check results
				if (temp == win)
				{
					// Retain element
					CFRetain (element);
					 result = element;
					break;
				}
			}

			CFRelease (windows);
		}

		CFRelease (application);
		return result;
	}

#endif
#ifdef ROBOT_OS_WIN

	////////////////////////////////////////////////////////////////////////////////

	#ifdef UNICODE
		extern  string _UTF8Encode (const wstring& value);
		extern wstring _UTF8Decode (const  string& value);
	#else
		extern  string _UTF8Encode (const  string& value);
		extern  string _UTF8Decode (const  string& value);
	#endif

	////////////////////////////////////////////////////////////////////////////////

	struct EnumWindowsData
	{
		regex		Title;	// Window title to look for
		bool		Empty;	// Whether title is empty
		int32		ProcID;	// Window PID to look for
		WindowList*	Result;	// Pointer to the results
	};

	////////////////////////////////////////////////////////////////////////////////

	static BOOL CALLBACK EnumWindowsProc (HWND hwnd, LPARAM lParam)
	{
		// Verify window visibility
		if (!IsWindowVisible (hwnd))
			return TRUE;

		// Retrieve lParam as EnumWindowsData
		auto data = (EnumWindowsData*) lParam;

		Window window;
		// Attempt to associate current window
		if (!window.SetHandle ((uintptr) hwnd))
			return TRUE;

		if (data->ProcID != 0 &&
			// Perform process ID filtering
			data->ProcID != window.GetPID())
			return TRUE;

		if (data->Empty ||
			// Try and match current window using regex
			regex_match (window.GetTitle(), data->Title))
		{
			// Add current window to result
			data->Result->push_back (window);
		}

		return TRUE;
	}

#endif



//----------------------------------------------------------------------------//
// Classes                                                             Window //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

struct Window::Data
{
#ifdef ROBOT_OS_LINUX

	::Window		XWin;		// Handle to an X11 window

#endif
#ifdef ROBOT_OS_MAC

	CGWindowID		CgID;		// Handle to a CGWindowID
	AXUIElementRef	AxID;		// Handle to a AXUIElementRef

#endif
#ifdef ROBOT_OS_WIN

	HWND			HWnd;		// Handle to a window HWND

#endif
};



//----------------------------------------------------------------------------//
// Constructors                                                        Window //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Window::Window (uintptr handle) : mData (new Window::Data(), [](Window::Data* data)
{
#ifdef ROBOT_OS_MAC

	// Release the AX element
	if (data->AxID != nullptr)
		CFRelease (data->AxID);

#endif

	// Free data
	delete data;
})
{
#ifdef ROBOT_OS_LINUX

	// If atoms loaded
	if (WM_PID == None)
	{
		// Load all necessary atom properties
		if (gDisplay != nullptr) LoadAtoms();
	}

	mData->XWin = 0;

#endif
#ifdef ROBOT_OS_MAC

	mData->CgID = 0;
	mData->AxID = 0;

#endif
#ifdef ROBOT_OS_WIN

	mData->HWnd = 0;

#endif

	SetHandle (handle);
}



//----------------------------------------------------------------------------//
// Functions                                                           Window //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Window::IsValid (void) const
{
#ifdef ROBOT_OS_LINUX

	if (mData->XWin == 0)
		return false;

	// Check for a valid X-Window display
	if (gDisplay == nullptr) return false;

	// Ignore X errors
	XDismissErrors xe;

	// Get the window PID property
	void* result = GetWindowProperty
			  (mData->XWin, WM_PID);
	if (result == nullptr) return false;

	// Free result and return true
	XFree (result); return true;

#endif
#ifdef ROBOT_OS_MAC

	if (mData->CgID == 0 ||
		mData->AxID == 0)
		return false;

	CFTypeRef r = nullptr;

	// Attempt to get the window role
	if (AXUIElementCopyAttributeValue
		(mData->AxID, kAXRoleAttribute,
		&r) == kAXErrorSuccess && r)
		{ CFRelease (r); return true; }

	return false;

#endif
#ifdef ROBOT_OS_WIN

	if (mData->HWnd == 0)
		return false;

	return IsWindow (mData->HWnd) != 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::Close (void)
{
	// Check window validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;

	// Close the window
	XDestroyWindow (gDisplay, mData->XWin);

#endif
#ifdef ROBOT_OS_MAC

	AXUIElementRef b = nullptr;

	// Retrieve the close button of this window
	if (AXUIElementCopyAttributeValue (mData->AxID,
		kAXCloseButtonAttribute, (CFTypeRef*) &b)
		== kAXErrorSuccess && b != nullptr)
	{
		// Simulate button press on the close button
		AXUIElementPerformAction (b, kAXPressAction);
		CFRelease (b);
	}

#endif
#ifdef ROBOT_OS_WIN

	PostMessage (mData->HWnd, WM_CLOSE, 0, 0);

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Window::IsTopMost (void) const
{
	// Check the window validity
	if (!IsValid()) return false;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;
	return GetState (mData->XWin, STATE_TOPMOST);

#endif
#ifdef ROBOT_OS_MAC

	return false; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_WIN

	return (GetWindowLongPtr (mData->HWnd,
		GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Window::IsBorderless (void) const
{
	// Check the window validity
	if (!IsValid()) return false;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;

	// Get the window hints property
	void* result = GetWindowProperty
			(mData->XWin, WM_HINTS);

	// Check the result and convert it
	if (result == nullptr) return false;
	auto decorations = reinterpret_cast
		<Hints*> (result)->Decorations;
	XFree (result); return decorations == 0;

#endif
#ifdef ROBOT_OS_MAC

	return false; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_WIN

	// CAUTION: The implementation  of this function
	// is a bit dodgy and may not always produce the
	// correct results. Run several tests beforehand
	// to see if this function is right for you.

	return (GetWindowLongPtr (mData->HWnd,
		GWL_STYLE) & WS_TILEDWINDOW) == 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Window::IsMinimized (void) const
{
	// Check the window validity
	if (!IsValid()) return false;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;
	return GetState (mData->XWin, STATE_MINIMIZE);

#endif
#ifdef ROBOT_OS_MAC

	CFBooleanRef data = nullptr;

	// Determine whether the window is minimized
	if (AXUIElementCopyAttributeValue (mData->AxID,
		kAXMinimizedAttribute, (CFTypeRef*) &data)
		== kAXErrorSuccess && data != nullptr)
	{
		// Convert resulting data into a bool
		bool result = CFBooleanGetValue (data);
		CFRelease (data); return result;
	}

	return false;

#endif
#ifdef ROBOT_OS_WIN

	return (GetWindowLongPtr (mData->HWnd,
			GWL_STYLE) & WS_MINIMIZE) != 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Window::IsMaximized (void) const
{
	// Check the window validity
	if (!IsValid()) return false;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;
	return GetState (mData->XWin, STATE_MAXIMIZE);

#endif
#ifdef ROBOT_OS_MAC

	return false; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_WIN

	return (GetWindowLongPtr (mData->HWnd,
			GWL_STYLE) & WS_MAXIMIZE) != 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetTopMost (bool state)
{
	// Check window validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;
	SetState (mData->XWin, STATE_TOPMOST, state);

#endif
#ifdef ROBOT_OS_MAC

	// WARNING: Unavailable

#endif
#ifdef ROBOT_OS_WIN

	SetWindowPos (mData->HWnd, state ?
		HWND_TOPMOST : HWND_NOTOPMOST, 0,
		0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetBorderless (bool state)
{
	// Check window validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;

	// Check atom value
	if (WM_HINTS != None)
	{
		// Create hints
		Hints hints = { 0 };
		hints.Flags = 2;
		hints.Decorations =
			state ? 0 : 1;

		// Set hints property with our version
		XChangeProperty (gDisplay, mData->XWin,
			WM_HINTS, WM_HINTS, 32, PropModeReplace,
			(unsigned char*) &hints, 5);
	}

#endif
#ifdef ROBOT_OS_MAC

	// WARNING: Unavailable

#endif
#ifdef ROBOT_OS_WIN

	// CAUTION: The implementation  of this function
	// is a bit dodgy and may not always produce the
	// correct results. Run several tests beforehand
	// to see if this function is right for you.

	LONG_PTR style = GetWindowLongPtr (mData->HWnd, GWL_STYLE);

	if (state)
		SetWindowLongPtr (mData->HWnd, GWL_STYLE, style & ~WS_TILEDWINDOW);
	else
		SetWindowLongPtr (mData->HWnd, GWL_STYLE, style |  WS_TILEDWINDOW);

	// Request recalculation of new window decorations
	SetWindowPos (mData->HWnd, nullptr, 0, 0, 0, 0,
		SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE |
		SWP_NOSIZE | SWP_FRAMECHANGED | SWP_NOACTIVATE);

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetMinimized (bool state)
{
	// Check window validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;
	SetState (mData->XWin, STATE_MINIMIZE, state);

#endif
#ifdef ROBOT_OS_MAC

	AXUIElementSetAttributeValue (mData->AxID,
		kAXMinimizedAttribute, state ?
		kCFBooleanTrue : kCFBooleanFalse);

#endif
#ifdef ROBOT_OS_WIN

	if (state)
		ShowWindow (mData->HWnd, SW_MINIMIZE);

	else if (IsMinimized())
		ShowWindow (mData->HWnd, SW_RESTORE);

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetMaximized (bool state)
{
	// Check window validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;
	SetState (mData->XWin, STATE_MINIMIZE, false);
	SetState (mData->XWin, STATE_MAXIMIZE, state);

#endif
#ifdef ROBOT_OS_MAC

	// WARNING: Unavailable

#endif
#ifdef ROBOT_OS_WIN

	if (IsMinimized())
		ShowWindow (mData->HWnd, SW_RESTORE);

	if (state)
		ShowWindow (mData->HWnd, SW_MAXIMIZE);

	else if (IsMaximized())
		ShowWindow (mData->HWnd, SW_RESTORE);

#endif
}

////////////////////////////////////////////////////////////////////////////////

Process Window::GetProcess (void) const
{
	return Process (GetPID());
}

////////////////////////////////////////////////////////////////////////////////

int32 Window::GetPID (void) const
{
	// Check window validity
	if (!IsValid()) return 0;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;

	// Get the window PID
	long* result = (long*)
		GetWindowProperty
		(mData->XWin, WM_PID);

	// Check result and convert it
	if (result == nullptr) return 0;
	auto pid = (int32) *result;
	XFree (result); return pid;

#endif
#ifdef ROBOT_OS_MAC

	pid_t pid = 0;
	// Attempt to retrieve the window pid
	if (AXUIElementGetPid (mData->AxID, &pid)
			== kAXErrorSuccess) return pid;

	return 0;

#endif
#ifdef ROBOT_OS_WIN

	DWORD  id = 0;
	GetWindowThreadProcessId (mData->HWnd, &id);
	return id;

#endif
}

////////////////////////////////////////////////////////////////////////////////

uintptr Window::GetHandle (void) const
{
#ifdef ROBOT_OS_LINUX

	return (uintptr) mData->XWin;

#endif
#ifdef ROBOT_OS_MAC

	return (uintptr) mData->CgID;

#endif
#ifdef ROBOT_OS_WIN

	return (uintptr) mData->HWnd;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Window::SetHandle (uintptr handle)
{
#ifdef ROBOT_OS_LINUX

	mData->XWin = (::Window) handle;

	if (handle == 0)
		return true;

	if (IsValid())
		return true;

	mData->XWin = 0;
	return false;

#endif
#ifdef ROBOT_OS_MAC

	// Release the AX element
	if (mData->AxID != nullptr)
		CFRelease (mData->AxID);

	// Reset both values
	mData->CgID = 0;
	mData->AxID = 0;

	if (handle == 0)
		return true;

	// Retrieve the window element
	auto cgID = (CGWindowID) handle;
	auto axID = GetUIElement (cgID);

	if (axID != nullptr)
	{
		mData->CgID = cgID;
		mData->AxID = axID;
		return true;
	}

	return false;

#endif
#ifdef ROBOT_OS_WIN

	mData->HWnd = (HWND) handle;

	if (handle == 0)
		return true;

	if (IsValid())
		return true;

	mData->HWnd = 0;
	return false;

#endif
}

////////////////////////////////////////////////////////////////////////////////

uintptr Window::GetHandleAx (void) const
{
#ifdef ROBOT_OS_LINUX

	return 0;

#endif
#ifdef ROBOT_OS_MAC

	return (uintptr)
		mData->AxID;

#endif
#ifdef ROBOT_OS_WIN

	return 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

string Window::GetTitle (void) const
{
	// Check if the window is valid
	if (!IsValid()) return string();

#ifdef ROBOT_OS_LINUX

	void* result;
	// Ignore X errors
	XDismissErrors xe;

	// Get window title (UTF-8)
	result = GetWindowProperty
		(mData->XWin, WM_NAME);

	// Check result value
	if (result != nullptr)
	{
		// Convert result to a string
		string name ((char*) result);
		XFree (result);

		if (!name.empty())
			return name;
	}

	// Get window title (ASCII)
	result = GetWindowProperty
		(mData->XWin, XA_WM_NAME);

	// Check result value
	if (result != nullptr)
	{
		// Convert result to a string
		string name ((char*) result);
		XFree (result); return name;
	}

	return string();

#endif
#ifdef ROBOT_OS_MAC

	CFStringRef data = nullptr;

	// Determine the current title of the window
	if (AXUIElementCopyAttributeValue (mData->AxID,
		kAXTitleAttribute, (CFTypeRef*) &data)
		== kAXErrorSuccess && data != nullptr)
	{
		char conv[512];
		// Convert result to a C-String
		CFStringGetCString (data, conv,
			512, kCFStringEncodingUTF8);
		CFRelease (data);
		return string (conv);
	}

	return string();

#endif
#ifdef ROBOT_OS_WIN

	TCHAR name[512];
	return GetWindowText
		(mData->HWnd, name, 512) > 0 ?
		_UTF8Encode (name) : string();

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetTitle (const char* title)
{
	// Check window & title validity
	if (!title || !IsValid()) return;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;

	// Convert title into non-const
	uint8* converted = (uint8*) title;
	int32  conLength =  strlen (title);

	// Change ASCII window name
	XChangeProperty (gDisplay,
		mData->XWin, XA_WM_NAME, XA_STRING, 8,
		PropModeReplace, converted, conLength);

	XChangeProperty (gDisplay,
		mData->XWin, XA_WM_ICON_NAME, XA_STRING,
		8, PropModeReplace, converted, conLength);

	// Check every atom that we want to use
	if (WM_NAME != None && WM_UTF8 != None)
	{
		// Change UTF-8 window name
		XChangeProperty (gDisplay,
			mData->XWin, WM_NAME, WM_UTF8,
			8, PropModeReplace, converted,
			conLength);
	}

#endif
#ifdef ROBOT_OS_MAC

	// Try to convert title into string reference
	CFStringRef name = CFStringCreateWithCString
		(nullptr, title, kCFStringEncodingUTF8);

	if (name != nullptr)
	{
		// Set the current title of the window
		AXUIElementSetAttributeValue (mData->AxID,
			kAXTitleAttribute, (CFTypeRef) name);

		CFRelease (name);
	}

#endif
#ifdef ROBOT_OS_WIN

	SetWindowText (mData->HWnd, _UTF8Decode (title).data());

#endif
}

////////////////////////////////////////////////////////////////////////////////

Bounds Window::GetBounds (void) const
{
	// Check if the window is valid
	if (!IsValid()) return Bounds();

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;

	Bounds client = GetClient();
	Bounds frame  = GetFrame (mData->XWin);
	return Bounds (client.X - frame.X,
				   client.Y - frame.Y,
				   client.W + frame.W,
				   client.H + frame.H);

#endif
#ifdef ROBOT_OS_MAC

	Bounds bounds;
	AXValueRef axp = nullptr;
	AXValueRef axs = nullptr;

	// Determine the current point of the window
	if (AXUIElementCopyAttributeValue (mData->AxID,
		kAXPositionAttribute, (CFTypeRef*) &axp)
		!= kAXErrorSuccess || axp == nullptr)
		goto exit;

	// Determine the current size of the window
	if (AXUIElementCopyAttributeValue (mData->AxID,
		kAXSizeAttribute, (CFTypeRef*) &axs)
		!= kAXErrorSuccess || axs == nullptr)
		goto exit;

	CGPoint p; CGSize s;
	// Attempt to convert both values into atomic types
	if (AXValueGetValue (axp, kAXValueCGPointType, &p) &&
		AXValueGetValue (axs, kAXValueCGSizeType,  &s))
		bounds = Bounds (p.x, p.y, s.width, s.height);

exit:
	if (axp != nullptr) CFRelease (axp);
	if (axs != nullptr) CFRelease (axs);
	return bounds;

#endif
#ifdef ROBOT_OS_WIN

	RECT rect = { 0 };
	GetWindowRect (mData->HWnd, &rect);

	return Bounds
		(rect.left, rect.top,
		 rect.right  - rect.left,
		 rect.bottom - rect.top);

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetBounds (const Bounds& bounds)
{
	SetBounds (bounds.X, bounds.Y,
			   bounds.W, bounds.H);
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetBounds (int32 x,
	  int32 y, int32 w, int32 h)
{
	// Check window validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;
	Bounds frame = GetFrame (mData->XWin);

	// Prepare win changes
	XWindowChanges changes;
	changes.x = x; changes.y = y;
	changes.width  = w - frame.W;
	changes.height = h - frame.H;

	// Apply new position and size of window
	XConfigureWindow (gDisplay, mData->XWin,
		CWX | CWY | CWWidth | CWHeight, &changes);

#endif
#ifdef ROBOT_OS_MAC

	// Convert bounds to CG values
	CGPoint p = CGPointMake (x, y);
	CGSize  s = CGSizeMake  (w, h);

	// Convert bounds to AX values
	AXValueRef axp = AXValueCreate (kAXValueCGPointType, (const void*) &p);
	AXValueRef axs = AXValueCreate (kAXValueCGSizeType,  (const void*) &s);

	if (axp != nullptr && axs != nullptr)
	{
		// Set the current bounds of the window
		AXUIElementSetAttributeValue (mData->AxID, kAXPositionAttribute, axp);
		AXUIElementSetAttributeValue (mData->AxID, kAXSizeAttribute,     axs);
	}

	if (axp != nullptr) CFRelease (axp);
	if (axs != nullptr) CFRelease (axs);

#endif
#ifdef ROBOT_OS_WIN

	SetWindowPos (mData->HWnd, nullptr, x, y,
		w, h, SWP_NOZORDER | SWP_NOOWNERZORDER);

#endif
}

////////////////////////////////////////////////////////////////////////////////

Bounds Window::GetClient (void) const
{
	// Check if the window is valid
	if (!IsValid()) return Bounds();

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;

	// Property variables
	::Window root, parent;
	::Window* children;
	unsigned int count;
	int32 x = 0, y = 0;

	// Check if the window is the root
	XQueryTree (gDisplay, mData->XWin,
		&root, &parent, &children, &count);
	if (children) XFree (children);

	// Retrieve window attributes
	XWindowAttributes attr = { 0 };
	XGetWindowAttributes (gDisplay,
				mData->XWin, &attr);

	// Coordinates must be translated
	if (parent != attr.root) XTranslateCoordinates
		(gDisplay, mData->XWin, attr.root, attr.x,
		 attr.y, &x, &y, &parent);

	// Coordinates can be left alone
	else { x = attr.x; y = attr.y; }

	// Return resulting window bounds
	return Bounds (x, y, attr.width, attr.height);

#endif
#ifdef ROBOT_OS_MAC

	return GetBounds();

#endif
#ifdef ROBOT_OS_WIN

	RECT rect = { 0 };
	GetClientRect (mData->HWnd, &rect);

	POINT point;
	point.x = rect.left;
	point.y = rect.top;

	// Convert the client point to screen
	ClientToScreen (mData->HWnd, &point);
	return Bounds (point.x, point.y,
			rect.right  - rect.left,
			rect.bottom - rect.top);

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetClient (const Bounds& bounds)
{
	SetClient (bounds.X, bounds.Y,
			   bounds.W, bounds.H);
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetClient (int32 x,
	  int32 y, int32 w, int32 h)
{
	// Check window validity
	if (!IsValid()) return;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;
	Bounds frame = GetFrame (mData->XWin);

	// Prepare win changes
	XWindowChanges changes;
	changes.x = x - frame.X;
	changes.y = y - frame.Y;
	changes.width  = w;
	changes.height = h;

	// Apply new position and size of window
	XConfigureWindow (gDisplay, mData->XWin,
		CWX | CWY | CWWidth | CWHeight, &changes);

#endif
#ifdef ROBOT_OS_MAC

	SetBounds (x, y, w, h);

#endif
#ifdef ROBOT_OS_WIN

	RECT rect;
	rect.left = x; rect.right  = x + w;
	rect.top  = y; rect.bottom = y + h;

	DWORD st = GetWindowLong (mData->HWnd, GWL_STYLE  );
	DWORD ex = GetWindowLong (mData->HWnd, GWL_EXSTYLE);

	// Compute bounds with decorations
	if (!AdjustWindowRectEx (&rect, st,
		GetMenu (mData->HWnd) != nullptr, ex))
		return;

	SetWindowPos (mData->HWnd, nullptr,
		rect.left, rect.top, rect.right -
		rect.left, rect.bottom - rect.top,
		SWP_NOZORDER | SWP_NOOWNERZORDER);

#endif
}

////////////////////////////////////////////////////////////////////////////////

Point Window::MapToClient (const Point& point) const
{
	// Check if the window is valid
	if (!IsValid()) return Point();
	return point - GetClient().GetPoint();
}

////////////////////////////////////////////////////////////////////////////////

Point Window::MapToScreen (const Point& point) const
{
	// Check if the window is valid
	if (!IsValid()) return Point();
	return point + GetClient().GetPoint();
}

////////////////////////////////////////////////////////////////////////////////

Bounds Window::MapToClient (const Bounds& bounds) const
{
	// Check if the window is valid
	if (!IsValid()) return Bounds();
	return Bounds (bounds.GetPoint() - GetClient().GetPoint(), bounds.GetSize());
}

////////////////////////////////////////////////////////////////////////////////

Bounds Window::MapToScreen (const Bounds& bounds) const
{
	// Check if the window is valid
	if (!IsValid()) return Bounds();
	return Bounds (bounds.GetPoint() + GetClient().GetPoint(), bounds.GetSize());
}

////////////////////////////////////////////////////////////////////////////////

WindowList Window::GetList (const char* title)
{
	return GetList (title, 0);
}

////////////////////////////////////////////////////////////////////////////////

Window Window::GetActive (void)
{
#ifdef ROBOT_OS_LINUX

	Window result;
	// Check X-Window display
	if (WM_ACTIVE == None ||
		gDisplay == nullptr)
		return result;

	// Ignore X errors
	XDismissErrors xe;

	// Get the current active window
	void* active = GetWindowProperty
		(XDefaultRootWindow (gDisplay),
		WM_ACTIVE);

	// Check result value
	if (active != nullptr)
	{
		// Extract window from the result
		long window = *((long*) active);
		XFree (active); if (window != 0)
		{
			// Set and return the foreground window
			result.mData->XWin = (::Window) window;
			return result;
		}
	}

	// Use input focus instead
	::Window window = None;
	int revert = RevertToNone;
	XGetInputFocus (gDisplay,
			&window, &revert);

	// Return foreground window
	result.mData->XWin = window;
	return result;

#endif
#ifdef ROBOT_OS_MAC

	Window result;
	// Ignore deprecated warnings
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wdeprecated-declarations"

	// NOTE: Until Apple actually removes
	// these functions, there's no real
	// reason to switch to the NS* flavor

	ProcessSerialNumber psn; pid_t pid;
	// Attempt to retrieve the front process
	if (GetFrontProcess (&psn      ) != 0 ||
		GetProcessPID   (&psn, &pid) != 0)
		return result;

	#pragma clang diagnostic pop

	// Create accessibility object using focused PID
	auto focused = AXUIElementCreateApplication (pid);
	if (focused == nullptr) return result; // Verify

	AXUIElementRef element;
	// Retrieve the currently focused window
	if (AXUIElementCopyAttributeValue (focused,
		kAXFocusedWindowAttribute, (CFTypeRef*)
		&element) == kAXErrorSuccess && element)
	{
		CGWindowID win = 0;
		// Use undocumented API to get WID
		if (_AXUIElementGetWindow (element,
			&win) == kAXErrorSuccess && win)
		{
			// Manually set internals
			result.mData->CgID = win;
			result.mData->AxID = element;
		}

		// Something went wrong
		else CFRelease (element);
	}

	CFRelease (focused);
	return result;

#endif
#ifdef ROBOT_OS_WIN

	// Attempt to get the foreground
	// window multiple times in case
	// the window has not been fully
	// activated yet.

	uint8 times = 0;
	while (++times < 20)
	{
		HWND handle;
		handle = GetForegroundWindow();
		if (handle != nullptr) return
			Window ((uintptr) handle);
		Sleep (20);
	}

	return Window();

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Window::SetActive (const Window& window)
{
	// Check if the window is valid
	if (!window.IsValid()) return;

#ifdef ROBOT_OS_LINUX

	// Ignore X errors
	XDismissErrors xe;

	// Go to the specified window's desktop
	SetDesktopForWindow (window.mData->XWin);

	// Check the atom value
	if (WM_ACTIVE != None)
	{
		// Retrieve the screen number
		XWindowAttributes attr = { 0 };
		XGetWindowAttributes (gDisplay,
			window.mData->XWin, &attr);
		int s = XScreenNumberOfScreen (attr.screen);

		// Prepare an event
		XClientMessageEvent e = { 0 };
		e.window = window.mData->XWin;
		e.format = 32;
		e.message_type = WM_ACTIVE;
		e.display = gDisplay;
		e.type = ClientMessage;
		e.data.l[0] = 2;
		e.data.l[1] = CurrentTime;

		// Send the message
		XSendEvent (gDisplay, XRootWindow (gDisplay, s), False,
			SubstructureNotifyMask | SubstructureRedirectMask,
			(XEvent*) &e);
	}

	else
	{
		// Attempt to raise the specified window
		XRaiseWindow (gDisplay, window.mData->XWin);

		// Set the specified window's input focus
		XSetInputFocus (gDisplay, window.mData->XWin,
						RevertToParent, CurrentTime);
	}

#endif
#ifdef ROBOT_OS_MAC

	// Attempt to raise the specified window object
	if (AXUIElementPerformAction (window.mData->AxID,
				  kAXRaiseAction) == kAXErrorSuccess)
	{
		pid_t pid = 0;
		// Attempt to retrieve the PID of the window
		if (AXUIElementGetPid (window.mData->AxID, &pid)
					!= kAXErrorSuccess || !pid) return;

		// Ignore deprecated warnings
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wdeprecated-declarations"

		// NOTE: Until Apple actually removes
		// these functions, there's no real
		// reason to switch to the NS* flavor

		ProcessSerialNumber psn;
		// Attempt to retrieve the process psn
		if (GetProcessForPID (pid, &psn) == 0)
		{
			// Gracefully activate process
			SetFrontProcessWithOptions (&psn,
			kSetFrontProcessFrontWindowOnly);
		}

		#pragma clang diagnostic pop
	}

#endif
#ifdef ROBOT_OS_WIN

	if (window.IsMinimized())
		ShowWindow (window.mData->HWnd, SW_RESTORE);

	SetForegroundWindow (window.mData->HWnd);

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Window::IsAxEnabled (bool options)
{
#ifdef ROBOT_OS_LINUX

	return true;

#endif
#ifdef ROBOT_OS_MAC

	// Statically load all required functions one time
	static dispatch_once_t once; dispatch_once (&once,
	^{
		// Open the framework
		void* handle = dlopen
			("/System/Library/Frameworks/Application"
			 "Services.framework/ApplicationServices", RTLD_LAZY);

		// Validate the handle
		if (handle != nullptr)
		{
			*(void**) (&gAXIsProcessTrustedWithOptions) =
				dlsym (handle, "AXIsProcessTrustedWithOptions");

			gkAXTrustedCheckOptionPrompt = (CFStringRef*)
				dlsym (handle, "kAXTrustedCheckOptionPrompt");
		}
	});

	// Check for new OSX 10.9 function
	if (gAXIsProcessTrustedWithOptions)
	{
		// Check whether to show prompt
		auto displayPrompt = options ?
			kCFBooleanTrue : kCFBooleanFalse;

		// Convert display prompt value into a dictionary
		const void* k[] = { *gkAXTrustedCheckOptionPrompt };
		const void* v[] = { displayPrompt };
		CFDictionaryRef o = CFDictionaryCreate
			(nullptr, k, v, 1, nullptr, nullptr);

		// Determine whether the process is actually trusted
		bool result = (*gAXIsProcessTrustedWithOptions) (o);

		// Free memory
		CFRelease (o);
		return result;
	}

	else
	{
		// Ignore deprecated warnings
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wdeprecated-declarations"

		// Check whether we have accessibility access
		return AXAPIEnabled() || AXIsProcessTrusted();
		#pragma clang diagnostic pop
	}

#endif
#ifdef ROBOT_OS_WIN

	return true;

#endif
}

////////////////////////////////////////////////////////////////////////////////

WindowList Window::GetList (const char* title, int32 pid)
{
	WindowList result;
	// Check if the title is empty
	bool empty = title == nullptr;
	regex regexp; if (!empty) {
		// Attempt to use a case-insensitive regex
		try { regexp = regex (title, regex::icase); }
		catch (...) { return result; }
	}

#ifdef ROBOT_OS_LINUX

	// Check if X-Window display is opened
	if (gDisplay == nullptr) return result;

	// Ignore X errors
	XDismissErrors xe;

	// Prepare enum data
	EnumWindowsData data;
	data.Title  = regexp;
	data.Empty  = empty;
	data.ProcID = pid;
	data.Result = &result;

	// Enumerate windows
	EnumWindows (XDefaultRootWindow (gDisplay), &data);
	return result;

#endif
#ifdef ROBOT_OS_MAC

	// Get windows for specified PID
	auto processPID = [&] (int32 pid)
	{
		// Create accessibility object using the process PID
		auto application = AXUIElementCreateApplication (pid);
		if (application == nullptr) return; // Verify result

		CFArrayRef windows = nullptr;
		// Get all windows associated with the app
		AXUIElementCopyAttributeValues (application,
			kAXWindowsAttribute, 0, 1024, &windows);

		if (windows != nullptr)
		{
			auto count = CFArrayGetCount (windows);
			// Loop all windows in the process
			for (CFIndex i = 0; i < count; ++i)
			{
				// Get the element at the index
				auto element = (AXUIElementRef)
					CFArrayGetValueAtIndex (windows, i);

				CGWindowID win = 0; Window window;
				// Use undocumented API to get WindowID
				if (_AXUIElementGetWindow (element, &win)
					!= kAXErrorSuccess || !win) continue;

				// Manually set internals
				window.mData->CgID = win;
				window.mData->AxID = element;
				CFRetain (element);

				// Attempt to associate and match the current window
				if (empty || regex_match (window.GetTitle(), regexp))
				{
					// Append window to result
					result.push_back (window);
				}
			}

			CFRelease (windows);
		}

		CFRelease (application);
	};

	if (pid == 0)
	{
		// Retrieve list of current processes
		ProcessList list = Process::GetList();
		for (uintptr i = 0; i < list.size(); ++i)
			processPID (list[i].GetPID());
	}

	else
	{
		// Assumed valid
		processPID (pid);
	}

	return result;

#endif
#ifdef ROBOT_OS_WIN

	// Prepare enum data
	EnumWindowsData data;
	data.Title  = regexp;
	data.Empty  = empty;
	data.ProcID = pid;
	data.Result = &result;

	// Enumerate windows
	EnumWindows (EnumWindowsProc, (LPARAM) &data);
	return result;

#endif
}



//----------------------------------------------------------------------------//
// Operators                                                           Window //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Window::operator == (const Window& window) const
{
	return GetHandle() == window.GetHandle();
}

////////////////////////////////////////////////////////////////////////////////

bool Window::operator != (const Window& window) const
{
	return GetHandle() != window.GetHandle();
}

////////////////////////////////////////////////////////////////////////////////

bool Window::operator == (uintptr handle) const
{
	return GetHandle() == handle;
}

////////////////////////////////////////////////////////////////////////////////

bool Window::operator != (uintptr handle) const
{
	return GetHandle() != handle;
}

ROBOT_NS_END
