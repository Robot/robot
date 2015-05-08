////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2015 Robot Developers                       //
//                       See LICENSE for licensing info                       //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
// Prefaces                                                                   //
//----------------------------------------------------------------------------//

#ifndef ROBOT_TEST_H
#define ROBOT_TEST_H

#ifdef VLD_LEAK_TEST
	// Used for MS VC++ leak testing
	#define VLD_AGGREGATE_DUPLICATES
	#include "vld.h"
#endif

#include <iostream>
using std::dec;
using std::hex;
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;
using std::setfill;

#include <fstream>
using std::fstream;
using std::ofstream;
using std::ifstream;

#include <string>
#include <cstring>
using std::string;
using std::strcmp;

#include <vector>
using std::vector;
using std::make_pair;

#include <algorithm>
using std::sort;
using std::is_sorted;
using std::unique;

#if (_MSC_VER != 1600)
	#include <thread>
	using std::thread;
#endif

#include "../Source/Robot.h"
using namespace Robot;



//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define VERIFY( cond )					\
	if (!(cond))						\
	{									\
		cout << #cond " Failed\n"		\
			 << __FILE__ << ":"			\
			 << __LINE__ << "\n\n";		\
		return false;					\
	}

////////////////////////////////////////////////////////////////////////////////

#define ALL( cont ) cont.begin(), cont.end()



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

bool TestTypes     (void);
bool TestTimer     (void);
bool TestKeyboard  (void);
bool TestMouse     (void);
bool TestProcess   (void);
bool TestWindow    (void);
bool TestMemory    (void);
bool TestTaskbar   (void);
bool TestScreen    (void);
bool TestClipboard (void);

#endif // ROBOT_TEST_H
