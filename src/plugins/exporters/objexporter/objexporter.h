#ifndef OBJEXPORTER_H
#define OBJExPORTER_H

#include <core/plugins/exporter_interface.h>

class ObjExporter : public QObject,
                    public ExporterInterface
{
    Q_OBJECT
    Q_INTERFACES(ExporterInterface)
    Q_PLUGIN_METADATA(IID ExporterInterface_iid FILE "objexporter.json")

public:
    ObjExporter(QObject *parent = 0);
    bool save( const std::string &path, const std::string &file, BasicMesh &model  );

    QString getName()
    {
        return "Wavefront OBJ Exporter";
    }

    QString getFileExtension()
    {
        return "obj";
    }
};

#endif // OBJIMPORTER_H
