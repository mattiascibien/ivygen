#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QObject>
#include <QDir>

#include <plugins/importer_interface.h>

class PluginLoader : public QObject
{
    Q_OBJECT
public:
    static PluginLoader& getInstance()
    {
        static PluginLoader instance;
        return instance;
    }

    void loadImporters();

    QList<ImporterInterface*> *getImporters()
    {
        return importers;
    }

private:
    PluginLoader();

    QDir pluginsDir;

    template<typename T>
    void loadPluginsFromDirectory(QString directory, QList<T*> *container);

    QList<ImporterInterface*> *importers;
};

#endif
