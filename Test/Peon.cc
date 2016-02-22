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

#include <iostream>
using std::dec;
using std::hex;
using std::cout;
using std::endl;

#include <stdint.h>
#include <cstring>
#include <iomanip>
using std::setw;
using std::setfill;

#include <fstream>
using std::ofstream;



//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define COMPARE( v1, v2 ) \
	if (v1 != v2) cout << setfill (' ') << setw (8) << #v1 << ": " << (v1 = v2) << endl;



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

static int16_t gInt16 = 0;
static int32_t gInt32 = 0;
static int64_t gInt64 = 0;

static float  gReal32 = 0;
static double gReal64 = 0;

static int16_t* gPtr16 = nullptr;
static int32_t* gPtr32 = nullptr;
static int64_t* gPtr64 = nullptr;

static int32_t* gArray = nullptr;

static struct
{
	int32_t Value1;
	int32_t Value2;
	int64_t Value3;

} gFinite = { 0 };

static struct
{
	float X;
	float Y;
	float Z;
	float W;

} gVector = { 0 };

static const uint8_t gPat1[] =
{
	0x04, 0x08, 0x15, 0x16, 0x23, 0x42, 0xDE, 0xAD,
	0x00, 0xB7, 0x2A, 0xED, 0xDA, 0xF6, 0xF9, 0x18,
	0xBE, 0xEF, 0x04, 0x08, 0x0F, 0x10, 0x17, 0x2A,
	0xB6, 0x83, 0x16, 0x8E, 0x68, 0x60, 0x8C, 0xF6,
};

static const uint8_t gPat2[] =
{
	0x04, 0x08, 0x0F, 0x10, 0x17, 0x2A, 0xDE, 0xAD,
	0xC1, 0xE1, 0x29, 0x6C, 0xDF, 0x12, 0xA2, 0x35,
	0xBE, 0xEF, 0x04, 0x08, 0x15, 0x16, 0x23, 0x42,
	0x65, 0x70, 0xBE, 0xBE, 0x29, 0x39, 0xB0, 0x94,
};

static uint8_t* gPat3 = nullptr;
static uint8_t* gPat4 = nullptr;



//----------------------------------------------------------------------------//
// Main                                                                       //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

