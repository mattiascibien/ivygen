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


#ifndef OBJLOADER_H
#define OBJLOADER_H


#include <string>
#include <fstream>
#include "BasicMesh.h"


/** a class for loading an OBJ file into a Mesh object */
class OBJLoader  
{

public:

	/** loads OBJ data from a file and stores it within a Mesh object */
	static bool loadOBJ( const std::string &path, const std::string &file, BasicMesh &model );

private:

	static bool parseOBJMaterialLibrary( BasicMesh &modelObj );

	static void parseOBJMaterial( BasicMesh &modelObj );

	static void parseOBJVertex( BasicMesh &modelObj );

	static void parseOBJNormal( BasicMesh &modelObj );

	static void parseOBJTexCoord( BasicMesh &modelObj );

	static void parseOBJFace( BasicMesh &modelObj );

	static void extractVertexInfo(std::string &v, unsigned int &vid, unsigned int &nid, unsigned int &tid);

private:

	/** the associated filestream */
	static std::ifstream fileStream;

	/** the currently used material when a face section is parsed */
	static BasicMaterial *currentMaterial;
};

#endif