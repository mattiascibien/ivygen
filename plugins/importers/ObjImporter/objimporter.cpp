#include "objimporter.h"


ObjImporter::ObjImporter(QObject *parent) :
    QGenericPlugin(parent)
{
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ObjImporter, ObjImporter)
#endif // QT_VERSION < 0x050000
