#include "configuration.h"

#include <QDir>

Configuration::Configuration()
    : settings(QDir::current().filePath( "config.ini"),
               QSettings::IniFormat, nullptr)
{

}

