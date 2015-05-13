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


#include "OBJLoader.h"
#include "BasicMesh.h"
#include <sstream>
#include <QtWidgets>


std::ifstream OBJLoader::fileStream;

BasicMaterial *OBJLoader::currentMaterial;



bool OBJLoader::loadOBJ( const std::string &path, const std::string &file, BasicMesh &model  )
{
	if ((path + file) == "") return true;

	fileStream.open( (path + file).c_str() );

	if (!fileStream.is_open())
	{
		QMessageBox::warning(NULL, "OBJ-Loader", "could not open file " + QString::fromStdString(path + file));

		return false;
	}

	model.file = file;

	model.path = path;


	//parse basic model

	currentMaterial = NULL;

	while (!fileStream.eof())
	{
		std::string tag;

		fileStream >> tag;

		if (tag == "mtllib")
		{
			parseOBJMaterialLibrary(model);
		}

		if (tag == "usemtl")
		{
			parseOBJMaterial(model);
		}

		if (tag == "v")
		{
			parseOBJVertex(model);
		}

		if (tag == "vn")
		{
			parseOBJNormal(model);
		}

		if (tag == "vt")
		{
			parseOBJTexCoord(model);
		}

		if (tag == "f")
		{
			parseOBJFace(model);
		}
	}

	fileStream.close();

	fileStream.clear();

	return true;
}



bool OBJLoader::parseOBJMaterialLibrary(BasicMesh &model)
{
	std::string mtllibFileName;

	fileStream >> mtllibFileName;


	//parse mtl file
	std::ifstream fileStream;

	fileStream.open( (model.path + mtllibFileName).c_str() );

	if (fileStream.is_open())
	{
		unsigned int materialCount = 1;

		
		while (!fileStream.eof())
		{
			std::string tag;

			fileStream >> tag;

			//parse material 
			if (tag == "newmtl")
			{
				model.materials.push_back( BasicMaterial() );
               
				model.materials.back().id = materialCount++;
				
				fileStream >> model.materials.back().name;
			}

			////parse ambient color
			//if (tag == "Ka")
			//{
			//	fileStream >> model.materials.back().ambientColor.r >> model.materials.back().ambientColor.g >> model.materials.back().ambientColor.b;
			//}

			////parse diffuse color
			//if (tag == "Kd")
			//{
			//	fileStream >> model.materials.back().diffuseColor.r >> model.materials.back().diffuseColor.g >> model.materials.back().diffuseColor.b;
			//}

			////parse specular color
			//if (tag == "Ks")
			//{
			//	fileStream >> model.materials.back().specularColor.r >> model.materials.back().specularColor.g >> model.materials.back().specularColor.b;
			//}

			//parse texture file
			if (tag == "map_Kd")
			{
                std::string texFileName;

				fileStream >> texFileName;

				model.materials.back().texFile = texFileName;				
			}
		}

		fileStream.close();

		return true;
	}
	else
	{
		QMessageBox::warning(NULL, "OBJ-Loader", "could not open material library " + QString::fromStdString(mtllibFileName));

		return false;
	}
}




void OBJLoader::parseOBJMaterial(BasicMesh &model)
{
	std::string mtlName;
	
	fileStream >> mtlName;


	for (std::vector<BasicMaterial>::iterator m = model.materials.begin(); m != model.materials.end(); ++m)
	{
		if (m->name == mtlName) currentMaterial = &(*m);
	}
}




void OBJLoader::parseOBJVertex(BasicMesh &model)
{
	BasicVertex vertex;

	fileStream >> vertex.pos.x >> vertex.pos.y >> vertex.pos.z;
	
	model.vertices.push_back(vertex);
}


void OBJLoader::parseOBJNormal(BasicMesh &model)
{
	BasicNormal normal;

	fileStream >> normal.dir.x >> normal.dir.y >> normal.dir.z;

	model.normals.push_back(normal);
}


void OBJLoader::parseOBJTexCoord(BasicMesh &model)
{
    BasicTexCoord texCoord;

	fileStream >> texCoord.pos.x >> texCoord.pos.y;

	model.texCoords.push_back(texCoord);
}


void OBJLoader::extractVertexInfo(std::string &v, unsigned int &vid, unsigned int &nid, unsigned int &tid)
{
	std::stringstream s(v);

	std::basic_string<char>::size_type A = v.find_first_of('/');

	std::basic_string<char>::size_type B = v.find_last_of('/');

	char tmpChar;


	if (A == std::basic_string<char>::npos)
	{
		s >> vid;
	}
	else
	{		
		if (A == B)
		{
			s >> vid >> tmpChar >> tid;
		}
		else
		{
			if (A+1 == B)
			{
				s >> vid >> tmpChar >> tmpChar >> nid;
			}
			else
			{
				s >> vid >> tmpChar >> tid >> tmpChar >> nid;
			}
		}
	}
}


void OBJLoader::parseOBJFace(BasicMesh &model)
{
    std::vector<std::string> vertices;
    std::string vertex;
    fileStream >> vertex;

    while(vertex != "f"
          && vertex != "v"
          && vertex != "vn"
          && vertex != "vt"
          && vertex !="mtllib"
          && vertex != "usemtl")
    {
        vertices.push_back(vertex);
        fileStream >> vertex;
    }

    fileStream.seekg(-1, std::ios::cur);

    std::string v0 = vertices[0];

    //face triangulation
    for(int i = 1; i < vertices.size() - 1; i += 2)
    {
        BasicTriangle triang;
        std::string v1 = vertices[i];
        std::string v2 = vertices[i+1];

        extractVertexInfo(v0, triang.v0id, triang.n0id, triang.t0id);
        extractVertexInfo(v1, triang.v1id, triang.n1id, triang.t1id);
        extractVertexInfo(v2, triang.v2id, triang.n2id, triang.t2id);

        if (currentMaterial != NULL)
            triang.matid = currentMaterial->id;
        else
            triang.matid = 0;

        model.triangles.push_back(triang);
    }
}

