/**************************************************************************************
**
**  Copyright (C) 2006 Thomas Luft, University of Konstanz. All rights reserved.
**
**  This file is part of the Ivy Generator Tool.
**
**  This program is free software; you can redistribute it and/or modify it
**  under the terms of the GNU General Public License as published by the
**  Free Software Foundation; either version 2 of the License, or (at your
**  option) any later version.
**  This program is distributed in the hope that it will be useful, but
**  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
**  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
**  for more details.
**  You should have received a copy of the GNU General Public License along
**  with this program; if not, write to the Free Software Foundation,
**  Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA 
**
***************************************************************************************/


#ifndef VECTOR2D_H
#define VECTOR2D_H


#include <cmath>
#include <cstdlib>
#include <limits>


class Vector2d
{
public:

	float		x;
	float		y;

	Vector2d() : x(0.0f), y(0.0f) {};

	Vector2d( const float& x, const float& y ) : x(x), y(y) {};

	friend inline Vector2d operator-( const Vector2d& vector )
	{
		return Vector2d( -vector.x, -vector.y );
	}

	friend inline Vector2d operator-( const Vector2d& vector1, const Vector2d& vector2 )
	{
		return Vector2d( vector1.x - vector2.x, vector1.y - vector2.y );
	}

	friend inline Vector2d operator+( const Vector2d& vector1, const Vector2d& vector2 )
	{
		return Vector2d( vector1.x + vector2.x, vector1.y + vector2.y );
	}

	friend inline Vector2d operator*( const float &value, const Vector2d& vector )
	{
		return vector * value;
	}

	friend inline Vector2d operator*( const Vector2d& vector, const float &value )
	{
		return Vector2d( vector.x * value, vector.y * value );
	}

	friend inline Vector2d operator/( const Vector2d& vector, const float &value )
	{
		return Vector2d( vector.x / value, vector.y / value );
	}

	friend inline void operator+=( Vector2d& vector1, const Vector2d& vector2 )
	{
		vector1.x += vector2.x;
		vector1.y += vector2.y;
	}

	friend inline void operator-=( Vector2d& vector1, const Vector2d& vector2 )
	{
		vector1.x -= vector2.x;
		vector1.y -= vector2.y;
	}

	friend inline void operator*=( Vector2d& vector, const float &value )
	{
		vector.x *= value;
		vector.y *= value;
	}

	friend inline void operator/=( Vector2d& vector, const float &value )
	{
		vector.x /= value;
		vector.y /= value;
	}

	friend inline bool operator ==( Vector2d& vector1, const Vector2d& vector2 )
	{
		return (vector1.x == vector2.x) && (vector1.y == vector2.y);
	}

	friend inline bool operator !=( Vector2d& vector1, const Vector2d& vector2 )
	{
		return (vector1.x != vector2.x) || (vector1.y != vector2.y);
	}

	inline float length()
	{
		return ( (float)sqrt( this->x * this->x + this->y * this->y) );
	}

	inline void normalize()
	{
		float length = getLength(*this);

		if (length > 0.0f) *this /= length;
	}

	static inline float dotProduct( const Vector2d& vector1, const Vector2d& vector2 )
	{
		return ( ( vector1.x * vector2.x ) + ( vector1.y * vector2.y ) );
	}

	static inline float crossProduct( const Vector2d& vector1, const Vector2d& vector2 )
	{
		return vector1.x * vector2.y - vector1.y * vector2.x;
	}

	static inline float getLength( const Vector2d& vector )
	{
		return ( (float)sqrt( vector.x * vector.x + vector.y * vector.y) );
	}

	static inline Vector2d getNormalized( const Vector2d& vector )
	{
		float length = getLength( vector );

		if (length > 0.0f) return vector / length; else return vector;
	}	

	static inline float getAngle( const Vector2d& vector1, const Vector2d& vector2 )
	{
		return acos( dotProduct( vector1, vector2 ) / ( getLength( vector1 ) * getLength( vector2 ) ) );
	}

	static inline bool getBarycentricCoordinates( const Vector2d& vector1, const Vector2d& vector2, const Vector2d& vector3, const Vector2d& position, float& alpha, float& beta, float& gamma )
	{
		float area = 0.5f * abs( crossProduct( vector2 - vector1, vector3 - vector1 ) );

		alpha = 0.5f * abs( crossProduct( vector2 - position, vector3 - position ) ) / area;

		beta = 0.5f * abs( crossProduct( vector1 - position, vector3 - position ) ) / area;

		gamma = 0.5f * abs( crossProduct( vector1 - position, vector2 - position ) ) / area;

		if (abs( 1.0f - alpha - beta - gamma ) > 0.00001f) return false;

		return true;
	}
    
	static inline Vector2d getRandomized()
	{
		return Vector2d::getNormalized( Vector2d( rand()/(float)RAND_MAX-0.5f, rand()/(float)RAND_MAX-0.5f ) );
	}

	static inline Vector2d getEpsilon()
	{
		return Vector2d(std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon());
	}

	static inline float vectorToPolar( const Vector2d& vector )
	{
		float phi = (vector.x == 0.0f) ? 0.0f : atan( vector.y / vector.x );

		if ( vector.x < 0.0f )
		{
			phi += 3.1415926535897932384626433832795f;
		}
		else
		{
			if ( vector.y < 0.0f )
			{
				phi += 2.0f * 3.1415926535897932384626433832795f;
			}
		}

		return phi;
	}

	static inline Vector2d polarToVector(const float& phi)
	{
		return Vector2d( cos(phi), sin(phi) );
	}
};


#endif