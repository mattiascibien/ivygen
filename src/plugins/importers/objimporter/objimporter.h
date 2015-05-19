#ifndef OBJIMPORTER_H
#define OBJIMPORTER_H

#include <importerplugin.h>


class ObjImporter : public ImporterPlugin
{
    Q_OBJECT
    Q_INTERFACES(ImporterPlugin)
    Q_PLUGIN_METADATA(IID "com.ivygen.importerplugin" FILE "objimporter.json")

public:
    ObjImporter(QObject *parent = 0);
    bool load( const std::string &path, const std::string &file, BasicMesh &model  );
};

#endif // OBJIMPORTER_H
