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


#ifndef OBJLOADER_H
#define OBJLOADER_H


#include <string>
#include <fstream>
#include "basic_mesh.h"

#include "core_global.h"

#include <QList>
#include <QHash>
#include <QString>
#include "plugins/importer_interface.h"
#include "plugins/exporter_interface.h"

/** a class for loading an OBJ file into a Mesh object */
class CORESHARED_EXPORT ModelLoader
{
private:
    static QHash<QString, ImporterInterface*> *importersMap;
    static QHash<QString, ExporterInterface*> *exportersMap;
public:

	/** loads OBJ data from a file and stores it within a Mesh object */
    static bool load( const std::string &path, const std::string &file, BasicMesh &model );

    static bool save( const std::string &path, const std::string &file, BasicMesh &model );

    static void initializeImporters(QList<ImporterInterface*> *importers);
    static void initializeExporters(QList<ExporterInterface*> *exporters);

    static QString importFilter()
    {
        QString out = "";
        for(QString ext : importersMap->keys())
        {
            out = out.append("."+ ext + " file (*." + ext + ");;");
        }
        return out.append("All Files (*.*)");
    }

    static QString exportFilter()
    {
        QString out = "";
        for(QString ext : exportersMap->keys())
        {
            out = out.append("."+ ext + " file (*." + ext + ");;");
        }
        return out.append("All Files (*.*)");
    }
};

#endif
