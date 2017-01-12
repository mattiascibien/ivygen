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


#include <QApplication>
#include "ivygeneratorwindow.h"

#include "plugin_loader.h"
#include <core/model_loader.h>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QPixmap pixmap(":/resources/images/splash.png");

    application.setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    
    application.setPalette(darkPalette);

    QSplashScreen splash(pixmap);
    splash.show();
    application.processEvents();

    application.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

    splash.showMessage("Loading importer plugins...", Qt::AlignBottom | Qt::AlignLeft, Qt::white);
    application.processEvents();
    ModelLoader::initializeImporters(PluginLoader::getInstance().loadImporters());
    ModelLoader::initializeExporters(PluginLoader::getInstance().loadExporters());

    splash.showMessage("Done loading.", Qt::AlignBottom | Qt::AlignLeft, Qt::white);
    application.processEvents();

    IvyGeneratorWindow ivyGenerator;
    ivyGenerator.show();
    splash.finish(&ivyGenerator);
    application.connect(&application, &QApplication::lastWindowClosed, &application, &QApplication::quit);

    return application.exec();
}
