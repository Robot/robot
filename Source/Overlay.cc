////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2017 Robot Developers                       //
//                       See LICENSE for licensing info                       //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
// Prefaces                                                                   //
//----------------------------------------------------------------------------//

#include "Overlay.h"
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Classes                                                            Overlay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

struct Overlay::Data
{
	// TODO:
};



//----------------------------------------------------------------------------//
// Constructors                                                       Overlay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Overlay::Overlay (const Bounds& bounds)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

////////////////////////////////////////////////////////////////////////////////

Overlay::Overlay (int32 x, int32 y,
				  int32 w, int32 h)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}



//----------------------------------------------------------------------------//
// Functions                                                          Overlay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Overlay::IsValid (void) const
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return false;

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Overlay::Create (const Bounds& bounds)
{
	return Create (bounds.X, bounds.Y,
				   bounds.W, bounds.H);
}

////////////////////////////////////////////////////////////////////////////////

bool Overlay::Create (int32 x,
	int32 y, int32 w, int32 h)
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return false;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::Destroy (void)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Overlay::IsTopMost (void) const
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return false;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::SetTopMost (bool state)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

////////////////////////////////////////////////////////////////////////////////

real32 Overlay::GetOpacity (void) const
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::SetOpacity (real32 opacity)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

////////////////////////////////////////////////////////////////////////////////

bool Overlay::IsVisible (void) const
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return false;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return false;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::SetVisible (bool visible)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

////////////////////////////////////////////////////////////////////////////////

uintptr Overlay::GetHandle (void) const
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return 0;

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return 0;

#endif
}

////////////////////////////////////////////////////////////////////////////////

Bounds Overlay::GetClient (void) const
{
#ifdef ROBOT_OS_LINUX

	// TODO:
	return Bounds();

#endif
#ifdef ROBOT_OS_MAC

	// TODO:
	return Bounds();

#endif
#ifdef ROBOT_OS_WIN

	// TODO:
	return Bounds();

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::SetClient (const Bounds& bounds)
{
	SetClient (bounds.X, bounds.Y,
			   bounds.W, bounds.H);
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::SetClient (int32 x,
	   int32 y, int32 w, int32 h)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

////////////////////////////////////////////////////////////////////////////////

Point Overlay::MapToClient (const Point& point) const
{
	// Check if the overlay is valid
	if (!IsValid()) return Point();
	return point - GetClient().GetPoint();
}

////////////////////////////////////////////////////////////////////////////////

Point Overlay::MapToScreen (const Point& point) const
{
	// Check if the overlay is valid
	if (!IsValid()) return Point();
	return point + GetClient().GetPoint();
}

////////////////////////////////////////////////////////////////////////////////

Bounds Overlay::MapToClient (const Bounds& bounds) const
{
	// Check if the overlay is valid
	if (!IsValid()) return Bounds();
	return Bounds (bounds.GetPoint() - GetClient().GetPoint(), bounds.GetSize());
}

////////////////////////////////////////////////////////////////////////////////

Bounds Overlay::MapToScreen (const Bounds& bounds) const
{
	// Check if the overlay is valid
	if (!IsValid()) return Bounds();
	return Bounds (bounds.GetPoint() + GetClient().GetPoint(), bounds.GetSize());
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::Clear (const Color& color)
{
	auto bounds = GetClient();
	Clear (bounds.X, bounds.Y,
		   bounds.W, bounds.H, color);
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::Clear (const Bounds& bounds,
					 const Color & color)
{
	Clear (bounds.X, bounds.Y,
		   bounds.W, bounds.H, color);
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::Clear (int32 x, int32 y,
					 int32 w, int32 h,
					 const Color& color)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::Render (const Image& image,
					  const Point& target)
{
#ifdef ROBOT_OS_LINUX

	// TODO:

#endif
#ifdef ROBOT_OS_MAC

	// TODO:

#endif
#ifdef ROBOT_OS_WIN

	// TODO:

#endif
}

////////////////////////////////////////////////////////////////////////////////

void Overlay::Render (const Image & image,
					  const Bounds& source,
					  const Point & target)
{
	// TODO: Requires merging #82
}



//----------------------------------------------------------------------------//
// Operators                                                          Overlay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Overlay::operator == (const Overlay& overlay) const
{
	return GetHandle() == overlay.GetHandle();
}

////////////////////////////////////////////////////////////////////////////////

bool Overlay::operator != (const Overlay& overlay) const
{
	return GetHandle() != overlay.GetHandle();
}

////////////////////////////////////////////////////////////////////////////////

bool Overlay::operator == (uintptr handle) const
{
	return GetHandle() == handle;
}

////////////////////////////////////////////////////////////////////////////////

bool Overlay::operator != (uintptr handle) const
{
	return GetHandle() != handle;
}

ROBOT_NS_END
