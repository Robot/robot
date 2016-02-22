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

#include "Bounds.h"
#include "Size.h"
#include "Point.h"

#include <algorithm>
using std::min;
using std::max;

ROBOT_NS_BEGIN



//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define NORM( l, r, t, b, x, y, w, h )	\
	int32 l = x, r = x, t = y, b = y;	\
	if (w < 0) l += w; else r += w;		\
	if (h < 0) t += h; else b += h;



//----------------------------------------------------------------------------//
// Constructors                                                        Bounds //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Bounds::Bounds (int32 value)
{
	X = value;
	Y = value;
	W = value;
	H = value;
}

////////////////////////////////////////////////////////////////////////////////

Bounds::Bounds (int32 x, int32 y, int32 w, int32 h)
{
	X = x;
	Y = y;
	W = w;
	H = h;
}

////////////////////////////////////////////////////////////////////////////////

Bounds::Bounds (const Point& p, const Size& s)
{
	X = p.X;
	Y = p.Y;
	W = s.W;
	H = s.H;
}



//----------------------------------------------------------------------------//
// Functions                                                           Bounds //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Bounds::IsZero (void) const
{
	return X == 0 && Y == 0 &&
		   W == 0 && H == 0;
}

////////////////////////////////////////////////////////////////////////////////

bool Bounds::IsEmpty (void) const
{
	return W == 0 || H == 0;
}

////////////////////////////////////////////////////////////////////////////////

bool Bounds::IsValid (void) const
{
	return W > 0 && H > 0;
}

////////////////////////////////////////////////////////////////////////////////

int32 Bounds::GetLeft (void) const
{
	return X;
}

////////////////////////////////////////////////////////////////////////////////

int32 Bounds::GetTop (void) const
{
	return Y;
}

////////////////////////////////////////////////////////////////////////////////

int32 Bounds::GetRight (void) const
{
	return X + W;
}

////////////////////////////////////////////////////////////////////////////////

