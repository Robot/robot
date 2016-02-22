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

#pragma once



//----------------------------------------------------------------------------//
// Version                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// Defines the current version of Robot as an integer and string value.

#define ROBOT_VERSION		0x020000
#define ROBOT_VERSION_STR	"2.0.0"



//----------------------------------------------------------------------------//
// Platforms                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// Defines operating system and architecture identification macros. Operating
/// system macros are defined using the OS_* identifier while architectures are
/// defined using the ARCH_* identifier.

#if defined (__linux__)

	#define ROBOT_OS_LINUX
	#if defined (__x86_64__)
		#define ROBOT_ARCH_64
	#else
		#define ROBOT_ARCH_32
	#endif

#elif defined (__APPLE__) && defined (__MACH__)

	#define ROBOT_OS_MAC
	#if defined (__x86_64__)
		#define ROBOT_ARCH_64
	#else
		#define ROBOT_ARCH_32
	#endif

#elif defined (_WIN32)

	#define ROBOT_OS_WIN
	#if defined (_WIN64)
		#define ROBOT_ARCH_64
	#else
		#define ROBOT_ARCH_32
	#endif

#else

	#error Your operating system is not supported

#endif



//----------------------------------------------------------------------------//
// Export                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// When building Robot as a DLL, BUILDING_ROBOT_SHARED needs to be defined.
/// When building a program which uses the Robot DLL, USING_ROBOT_SHARED needs
/// to be defined. When building Robot as either a static library or building a
/// program which uses Robot as a static library, neither BUILDING_ROBOT_SHARED
/// nor USING_ROBOT_SHARED need to be defined.

#if defined (BUILDING_ROBOT_SHARED) && defined (USING_ROBOT_SHARED)
#error Both BUILDING_ROBOT_SHARED and USING_ROBOT_SHARED are defined - please \
	   check your build configuration to ensure that only one of these is set
#endif

#ifdef ROBOT_OS_WIN

	#if defined (BUILDING_ROBOT_SHARED)
		#define ROBOT_EXPORT __declspec (dllexport)
	#elif defined (USING_ROBOT_SHARED)
		#define ROBOT_EXPORT __declspec (dllimport)
	#else
		#define ROBOT_EXPORT
	#endif

#elif (__GNUC__ >= 4)
	#define ROBOT_EXPORT __attribute__ ((visibility ("default")))
#else
	#define ROBOT_EXPORT
#endif



//----------------------------------------------------------------------------//
// Namespace                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// This library requires namespace encapsulation to prevent naming conflicts
/// with system libraries. To use a different name, modify the ROBOT_NS macro.
/// The ROBOT_NS_* family of macros can be used to define and use namespaces
/// without depending on explicit names.

#ifndef ROBOT_NS
#define ROBOT_NS Robot
#endif

#ifdef ROBOT_NS

	#define ROBOT_NS_BEGIN			namespace ROBOT_NS {
	#define ROBOT_NS_END			}

	#define ROBOT_NS_PRE( name )	ROBOT_NS::name
	#define ROBOT_NS_FWD( name )	\
			ROBOT_NS_BEGIN			\
				class name;			\
			ROBOT_NS_END

	#define ROBOT_NS_USE_ALL		using namespace ROBOT_NS;
	#define ROBOT_NS_USE( name )	using ROBOT_NS::name;

#else

	#error This library must be encapsulated in a namespace

#endif



//----------------------------------------------------------------------------//
// Types                                                                      //
//----------------------------------------------------------------------------//

ROBOT_NS_BEGIN

typedef signed char			int8;		// Signed  8-Bit integer
typedef signed short		int16;		// Signed 16-Bit integer
typedef signed int			int32;		// Signed 32-Bit integer
typedef signed long long	int64;		// Signed 64-Bit integer

typedef unsigned char		uint8;		// Unsigned  8-Bit integer
typedef unsigned short		uint16;		// Unsigned 16-Bit integer
typedef unsigned int		uint32;		// Unsigned 32-Bit integer
typedef unsigned long long	uint64;		// Unsigned 64-Bit integer

typedef float				real32;		// 32-Bit float value
typedef double				real64;		// 64-Bit float value

#ifdef ROBOT_ARCH_64

	typedef  int64			 intptr;	//   Signed pointer integer
	typedef uint64			uintptr;	// Unsigned pointer integer

#else

	typedef  int32			 intptr;	//   Signed pointer integer
	typedef uint32			uintptr;	// Unsigned pointer integer

#endif

ROBOT_NS_END
