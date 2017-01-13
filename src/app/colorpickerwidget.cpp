#include "colorpickerwidget.h"
#include "ui_colorpickerwidget.h"

#include <QColorDialog>

ColorPickerWidget::ColorPickerWidget(QWidget *parent, QColor color) :
    QWidget(parent),
    ui(new Ui::ColorPickerWidget)
{
    ui->setupUi(this);
    setColor(color);
}

ColorPickerWidget::~ColorPickerWidget()
{
    delete ui;
}

void ColorPickerWidget::on_pickButton_clicked()
{
    setColor(QColorDialog::getColor(this->color));
}

void ColorPickerWidget::setColor(const QColor& color)
{
    this->color = color;

    QPalette palette = ui->colorLabel->palette();
    palette.setColor(ui->colorLabel->backgroundRole(), this->color );
    ui->colorLabel->setPalette(palette);

    ui->colorLineEdit->setText(this->color.name());
}
