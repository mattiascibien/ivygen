#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "configuration.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->clearColorPicker->setColor(Configuration::getInstance().getClearColor());
    ui->gridColorPicker->setColor(Configuration::getInstance().getGridColor());

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}



void SettingsDialog::on_SettingsDialog_accepted()
{
    Configuration::getInstance().setClearColor(ui->clearColorPicker->getColor());
    Configuration::getInstance().setGridColor(ui->gridColorPicker->getColor());

    Configuration::getInstance().save();
}
