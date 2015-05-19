#ifndef OBJIMPORTER_H
#define OBJIMPORTER_H

#include <QGenericPlugin>

#include <extensibility/importerplugin.h>


class ObjImporter : public ImporterPlugin
{
    Q_INTERFACES(ImporterPlugin)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ivygen.importerplugin" FILE "ObjImporter.json")

public:
    ObjImporter(QObject *parent = 0);
};

#endif // OBJIMPORTER_H
