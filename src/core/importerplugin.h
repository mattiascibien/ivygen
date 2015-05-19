#ifndef IMPORTERPLUGIN_H
#define IMPORTERPLUGIN_H

#include <QObject>

#include <core/BasicMesh.h>

class ImporterPlugin : public QObject
{
    Q_OBJECT
public:
    explicit ImporterPlugin(QObject *parent = 0);
    virtual void load( const std::string &path, const std::string &file, BasicMesh &model  ) = 0;

signals:

public slots:
};

Q_DECLARE_INTERFACE(ImporterPlugin, "com.ivygen.importerplugin")

#endif // IMPORTERPLUGIN_H
