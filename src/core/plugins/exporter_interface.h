#ifndef EXPORTERINTERFACE_H
#define EXPORTERINTERFACE_H

#include <core/basic_mesh.h>

#include <QtPlugin>

#define ExporterInterface_iid "com.ivygen.exporterplugin"

class ExporterInterface
{
public:
    virtual ~ExporterInterface() {}

    virtual bool save( const std::string &path, const std::string &file, BasicMesh &model  ) = 0;

    virtual QString getName() = 0;

    virtual QString getFileExtension() = 0;
};

Q_DECLARE_INTERFACE(ExporterInterface, ExporterInterface_iid)

#endif
