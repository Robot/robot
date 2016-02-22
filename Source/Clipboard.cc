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

#include "Clipboard.h"
using std::string;

#ifdef ROBOT_OS_MAC

	#include <AppKit/AppKit.h>

#endif
#ifdef ROBOT_OS_WIN

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	using std::wstring;

	#ifdef UNICODE
		// Use the UNICODE transfer format
		#define TEXT_FORMAT CF_UNICODETEXT
	#else
		// Use TEXT transfer format
		#define TEXT_FORMAT CF_TEXT
	#endif

#endif
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

#ifdef ROBOT_OS_MAC

	////////////////////////////////////////////////////////////////////////////////

	static NSPasteboard* GetPasteboard (void) { return [NSPasteboard generalPasteboard]; }

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

	class AutoClipboard
	{
	public:
		//----------------------------------------------------------------------------//
		// Constructors                                                               //
		//----------------------------------------------------------------------------//

		 AutoClipboard (void) { mOpened = false; }
		~AutoClipboard (void) { Close();         }

	public:
		//----------------------------------------------------------------------------//
		// Functions                                                                  //
		//----------------------------------------------------------------------------//

		////////////////////////////////////////////////////////////////////////////////

		bool IsOpened (void)
		{
			return mOpened;
		}

		////////////////////////////////////////////////////////////////////////////////

		bool Open (HWND owner = nullptr)
		{
			if (mOpened) return true;

			// Attempt to open the clipboard
			// multiple times in case another
			// application has a lock on it.

			uint8    times = 0;
			while (++times < 6)
			{
				// Attempt to open clipboard
				if (OpenClipboard (nullptr))
					return mOpened = true;

				Sleep (5);
			}

			return false;
		}

		////////////////////////////////////////////////////////////////////////////////

		void Close (void)
		{
			if (mOpened)
			{
				CloseClipboard();
				mOpened = false;
			}
		}

	private:
		//----------------------------------------------------------------------------//
		// Fields                                                                     //
		//----------------------------------------------------------------------------//

		bool mOpened; // If clipboard is open
	};

#endif