int main (int argc, const char* argv[], const char* env[])
{
	static const uint8_t sPat5[] =
	{
		0x04, 0x08, 0x15, 0x16, 0x23, 0x42, 0xDE, 0xAD,
		0x3C, 0xC7, 0xCF, 0xE6, 0x8F, 0x6F, 0xD7, 0x9A,
		0xBE, 0xEF, 0x04, 0x08, 0x0F, 0x10, 0x17, 0x2A,
		0x21, 0x8B, 0xA2, 0xFA, 0x7A, 0x7B, 0x16, 0x6C,
	};

	static const uint8_t sPat6[] =
	{
		0x04, 0x08, 0x0F, 0x10, 0x17, 0x2A, 0xDE, 0xAD,
		0x6B, 0xB1, 0xBC, 0x28, 0xA4, 0xE8, 0xDE, 0xB3,
		0xBE, 0xEF, 0x04, 0x08, 0x15, 0x16, 0x23, 0x42,
		0x09, 0x0D, 0x5B, 0x11, 0x54, 0xC7, 0x5B, 0x01,
	};

	gPat3  = new uint8_t[32];
	gPat4  = new uint8_t[32];
	gArray = new int32_t[ 4];

	if (gPat3 > gPat4)
		std::swap (gPat3, gPat4);

	memcpy (gPat3, sPat5, 32);
	memcpy (gPat4, sPat6, 32);

	//----------------------------------------------------------------------------//

	cout << "\nROBOT MEMORY PEON\n"
		 << "------------------------------\n"
		 << "(C) 2010-2016 Robot Developers\n\n";

	//----------------------------------------------------------------------------//

	// Useful for testing Process::Create
	if (argc > 1)
	{
		// Test current work directory
		const char* file = "PEON-TEST";
		ofstream data (file); if (data)
		{
			data << "Hello World!";
			data.close();
		}

		// Probably in a non-writable directory
		else cout << "Could not write file\n";

		cout << "\n\nARGUMENTS\n"
			 << "------------------------------\n";

		// Print all command line args
		for (int i = 0; i < argc; ++i)
		{
			cout << setw (2) << i
				 << ": " << argv[i] << endl;
		}

		cout << "\n\n\nENVIRONMENT\n"
			 << "------------------------------\n";

		// Print the environment
		for (const char** e = env;
			 *e != nullptr; ++e) {
			cout << *e << endl;
		}

		cout << endl; return 0;
	}

	//----------------------------------------------------------------------------//

	cout << "This application is designed to be used alongside the memory\n"
		 << "testing module of the Robot testing suite. It will assist in\n"
		 << "the  testing  of remote  memory  manipulation  functions  by\n"
		 << "providing a consistent environment  where errors are able to\n"
		 << "be detected automatically on all target platforms.\n\n";

	cout << std::uppercase << "STATIC OFFSETS\n"
		 << "------------------------------\n"
		 << " Int16: 0x" << hex << (uintptr_t) &gInt16  << " | " << dec << sizeof (gInt16 ) << "\n"
		 << " Int32: 0x" << hex << (uintptr_t) &gInt32  << " | " << dec << sizeof (gInt32 ) << "\n"
		 << " Int64: 0x" << hex << (uintptr_t) &gInt64  << " | " << dec << sizeof (gInt64 ) << "\n\n"

		 << "Real32: 0x" << hex << (uintptr_t) &gReal32 << " | " << dec << sizeof (gReal32) << "\n"
		 << "Real64: 0x" << hex << (uintptr_t) &gReal64 << " | " << dec << sizeof (gReal64) << "\n\n"

		 << " Ptr16: 0x" << hex << (uintptr_t) &gPtr16  << " | " << dec << sizeof (gPtr16 ) << "\n"
		 << " Ptr32: 0x" << hex << (uintptr_t) &gPtr32  << " | " << dec << sizeof (gPtr32 ) << "\n"
		 << " Ptr64: 0x" << hex << (uintptr_t) &gPtr64  << " | " << dec << sizeof (gPtr64 ) << "\n\n"

		 << " Array: 0x" << hex << (uintptr_t) &gArray  << " | " << dec << sizeof (gArray ) << "\n\n"
		 << "Finite: 0x" << hex << (uintptr_t) &gFinite << " | " << dec << sizeof (gFinite) << "\n"
		 << "Vector: 0x" << hex << (uintptr_t) &gVector << " | " << dec << sizeof (gVector) << "\n\n"

		 << "  Pat1: 0x" << hex << (uintptr_t) &gPat1   << " | " << dec << sizeof (gPat1  ) << "\n"
		 << "  Pat2: 0x" << hex << (uintptr_t) &gPat2   << " | " << dec << sizeof (gPat2  ) << "\n"
		 << "  Pat3: 0x" << hex << (uintptr_t) &gPat3   << " | " << dec << sizeof (gPat3  ) << "\n"
		 << "  Pat4: 0x" << hex << (uintptr_t) &gPat4   << " | " << dec << sizeof (gPat4  ) << "\n"
		 << "  Pat5: 0x" << hex << (uintptr_t) &sPat5   << " | " << dec << sizeof (sPat5  ) << "\n"
		 << "  Pat6: 0x" << hex << (uintptr_t) &sPat6   << " | " << dec << sizeof (sPat6  ) << "\n";

	//----------------------------------------------------------------------------//

	int16_t int16 = 0;
	int32_t int32 = 0;
	int64_t int64 = 0;

	float  real32 = 0;
	double real64 = 0;

	int16_t* ptr16 = nullptr;
	int32_t* ptr32 = nullptr;
	int64_t* ptr64 = nullptr;

	int32_t array[4] = { 0 };

	int32_t finite1 = 0;
	int32_t finite2 = 0;
	int64_t finite3 = 0;

	float   vectorX = 0;
	float   vectorY = 0;
	float   vectorZ = 0;
	float   vectorW = 0;

	//----------------------------------------------------------------------------//

	while (true)
	{
		 int16 = gInt16  = 100;
		 int32 = gInt32  = 200;
		 int64 = gInt64  = 300;

		real32 = gReal32 = 400;
		real64 = gReal64 = 500;

		 ptr16 = gPtr16 = &gInt16;
		 ptr32 = gPtr32 = &gInt32;
		 ptr64 = gPtr64 = &gInt64;

		array[0] = gArray[0] = 0;
		array[1] = gArray[1] = 1;
		array[2] = gArray[2] = 2;
		array[3] = gArray[3] = 3;

		finite1 = gFinite.Value1 = 10;
		finite2 = gFinite.Value2 = 20;
		finite3 = gFinite.Value3 = 30;

		vectorX = gVector.X = 1.75f;
		vectorY = gVector.Y = 2.75f;
		vectorZ = gVector.Z = 3.75f;
		vectorW = gVector.W = 4.75f;

		cout << "\nPress enter to test changes\n"
			 << "------------------------------";
		getchar();

		COMPARE (int16, gInt16);
		COMPARE (int32, gInt32);
		COMPARE (int64, gInt64);

		COMPARE (real32, gReal32);
		COMPARE (real64, gReal64);

		COMPARE (ptr16, gPtr16);
		COMPARE (ptr32, gPtr32);
		COMPARE (ptr64, gPtr64);

		COMPARE (array[0], gArray[0]);
		COMPARE (array[1], gArray[1]);
		COMPARE (array[2], gArray[2]);
		COMPARE (array[3], gArray[3]);

		COMPARE (finite1, gFinite.Value1);
		COMPARE (finite2, gFinite.Value2);
		COMPARE (finite3, gFinite.Value3);

		COMPARE (vectorX, gVector.X);
		COMPARE (vectorY, gVector.Y);
		COMPARE (vectorZ, gVector.Z);
		COMPARE (vectorW, gVector.W);

		cout << "\nPress enter to reset changes\n"
			 << "------------------------------";
		getchar();
		cout << "Changes have been reset...\n";
	}

	delete[] gPat3;
	delete[] gPat4;
	delete[] gArray;

	return 0;
}
