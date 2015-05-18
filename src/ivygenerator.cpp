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


#include "ivygenerator.h"
#include "Common.h"
#include "ModelLoader.h"
#include "OBJWriter.h"


#include "aboutdialog.h"

IvyGeneratorWindow::IvyGeneratorWindow(QWidget *parent) : QMainWindow(parent)
{
	setGeometry(100, 100, 1024, 768);

    setWindowTitle("IvyGenerator"); //  - Copyright (c) 2007 Thomas Luft");

    aboutWindow = new AboutDialog(this);

	Common::renderWidget = new RenderWidget();

	Common::setupWidget = new SetupWidget();

    Common::birthWidget = new BirthWidget();


    growWidget = new QDockWidget("Settings", this);

    growWidget->setWidget(Common::setupWidget);

    addDockWidget(Qt::RightDockWidgetArea, growWidget, Qt::Vertical);

    birthWidget = new QDockWidget("Birth", this);

    birthWidget->setWidget(Common::birthWidget);

    addDockWidget(Qt::RightDockWidgetArea, birthWidget, Qt::Vertical);

    setCentralWidget(Common::renderWidget);

    setupActions();
}

void IvyGeneratorWindow::setupActions()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");

    importObjMtlAction = new QAction("Import OBJ+MTL...", this);
    connect(importObjMtlAction, SIGNAL(triggered(bool)), this, SLOT(onImportObj()));
    fileMenu->addAction(importObjMtlAction);

    exportObjMtlAction = new QAction("Export OBJ+MTL...", this);
    connect(exportObjMtlAction, SIGNAL(triggered(bool)), this, SLOT(onExportObj()));
    fileMenu->addAction(exportObjMtlAction);

    fileMenu->addSeparator();

    exitAction = new QAction("Exit", this);
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
    fileMenu->addAction(exitAction);


    QMenu *editMenu = menuBar()->addMenu("&Edit");

    flipNormalsAction = new QAction("Flip Normals", this);
    connect(flipNormalsAction, SIGNAL(triggered(bool)), this, SLOT(onFlipNormals()));
    editMenu->addAction(flipNormalsAction);

    QMenu *viewMenu = menuBar()->addMenu("&View");

    backfaceCullingAction = new QAction("Backface culling", this);
    backfaceCullingAction->setCheckable(true);
    connect(backfaceCullingAction, SIGNAL(toggled(bool)), Common::renderWidget, SLOT(toggleBackfaceCulling(bool)));
    viewMenu->addAction(backfaceCullingAction);

    wireframeAction = new QAction("Wireframe", this);
    wireframeAction->setCheckable(true);
    connect(wireframeAction, SIGNAL(toggled(bool)), Common::renderWidget, SLOT(toggleWireframe(bool)));
    viewMenu->addAction(wireframeAction);

    viewMenu->addSeparator();

    viewMenu->addAction(growWidget->toggleViewAction());
    viewMenu->addAction(birthWidget->toggleViewAction());

    QMenu *helpMenu = menuBar()->addMenu("&Help");

    aboutAction = new QAction("About...", this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(onAbout()));
    helpMenu->addAction(aboutAction);

    aboutQtAction = new QAction("About Qt...", this);
    connect(aboutQtAction, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    helpMenu->addAction(aboutQtAction);
}

void IvyGeneratorWindow::onImportObj()
{
    QString fileString = QFileDialog::getOpenFileName(NULL, "Open 3D Model", "", "Wavefront Obj (*.obj)");

    if (fileString != "")
    {
        QFileInfo fileInfo(fileString);

        QString path = fileInfo.path() + "/";

        QString file = fileInfo.completeBaseName() + ".obj";


        Common::mesh.reset();


        ModelLoader::loadOBJ( path.toStdString(), file.toStdString(), Common::mesh );


        Common::mesh.loadTextures();

        Common::mesh.prepareData();

        Common::mesh.calculateVertexNormals();

        Common::mesh.prepareData();

        Common::mesh.createDisplayList();

        Common::camera.placeNicely( Common::mesh );

        Common::setupWidget->update();
    }
}


void IvyGeneratorWindow::onExportObj()
{
    QString fileString = QFileDialog::getSaveFileName(NULL, "Save Ivy Object", "", "Wavefront Obj (*.obj)");

    if (fileString != "")
    {
        QFileInfo fileInfo(fileString);

        QString path = fileInfo.path() + "/";

        QString file = fileInfo.completeBaseName() + ".obj";


        OBJWriter::writeOBJ( path.toStdString(), file.toStdString(), Common::ivy );
    }
}

void IvyGeneratorWindow::onFlipNormals()
{
    Common::mesh.flipNormals();

    Common::mesh.createDisplayList();

    Common::renderWidget->updateGL();
}

void IvyGeneratorWindow::onAbout()
{
    aboutWindow->show();
}


IvyGeneratorWindow::~IvyGeneratorWindow()
{
}