//----------------------------------------------------------------------------//
// Functions                                                        Clipboard //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Clipboard::Clear (void)
{
#ifdef ROBOT_OS_LINUX

	return false; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_MAC

	// Get a pointer to the pasteboard
	NSPasteboard* board = GetPasteboard();
	if (board == nullptr) return false;
	[board clearContents]; return true;

#endif
#ifdef ROBOT_OS_WIN

	AutoClipboard clipboard;
	// Acquire the clipboard lock
	if (!clipboard.Open (nullptr))
		return false;

	// Attempt to empty the clipboard
	return EmptyClipboard() != FALSE;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Clipboard::HasText (void)
{
#ifdef ROBOT_OS_LINUX

	return false; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_MAC

	// Get a pointer to the pasteboard
	NSPasteboard* board = GetPasteboard();
	if (board == nullptr) return false;

	// Get the currently available types on the pasteboard
	NSString* available = [board availableTypeFromArray:
		[NSArray arrayWithObject: NSPasteboardTypeString]];

	// Verify that the type we want is currently available
	if ([available isEqualToString: NSPasteboardTypeString])
		return true; else return false;

#endif
#ifdef ROBOT_OS_WIN

	return IsClipboardFormatAvailable (TEXT_FORMAT) != FALSE;

#endif
}

////////////////////////////////////////////////////////////////////////////////

string Clipboard::GetText (void)
{
	string result;

#ifdef ROBOT_OS_LINUX

	return result; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_MAC

	// Get a pointer to the pasteboard
	NSPasteboard* board = GetPasteboard();
	if (board == nullptr) return result;

	// Get the currently available types on the pasteboard
	NSString* available = [board availableTypeFromArray:
		[NSArray arrayWithObject: NSPasteboardTypeString]];

	// Verify that the type we want is currently available
	if ([available isEqualToString: NSPasteboardTypeString])
	{
		NSString* data =
			[board stringForType: NSPasteboardTypeString];

		// Check and convert data into a UTF-8 string
		if (data != nullptr) result = [data UTF8String];
	}

	return result;

#endif
#ifdef ROBOT_OS_WIN

	AutoClipboard clipboard;
	HANDLE memory = nullptr;
	LPTSTR buffer = nullptr;

	// Check if the clipboard format is available
	if (!IsClipboardFormatAvailable (TEXT_FORMAT) ||
		!clipboard.Open (nullptr)) return result;

	// Get the clipboard data in defined format
	if (memory = GetClipboardData (TEXT_FORMAT))
	{
		if (buffer = (LPTSTR) GlobalLock (memory))
		{
			// Convert clipboard to UTF-8
			result = _UTF8Encode (buffer);
			GlobalUnlock (memory);
		}
	}

	return result;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Clipboard::SetText (const char* text)
{
	bool result = false;
	// Check whether the text is valid
	if (text == nullptr) return false;

#ifdef ROBOT_OS_LINUX

	return result; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_MAC

	// Get a pointer to the pasteboard
	NSPasteboard* board = GetPasteboard();
	if (board == nullptr) return false;

	// Clear all contents
	[board clearContents];

	result = [board
		// Set clipboard text using UTF-8 representation
		setString: [NSString stringWithUTF8String: text]
		forType: NSPasteboardTypeString] ? true : false;

	return result;

#endif
#ifdef ROBOT_OS_WIN

	AutoClipboard clipboard;
	HANDLE memory = nullptr;
	LPTSTR buffer = nullptr;

	// Acquire the clipboard lock
	if (!clipboard.Open (nullptr))
		return false;

	// Convert text to wide string
	auto data = _UTF8Decode (text);
	// Calculate size including the null terminator
	auto size = (data.size() + 1) * sizeof (TCHAR);

	// Allocate global memory for storing text data
	if (memory = GlobalAlloc (GMEM_MOVEABLE, size))
	{
		if (buffer = (LPTSTR) GlobalLock (memory))
		{
			// Copy the clipboard data buffer
			memcpy (buffer, data.data(), size);
			GlobalUnlock (memory);

			// Empty the clipboard and place the data
			if (EmptyClipboard() && SetClipboardData
				(TEXT_FORMAT, memory)) result = true;
		}
	}

	// Free memory in case of error
	if (!result) GlobalFree (memory);
	return result;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Clipboard::HasImage (void)
{
#ifdef ROBOT_OS_LINUX

	return false; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_MAC

	// Get a pointer to the pasteboard
	NSPasteboard* board = GetPasteboard();
	if (board == nullptr) return false;

	// Check whether an image is currently available
	BOOL available = [board canReadObjectForClasses:
		[NSArray arrayWithObject: [NSImage class] ]
		options: [NSDictionary dictionary] ];

	// Return resulting availability
	return available ? true : false;

#endif
#ifdef ROBOT_OS_WIN

	return IsClipboardFormatAvailable (CF_BITMAP) != FALSE;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Clipboard::GetImage (Image& image)
{
	bool result = false;

#ifdef ROBOT_OS_LINUX

	return result; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_MAC

	// Get a pointer to the pasteboard
	NSPasteboard* board = GetPasteboard();
	if (board == nullptr) return false;

	// Retrieve an image from pasteboard
	NSImage* nsImage = [[NSImage alloc]
			 initWithPasteboard: board];

	if (nsImage != nullptr)
	{
		CGImageRef cgImage = [nsImage CGImageForProposedRect: nullptr
		   context: [NSGraphicsContext currentContext] hints: nullptr];

		if (cgImage != nullptr)
		{
			uint16 w = (uint16) CGImageGetWidth  (cgImage);
			uint16 h = (uint16) CGImageGetHeight (cgImage);
			if (w > 0 && h > 0)
			{
				image.Create (w, h);
				// Bind the result buffer to a graphics context for drawing
				CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();

				CGContextRef context = CGBitmapContextCreate
					(image.GetData(), w, h, 8, w * sizeof (uint32), colorSpace,
					kCGBitmapByteOrder32Host | kCGImageAlphaPremultipliedFirst);

				CGColorSpaceRelease (colorSpace);
				// Draw the clipboard image into the buffer and release context
				CGContextDrawImage (context, CGRectMake (0, 0, w, h), cgImage);
				CGContextFlush (context); CGContextRelease (context);
				result = true;
			}

			CFRelease (cgImage);
		}

		[nsImage release];
	}

	return result;

#endif
#ifdef ROBOT_OS_WIN

	AutoClipboard clipboard;
	HANDLE memory = nullptr;
	BITMAP bitmap = { 0 };

	// Check if the clipboard format is available
	if (!IsClipboardFormatAvailable (CF_BITMAP) ||
		!clipboard.Open (nullptr)) return false;

	// Get the clipboard data in image format
	if (memory = GetClipboardData (CF_BITMAP))
	{
		// Attempt to get bitmap information from handle
		if (GetObject (memory, sizeof (bitmap), &bitmap))
		{
			uint16 w = (uint16) abs (bitmap.bmWidth );
			uint16 h = (uint16) abs (bitmap.bmHeight);
			if (w > 0 && h > 0)
			{
				// Convert the windows bitmap value
				image.Create (w, h); BITMAPINFO bmi;
				memset (&bmi, 0, sizeof (BITMAPINFO));

				bmi.bmiHeader.biSize        = sizeof (BITMAPINFOHEADER);
				bmi.bmiHeader.biWidth       =  w;
				bmi.bmiHeader.biHeight      = -h;
				bmi.bmiHeader.biPlanes      = 1;
				bmi.bmiHeader.biBitCount    = 32;
				bmi.bmiHeader.biCompression = BI_RGB;

				HDC hdc = GetDC (nullptr);
				// Convert bitmap pixels to Image
				GetDIBits (hdc, (HBITMAP) memory,
					0, h, image.GetData(),  &bmi,
					DIB_RGB_COLORS);

				ReleaseDC (nullptr, hdc);
				result = true;
			}
		}
	}

	return result;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Clipboard::SetImage (const Image& image)
{
	bool result = false;
	// Check whether the image is valid
	if (!image.IsValid()) return false;

#ifdef ROBOT_OS_LINUX

	return result; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_MAC

	// Get a pointer to the pasteboard
	NSPasteboard* board = GetPasteboard();
	if (board == nullptr) return false;

	uint16 w = image.GetWidth ();
	uint16 h = image.GetHeight();

	// Bind the image buffer to a graphics context for copying
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();

	CGContextRef context = CGBitmapContextCreate
		(image.GetData(), w, h, 8, w * sizeof (uint32), colorSpace,
		 kCGBitmapByteOrder32Host | kCGImageAlphaPremultipliedFirst);

	CGColorSpaceRelease (colorSpace);
	// Create a core graphics image from the buffer image data
	CGImageRef cgImage = CGBitmapContextCreateImage (context);
	if (cgImage != nullptr)
	{
		// Get NSImage representation
		NSImage* ni = [[NSImage alloc]
			initWithCGImage: cgImage size: NSZeroSize];

		if (ni != nullptr)
		{
			// Clear all contents
			[board clearContents];

			// Set clipboard data using specified image
			NSArray* obj = [NSArray arrayWithObject: ni];
			if ([board writeObjects: obj]) result = true;
			[ni release];
		}

		// Perform clean up
		CFRelease (cgImage);
	}

	// Perform clean up
	CFRelease (context);
	return result;

#endif
#ifdef ROBOT_OS_WIN

	AutoClipboard clipboard;
	// Acquire the clipboard lock
	if (!clipboard.Open (nullptr))
		return false;

	// Convert to windows bitmap
	uint16 w = image.GetWidth ();
	uint16 h = image.GetHeight();
	BITMAPINFO bmi;
	memset (&bmi, 0, sizeof (BITMAPINFO));

	bmi.bmiHeader.biSize        = sizeof (BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       =  w;
	bmi.bmiHeader.biHeight      = -h;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	HDC hdc = GetDC (nullptr);
	// Create a compatible bitmap to store the image
	HBITMAP bitmap = CreateCompatibleBitmap (hdc, w, h);
	if (!bitmap) { ReleaseDC (0, hdc); return false; }

	// Convert image to bitmap
	SetDIBits (hdc, bitmap,
		0, h, image.GetData(),
		&bmi, DIB_RGB_COLORS);

	ReleaseDC (nullptr, hdc);

	// Empty the clipboard and place the data
	if (EmptyClipboard() && SetClipboardData
		(CF_BITMAP, bitmap)) result = true;

	// Delete bitmap in case of error
	if (!result) DeleteObject (bitmap);
	return result;

#endif
}

////////////////////////////////////////////////////////////////////////////////

uint64 Clipboard::GetSequence (void)
{
#ifdef ROBOT_OS_LINUX

	return 0; // WARNING: Unavailable

#endif
#ifdef ROBOT_OS_MAC

	NSPasteboard* board = GetPasteboard();
	return board ? [board changeCount] : 0;

#endif
#ifdef ROBOT_OS_WIN

	return GetClipboardSequenceNumber();

#endif
}

ROBOT_NS_END
