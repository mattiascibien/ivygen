#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QObject>
#include <QDir>

#include <core/plugins/importer_interface.h>
#include <core/plugins/exporter_interface.h>

class PluginLoader : public QObject
{
    Q_OBJECT
public:
    static PluginLoader& getInstance()
    {
        static PluginLoader instance;
        return instance;
    }

    QList<ImporterInterface*> *loadImporters();
    QList<ExporterInterface*> *loadExporters();

    QList<ImporterInterface*> *getImporters()
    {
        return importers;
    }

    QList<ExporterInterface*> *getExporters()
    {
        return exporters;
    }

private:
    PluginLoader();

    QDir pluginsDir;

    template<typename T>
    void loadPluginsFromDirectory(QString directory, QList<T*> *container);

    QList<ImporterInterface*> *importers;
    QList<ExporterInterface*> *exporters;
};

#endif