int32 Bounds::GetBottom (void) const
{
	return Y + H;
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::SetLeft (int32 l)
{
	X = l;
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::SetTop (int32 t)
{
	Y = t;
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::SetRight (int32 r)
{
	W = r - X;
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::SetBottom (int32 b)
{
	H = b - Y;
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::GetLTRB (int32& l,
  int32& t, int32& r, int32& b)
{
	l = X;
	t = Y;
	r = X + W;
	b = Y + H;
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::SetLTRB (int32 l,
	int32 t, int32 r, int32 b)
{
	X = l;
	Y = t;
	W = r - l;
	H = b - t;
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::Normalize (void)
{
	if (W < 0) { X += W; W = -W; }
	if (H < 0) { Y += H; H = -H; }
}

////////////////////////////////////////////////////////////////////////////////

bool Bounds::Contains (const Point& p, bool inclusive) const
{
	return Contains (p.X, p.Y, inclusive);
}

////////////////////////////////////////////////////////////////////////////////

bool Bounds::Contains (int32 x, int32 y, bool inclusive) const
{
	// Normalize negative rectangle
	NORM (l, r, t, b, X, Y, W, H);

	return inclusive ?
		l <= x && x <= r && t <= y && y <= b :
		l <  x && x <  r && t <  y && y <  b;
}

////////////////////////////////////////////////////////////////////////////////

bool Bounds::Contains (const Bounds& b, bool inclusive) const
{
	return Contains (b.X, b.Y, b.W, b.H, inclusive);
}

////////////////////////////////////////////////////////////////////////////////

bool Bounds::Contains (int32 x, int32 y,
	int32 w, int32 h, bool inclusive) const
{
	if ((W == 0 && H == 0) || (w == 0 && h == 0))
		return false;

	// Normalize negative rectangles
	NORM (l1, r1, t1, b1, X, Y, W, H);
	NORM (l2, r2, t2, b2, x, y, w, h);

	return inclusive ?
		l1 <= l2 && r1 >= r2 && t1 <= t2 && b1 >= b2 :
		l1 <  l2 && r1 >  r2 && t1 <  t2 && b1 >  b2;
}

////////////////////////////////////////////////////////////////////////////////

bool Bounds::Intersects (const Bounds& b, bool inclusive) const
{
	return Intersects (b.X, b.Y, b.W, b.H, inclusive);
}

////////////////////////////////////////////////////////////////////////////////

bool Bounds::Intersects (int32 x, int32 y,
	int32 w, int32 h, bool inclusive) const
{
	if ((W == 0 && H == 0) || (w == 0 && h == 0))
		return false;

	// Normalize negative rectangles
	NORM (l1, r1, t1, b1, X, Y, W, H);
	NORM (l2, r2, t2, b2, x, y, w, h);

	return inclusive ?
		l1 <= r2 && r1 >= l2 && t1 <= b2 && b1 >= t2 :
		l1 <  r2 && r1 >  l2 && t1 <  b2 && b1 >  t2;
}

////////////////////////////////////////////////////////////////////////////////

Point Bounds::GetPoint (void) const
{
	return Point (X, Y);
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::SetPoint (const Point& p)
{
	X = p.X;
	Y = p.Y;
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::SetPoint (int32 x, int32 y)
{
	X = x;
	Y = y;
}

////////////////////////////////////////////////////////////////////////////////

Size Bounds::GetSize (void) const
{
	return Size (W, H);
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::SetSize (const Size& s)
{
	W = s.W;
	H = s.H;
}

////////////////////////////////////////////////////////////////////////////////

void Bounds::SetSize (int32 w, int32 h)
{
	W = w;
	H = h;
}

////////////////////////////////////////////////////////////////////////////////

Point Bounds::GetCenter (void) const
{
	return Point
		(X + (int32) (W * 0.5f),
		 Y + (int32) (H * 0.5f));
}



//----------------------------------------------------------------------------//
// Operators                                                           Bounds //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Bounds& Bounds::operator |= (const Bounds& bounds)
{
	return *this = *this | bounds;
}

////////////////////////////////////////////////////////////////////////////////

Bounds& Bounds::operator &= (const Bounds& bounds)
{
	return *this = *this & bounds;
}

////////////////////////////////////////////////////////////////////////////////

Bounds Bounds::operator | (const Bounds& bounds) const
{
	int32 x = bounds.X;
	int32 y = bounds.Y;
	int32 w = bounds.W;
	int32 h = bounds.H;

	Bounds result;
	// Normalize negative rectangles
	NORM (l1, r1, t1, b1, X, Y, W, H);
	NORM (l2, r2, t2, b2, x, y, w, h);

	if (W == 0 && H == 0)
	{
		result.SetLTRB (l2, t2, r2, b2);
		return result;
	}

	if (w == 0 && h == 0)
	{
		result.SetLTRB (l1, t1, r1, b1);
		return result;
	}

	result.SetLTRB
		(min (l1, l2), min (t1, t2),
		 max (r1, r2), max (b1, b2));
	return result;
}

////////////////////////////////////////////////////////////////////////////////

Bounds Bounds::operator & (const Bounds& bounds) const
{
	int32 x = bounds.X;
	int32 y = bounds.Y;
	int32 w = bounds.W;
	int32 h = bounds.H;

	if ((W == 0 && H == 0) || (w == 0 && h == 0))
		return Bounds();

	// Normalize negative rectangles
	NORM (l1, r1, t1, b1, X, Y, W, H);
	NORM (l2, r2, t2, b2, x, y, w, h);

	// Check for bounds intersection
	if (l1 > r2 || r1 < l2 || t1 > b2 || b1 < t2)
		return Bounds();

	Bounds result;
	result.SetLTRB (max (l1, l2), max (t1, t2),
					min (r1, r2), min (b1, b2));
	return result;
}

////////////////////////////////////////////////////////////////////////////////

bool Bounds::operator == (const Bounds& bounds) const
{
	return X == bounds.X && Y == bounds.Y &&
		   W == bounds.W && H == bounds.H;
}

////////////////////////////////////////////////////////////////////////////////

bool Bounds::operator != (const Bounds& bounds) const
{
	return X != bounds.X || Y != bounds.Y ||
		   W != bounds.W || H != bounds.H;
}

ROBOT_NS_END
