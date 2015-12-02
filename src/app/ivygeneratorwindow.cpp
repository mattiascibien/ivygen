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


#include "ivygeneratorwindow.h"
#include <Common.h>
#include <model_loader.h>
#include <obj_writer.h>

#include "aboutdialog.h"
#include "settingsdialog.h"

#include "configuration.h"

IvyGeneratorWindow::IvyGeneratorWindow(QWidget *parent) : QMainWindow(parent)
{
	setGeometry(100, 100, 1024, 768);

    setWindowTitle("IvyGenerator"); //  - Copyright (c) 2007 Thomas Luft");

    aboutWindow = new AboutDialog(this);
    settingsDialog = new SettingsDialog(this);

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

    importObjMtlAction = new QAction("Import...", this);
    importObjMtlAction->setIcon(QIcon(":/resources/images/document-import.svg"));
    connect(importObjMtlAction, &QAction::triggered, this, &IvyGeneratorWindow::onImportObj);
    fileMenu->addAction(importObjMtlAction);

    exportObjMtlAction = new QAction("Export...", this);
    exportObjMtlAction->setIcon(QIcon(":/resources/images/document-export.svg"));
    connect(exportObjMtlAction, &QAction::triggered, this, &IvyGeneratorWindow::onExportObj);
    fileMenu->addAction(exportObjMtlAction);

    fileMenu->addSeparator();

    exitAction = new QAction("Exit", this);
    connect(exitAction, &QAction::triggered, this, &IvyGeneratorWindow::close);
    fileMenu->addAction(exitAction);


    QMenu *editMenu = menuBar()->addMenu("&Edit");

    flipNormalsAction = new QAction("Flip Normals", this);
    connect(flipNormalsAction, &QAction::triggered, this, &IvyGeneratorWindow::onFlipNormals);
    editMenu->addAction(flipNormalsAction);

    editMenu->addSeparator();

    preferencesAction = new QAction("Preferences", this);
    connect(preferencesAction, &QAction::triggered, this, &IvyGeneratorWindow::onPreferences);
    editMenu->addAction(preferencesAction);

    QMenu *viewMenu = menuBar()->addMenu("&View");

    backfaceCullingAction = new QAction("Backface culling", this);
    backfaceCullingAction->setCheckable(true);
    connect(backfaceCullingAction, &QAction::toggled, Common::renderWidget, &RenderWidget::toggleBackfaceCulling);
    viewMenu->addAction(backfaceCullingAction);

    wireframeAction = new QAction("Wireframe", this);
    wireframeAction->setCheckable(true);
    connect(wireframeAction, &QAction::toggled, Common::renderWidget, &RenderWidget::toggleWireframe);
    viewMenu->addAction(wireframeAction);

    viewMenu->addSeparator();

    viewMenu->addAction(growWidget->toggleViewAction());
    viewMenu->addAction(birthWidget->toggleViewAction());

    QMenu *helpMenu = menuBar()->addMenu("&Help");

    aboutAction = new QAction("About...", this);
    aboutAction->setIcon(QIcon(":/resources/images/help-about.svg"));
    connect(aboutAction, &QAction::triggered, this, &IvyGeneratorWindow::onAbout);
    helpMenu->addAction(aboutAction);

    aboutQtAction = new QAction("About Qt...", this);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
    helpMenu->addAction(aboutQtAction);
}

void IvyGeneratorWindow::onImportObj()
{
    QString fileString = QFileDialog::getOpenFileName(
                this, "Open 3D Model", Configuration::getInstance().getImportPath(),
                                                      ModelLoader::importFilter());

    if (fileString != "")
    {
        QFileInfo fileInfo(fileString);

        QString path = fileInfo.path() + "/";

        QString file = fileInfo.fileName();


        Common::mesh.reset();


        ModelLoader::load( path.toStdString(), file.toStdString(), Common::mesh );


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
    QString fileString = QFileDialog::getSaveFileName(this, "Save Ivy Object", Configuration::getInstance().getExportPath(), "Wavefront Obj (*.obj)");

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

void IvyGeneratorWindow::onPreferences()
{
    settingsDialog->show();
}


IvyGeneratorWindow::~IvyGeneratorWindow()
{
}

