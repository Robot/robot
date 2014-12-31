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
	// Use for MSVC VLD leak testing
	#define VLD_AGGREGATE_DUPLICATES
	#include "vld.h"
#endif

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <fstream>
using std::fstream;
using std::ofstream;
using std::ifstream;

#include <string>
#include <cstring>
using std::string;
using std::strcmp;

#if (_MSC_VER != 1600)
	#include <thread>
	using std::thread;
#endif

#include <vector>
using std::vector;
using std::make_pair;

#include "../Source/Robot.h"
using namespace Robot;



//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define VERIFY( cond )				\
	if (!(cond))					\
	{								\
		cout << #cond " Failed\n"	\
			 << __FILE__ << ":"		\
			 << __LINE__ << "\n\n";	\
		return false;				\
	}



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

bool TestTypes    (void);
bool TestKeyboard (void);
bool TestMouse    (void);
bool TestTaskbar  (void);
bool TestProcess  (void);
bool TestWindow   (void);
bool TestModule   (void);
bool TestMemory   (void);
bool TestScreen   (void);

#endif // ROBOT_TEST_H
