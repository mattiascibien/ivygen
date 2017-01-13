#ifndef PATHEDIT_H
#define PATHEDIT_H

#include <QWidget>

namespace Ui {
class PathEdit;
}

class PathEdit : public QWidget
{
    Q_OBJECT

public:
    explicit PathEdit(QWidget *parent = 0);
    ~PathEdit();

    void setPath(const QString path);

    QString path() const
    {
        return m_path;
    }

private:
    Ui::PathEdit *ui;
    QString m_path;

private slots:
    void choosePath();

    void updatePath(QString text);
};

#endif // PATHEDIT_H
