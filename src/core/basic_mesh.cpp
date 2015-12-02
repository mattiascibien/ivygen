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


#include "basic_mesh.h"
#include <QtOpenGL>


BasicMesh::BasicMesh()
{
	boundingSphereRadius = 1.0f;

	boundingSpherePos = Vector3d(0.0f, 0.0f, 0.0f);

	displayListObj = 0;
}


BasicMesh::~BasicMesh()
{
	for (std::vector<BasicMaterial>::iterator m = materials.begin(); m != materials.end(); ++m)
	{
		if (glIsTexture(m->texObj))
		{
			glDeleteTextures(1, &m->texObj);
		}
	}

	if (glIsList(displayListObj))
	{
        glDeleteLists(displayListObj, 1 + (GLsizei)materials.size());
	}
}


void BasicMesh::reset()
{
	for (std::vector<BasicMaterial>::iterator m = materials.begin(); m != materials.end(); ++m)
	{
		if (glIsTexture(m->texObj))
		{
			glDeleteTextures(1, &m->texObj);
		}
	}

	if (glIsList(displayListObj))
	{
        glDeleteLists(displayListObj, 1 + (GLsizei)materials.size());
	}

	file = "";

	path = "";

	vertices.clear();

	normals.clear();

	texCoords.clear();

	triangles.clear();

	materials.clear();

	boundingSphereRadius = 1.0f;

	boundingSpherePos = Vector3d(0.0f, 0.0f, 0.0f);
}


void BasicMesh::prepareData()
{
	//update pointers of triangle
	for (std::vector<BasicTriangle>::iterator t = triangles.begin(); t != triangles.end(); ++t)
	{
		t->v0 = &vertices[t->v0id - 1];
		t->v1 = &vertices[t->v1id - 1];
		t->v2 = &vertices[t->v2id - 1];

		if (t->n0id != 0) t->n0 = &normals[t->n0id - 1];
		if (t->n1id != 0) t->n1 = &normals[t->n1id - 1];
		if (t->n2id != 0) t->n2 = &normals[t->n2id - 1];

		if (t->t0id != 0) t->t0 = &texCoords[t->t0id - 1];
		if (t->t1id != 0) t->t1 = &texCoords[t->t1id - 1];
		if (t->t2id != 0) t->t2 = &texCoords[t->t2id - 1];
 
		if (t->matid != 0) t->mat = &materials[t->matid - 1];
	}

	//compute bounding sphere
	boundingSpherePos = Vector3d(0.0f, 0.0f, 0.0f);

	for (std::vector<BasicVertex>::iterator v = vertices.begin(); v != vertices.end(); ++v)
	{
		boundingSpherePos += v->pos;
	}

	boundingSpherePos /= (float)vertices.size();

	boundingSphereRadius = 0.0f;

	for (std::vector<BasicVertex>::iterator v = vertices.begin(); v != vertices.end(); ++v)
	{
		boundingSphereRadius = std::max<float>(boundingSphereRadius, Vector3d::getLength(v->pos - boundingSpherePos));
	}
}



void BasicMesh::calculateVertexNormals()
{
	normals.clear();

	normals.resize( vertices.size() );


	for (std::vector<BasicTriangle>::iterator t = triangles.begin(); t != triangles.end(); ++t)
	{
		Vector3d tmp1(t->v0->pos - t->v1->pos);

		Vector3d tmp2(t->v1->pos - t->v2->pos);

		t->norm = Vector3d::crossProduct( tmp1, tmp2 );

		t->norm.normalize();
	}


	for (std::vector<BasicTriangle>::iterator t = triangles.begin(); t != triangles.end(); ++t)
	{
		t->n0id = t->v0id;

		t->n0 = &normals[t->n0id - 1];

		t->n0->dir += t->norm;


		t->n1id = t->v1id;

		t->n1 = &normals[t->n1id - 1];

		t->n1->dir += t->norm;


		t->n2id = t->v2id;

		t->n2 = &normals[t->n2id - 1];

		t->n2->dir += t->norm;	
	}

	for (std::vector<BasicNormal>::iterator n = normals.begin(); n != normals.end(); ++n)
	{
		n->dir.normalize();
	}
}


void BasicMesh::flipNormals()
{
	for (std::vector<BasicTriangle>::iterator t = triangles.begin(); t != triangles.end(); ++t)
	{
		t->norm *= -1.0f;
	}

	for (std::vector<BasicNormal>::iterator n = normals.begin(); n != normals.end(); ++n)
	{
		n->dir *= -1.0f;
	}
}


