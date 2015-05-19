#include "objimporter.h"


ObjImporter::ObjImporter(QObject *parent) :
    ImporterPlugin(parent)
{
}

void ObjImporter::load(const std::string &path, const std::string &file, BasicMesh &model)
{

}

Q_PLUGIN_METADATA(IID "com.ivygen.importerplugin")
