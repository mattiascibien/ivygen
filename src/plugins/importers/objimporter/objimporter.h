#ifndef OBJIMPORTER_H
#define OBJIMPORTER_H

#include <core/plugins/importer_interface.h>


class ObjImporter : public QObject,
                    public ImporterInterface
{
    Q_OBJECT
    Q_INTERFACES(ImporterInterface)
    Q_PLUGIN_METADATA(IID ImporterInterface_iid FILE "objimporter.json")

public:
    ObjImporter(QObject *parent = 0);
    bool load( const std::string &path, const std::string &file, BasicMesh &model  );

    QString getName()
    {
        return "Wavefront OBJ Importer";
    }

    QString getFileExtension()
    {
        return "obj";
    }
};

#endif // OBJIMPORTER_H
