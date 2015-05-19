#ifndef IMPORTERPLUGIN_H
#define IMPORTERPLUGIN_H

#include <QObject>

class ImporterPlugin : public QObject
{
    Q_OBJECT
public:
    explicit ImporterPlugin(QObject *parent = 0);

signals:

public slots:
};

Q_DECLARE_INTERFACE(ImporterPlugin, "com.ivygen.importerplugin")

#endif // IMPORTERPLUGIN_H
