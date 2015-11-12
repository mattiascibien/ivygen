#include "pathedit.h"
#include "ui_pathedit.h"

#include <QFileDialog>

PathEdit::PathEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PathEdit)
{
    ui->setupUi(this);

    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePath(QString)));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(choosePath()));
}

PathEdit::~PathEdit()
{
    delete ui;
}

void PathEdit::setPath(const QString path)
{
    ui->lineEdit->setText(path);
}

void PathEdit::choosePath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(!dir.isNull() && !dir.isEmpty())
    {
       setPath(dir);
    }
}

void PathEdit::updatePath(QString text)
{
    m_path = text;
}
