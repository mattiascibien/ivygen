#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include <QtDesigner/QtDesigner>
#include <QWidget>


namespace Ui {
class ColorPickerWidget;
}

class ColorPickerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPickerWidget(QWidget *parent = 0, QColor color = Qt::white);
    ~ColorPickerWidget();

public slots:
    QColor getColor()
    {
        return color;
    }

    void setColor(const QColor& color);

private slots:
    void on_pickButton_clicked();

private:
    Ui::ColorPickerWidget *ui;

    QColor color;
};

#endif // COLORPICKERWIDGET_H
