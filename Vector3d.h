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


#ifndef VECTOR3D_H
#define VECTOR3D_H


#include <cmath>
#include <cstdlib>
#include <limits>


class Vector3d
{
public:

	float		x;
	float		y;
	float		z;

	Vector3d() : x(0.0f), y(0.0f), z(0.0f) {};

	Vector3d( const float &x, const float &y, const float &z ) : x(x), y(y), z(z) {};

	friend inline Vector3d operator-( const Vector3d& vector )
	{
		return Vector3d( -vector.x, -vector.y, -vector.z );
	}

	friend inline Vector3d operator-( const Vector3d& vector1, const Vector3d& vector2 )
	{
		return Vector3d( vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z );
	}

	friend inline Vector3d operator+( const Vector3d& vector1, const Vector3d& vector2 )
	{
		return Vector3d( vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z );
	}

	friend inline Vector3d operator*( const float &value, const Vector3d& vector )
	{
		return vector * value;
	}

	friend inline Vector3d operator*( const Vector3d& vector, const float &value )
	{
		return Vector3d( vector.x * value, vector.y * value, vector.z * value );
	}

	friend inline Vector3d operator/( const Vector3d& vector, const float &value )
	{
		return Vector3d( vector.x / value, vector.y / value, vector.z / value );
	}

	friend inline void operator+=( Vector3d& vector1, const Vector3d& vector2 )
	{
		vector1.x += vector2.x;
		vector1.y += vector2.y;
		vector1.z += vector2.z;
	}

	friend inline void operator-=( Vector3d& vector1, const Vector3d& vector2 )
	{
		vector1.x -= vector2.x;
		vector1.y -= vector2.y;
		vector1.z -= vector2.z;
	}

	friend inline void operator*=( Vector3d& vector, const float &value )
	{
		vector.x *= value;
		vector.y *= value;
		vector.z *= value;
	}

	friend inline void operator/=( Vector3d& vector, const float &value )
	{
		vector.x /= value;
		vector.y /= value;
		vector.z /= value;
	}

	friend inline bool operator ==( Vector3d& vector1, const Vector3d& vector2 )
	{
		return (vector1.x == vector2.x) && (vector1.y == vector2.y) && (vector1.z == vector2.z);
	}

	friend inline bool operator !=( Vector3d& vector1, const Vector3d& vector2 )
	{
		return (vector1.x != vector2.x) || (vector1.y != vector2.y) || (vector1.z != vector2.z);
	}

	inline float length()
	{
		return ( (float)sqrt( this->x * this->x + this->y * this->y + this->z * this->z ) );
	}

	inline void normalize()
	{
		float length = getLength(*this);

		if (length > 0.0f) *this /= length;
	}

	static inline float dotProduct( const Vector3d& vector1, const Vector3d& vector2 )
	{
		return ( ( vector1.x * vector2.x ) + ( vector1.y * vector2.y ) + ( vector1.z * vector2.z ) );
	}

	static inline Vector3d crossProduct( const Vector3d& vector1, const Vector3d& vector2 )
	{
		return Vector3d( vector1.y * vector2.z - vector1.z * vector2.y,
						 vector1.z * vector2.x - vector1.x * vector2.z,
						 vector1.x * vector2.y - vector1.y * vector2.x );
	}

	static inline float getLength( const Vector3d& vector )
	{
		return ( (float)sqrt( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z ) );
	}

	static inline Vector3d getNormalized( const Vector3d& vector )
	{
		float length = getLength( vector );

		if (length > 0.0f) return vector / length; else return vector;
	}

	static inline float getAngle( const Vector3d& vector1, const Vector3d& vector2 )
	{
		float length1 = getLength( vector1 );

		float length2 = getLength( vector2 );

		if ((length1 == 0) || (length2 == 0)) return 0.0f;
	
		return acos( dotProduct( vector1, vector2 ) / ( length1 * length2 ) );
	}