bool BasicMesh::loadTextures()
{
	for (std::vector<BasicMaterial>::iterator m = materials.begin(); m != materials.end(); ++m)
	{
		QImage texImage(QString::fromStdString( path + m->texFile ), "PNG");
		
		if (texImage.isNull()) return false;


		if (glIsTexture(m->texObj))
		{
			glDeleteTextures(1, &m->texObj);
		}

		glGenTextures(1, &m->texObj);

		glBindTexture( GL_TEXTURE_2D, m->texObj);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );


		if ((texImage.format() == QImage::Format_ARGB32) || (texImage.format() == QImage::Format_RGB32))
		{
			QImage texImageSwapped = texImage.rgbSwapped();

			QImage texImageMirrored = texImageSwapped.mirrored(true, true);

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, texImageMirrored.width(), texImageMirrored.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImageMirrored.bits() );
		}
	}

	return true;
}


void BasicMesh::createDisplayList(bool smooth)
{
	if (glIsList(displayListObj))
	{
        glDeleteLists(displayListObj, 1 + (GLsizei)materials.size());
	}


    displayListObj = glGenLists(1 + (GLsizei)materials.size());


	if (materials.size() == 0)
	{
		glNewList(displayListObj, GL_COMPILE);

		glBegin(GL_TRIANGLES);
		
		for (std::vector<BasicTriangle>::iterator t = triangles.begin(); t != triangles.end(); ++t)
		{
			if (smooth)
			{
				if (t->n0 != NULL) glNormal3f(t->n0->dir.x, t->n0->dir.y, t->n0->dir.z);
				if (t->v0 != NULL) glVertex3f(t->v0->pos.x, t->v0->pos.y, t->v0->pos.z);

				if (t->n1 != NULL) glNormal3f(t->n1->dir.x, t->n1->dir.y, t->n1->dir.z);
				if (t->v1 != NULL) glVertex3f(t->v1->pos.x, t->v1->pos.y, t->v1->pos.z);

				if (t->n2 != NULL) glNormal3f(t->n2->dir.x, t->n2->dir.y, t->n2->dir.z);
				if (t->v2 != NULL) glVertex3f(t->v2->pos.x, t->v2->pos.y, t->v2->pos.z);
			}
			else
			{
				glNormal3f(t->norm.x, t->norm.y, t->norm.z);

				if (t->v0 != NULL) glVertex3f(t->v0->pos.x, t->v0->pos.y, t->v0->pos.z);
				if (t->v1 != NULL) glVertex3f(t->v1->pos.x, t->v1->pos.y, t->v1->pos.z);
				if (t->v2 != NULL) glVertex3f(t->v2->pos.x, t->v2->pos.y, t->v2->pos.z);
			}
		}

		glEnd();

		glEndList();
	}
	else
	{
		for (std::vector<BasicMaterial>::iterator m = materials.begin(); m != materials.end(); ++m)
		{
			glNewList(displayListObj + (m - materials.begin()), GL_COMPILE);

			glBindTexture(GL_TEXTURE_2D, m->texObj);

			glBegin(GL_TRIANGLES);
			
			for (std::vector<BasicTriangle>::iterator t = triangles.begin(); t != triangles.end(); ++t)
			{
				if (t->matid != m->id) continue;

				if (smooth)
				{
					if (t->t0 != NULL) glTexCoord2f(t->t0->pos.x, t->t0->pos.y);
					if (t->n0 != NULL) glNormal3f(t->n0->dir.x, t->n0->dir.y, t->n0->dir.z);
					if (t->v2 != NULL) glVertex3f(t->v0->pos.x, t->v0->pos.y, t->v0->pos.z);

					if (t->t1 != NULL) glTexCoord2f(t->t1->pos.x, t->t1->pos.y);
					if (t->n1 != NULL) glNormal3f(t->n1->dir.x, t->n1->dir.y, t->n1->dir.z);
					if (t->v2 != NULL) glVertex3f(t->v1->pos.x, t->v1->pos.y, t->v1->pos.z);

					if (t->t2 != NULL) glTexCoord2f(t->t2->pos.x, t->t2->pos.y);
					if (t->n2 != NULL) glNormal3f(t->n2->dir.x, t->n2->dir.y, t->n2->dir.z);
					if (t->v2 != NULL) glVertex3f(t->v2->pos.x, t->v2->pos.y, t->v2->pos.z);
				}
				else
				{
					glNormal3f(t->norm.x, t->norm.y, t->norm.z);

					if (t->t0 != NULL) glTexCoord2f(t->t0->pos.x, t->t0->pos.y);
					if (t->v2 != NULL) glVertex3f(t->v0->pos.x, t->v0->pos.y, t->v0->pos.z);

					if (t->t1 != NULL) glTexCoord2f(t->t1->pos.x, t->t1->pos.y);
					if (t->v2 != NULL) glVertex3f(t->v1->pos.x, t->v1->pos.y, t->v1->pos.z);

					if (t->t2 != NULL) glTexCoord2f(t->t2->pos.x, t->t2->pos.y);
					if (t->v2 != NULL) glVertex3f(t->v2->pos.x, t->v2->pos.y, t->v2->pos.z);
				}
				
			}

			glEnd();

			glEndList();
		}
	}
}

