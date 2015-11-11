#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
#ifdef GIT_VERSION 
    ui->labelVersion->setText(QString(GIT_VERSION));
#else
    ui->labelVersion->setText(QString("Unknown version"));
#endif
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
