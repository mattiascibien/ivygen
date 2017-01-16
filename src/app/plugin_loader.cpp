#include "plugin_loader.h"

#include <QApplication>
#include <QPluginLoader>

#include <QMessageBox>

QList<ImporterInterface*>* PluginLoader::loadImporters()
{
    importers = new QList<ImporterInterface*>();

    loadPluginsFromDirectory("importers", importers);

    return importers;
}

PluginLoader::PluginLoader()
{
    pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
            pluginsDir.cdUp();
            pluginsDir.cdUp();
            pluginsDir.cdUp();
        }
#endif
    pluginsDir.cd("plugins");
}

template<typename T>
void PluginLoader::loadPluginsFromDirectory(QString directory, QList<T*> *container)
{
    pluginsDir.cd(directory);

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
        {
            T* iPlugin = qobject_cast<T*>(plugin);
            if(iPlugin)
                container->append(iPlugin);
        }
    }

    pluginsDir.cdUp();
}

QList<ExporterInterface *> *PluginLoader::loadExporters()
{
    exporters = new QList<ExporterInterface*>();

    loadPluginsFromDirectory("exporters", exporters);

    return exporters;
}
