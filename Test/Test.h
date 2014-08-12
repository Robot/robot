////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2014 Robot Developers                       //
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
using std::cout;
using std::endl;

#include <cstring>
using std::strcmp;

#include "../Source/Robot.h"
using namespace Robot;



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
