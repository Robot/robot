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

#include "Global.h"
ROBOT_NS_BEGIN
	class Size;
	class Point;



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Bounds
{
public:
	Bounds						(int32 value = 0);

	Bounds						(int32 x, int32 y,
								 int32 w, int32 h);

	Bounds						(const Point& p,
								 const  Size& s);

public:
	bool		IsZero			(void) const;
	bool		IsEmpty			(void) const;
	bool		IsValid			(void) const;

	int32		GetLeft			(void) const;
	int32		GetTop			(void) const;
	int32		GetRight		(void) const;
	int32		GetBottom		(void) const;

	void		SetLeft			(int32 l);
	void		SetTop			(int32 t);
	void		SetRight		(int32 r);
	void		SetBottom		(int32 b);

	void		GetLTRB			(int32& l, int32& t,
								 int32& r, int32& b);
	void		SetLTRB			(int32  l, int32  t,
								 int32  r, int32  b);

	void		Normalize		(void);

	bool		Contains		(const Point& p,
								 bool inclusive = true) const;
	bool		Contains		(int32 x, int32 y,
								 bool inclusive = true) const;

	bool		Contains		(const Bounds& b,
								 bool inclusive = true) const;
	bool		Contains		(int32 x, int32 y,
								 int32 w, int32 h,
								 bool inclusive = true) const;

	bool		Intersects		(const Bounds& b,
								 bool inclusive = true) const;
	bool		Intersects		(int32 x, int32 y,
								 int32 w, int32 h,
								 bool inclusive = true) const;

	Point		GetPoint		(void) const;
	void		SetPoint		(const    Point& p);
	void		SetPoint		(int32 x, int32  y);

	Size		GetSize			(void) const;
	void		SetSize			(const    Size& s);
	void		SetSize			(int32 w, int32 h);

	Point		GetCenter		(void) const;

public:
	Bounds&		operator |=		(const Bounds& bounds);
	Bounds&		operator &=		(const Bounds& bounds);

	Bounds		operator |		(const Bounds& bounds) const;
	Bounds		operator &		(const Bounds& bounds) const;

	bool		operator ==		(const Bounds& bounds) const;
	bool		operator !=		(const Bounds& bounds) const;

public:
	int32		X;				// Top-left X coordinate
	int32		Y;				// Top-left Y coordinate
	int32		W;				// Total bounds width
	int32		H;				// Total bounds height
};

ROBOT_NS_END
