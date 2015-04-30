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


#include "RenderWidget.h"
#include "Common.h"
#include <limits>

#include <GL/gl.h>
#include <GL/glu.h>


RenderWidget::RenderWidget(QWidget *parent) : QGLWidget(parent)
{
	QGLFormat format;

	format.setAccum(false);

	format.setAlpha(true);

	format.setAlphaBufferSize(8);

	format.setDepth(true);

	format.setDepthBufferSize(32);

	format.setDirectRendering(true);

	format.setDoubleBuffer(true);

	format.setOverlay(false);

	format.setPlane(0);

	format.setRgba(true);

	format.setSampleBuffers(false);

	format.setStencil(false);

	format.setStereo(false);

	setFormat( format );

	imageNr = 0;
}


RenderWidget::~RenderWidget()
{

}


void RenderWidget::paintGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClearDepth(1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//camera, initialize projection, view, and model matrix
	setupCamera();

	//light, e.g. ambient and three diffuse lights
	setupLight();

	//mesh, e.g. basic mesh, ivy mesh
	renderScene();

	//auxiliary objects, e.g. grid, ivy skeleton
	renderAuxiliary();

    renderPivot();
}


void RenderWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}


void RenderWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->globalPos();
}


void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
	QPoint d = lastPos - event->globalPos();


	//dolly camera
	if ((event->buttons() == Qt::RightButton) && (event->modifiers() == Qt::NoModifier))
	{
		Vector3d dir, right, up;

		Common::camera.computeVectorBasis(dir, right, up);

		Common::camera.pos += dir * (d.x() + d.y()) * Common::mesh.boundingSphereRadius * 0.002f;
	}	

	//rotate camera (head and pitch)
	if ((event->buttons() == Qt::LeftButton) && (event->modifiers() == Qt::NoModifier))
	{
		Common::camera.head -= d.x() * 0.002f;

		Common::camera.pitch -= d.y() * 0.002f;
	}

	//track camera (right and up vector movements)
	if ((event->buttons() == Qt::LeftButton) && (event->modifiers() == Qt::ControlModifier))
	{
		Vector3d dir, right, up;

		Common::camera.computeVectorBasis(dir, right, up);

		Common::camera.pos -= right * d.x() * Common::mesh.boundingSphereRadius * 0.002f;

		Common::camera.pos += up * d.y() * Common::mesh.boundingSphereRadius * 0.002f;
	}

	//orbit camera (rotate camera around camera pivot)
	if ((event->buttons() == Qt::LeftButton) && (event->modifiers() == Qt::ShiftModifier))
	{
		Vector3d dir, right, up;

		Common::camera.computeVectorBasis(dir, right, up);

		Common::camera.pos = Vector3d::rotateAroundAxis( Common::camera.pos, Common::camera.pivot, up, +d.x() * 0.002f );

		Common::camera.head += d.x() * 0.002f;

		Common::camera.computeVectorBasis(dir, right, up);

		Common::camera.pos = Vector3d::rotateAroundAxis( Common::camera.pos, Common::camera.pivot, right, +d.y() * 0.002f );

		Common::camera.pitch += d.y() * 0.002f;
	}

	lastPos = event->globalPos();

	updateGL();
}


void RenderWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	//get projection, model and view matrix
	setupCamera();

	glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

	double projMatrix[16];

	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	double modelMatrix[16];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

	int viewPort[4];

	glGetIntegerv(GL_VIEWPORT, viewPort);


	//compute a "click ray"
	double x0, y0, z0, x1, y1, z1;

	gluUnProject(event->pos().x(), height() - event->pos().y(), 0.0, modelMatrix, projMatrix, viewPort, &x0, &y0, &z0);

	gluUnProject(event->pos().x(), height() - event->pos().y(), 1.0, modelMatrix, projMatrix, viewPort, &x1, &y1, &z1);
	
	Vector3d p0(x0, y0, z0);

	Vector3d p1(x1, y1, z1);


	//find intersecting triangle with minimum distance
	float minDistance = (std::numeric_limits<float>::max)();

	Vector3d seedPoint;

	for (std::vector<BasicTriangle>::iterator t = Common::mesh.triangles.begin(); t != Common::mesh.triangles.end(); ++t)
	{
		//compute intersection with triangle plane parametrically: intersectionPoint = p0 + (p1 - p0) * t0;
		float t0 = -Vector3d::dotProduct( t->norm, p0 - t->v0->pos ) / Vector3d::dotProduct( t->norm, p1 - p0 );

		//plane intersection
		if ((t0 >= 0.0f) && ( t0 <= 1.0f))
		{
			//intersection point
			Vector3d intersectionPoint = p0 + (p1 - p0) * t0;

			float alpha, beta, gamma;

			//triangle intersection
			if (Vector3d::getBarycentricCoordinates(t->v0->pos, t->v1->pos, t->v2->pos, intersectionPoint, alpha, beta, gamma))
			{                    
				//test on entry or exit of the triangle mesh
				bool entry = Vector3d::dotProduct( t->norm, p1 - p0) < 0.0f ? true : false;

				if (entry)
				{
					//commpute distance
					float distance = Vector3d::getLength(intersectionPoint - p0);

					if (distance < minDistance)
					{
						minDistance = distance;

						seedPoint = intersectionPoint;
					}
				}
			}
		}
	}

	//no triangle found 
	if (minDistance == (std::numeric_limits<float>::max)())
	{
		//compute intersection with grid plane
		float t = -p0.y / (p1.y - p0.y);
        
		seedPoint = p0 + t * (p1 - p0);
	}

	Common::ivy.seed(seedPoint);


	updateGL();
}


void RenderWidget::setupCamera()
{
	//setup view and projection matrix
	glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

	Common::camera.clipNear = 0.01 * Common::mesh.boundingSphereRadius;

	Common::camera.clipFar = 10.0f * Common::mesh.boundingSphereRadius;

	Common::camera.aspectRatio = (float)width() / (float)height();

	Common::camera.setupPerspectiveMatrix();

	Common::camera.setupViewMatrix();
}


void RenderWidget::setupLight()
{
	glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();


	glEnable(GL_LIGHT0);

	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHT2);


	float brightWhite[4] = {0.9f, 0.9f, 0.8f, 1.0f};

	float white[4] = {0.4f, 0.4f, 0.4f, 1.0f};

	float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	float blueDim[4] = {0.1f, 0.1f, 0.15f, 1.0f};


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, blueDim);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	


    float pos0[4] = {Common::mesh.boundingSpherePos.x - Common::mesh.boundingSphereRadius * 2.0f,
					 Common::mesh.boundingSpherePos.y + Common::mesh.boundingSphereRadius * 1.0f,
					 Common::mesh.boundingSpherePos.z + Common::mesh.boundingSphereRadius * 2.0f,
					 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, pos0);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, brightWhite );

	glLightfv(GL_LIGHT0, GL_AMBIENT, black );


	float pos1[4] = {Common::mesh.boundingSpherePos.x + Common::mesh.boundingSphereRadius * 1.0f,
					 Common::mesh.boundingSpherePos.y + Common::mesh.boundingSphereRadius * 0.7f,
					 Common::mesh.boundingSpherePos.z + Common::mesh.boundingSphereRadius * 0.0f,
					 1.0f};

	glLightfv(GL_LIGHT1, GL_POSITION, pos1);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0f);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, white );

	glLightfv(GL_LIGHT1, GL_AMBIENT, black );


	float pos2[4] = {Common::mesh.boundingSpherePos.x - Common::mesh.boundingSphereRadius * 0.0f,
					 Common::mesh.boundingSpherePos.y + Common::mesh.boundingSphereRadius * 0.7f,
					 Common::mesh.boundingSpherePos.z - Common::mesh.boundingSphereRadius * 1.0f,
					 1.0f};

	glLightfv(GL_LIGHT2, GL_POSITION, pos2);

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0f);

	glLightfv(GL_LIGHT2, GL_DIFFUSE, white );

	glLightfv(GL_LIGHT2, GL_AMBIENT, black );
}


