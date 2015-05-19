#ifndef OBJIMPORTER_H
#define OBJIMPORTER_H

#include <QGenericPlugin>


class ObjImporter : public QGenericPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "ObjImporter.json")
#endif // QT_VERSION >= 0x050000

public:
    ObjImporter(QObject *parent = 0);
};

#endif // OBJIMPORTER_H
