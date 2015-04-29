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


#ifndef COMMON_H
#define COMMON_H


#include "BasicMesh.h"
#include "Camera.h"
#include "Ivy.h"
#include "RenderWidget.h"
#include "SetupWidget.h"


/** general class for global object and attributes */
class Common
{

public:

	/** the scene mesh that the ivy could climb */
	static BasicMesh mesh;

	/** the general camera */
	static Camera camera;

	/** the ivy object, derived from BasicMesh */
	static Ivy ivy;

	/** the opengl rendering widget */
	static RenderWidget *renderWidget;

	/** the setup widget right-hand */
	static SetupWidget *setupWidget;
};


#endif


