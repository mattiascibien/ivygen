#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QSettings>
#include <QColor>

class Configuration
{
    Q_OBJECT
public:
    static Configuration& getInstance()
    {
        static Configuration instance;
        return instance;
    }

    QColor getClearColor()
    {
        return settings.value("Rendering/clear_color", QColor(Qt::white)).value<QColor>();
    }

    QColor getGridColor()
    {
        return settings.value("Rendering/grid_color", QColor(Qt::gray)).value<QColor>();
    }

    void setClearColor(QColor color)
    {
        settings.setValue("Rendering/clear_color", color);
    }

    void setGridColor(QColor color)
    {
        settings.setValue("Rendering/grid_color", color);
    }

    void save()
    {
        settings.sync();
        emit settingsSaved();
    }

private:
    Configuration();

    QSettings settings;

 signals:
    settingsSaved();
};

#endif // CONFIGURATION_H
