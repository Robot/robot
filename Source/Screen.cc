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

#include "Screen.h"
#ifdef ROBOT_OS_LINUX

	#include <X11/Xlib.h>
	#include <X11/Xatom.h>
	#include <X11/Xutil.h>
	#include <X11/extensions/Xinerama.h>

	// Reference default display
	 extern Display* _Robot_Display;
	#define gDisplay _Robot_Display

#endif
#ifdef ROBOT_OS_MAC

	#include <AppKit/AppKit.h>

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
	// Functions                                                                  //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////

	static bool IsXineramaAvailable (void)
	{
		// Initialize only one time
		static int8 available = -1;

		// If not checked yet
		if (available == -1)
		{
			available = 0;
			// Check for a valid X-Window display
			if (gDisplay == nullptr) return false;

			// Check for Xinerama extension
			int32 major, minor, evt, error;
			if (!XQueryExtension (gDisplay,
				"XINERAMA", &major, &evt, &error))
				return false;

			// Check the Xinerama version value
			if (!XineramaQueryVersion (gDisplay,
				&major, &minor)) return false;

			available = 1;
		}

		return available == 1;
	}

#endif
#ifdef ROBOT_OS_WIN

	////////////////////////////////////////////////////////////////////////////////

	#ifndef DWM_EC_DISABLECOMPOSITION
	#define DWM_EC_DISABLECOMPOSITION 0
	#endif
	#ifndef DWM_EC_ENABLECOMPOSITION
	#define DWM_EC_ENABLECOMPOSITION  1
	#endif

	typedef HRESULT (WINAPI *DWMENABLECOMPOSITION   ) (UINT  enabled);
	typedef HRESULT (WINAPI *DWMISCOMPOSITIONENABLED) (BOOL* enabled);

	static DWMENABLECOMPOSITION    gDwmEnableComposition    = nullptr;
	static DWMISCOMPOSITIONENABLED gDwmIsCompositionEnabled = nullptr;

	////////////////////////////////////////////////////////////////////////////////

	static void InitializeDWM (void)
	{
		// Initialize DWM functions once
		static bool initialized = false;

		if (!initialized)
		{
			initialized = true;
			// Attempt to load windows DWMAPI DLL library
			HMODULE d = LoadLibrary (TEXT ("Dwmapi.dll"));
			if (d != nullptr)
			{
				// Attempt to load the DWM Aero functionality
				gDwmEnableComposition = (DWMENABLECOMPOSITION)
					GetProcAddress (d, "DwmEnableComposition");

				gDwmIsCompositionEnabled = (DWMISCOMPOSITIONENABLED)
					GetProcAddress (d, "DwmIsCompositionEnabled");
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////

	static BOOL CALLBACK EnumMonitorsProc (HMONITOR hmon, HDC, LPRECT, LPARAM lParam)
	{
		// Retrieve lParam data as a screen deque
		ScreenList* screens = (ScreenList*) lParam;

		MONITORINFO info = { 0 };
		info.cbSize =
			sizeof (MONITORINFO);

		// Attempt to retrieve screen info
		if (!GetMonitorInfo (hmon, &info))
			return TRUE;

		Bounds bounds;
		// Process the total bounds of the current screen
		bounds.SetLTRB (info.rcMonitor.left, info.rcMonitor.top,
					info.rcMonitor.right, info.rcMonitor.bottom);

		Bounds usable;
		// Process the usable bounds of the current screen
		usable.SetLTRB (info.rcWork.left, info.rcWork.top,
					info.rcWork.right, info.rcWork.bottom);

		// Check if the current screen is primary
		if (info.dwFlags & MONITORINFOF_PRIMARY)
			screens->push_front (new Screen (bounds, usable));
		else
			screens->push_back  (new Screen (bounds, usable));

		return TRUE;
	}

#endif



//----------------------------------------------------------------------------//
// Fields                                                              Screen //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Bounds Screen::mTotalBounds;
Bounds Screen::mTotalUsable;
ScreenList Screen::mScreens;



//----------------------------------------------------------------------------//
// Constructors                                                        Screen //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Screen::Screen (void)
{
}

////////////////////////////////////////////////////////////////////////////////

Screen::Screen (const Bounds& bounds,
				const Bounds& usable)
{
	mBounds = bounds;
	mUsable = usable;
}



//----------------------------------------------------------------------------//
// Functions                                                           Screen //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Bounds Screen::GetBounds   (void) const { return mBounds;                }
Bounds Screen::GetUsable   (void) const { return mUsable;                }
bool   Screen::IsPortrait  (void) const { return mBounds.W <  mBounds.H; }
bool   Screen::IsLandscape (void) const { return mBounds.W >= mBounds.H; }

////////////////////////////////////////////////////////////////////////////////

bool Screen::Synchronize (void)
{
	// Loop through every available screen value
	for (uintptr i = 0; i < mScreens.size(); ++i)
		delete mScreens[i];

	// Clear screens
	mScreens.clear();
	mTotalBounds = Bounds();
	mTotalUsable = Bounds();

#ifdef ROBOT_OS_LINUX

	// Check for a valid X-Window display
	if (gDisplay == nullptr) return false;

	// Ignore X errors
	XDismissErrors xe;

	// Get the net_workarea atom for later
	static Atom NET_WORKAREA = XInternAtom
		(gDisplay, "_NET_WORKAREA", True);

	// Get the screen count traditionally
	int32 count = XScreenCount (gDisplay);

	// Used Xinerama to get screens
	bool isVirtualDesktop = true;

	// Ignore the Xinerama extension when
	// the display is using multi-screens
	// with multiple root windows
	if (count == 1 && IsXineramaAvailable() &&
		XineramaIsActive (gDisplay) == True)
	{
		int32 xineCount = 0;
		// Query for screen info
		XineramaScreenInfo* info =
			XineramaQueryScreens (gDisplay, &xineCount);

		if (info != nullptr)
		{
			// Iterate through every screen and append it
			for (int32 i = 0, j = 0; i < xineCount; ++i)
			{
				Bounds bounds (info[i].x_org, info[i].y_org,
							   info[i].width, info[i].height);

				// Merge any cloned or mirrored screens with the previous one
				if (j > 0 && mScreens[j-1]->mBounds.Intersects (bounds, false))
				{
					if (mScreens[j  ]->mBounds.W * mScreens[j  ]->mBounds.H >
						mScreens[j-1]->mBounds.W * mScreens[j-1]->mBounds.H)
					{
						mScreens[j-1]->mBounds = bounds;
						mScreens[j-1]->mUsable = bounds;
					}
				}

				else
				{
					++j;
					// Append the current screen to the screens list
					mScreens.push_back (new Screen (bounds, bounds));
				}
			}

			XFree (info);
		}
	}

	// Traditional method
	if (mScreens.empty())
	{
		isVirtualDesktop = false;
		// Retrieve default screen of the system
		int32 primary = XDefaultScreen (gDisplay);

		// Loop and append every screen
		for (int32 i = 0; i < count; ++i)
		{
			auto screen = XScreenOfDisplay (gDisplay, i);
			Bounds bounds (0, 0, XWidthOfScreen  (screen),
								 XHeightOfScreen (screen));

			if (i == primary)
				mScreens.push_front (new Screen (bounds, bounds));
			else
				mScreens.push_back  (new Screen (bounds, bounds));
		}
	}

	// Validate workarea atom
	if (NET_WORKAREA != None)
	{
		// Loop through every available screen value
		for (uintptr i = 0; i < mScreens.size(); ++i)
		{
			::Window win = XRootWindow
				(gDisplay, isVirtualDesktop ?
				XDefaultScreen (gDisplay) : i);

			// Property variables
			Atom type; int format;
			unsigned long  nItems;
			unsigned long  bAfter;
			unsigned char* result = nullptr;

			// Retrieve and validate the net_workarea property
			if (!XGetWindowProperty (gDisplay, win, NET_WORKAREA,
				0, 4, False, AnyPropertyType, &type, &format,
				&nItems, &bAfter, &result) && result != nullptr &&
				type == XA_CARDINAL && format == 32 && nItems == 4)
			{
				long* usable = (long*) result;
				mScreens[i]->mUsable = Bounds
					((int32) usable[0], (int32) usable[1],
					 (int32) usable[2], (int32) usable[3]);

				if (isVirtualDesktop)
					// Confine the work area to the current screen
					mScreens[i]->mUsable &= mScreens[i]->mBounds;
			}

			// Free the result if it got allocated
			if (result != nullptr) XFree (result);
		}
	}

#endif
#ifdef ROBOT_OS_MAC

	// Retrieves the list of available screens
	NSMutableArray* screens = [NSMutableArray
		  arrayWithArray: [NSScreen screens]];

	if ([screens count] == 0)
	{
		// Add main screen if screens are empty
		NSScreen* screen = [NSScreen mainScreen];
		if (screen) [screens addObject: screen];
	}

	// Loop through every screen on the system
	for (int32 i = 0; i < [screens count]; ++i)
	{
		// Retrieve the pointer to the current screen
		NSScreen* screen = [screens objectAtIndex: i];
		if (screen == nullptr) continue;

		// Retrieve the unique ID for the current screen
		CGDirectDisplayID id = [[[screen deviceDescription]
		objectForKey: @"NSScreenNumber"] unsignedIntValue];

		// Ignore mirrored screens (based on Qt implementation)
		if ([screens count] > 1 && CGDisplayIsInMirrorSet (id))
		{
			CGDirectDisplayID primary = CGDisplayMirrorsDisplay (id);
			if (primary != kCGNullDirectDisplay && primary != id)
				continue;
		}

		// Retrieve screen dimensions
		Bounds bounds; Bounds usable;
		NSRect frame = [screen frame];

		if (mScreens.empty())
		{
			NSRect visible = [screen visibleFrame];
			bounds = Bounds (frame.origin.x, frame.origin.y,
					 frame.size.width, frame.size.height);

			// The primary screen is the only one which
			// can have a different usable bounds region
			usable = Bounds (visible.origin.x, bounds.H -
				(visible.origin.y + visible.size.height),
				visible.size.width, visible.size.height);
		}

		else
		{
			bounds = usable = Bounds (frame.origin.x,
				mScreens[0]->mBounds.H -
				(frame.origin.y + frame.size.height),
				frame.size.width, frame.size.height);
		}

		// Append the current screen to the screens list
		mScreens.push_back (new Screen (bounds, usable));
	}

#endif
#ifdef ROBOT_OS_WIN

	// Populate the screen list with system screens
	if (EnumDisplayMonitors (0, 0, EnumMonitorsProc,
		(LPARAM) &mScreens) == FALSE) return false;

#endif

	// Check if one valid screen exists
	if (mScreens.empty()) return false;

	// Loop through every available screen value
	for (uintptr i = 0; i < mScreens.size(); ++i)
	{
		// Add to the current running total
		mTotalBounds |= mScreens[i]->mBounds;
		mTotalUsable |= mScreens[i]->mUsable;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

Screen* Screen::GetMain (void)
{
	// The primary screen is always the first element
	return mScreens.empty() ? nullptr : mScreens[0];
}

////////////////////////////////////////////////////////////////////////////////

ScreenList Screen::GetList (void) { return mScreens; }

////////////////////////////////////////////////////////////////////////////////

Screen* Screen::GetScreen (const Window& window)
{
	// Check whether the window is valid
	if (!window.IsValid()) return nullptr;

	// Get screen holding largest part of the window
	return GetScreen (window.GetBounds().GetCenter());
}

////////////////////////////////////////////////////////////////////////////////

Screen* Screen::GetScreen (const Point& point)
{
	return GetScreen (point.X, point.Y);
}

////////////////////////////////////////////////////////////////////////////////

Screen* Screen::GetScreen (int32 px, int32 py)
{
	// Loop through every available screen value
	for (uintptr i = 0; i < mScreens.size(); ++i)
	{
		if (mScreens[i]->mBounds.Contains (px, py))
			return mScreens[i];
	}

	// The primary screen is always the first element
	return mScreens.empty() ? nullptr : mScreens[0];
}

////////////////////////////////////////////////////////////////////////////////

bool Screen::GrabScreen (Image& image, const Bounds&
						 bounds, const Window& window)
{
	return GrabScreen (image,
		  bounds.X, bounds.Y,
		  bounds.W, bounds.H, window);
}

////////////////////////////////////////////////////////////////////////////////

bool Screen::GrabScreen (Image& image,
	int32 x, int32 y, int32 w, int32 h,
	const Window& window)
{
	if (w <= 0 || h <= 0)
	{
		// Get the size of windows' client
		Bounds bounds = window.GetClient();
		if (bounds.W <= 0 || bounds.H <= 0)
		{
			bounds.W = mTotalBounds.W;
			bounds.H = mTotalBounds.H;
		}

		// Compute bounds values
		if (w <= 0) w = bounds.W;
		if (h <= 0) h = bounds.H;
	}

	if (w <= 0 || h <= 0) return false;

#ifdef ROBOT_OS_LINUX

	// Check for a valid X-Window display
	if (gDisplay == nullptr) return false;

	// Ignore X errors
	XDismissErrors xe;

	// Get an X11 handle to the window
	::Window win = !window.IsValid() ?
		XDefaultRootWindow (gDisplay) :
		window.GetHandle();

	// Grab the current contents of window
	XImage* img = XGetImage (gDisplay, win,
			x, y, w, h, AllPlanes, ZPixmap);
	if (img == nullptr) return false;

	Color* data; bool result = false;
	// Resize the resulting image value
	w =  img->width;  h =  img->height;
	if (w <= 0 || h <= 0) goto exit;
	image.Create (w, h);

	data = (Color*) image.GetData();
	// Convert the resulting XImage
	for (int32 yy = 0; yy < h; ++yy)
	for (int32 xx = 0; xx < w; ++xx, ++data)
	{
		auto pixel = XGetPixel (img, xx, yy);
		data->R = (pixel & img->red_mask  ) >> 16;
		data->G = (pixel & img->green_mask) >>  8;
		data->B = (pixel & img->blue_mask ) >>  0;
		data->A = 255;
	}

	result = true;

exit:
	// Dispose of image
	XDestroyImage (img);
	return result;

#endif
#ifdef ROBOT_OS_MAC

	CGImageRef img = nullptr;
	// Verify the window
	if (window.IsValid())
	{
		// Convert window point to screen coordinates
		Point p = window.MapToScreen (Point (x, y));
		img = CGWindowListCreateImage (CGRectMake (p.X, p.Y, w, h),
			  kCGWindowListOptionIncludingWindow, (CGWindowID)
			  window.GetHandle(), kCGWindowImageBoundsIgnoreFraming);
	}

	else
	{
		img = CGWindowListCreateImage (CGRectMake (x, y, w, h),
			  kCGWindowListOptionOnScreenOnly, kCGNullWindowID,
			  kCGWindowImageBoundsIgnoreFraming);
	}

	// Verify that results are valid
	if (img == nullptr) return false;

	bool result  = false;
	CGContextRef context;
	CGColorSpaceRef colorSpace;
	// Resize the resulting image value
	w = (int32) CGImageGetWidth  (img);
	h = (int32) CGImageGetHeight (img);
	if (w <= 0 || h <= 0) goto exit;
	image.Create (w, h);

	// Bind result buffer to a graphics context
	colorSpace = CGColorSpaceCreateDeviceRGB();

	context = CGBitmapContextCreate
		(image.GetData(), w, h, 8, w * sizeof (uint32), colorSpace,
		kCGBitmapByteOrder32Host | kCGImageAlphaPremultipliedFirst);

	CGColorSpaceRelease (colorSpace);
	// Draw clipboard image into the buffer and release context
	CGContextDrawImage (context, CGRectMake (0, 0, w, h), img);
	CGContextFlush (context); CGContextRelease (context);
	result = true;

exit:
	// Dispose of image
	CGImageRelease (img);
	return result;

#endif
#ifdef ROBOT_OS_WIN

	// Retrieve the current window handle
	HWND hwnd = (HWND) window.GetHandle();

	// Retrieve the source DC
	HDC hSrcDC = GetDC (hwnd);

	// Create compatible destination objects
	HDC hDstDC = CreateCompatibleDC (hSrcDC);
	HBITMAP hImage = CreateCompatibleBitmap (hSrcDC, w, h);
	HGDIOBJ hOldDC = SelectObject (hDstDC, hImage);

	// Copy the specified screen area in the source DC
	BitBlt (hDstDC, 0, 0, w, h, hSrcDC, x, y, SRCCOPY);

	// Convert the windows bitmap value
	image.Create (w, h); BITMAPINFO bmi;
	memset (&bmi, 0, sizeof (BITMAPINFO));

	bmi.bmiHeader.biSize        = sizeof (BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       =  w;
	bmi.bmiHeader.biHeight      = -h;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	GetDIBits (hDstDC, hImage, 0, h, image.
		GetData(), &bmi, DIB_RGB_COLORS);

	// Dispose & clean up objects
	SelectObject (hDstDC, hOldDC);
	DeleteDC (hDstDC);
	ReleaseDC (hwnd, hSrcDC);
	DeleteObject (hImage);
	return true;

#endif
}

////////////////////////////////////////////////////////////////////////////////

Bounds Screen::GetTotalBounds (void) { return mTotalBounds; }
Bounds Screen::GetTotalUsable (void) { return mTotalUsable; }

////////////////////////////////////////////////////////////////////////////////

bool Screen::IsCompositing (void)
{
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_LINUX)

	return true;

#endif
#ifdef ROBOT_OS_WIN

	InitializeDWM(); BOOL enabled = FALSE;
	if (gDwmIsCompositionEnabled != nullptr)
		gDwmIsCompositionEnabled (&enabled);
	return enabled != FALSE;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Screen::SetCompositing (bool enabled)
{
#ifdef ROBOT_OS_WIN

	InitializeDWM();
	if (gDwmEnableComposition != nullptr)
		gDwmEnableComposition (enabled ?
			DWM_EC_ENABLECOMPOSITION :
			DWM_EC_DISABLECOMPOSITION);

#endif
}

ROBOT_NS_END
