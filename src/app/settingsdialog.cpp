#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "configuration.h"
#include "plugin_loader.h"

void SettingsDialog::reloadSettings()
{
    ui->importPathEdit->setPath(Configuration::getInstance().getImportPath());
    ui->exportPathEdit->setPath(Configuration::getInstance().getExportPath());

    ui->clearColorPicker->setColor(Configuration::getInstance().getClearColor());
    ui->gridColorPicker->setColor(Configuration::getInstance().getGridColor());
}

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    populatePlugins();

    reloadSettings();

    ui->stackedWidget->setCurrentIndex(0);

    connect(&Configuration::getInstance(), &Configuration::settingsSaved, this, &SettingsDialog::reloadSettings);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}



void SettingsDialog::on_SettingsDialog_accepted()
{
    Configuration::getInstance().setImportPath(ui->importPathEdit->path());
    Configuration::getInstance().setExportPath(ui->exportPathEdit->path());

    Configuration::getInstance().setClearColor(ui->clearColorPicker->getColor());
    Configuration::getInstance().setGridColor(ui->gridColorPicker->getColor());

    Configuration::getInstance().save();
}

void SettingsDialog::on_SettingsDialog_rejected()
{
    //Save without updating so it triggers the settingsChanged signal
    Configuration::getInstance().save();
}

void SettingsDialog::populatePlugins()
{
    QList<ImporterInterface*> *importers = PluginLoader::getInstance().getImporters();

    for(ImporterInterface* importer : *importers)
    {
        ui->importersListWidget->addItem(importer->getName());
    }
}