	static inline bool getBarycentricCoordinates( const Vector3d& vector1, const Vector3d& vector2, const Vector3d& vector3, const Vector3d& position, float& alpha, float& beta, float& gamma )
	{
		float area = 0.5f * getLength( crossProduct( vector2 - vector1, vector3 - vector1 ) );

		alpha = 0.5f * getLength( crossProduct( vector2 - position, vector3 - position ) ) / area;

		beta = 0.5f * getLength( crossProduct( vector1 - position, vector3 - position ) ) / area;

		gamma = 0.5f * getLength( crossProduct( vector1 - position, vector2 - position ) ) / area;

		//if (abs( 1.0f - alpha - beta - gamma ) > std::numeric_limits<float>::epsilon()) return false;
		if (abs( 1.0f - alpha - beta - gamma ) > 0.00001f) return false;

		return true;
	}

	static inline Vector3d rotateX( const Vector3d& vector, const float& angle )
	{
		float cosTheta = cos( angle );
		float sinTheta = sin( angle );

		return rotateX( vector, cosTheta, sinTheta );

	}

	static inline Vector3d rotateX( const Vector3d& vector, const float& cosTheta, const float& sinTheta )
	{
		return Vector3d( vector.x,
						 vector.y * cosTheta - vector.z * sinTheta, 
						 vector.z * cosTheta + vector.y * sinTheta );
	}

	static inline Vector3d rotateY( const Vector3d& vector, const float& angle )
	{
		float cosTheta = cos( angle );
		float sinTheta = sin( angle );

		return rotateY( vector, cosTheta, sinTheta );
	}

	static inline Vector3d rotateY( const Vector3d& vector, const float& cosTheta, const float& sinTheta )
	{
		return Vector3d( vector.x * cosTheta + vector.z * sinTheta,
						 vector.y,
						 vector.z * cosTheta - vector.x * sinTheta );
	}

	static inline Vector3d rotateZ( const Vector3d& vector, const float& angle )
	{
		float cosTheta = cos( angle );
		float sinTheta = sin( angle );

		return rotateZ( vector, cosTheta, sinTheta );
	}

	static inline Vector3d rotateZ( const Vector3d& vector, const float& cosTheta, const float& sinTheta )
	{
		return Vector3d( vector.x * cosTheta - vector.y * sinTheta,
						 vector.y * cosTheta + vector.x * sinTheta,
						 vector.z );
	}

	static inline Vector3d rotateAroundAxis( const Vector3d& vector, const Vector3d& axisPosition, const Vector3d& axis, const float& angle )
	{
		//determining the sinus and cosinus of the rotation angle
		float cosTheta = cos( angle );
		float sinTheta = sin( angle );

		//Vector3 from the given axis point to the initial point
		Vector3d direction = vector - axisPosition;

		//new vector which will hold the direction from the given axis point to the new rotated point 
		Vector3d newDirection;

		//x-component of the direction from the given axis point to the rotated point
		newDirection.x = ( cosTheta + ( 1 - cosTheta ) * axis.x * axis.x ) * direction.x +
						 ( ( 1 - cosTheta ) * axis.x * axis.y - axis.z * sinTheta ) * direction.y +
						 ( ( 1 - cosTheta ) * axis.x * axis.z + axis.y * sinTheta ) * direction.z;

		//y-component of the direction from the given axis point to the rotated point
		newDirection.y = ( ( 1 - cosTheta ) * axis.x * axis.y + axis.z * sinTheta ) * direction.x +
						 ( cosTheta + ( 1 - cosTheta ) * axis.y * axis.y ) * direction.y +
						 ( ( 1 - cosTheta ) * axis.y * axis.z - axis.x * sinTheta ) * direction.z;

		//z-component of the direction from the given axis point to the rotated point
		newDirection.z = ( ( 1 - cosTheta ) * axis.x * axis.z - axis.y * sinTheta ) * direction.x +
						 ( ( 1 - cosTheta ) * axis.y * axis.z + axis.x * sinTheta ) * direction.y +
						 ( cosTheta + ( 1 - cosTheta ) * axis.z * axis.z) * direction.z;

		//returning the result by addind the new direction vector to the given axis point
		return axisPosition + newDirection;
	}

	static inline Vector3d getRandomized()
	{
		return Vector3d::getNormalized( Vector3d( rand()/(float)RAND_MAX-0.5f, rand()/(float)RAND_MAX-0.5f, rand()/(float)RAND_MAX-0.5f ) );
	}

	static inline Vector3d getEpsilon()
	{
		return Vector3d(std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon());
	}

};


#endif