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


#ifndef IVYGENERATOR_H
#define IVYGENERATOR_H


#include <QtWidgets>

class AboutDialog;

/** the main window containing the opengl widget and the setup area */
class IvyGeneratorWindow : public QMainWindow
{
    Q_OBJECT

public:

    IvyGeneratorWindow(QWidget *parent = 0);

    ~IvyGeneratorWindow();

private:
    AboutDialog *aboutWindow;
    //File
    QAction *importObjMtlAction;
    QAction *exportObjMtlAction;
    QAction *exitAction;

    //Edit
    QAction *flipNormalsAction;

    //View
    QAction *backfaceCullingAction;
    QAction *wireframeAction;

    //Help
    QAction *aboutAction;
    QAction *aboutQtAction;

    //DockWidgets
    QDockWidget *growWidget;
    QDockWidget *birthWidget;

    void setupActions();


public slots:
    void onImportObj();

    void onExportObj();

    void onFlipNormals();

    void onAbout();
};

#endif
