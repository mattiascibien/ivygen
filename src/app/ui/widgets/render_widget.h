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


#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QGLWidget>


/** main opengl render widget */
class RenderWidget : public QGLWidget
{
    Q_OBJECT

public:

    RenderWidget(QWidget *parent = 0);

    ~RenderWidget();

	/** export a single screenshot */
	void writeScreen();


public slots:
    void toggleWireframe(bool on)
    {
        wireFrame = on;
        updateGL();
    }

    void toggleBackfaceCulling(bool on)
    {
        backfaceCulling = on;
        updateGL();
    }
    void reloadColors();

protected:

	void paintGL();

	void resizeGL(int width, int height);

	void mousePressEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event);

	void mouseDoubleClickEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent* event);

	/** initialize projection and view matrix */
	void setupCamera();

	/** initialize light settings */
	void setupLight();

	/** draw the scene mesh and the ivy mesh */
	void renderScene();

    /** draw the pivot */
    void renderPivot();

	/** draw auxiliary stuff */
	void renderAuxiliary();

private:
    bool wireFrame;
    bool backfaceCulling;

	QPoint lastPos;

	unsigned int imageNr;

    QColor clearColor;
    QColor gridColor;
};

#endif
