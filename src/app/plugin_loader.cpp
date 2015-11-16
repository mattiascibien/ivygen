#include "plugin_loader.h"

#include <QApplication>
#include <QPluginLoader>

#include <QMessageBox>

#include <plugins/importer_interface.h>

void PluginLoader::loadImporters()
{
    loadDirectory("importers");
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

void PluginLoader::loadDirectory(QString directory)
{
    pluginsDir.cd(directory);

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
        {

        }
    }

    pluginsDir.cdUp();
}
