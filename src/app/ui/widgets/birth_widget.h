#ifndef BIRTHWIDGET_H
#define BIRTHWIDGET_H

#include <QWidget>
#include <QLabel>

class BirthWidget : public QWidget
{
    Q_OBJECT
public:
    BirthWidget(QWidget *parent = 0);

signals:

public slots:

    void update();

private slots:
    void setIvyLeafSize(int value);

    void setIvyBranchSize(int value);

    void setLeafProbability(int value);

    void onBirth();

private:
    QLabel *ivyLeafSizeLabel;

    QLabel *ivyBranchSizeLabel;

    QLabel *leafProbabilityLabel;
};

#endif // BIRTHWIDGET_H
