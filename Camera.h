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


#ifndef CAMERA_H
#define CAMERA_H


#define PI 3.1415926535897932384626433832795

#include "Vector3d.h"
#include "BasicMesh.h"


/** a simple camera class providing camera position and alignment */
class Camera
{

public:

	Camera();

	~Camera();

	/** computes a vector basis (dir, right, up) of the current alignment (head, pitch, bank) */
	void computeVectorBasis(Vector3d &dir, Vector3d &right, Vector3d &up);

	/** initializes the perspective matrix */
	void setupPerspectiveMatrix();

	/** initializes the view matrix */
	void setupViewMatrix();

	/** initializes the camera according to a mesh*/
	void placeNicely(const BasicMesh& mesh);

public:

	/** position */
	Vector3d pos;

	/** heading angle */
	float head;

	/** pitching angle */
	float pitch;

	/** banking angle */
	float bank;

	/** pivot center */
	Vector3d pivot;

	/** near clipping plane */
	float clipNear;

	/** far clipping plane */
	float clipFar;

	/** vertical field of view, in degree */
	float fovy;

	/** ascpect ration of witdh over height */
	float aspectRatio;
};


#endif


