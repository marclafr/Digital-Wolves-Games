// ----------------------------------------------------
// Point class    -----------
// ----------------------------------------------------

#ifndef __C2POINT_H__
#define __C2POINT_H__

#include "c2Defs.h"
#include <math.h>

template<class TYPE>
class c2Point
{
public:

	TYPE x, y;

	c2Point()
	{}

	c2Point(const c2Point<TYPE>& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	c2Point(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	c2Point& create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	c2Point operator -(const c2Point &v) const
	{
		c2Vector2 r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	c2Point operator + (const c2Point &v) const
	{
		c2Vector2 r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const c2Point& operator -=(const c2Point &v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const c2Point& operator +=(const c2Point &v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const c2Point& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const c2Point& v) const
	{
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	c2Point& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	c2Point& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const c2Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return sqrtf((fx*fx) + (fy*fy));
	}

	TYPE DistanceNoSqrt(const c2Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx*fx) + (fy*fy);
	}

	TYPE DistanceManhattan(const c2Point& v) const
	{
		return abs(v.x - x) + abs(v.y - y);
	}
};

typedef c2Point<int> iPoint;
typedef c2Point<float> fPoint;

#endif // __C2POINT_H__