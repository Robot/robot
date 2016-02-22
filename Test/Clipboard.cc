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

static bool TestLinux (void)
{
#ifdef ROBOT_OS_LINUX

	VERIFY (!Clipboard::Clear  ());

	VERIFY (!Clipboard::HasText());
	VERIFY ( Clipboard::GetText().empty());
	VERIFY (!Clipboard::SetText (nullptr));
	VERIFY (!Clipboard::SetText (""     ));
	VERIFY (!Clipboard::SetText ("Hello"));
	VERIFY (!Clipboard::SetText ("World"));

	Image image;
	VERIFY (!Clipboard::HasImage());
	VERIFY (!Clipboard::GetImage (image));
	VERIFY (!Clipboard::SetImage (image));
	VERIFY (!image.IsValid());

	VERIFY (Clipboard::GetSequence() == 0);
	VERIFY (Clipboard::GetSequence() ==
			Clipboard::GetSequence());

#endif

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestText (void)
{
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_WIN)

	uint64 s = 0;
	Image image; string text;
	string big (1048576, '0');

	auto test = [&] (const char* str, bool write) -> bool
	{
		if (write)
			VERIFY (Clipboard::SetText (str));

		VERIFY ( Clipboard::HasText ());
		VERIFY (!Clipboard::HasImage());

		VERIFY (Clipboard::GetSequence() != 0);
		VERIFY (Clipboard::GetSequence() != s);
		VERIFY (Clipboard::GetSequence() ==
				Clipboard::GetSequence());
			s = Clipboard::GetSequence();

		VERIFY (!Clipboard::GetImage (image)); VERIFY (!image.IsValid());
		VERIFY (!Clipboard::GetImage (image)); VERIFY (!image.IsValid());
		text = Clipboard::GetText(); VERIFY (text == str);
		text = Clipboard::GetText(); VERIFY (text == str);
		return true;
	};

	//----------------------------------------------------------------------------//

	VERIFY (test ("",         true));
	VERIFY (test ("Hello",    true)); cout << "Try pasting the text"; getchar();
	VERIFY (test ("World",    true)); cout << "Try pasting the text"; getchar();
	VERIFY (test (big.data(), true));

	//----------------------------------------------------------------------------//

	VERIFY (Clipboard::Clear());
	cout << "Copy Hello and press enter  "; getchar(); VERIFY (test ("Hello", false));
	VERIFY (Clipboard::Clear());
	cout << "Copy World and press enter\n"; getchar(); VERIFY (test ("World", false));

	//----------------------------------------------------------------------------//

	VERIFY (!Clipboard::SetText (nullptr));

	VERIFY ( Clipboard::HasText ());
	VERIFY (!Clipboard::HasImage());

	VERIFY (Clipboard::GetSequence() != 0);
	VERIFY (Clipboard::GetSequence() == s);
	VERIFY (Clipboard::GetSequence() ==
			Clipboard::GetSequence());

	VERIFY (!Clipboard::GetImage (image)); VERIFY (!image.IsValid());
	VERIFY (!Clipboard::GetImage (image)); VERIFY (!image.IsValid());
	text = Clipboard::GetText(); VERIFY (text == "World");
	text = Clipboard::GetText(); VERIFY (text == "World");

	//----------------------------------------------------------------------------//

	VERIFY ( Clipboard::Clear   ());
	VERIFY (!Clipboard::HasText ());
	VERIFY (!Clipboard::HasImage());

	VERIFY (Clipboard::GetSequence() != 0);
	VERIFY (Clipboard::GetSequence() != s);
	VERIFY (Clipboard::GetSequence() ==
			Clipboard::GetSequence());
		s = Clipboard::GetSequence();

	VERIFY (!Clipboard::GetImage (image)); VERIFY (!image.IsValid());
	VERIFY (!Clipboard::GetImage (image)); VERIFY (!image.IsValid());
	text = Clipboard::GetText(); VERIFY (text == string());
	text = Clipboard::GetText(); VERIFY (text == string());

#endif

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestImage (void)
{
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_WIN)

	uint64 s = 0;
	Image image; string text;

	Image pattern (9, 5);
	// Create test clipboard image
	for (uint32 i = 0; i < 5; ++i)
	{
		uint8 a = 0;
		switch (i) {
			case 0: a = 255; break; case 1: a = 192; break;
			case 2: a = 128; break; case 3: a =  64; break;
		}

		pattern.SetPixel (0, i, Color (  0,   0,   0, a));
		pattern.SetPixel (1, i, Color (128, 128, 128, a));
		pattern.SetPixel (2, i, Color (255, 255, 255, a));
		pattern.SetPixel (3, i, Color (255,   0,   0, a));
		pattern.SetPixel (4, i, Color (  0, 255,   0, a));
		pattern.SetPixel (5, i, Color (  0,   0, 255, a));
		pattern.SetPixel (6, i, Color (255, 255,   0, a));
		pattern.SetPixel (7, i, Color (  0, 255, 255, a));
		pattern.SetPixel (8, i, Color (255,   0, 255, a));
	}

	auto test = [&] (const Image& img) -> bool
	{
		if (img.IsValid())
			VERIFY (Clipboard::SetImage (img));

		VERIFY (!Clipboard::HasText ());
		VERIFY ( Clipboard::HasImage());

		VERIFY (Clipboard::GetSequence() != 0);
		VERIFY (Clipboard::GetSequence() != s);
		VERIFY (Clipboard::GetSequence() ==
				Clipboard::GetSequence());
			s = Clipboard::GetSequence();

		VERIFY (Clipboard::GetImage (image)); VERIFY (image.IsValid());
		VERIFY (Clipboard::GetImage (image)); VERIFY (image.IsValid());
		text = Clipboard::GetText(); VERIFY (text == string());
		text = Clipboard::GetText(); VERIFY (text == string());
		return true;
	};

	//----------------------------------------------------------------------------//

	VERIFY (test (pattern));
	VERIFY (pattern.IsValid() == true);
	VERIFY (pattern.GetWidth () ==  9);
	VERIFY (pattern.GetHeight() ==  5);
	VERIFY (pattern.GetLength() == 45);
	VERIFY (pattern.GetLimit () == 45);

	VERIFY (pattern.IsValid  () == image.IsValid  ());
	VERIFY (pattern.GetWidth () == image.GetWidth ());
	VERIFY (pattern.GetHeight() == image.GetHeight());
	VERIFY (pattern.GetLength() == image.GetLength());
	VERIFY (pattern.GetLimit () == image.GetLimit ());

	VERIFY (image.GetPixel (0, 0) == Color (  0,   0,   0));
	VERIFY (image.GetPixel (1, 0) == Color (128, 128, 128));
	VERIFY (image.GetPixel (2, 0) == Color (255, 255, 255));
	VERIFY (image.GetPixel (3, 0) == Color (255,   0,   0));
	VERIFY (image.GetPixel (4, 0) == Color (  0, 255,   0));
	VERIFY (image.GetPixel (5, 0) == Color (  0,   0, 255));
	VERIFY (image.GetPixel (6, 0) == Color (255, 255,   0));
	VERIFY (image.GetPixel (7, 0) == Color (  0, 255, 255));
	VERIFY (image.GetPixel (8, 0) == Color (255,   0, 255));

	//----------------------------------------------------------------------------//

	tga_write_bgr ("pattern_orig.tga", (uint8*) image  .GetData(), image.GetWidth(), image.GetHeight(), 32);
	tga_write_bgr ("pattern_copy.tga", (uint8*) pattern.GetData(), image.GetWidth(), image.GetHeight(), 32);
	cout << "Try pasting the image"; getchar();

	VERIFY (Clipboard::Clear());
	cout << "Copy JPG image and press enter"; getchar(); VERIFY (test (Image()));
	tga_write_bgr ("JPG.tga", (uint8*) image.GetData(), image.GetWidth(), image.GetHeight(), 32);

	VERIFY (Clipboard::Clear());
	cout << "Copy PNG image and press enter"; getchar(); VERIFY (test (Image()));
	tga_write_bgr ("PNG.tga", (uint8*) image.GetData(), image.GetWidth(), image.GetHeight(), 32);

	cout << "Please verify saved images\n"; getchar();

	//----------------------------------------------------------------------------//

	VERIFY (!Clipboard::SetImage (Image()));

	VERIFY (!Clipboard::HasText ());
	VERIFY ( Clipboard::HasImage());

	VERIFY (Clipboard::GetSequence() != 0);
	VERIFY (Clipboard::GetSequence() == s);
	VERIFY (Clipboard::GetSequence() ==
			Clipboard::GetSequence());

	VERIFY (Clipboard::GetImage (image)); VERIFY (image.IsValid());
	VERIFY (Clipboard::GetImage (image)); VERIFY (image.IsValid());
	text = Clipboard::GetText(); VERIFY (text == string());
	text = Clipboard::GetText(); VERIFY (text == string());

	//----------------------------------------------------------------------------//

	image.Destroy();
	VERIFY ( Clipboard::Clear   ());
	VERIFY (!Clipboard::HasText ());
	VERIFY (!Clipboard::HasImage());

	VERIFY (Clipboard::GetSequence() != 0);
	VERIFY (Clipboard::GetSequence() != s);
	VERIFY (Clipboard::GetSequence() ==
			Clipboard::GetSequence());
		s = Clipboard::GetSequence();

	VERIFY (!Clipboard::GetImage (image)); VERIFY (!image.IsValid());
	VERIFY (!Clipboard::GetImage (image)); VERIFY (!image.IsValid());
	text = Clipboard::GetText(); VERIFY (text == string());
	text = Clipboard::GetText(); VERIFY (text == string());

#endif

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestSpeed (void)
{
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_WIN)

	Timer timer;
	uint64 s = 0;
	Image image (1920, 1080); string text;

	string smallText ("Small Size");
	string largeText (1048576, '0');

	Image smallImage (  16,   16);
	Image largeImage (1920, 1080);
	smallImage.Fill  ( 192,  128, 64);
	largeImage.Fill  ( 192,  128, 64);

	//----------------------------------------------------------------------------//

	cout << "Small text  (ms): ";
	for (uint32 i = 0; i < 5; ++i)
	{
		timer.Start();

		VERIFY (Clipboard::SetText (smallText.data()));
		uint64 clipSequence = Clipboard::GetSequence();
		VERIFY (clipSequence != s); s = clipSequence;

		VERIFY (!Clipboard::GetText().empty());
		VERIFY (Clipboard::GetSequence() == s);

		cout << timer() << " ";
	}

	VERIFY (Clipboard::Clear());
	cout << endl;

	//----------------------------------------------------------------------------//

	cout << "Large text  (ms): ";
	for (uint32 i = 0; i < 5; ++i)
	{
		timer.Start();

		VERIFY (Clipboard::SetText (largeText.data()));
		uint64 clipSequence = Clipboard::GetSequence();
		VERIFY (clipSequence != s); s = clipSequence;

		VERIFY (!Clipboard::GetText().empty());
		VERIFY (Clipboard::GetSequence() == s);

		cout << timer() << " ";
	}

	VERIFY (Clipboard::Clear());
	cout << endl;

	//----------------------------------------------------------------------------//

	cout << "Small image (ms): ";
	for (uint32 i = 0; i < 5; ++i)
	{
		timer.Start();

		VERIFY (Clipboard::SetImage (smallImage));
		uint64 clipSequence = Clipboard::GetSequence();
		VERIFY (clipSequence != s); s = clipSequence;

		VERIFY (Clipboard::GetImage (image));
		VERIFY (Clipboard::GetSequence() == s);

		cout << timer() << " ";
	}

	VERIFY (Clipboard::Clear());
	cout << endl;

	//----------------------------------------------------------------------------//

	cout << "Large image (ms): ";
	for (uint32 i = 0; i < 5; ++i)
	{
		timer.Start();

		VERIFY (Clipboard::SetImage (largeImage));
		uint64 clipSequence = Clipboard::GetSequence();
		VERIFY (clipSequence != s); s = clipSequence;

		VERIFY (Clipboard::GetImage (image));
		VERIFY (Clipboard::GetSequence() == s);

		cout << timer() << " ";
	}

	VERIFY (Clipboard::Clear());
	cout << endl;

#endif

	return true;
}



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool TestClipboard (void)
{
	cout << "BEGIN CLIPBOARD TESTING\n------------------------------\n";

	cout << "Warning: Some set of tests cannot be automated\n"
		 << "         Please execute the following commands\n\n";

	if (!TestLinux()) { cout << ">> Linux Failed\n\n"; return false; }
	if (!TestText ()) { cout << ">> Text Failed \n\n"; return false; }
	if (!TestImage()) { cout << ">> Image Failed\n\n"; return false; }
	if (!TestSpeed()) { cout << ">> Speed Failed\n\n"; return false; }
	cout << ">> Success\n\n"; return true;
}
