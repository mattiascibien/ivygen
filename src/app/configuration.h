#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QSettings>
#include <QColor>

class Configuration : public QObject
{
    Q_OBJECT
public:
    static Configuration& getInstance()
    {
        static Configuration instance;
        return instance;
    }

    //General
    QString getExportPath();
    void setExportPath(QString path);

    QString getImportPath();
    void setImportPath(QString path);

    //Rendering
    QColor getClearColor();
    void setClearColor(QColor color);

    QColor getGridColor();
    void setGridColor(QColor color);

    void save();

private:
    Configuration();

    QSettings settings;

signals:
    void settingsSaved();
};

#endif // CONFIGURATION_H
