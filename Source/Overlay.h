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

#pragma once

#include "Types.h"
#include <memory>

ROBOT_NS_BEGIN

#ifdef ROBOT_OS_WIN
	#pragma warning (push)
	// Ignore the VS C4251 warning
	#pragma warning (disable:4251)
#endif



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

class ROBOT_EXPORT Overlay
{
public:
	explicit			Overlay			(const Bounds&  b);
	explicit			Overlay			(int32 x, int32 y,
										 int32 w, int32 h);

public:
	bool				IsValid			(void) const;

	bool				Create			(const Bounds&  b);
	bool				Create			(int32 x, int32 y,
										 int32 w, int32 h);
	void				Destroy			(void);

	bool				 IsTopMost		(void) const;
	void				SetTopMost		(bool state);

	real32				GetOpacity		(void) const;
	void				SetOpacity		(real32 opacity);

	bool				 IsVisible		(void) const;
	void				SetVisible		(bool visible);

	uintptr				GetHandle		(void) const;

	Bounds				GetClient		(void) const;
	void				SetClient		(const Bounds&  b);
	void				SetClient		(int32 x, int32 y,
										 int32 w, int32 h);

	Point				MapToClient		(const  Point&  point) const;
	Point				MapToScreen		(const  Point&  point) const;
	Bounds				MapToClient		(const Bounds& bounds) const;
	Bounds				MapToScreen		(const Bounds& bounds) const;

	void				Clear			(const Color& color = 0);

	void				Clear			(const Bounds& b,
										 const Color & color = 0);

	void				Clear			(int32 x, int32 y,
										 int32 w, int32 h,
										 const Color& color = 0);

	void				Render			(const Image& image,
										 const Point& target = 0);

	void				Render			(const Image & image,
										 const Bounds& source,
										 const Point & target = 0);

public:
	bool				operator ==		(const Overlay& overlay) const;
	bool				operator !=		(const Overlay& overlay) const;

	bool				operator ==		(uintptr handle) const;
	bool				operator !=		(uintptr handle) const;

private:
	struct Data;
	std::shared_ptr<Data> mData;		// Shared information
};

#ifdef ROBOT_OS_WIN
	#pragma warning (pop)
#endif

ROBOT_NS_END
