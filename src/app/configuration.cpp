#include "configuration.h"

#include <QDir>
#include <QStandardPaths>

QString Configuration::getExportPath()
{
    return settings.value("General/export_path", QStandardPaths::HomeLocation).toString();
}

void Configuration::setExportPath(QString path)
{
    path = path.trimmed();

    if(path.isNull() || path.isEmpty())
        path = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);

    settings.setValue("General/export_path", path);
}

QString Configuration::getImportPath()
{
    return settings.value("General/import_path", QStandardPaths::HomeLocation).toString();
}

void Configuration::setImportPath(QString path)
{
    path = path.trimmed();

    if(path.isNull() || path.isEmpty())
        path = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);

    settings.setValue("General/import_path", path);
}

QColor Configuration::getClearColor()
{
    return settings.value("Rendering/clear_color", QColor(53,53,53)).value<QColor>();
}

QColor Configuration::getGridColor()
{
    return settings.value("Rendering/grid_color", QColor(Qt::gray)).value<QColor>();
}

void Configuration::setClearColor(QColor color)
{
    settings.setValue("Rendering/clear_color", color);
}

void Configuration::setGridColor(QColor color)
{
    settings.setValue("Rendering/grid_color", color);
}

void Configuration::save()
{
    settings.sync();
    emit settingsSaved();
}

Configuration::Configuration()
    : settings(QDir::current().filePath( "config.ini"),
               QSettings::IniFormat, nullptr)
{

}

