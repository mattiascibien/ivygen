#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "configuration.h"

void SettingsDialog::reloadSettings()
{
    ui->importPathLineEdit->setText(Configuration::getInstance().getImportPath());
    ui->exportPathLineEdit->setText(Configuration::getInstance().getExportPath());

    ui->clearColorPicker->setColor(Configuration::getInstance().getClearColor());
    ui->gridColorPicker->setColor(Configuration::getInstance().getGridColor());
}

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    reloadSettings();

    connect(&Configuration::getInstance(), SIGNAL(settingsSaved()), this, SLOT(reloadSettings()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}



void SettingsDialog::on_SettingsDialog_accepted()
{
    Configuration::getInstance().setImportPath(ui->importPathLineEdit->text());
    Configuration::getInstance().setExportPath(ui->exportPathLineEdit->text());

    Configuration::getInstance().setClearColor(ui->clearColorPicker->getColor());
    Configuration::getInstance().setGridColor(ui->gridColorPicker->getColor());

    Configuration::getInstance().save();
}

void SettingsDialog::on_SettingsDialog_rejected()
{
    //Save without updating so it triggers the settingsChanged signal
    Configuration::getInstance().save();
}
