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

#include "Timer.h"
#ifdef ROBOT_OS_LINUX

	#include <time.h>
	#include <unistd.h>

#endif
#ifdef ROBOT_OS_MAC

	#include <unistd.h>
	#include <mach/mach_time.h>

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

////////////////////////////////////////////////////////////////////////////////

static const uint64 gInvalid
		= 0x8000000000000000;



//----------------------------------------------------------------------------//
// Constructors                                                         Timer //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Timer::Timer (void)
{
	mStarted = gInvalid;
}



//----------------------------------------------------------------------------//
// Functions                                                            Timer //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void Timer::Start (void)
{
	mStarted = GetCpuTime();
}

////////////////////////////////////////////////////////////////////////////////

uint64 Timer::Reset (void)
{
	// Check if timer started
	if (mStarted == gInvalid)
		return 0;

	else
	{
		auto old = mStarted;
		mStarted = gInvalid;
		return GetCpuTime() - old;
	}
}

////////////////////////////////////////////////////////////////////////////////

uint64 Timer::Restart (void)
{
	// Check if timer started
	if (mStarted == gInvalid)
	{
		mStarted = GetCpuTime();
		return 0;
	}

	else
	{
		auto old = mStarted;
		mStarted = GetCpuTime();
		return mStarted - old;
	}
}

////////////////////////////////////////////////////////////////////////////////

uint64 Timer::GetElapsed (void) const
{
	// Check if timer started
	if (mStarted == gInvalid)
		return 0;

	return GetCpuTime() - mStarted;
}

////////////////////////////////////////////////////////////////////////////////

bool Timer::HasStarted (void) const
{
	return mStarted != gInvalid;
}

////////////////////////////////////////////////////////////////////////////////

bool Timer::HasExpired (uint64 time) const
{
	// Check if timer started
	if (mStarted == gInvalid)
		return true;

	return GetElapsed() > time;
}

////////////////////////////////////////////////////////////////////////////////

void Timer::Sleep (const Range& time)
{
	// Generate a random range
	int32 d = time.GetRandom();

	// Skip negatives
	if (d < 0) return;

#ifdef ROBOT_OS_LINUX

	usleep (d * 1000);

#endif
#ifdef ROBOT_OS_MAC

	usleep (d * 1000);

#endif
#ifdef ROBOT_OS_WIN

	::Sleep (d);

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Timer::Sleep
	(uint32 minimum, uint32 maximum)
{
	Sleep (Range (minimum, maximum));
}

////////////////////////////////////////////////////////////////////////////////

uint64 Timer::GetCpuTime (void)
{
#ifdef ROBOT_OS_LINUX

	// Gets the current time and returns it in milliseconds
	timespec time; clock_gettime  (CLOCK_MONOTONIC, &time);
	return (time.tv_sec * 1000) + (time.tv_nsec / 1000000);

#endif
#ifdef ROBOT_OS_MAC

	// Initialize the mach timebase info one time
	static mach_timebase_info_data_t info = { 0 };
	if (info.denom == 0) mach_timebase_info (&info);

	// Retrieve current absolute time
	uint64 time = mach_absolute_time();

	// Return the current absolute time in milliseconds
	return (time * info.numer) / (info.denom * 1000000);

#endif
#ifdef ROBOT_OS_WIN

	// Initialize timer info once
	static uint64 frequency = 0;
	static bool initialized = false;

	if (!initialized)
	{
		// Get performance counter frequency
		initialized = true; LARGE_INTEGER f;
		if (QueryPerformanceFrequency (&f))
			frequency = (uint64) f.QuadPart;
	}

	if (frequency > 0)
	{
		// Retrieve performance counter value
		uint64 time = 0; LARGE_INTEGER counter;
		if (QueryPerformanceCounter (&counter))
			time = (uint64) counter.QuadPart;

		// Return result in milliseconds
		return (time * 1000) / frequency;
	}

	// Should never happen
	return GetTickCount64();

#endif
}



//----------------------------------------------------------------------------//
// Operators                                                            Timer //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

uint64 Timer::operator () (void) const
{
	// Check if timer started
	if (mStarted == gInvalid)
		return 0;

	return GetCpuTime() - mStarted;
}

////////////////////////////////////////////////////////////////////////////////

bool Timer::operator < (const Timer& timer) const
{
	if (timer.mStarted == gInvalid) return false;
	if (this->mStarted == gInvalid) return true;
	return mStarted > timer.mStarted;
}

////////////////////////////////////////////////////////////////////////////////

bool Timer::operator > (const Timer& timer) const
{
	if (this->mStarted == gInvalid) return false;
	if (timer.mStarted == gInvalid) return true;
	return mStarted < timer.mStarted;
}

////////////////////////////////////////////////////////////////////////////////

bool Timer::operator <= (const Timer& timer) const
{
	if (this->mStarted == gInvalid) return true;
	if (timer.mStarted == gInvalid) return false;
	return mStarted >= timer.mStarted;
}

////////////////////////////////////////////////////////////////////////////////

bool Timer::operator >= (const Timer& timer) const
{
	if (timer.mStarted == gInvalid) return true;
	if (this->mStarted == gInvalid) return false;
	return mStarted <= timer.mStarted;
}

////////////////////////////////////////////////////////////////////////////////

bool Timer::operator == (const Timer& timer) const
{
	return mStarted == timer.mStarted;
}

////////////////////////////////////////////////////////////////////////////////

bool Timer::operator != (const Timer& timer) const
{
	return mStarted != timer.mStarted;
}

ROBOT_NS_END
