#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QObject>
#include <QDir>
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

private:
    PluginLoader();

    QDir pluginsDir;

    void loadDirectory(QString directory);
};

#endif
