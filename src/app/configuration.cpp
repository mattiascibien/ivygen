#include "configuration.h"

#include <QDir>

QString Configuration::getExportPath()
{
    return settings.value("General/export_path", "C:\\").toString();
}

void Configuration::setExportPath(QString path)
{
    settings.setValue("General/export_path", path);
}

QString Configuration::getImportPath()
{
    return settings.value("General/import_path", "C:\\").toString();
}

void Configuration::setImportPath(QString path)
{
    settings.setValue("General/import_path", path);
}

QColor Configuration::getClearColor()
{
    return settings.value("Rendering/clear_color", QColor(Qt::white)).value<QColor>();
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

