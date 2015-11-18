#include "pathedit.h"
#include "ui_pathedit.h"

#include <QFileDialog>

PathEdit::PathEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PathEdit)
{
    ui->setupUi(this);

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &PathEdit::updatePath);
    connect(ui->pushButton, &QPushButton::clicked, this,&PathEdit::choosePath);
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