void RenderWidget::renderScene()
{
	glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    if(backfaceCulling && !wireFrame)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    glFrontFace(GL_FRONT);

    if(wireFrame)
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);

	glAlphaFunc(GL_GREATER, 0.7f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);


	//basic mesh
	if (Common::mesh.materials.size() == 0)
	{
		glCallList(Common::mesh.displayListObj);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_ALPHA_TEST);

		glEnable(GL_BLEND);

		for (std::vector<BasicMaterial>::iterator ma = Common::mesh.materials.begin(); ma != Common::mesh.materials.end(); ++ma)
		{
			glCallList(Common::mesh.displayListObj + (ma - Common::mesh.materials.begin()));
		}

		glDisable(GL_BLEND);

		glDisable(GL_ALPHA_TEST);

		glDisable(GL_TEXTURE_2D);
	}

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	//ivy mesh
	if (Common::ivy.materials.size() > 0)
	{
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_ALPHA_TEST);

		glEnable(GL_BLEND);

		for (std::vector<BasicMaterial>::iterator ma = Common::ivy.materials.begin(); ma != Common::ivy.materials.end(); ++ma)
		{
			glCallList(Common::ivy.displayListObj + (ma - Common::ivy.materials.begin()));
		}

		glDisable(GL_BLEND);
	
		glDisable(GL_ALPHA_TEST);

		glDisable(GL_TEXTURE_2D);
	}


	glDisable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);
}


void RenderWidget::renderAuxiliary()
{
	glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();


	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);



	//ivy roots
	glColor3f(0.0f, 1.0f, 0.0f);

	glPointSize(8.0f);

	glBegin(GL_POINTS);

		for (std::vector<IvyRoot>::iterator root = Common::ivy.roots.begin(); root != Common::ivy.roots.end(); ++root)
		{
			if (root->parents == 0) glVertex3f(root->nodes.front().pos.x, root->nodes.front().pos.y, root->nodes.front().pos.z);
		}

	glEnd();



	//ivy skeleton
	if (Common::ivy.materials.size() == 0)
	{
		glColor3f(0.3f, 0.4f, 1.0f);

		//roots and branches
		for (std::vector<IvyRoot>::iterator root = Common::ivy.roots.begin(); root != Common::ivy.roots.end(); ++root)
		{
			glBegin(GL_LINE_STRIP);

			for (std::vector<IvyNode>::iterator node = root->nodes.begin(); node != root->nodes.end(); ++node)
			{
				glVertex3f(node->pos.x, node->pos.y, node->pos.z);
			}

			glEnd();
		}

		glColor3f(0.3f, 0.8f, 0.6f);

		//adhesion vectors
		for (std::vector<IvyRoot>::iterator root = Common::ivy.roots.begin(); root != Common::ivy.roots.end(); ++root)
		{
			glBegin(GL_LINES);

			for (std::vector<IvyNode>::iterator node = root->nodes.begin(); node != root->nodes.end(); ++node)
			{
				glVertex3f(node->pos.x, node->pos.y, node->pos.z);

				glVertex3f(node->pos.x + node->adhesionVector.x * 0.1f, node->pos.y + node->adhesionVector.y * 0.1f, node->pos.z + node->adhesionVector.z * 0.1f);
			}

			glEnd();
		}
	}

    glScalef(2.0f * Common::mesh.boundingSphereRadius, 2.0f * Common::mesh.boundingSphereRadius, 2.0f * Common::mesh.boundingSphereRadius);

    glLineWidth(1.5f);

    //draw x axis
    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.1f, 0.0f, 0.0f);

    glEnd();

    //draw y axis
    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);

    glEnd();

    //draw z axis
    glBegin(GL_LINES);

    glColor3f(0.0f, 0.0f, 1.0f);

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.1f);

    glEnd();

    glLineWidth(1.0f);

    //grid
    glColor4f(0.6f, 0.6f, 0.6f, 1.0f);

	for (float x = -1.0f; x <= 1.0f + 0.01f; x += 0.05f)
	{
		for (float z = -1.0f; z <= 1.0f + 0.01f; z += 0.05f)
		{
			glBegin(GL_LINES);

			glVertex3f(x, 0.0f, -1.0f);
			glVertex3f(x, 0.0f,  1.0f);

			glVertex3f(-1.0f, 0.0f, z);
			glVertex3f( 1.0f, 0.0f, z);

			glEnd();
		}
	}

	glColor3f(1.0f, 1.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);
}


void RenderWidget::renderPivot()
{

}


void RenderWidget::writeScreen()
{
	paintGL();

	glFinish();

	swapBuffers();

	QImage frameBuffer = grabFrameBuffer(false);

	QString fileName = "export/ivy_animation_" + QString::number(imageNr).rightJustified(5, '0') + ".png";

	frameBuffer.save(fileName, "PNG");

	++imageNr;
}
