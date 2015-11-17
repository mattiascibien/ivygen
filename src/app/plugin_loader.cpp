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

#if defined(Q_OS_WIN)
    pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    //TODO: mac
#else
    //TODO: unix
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
