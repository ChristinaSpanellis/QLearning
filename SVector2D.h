#ifndef S2DVECTOR_H
#define S2DVECTOR_H

#include <math.h>

/////////////////////////////////////////////////////////////////////
//
//	2D Vector structure and methods
//
/////////////////////////////////////////////////////////////////////
template <class T>
struct SVector2D
{
	T x, y;
	
	SVector2D(T a = 0.0f, T b = 0.0f):x(a),y(b){}


	//we need some overloaded operators
	SVector2D<T> &operator+=(const SVector2D<T> &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	SVector2D<T> &operator-=(const SVector2D<T> &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	SVector2D<T> &operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

  	SVector2D<T> &operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	SVector2D<T> &operator=(const SVector2D<T> &rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}
};

//overload the * operator
template <typename P>
SVector2D<P> operator*(const SVector2D<P> &lhs, P rhs)
{
  SVector2D<P> result(lhs);
  result *= rhs;
  return result;
}

template <typename P>
SVector2D<P> operator*(P lhs, const SVector2D<P> &rhs)
{
  SVector2D<P> result(rhs);
  result *= lhs;
  return result;
}

//overload the - operator
template <typename P>
SVector2D<P> operator-(const SVector2D<P> &lhs, const SVector2D<P> &rhs)
{
  SVector2D<P> result(lhs);
  result.x -= rhs.x;
  result.y -= rhs.y;
  
  return result;
}
//------------------------- Vec2DLength -----------------------------
//
//	returns the euclidian length of a 2D vector
//--------------------------------------------------------------------
template <typename P>
double Vec2DLength(const SVector2D<P> &v)
{
	return sqrt((double)v.x * (double)v.x + (double)v.y * (double)v.y);
}

//------------------------- Vec2DNormalize -----------------------------
//
//	normalizes a 2D Vector
//--------------------------------------------------------------------
template <typename P>
void Vec2DNormalize(SVector2D<P> &v)
{
	double vector_length = Vec2DLength<P>(v);

	v.x = v.x / vector_length;
	v.y = v.y / vector_length;
}

//------------------------- Vec2DDot --------------------------
//
//	calculates the dot product
//--------------------------------------------------------------------
template <typename P>
double Vec2DDot(SVector2D<P> &v1, SVector2D<P> &v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

//------------------------ Vec2DSign --------------------------------
//
//  returns positive if v2 is clockwise of v1, minus if anticlockwise
//-------------------------------------------------------------------
template <typename P>
int Vec2DSign(SVector2D<P> &v1, SVector2D<P> &v2)
{
  if (v1.y*v2.x > v1.x*v2.y)
  { 
    return 1;
  }
  else 
  {
    return -1;
  }
}

#endif