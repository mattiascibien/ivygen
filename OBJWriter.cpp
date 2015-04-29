/*************************************************************************************
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


#include "OBJWriter.h"
#include <QtWidgets>
#include <iostream>
#include <fstream>



bool OBJWriter::writeOBJ(const std::string& path, const std::string& file, BasicMesh& model)
{
	//open file
	std::ofstream fileStream;

	fileStream.open( (path + file).c_str() );
	
	if (!fileStream.is_open())
	{
		QMessageBox::warning(NULL, "OBJ-Loader", "could not open file " + QString::fromStdString(path + file));

		return false;
	}


    //export material library
	std::string	matlibFileName = file.substr(0, file.size()-3) + "mtl";

	if (model.materials.size() != 0)
	{
		fileStream << "mtllib " << matlibFileName << "\n";

		std::ofstream matlibStream;
		
		matlibStream.open( (path + matlibFileName).c_str() );

		for (std::vector<BasicMaterial>::const_iterator m = model.materials.begin(); m != model.materials.end(); ++m)
		{
			matlibStream << "newmtl " << m->name << "\n";

			matlibStream << "map_Kd " << m->texFile << "\n\n";
		}

		matlibStream.flush();

		matlibStream.close();

		matlibStream.clear();
	}
	


	//export vertices
	for (std::vector<BasicVertex>::const_iterator v = model.vertices.begin(); v != model.vertices.end(); ++v)
	{
		fileStream << "v " << v->pos.x << " " << v->pos.y << " " << v->pos.z << "\n";
	}


	//export normals
	for (std::vector<BasicNormal>::const_iterator n = model.normals.begin(); n != model.normals.end(); ++n)
	{
		fileStream << "vn " << n->dir.x << " " << n->dir.y << " " << n->dir.z << "\n";
	}

	
	//export texCoords
	for (std::vector<BasicTexCoord>::const_iterator t = model.texCoords.begin(); t != model.texCoords.end(); ++t)
	{
		fileStream << "vt " << t->pos.x << " " << t->pos.y << "\n";
	}


	//export triangles
	if (model.materials.size() == 0)
	{
		for (std::vector<BasicTriangle>::const_iterator t = model.triangles.begin(); t != model.triangles.end(); ++t)
		{
			if ((model.texCoords.size() == 0) && (model.normals.size() == 0))
			{
                fileStream << "f " << t->v0id << " " << t->v1id << " " << t->v2id << "\n";
			}

			if ((model.texCoords.size() != 0) && (model.normals.size() == 0))
			{
				 fileStream << "f " << t->v0id << "/" << t->t0id << " " << t->v1id << "/" << t->t1id << " " << t->v2id << "/" << t->t2id << "\n";
			}

			if ((model.texCoords.size() == 0) && (model.normals.size() != 0))
			{
				 fileStream << "f " << t->v0id << "//" << t->n0id << " " << t->v1id << "//" << t->n1id << " " << t->v2id << "//" << t->n2id << "\n";
			}

			if ((model.texCoords.size() != 0) && (model.normals.size() != 0))
			{
				fileStream << "f " << t->v0id << "/" << t->t0id << "/" << t->n0id << " " << t->v1id << "/" << t->t1id << "/" << t->n1id << " " << t->v2id << "/" << t->t2id << "/" << t->n2id << "\n";
			}
		}
	}
	else
	{
		for (std::vector<BasicMaterial>::const_iterator m = model.materials.begin(); m != model.materials.end(); ++m)
		{
			fileStream << "usemtl " << m->name << "\n";

			for (std::vector<BasicTriangle>::const_iterator t = model.triangles.begin(); t != model.triangles.end(); ++t)
			{
				if (t->matid != m->id) continue;

				if ((model.texCoords.size() == 0) && (model.normals.size() == 0))
				{
					fileStream << "f " << t->v0id << " " << t->v1id << " " << t->v2id << "\n";
				}

				if ((model.texCoords.size() != 0) && (model.normals.size() == 0))
				{
					fileStream << "f " << t->v0id << "/" << t->t0id << " " << t->v1id << "/" << t->t1id << " " << t->v2id << "/" << t->t2id << "\n";
				}

				if ((model.texCoords.size() == 0) && (model.normals.size() != 0))
				{
					fileStream << "f " << t->v0id << "//" << t->n0id << " " << t->v1id << "//" << t->n1id << " " << t->v2id << "//" << t->n2id << "\n";
				}

				if ((model.texCoords.size() != 0) && (model.normals.size() != 0))
				{
					fileStream << "f " << t->v0id << "/" << t->t0id << "/" << t->n0id << " " << t->v1id << "/" << t->t1id << "/" << t->n1id << " " << t->v2id << "/" << t->t2id << "/" << t->n2id << "\n";
				}
			}
		}
	}

	fileStream.flush();

	fileStream.close();

	fileStream.clear();

	return true;
}
