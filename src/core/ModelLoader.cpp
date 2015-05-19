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


#include "ModelLoader.h"
#include "BasicMesh.h"

#include "objloader/objLoader.h"

#include <QMessageBox>
#include <QDir>


bool ModelLoader::load( const std::string &path, const std::string &file, BasicMesh &model  )
{
    objLoader loader;
    QDir::setCurrent(QString::fromStdString(path));
    int loadResult = loader.load((path + file).c_str());

    for(int i = 0; i < loader.vertexCount; i++)
    {
        obj_vector *currentVertex = loader.vertexList[i];
        BasicVertex vertex;

        vertex.pos.x = currentVertex->e[0];
        vertex.pos.y = currentVertex->e[1];
        vertex.pos.z = currentVertex->e[2];

        model.vertices.push_back(vertex);
    }

    for(int i = 0; i < loader.normalCount; i++)
    {
        obj_vector *currentNormal = loader.normalList[i];
        BasicNormal normal;

        normal.dir.x = currentNormal->e[0];
        normal.dir.y = currentNormal->e[1];
        normal.dir.z = currentNormal->e[2];

        model.normals.push_back(normal);
    }

    for(int i = 0; i < loader.textureCount; i++)
    {
        obj_vector *currentTexCoord = loader.textureList[i];
        BasicTexCoord texCoord;

        texCoord.pos.x = currentTexCoord->e[0];
        texCoord.pos.y = currentTexCoord->e[1];

        model.texCoords.push_back(texCoord);
    }

    for(int i = 0; i < loader.materialCount; i++)
    {
        obj_material *currentMat = loader.materialList[i];
        BasicMaterial material;

        material.name = currentMat->name;
        material.texFile = currentMat->texture_filename;
        material.id = i+1;

        model.materials.push_back(material);
    }

    for(int i = 0; i < loader.faceCount; i++)
    {
        obj_face *currentFace = loader.faceList[i];

        //Automatically triangulate faces
        for(int j=1; j <currentFace->vertex_count -1; j++)
        {
            BasicTriangle triangle;

            triangle.v0id = currentFace->vertex_index[0]+1;
            triangle.n0id = currentFace->normal_index[0]+1;
            triangle.t0id = currentFace->texture_index[0]+1;

            triangle.v1id = currentFace->vertex_index[j]+1;
            triangle.n1id = currentFace->normal_index[j]+1;
            triangle.t1id = currentFace->texture_index[j]+1;

            triangle.v2id = currentFace->vertex_index[j+1]+1;
            triangle.n2id = currentFace->normal_index[j+1]+1;
            triangle.t2id = currentFace->texture_index[j+1]+1;

            triangle.matid = currentFace->material_index+1;


            model.triangles.push_back(triangle);
        }
    }

    return loadResult == 1;
}
