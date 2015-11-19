#include "birth_widget.h"

#include <QVBoxLayout>

#include "Common.h"

BirthWidget::BirthWidget(QWidget *parent) : QWidget(parent)
{
    QSlider *ivyLeafSizeSlider = new QSlider();

    ivyLeafSizeSlider->setOrientation(Qt::Horizontal);

    ivyLeafSizeSlider->setRange(0, 1000);

    ivyLeafSizeSlider->setValue(Common::ivy.ivyLeafSize * 500.0f);

    ivyLeafSizeSlider->setToolTip("defines the size of the leaf geometry relative to the ivy size");

    connect(ivyLeafSizeSlider, &QSlider::valueChanged, this, &BirthWidget::setIvyLeafSize);


    QSlider *ivyBranchSizeSlider = new QSlider();

    ivyBranchSizeSlider->setOrientation(Qt::Horizontal);

    ivyBranchSizeSlider->setRange(0, 1000);

    ivyBranchSizeSlider->setValue(Common::ivy.ivyBranchSize * 2000.0f);

    ivyBranchSizeSlider->setToolTip("defines the diameter of the branch geometry relative to the ivy size");

    connect(ivyBranchSizeSlider, &QSlider::valueChanged, this, &BirthWidget::setIvyBranchSize);


    ivyLeafSizeLabel = new QLabel();

    ivyLeafSizeLabel->setText("ivy leaf size: " + QString::number(Common::ivy.ivyLeafSize));

    ivyLeafSizeLabel->setToolTip("defines the size of the leaf geometry relative to the ivy size");


    leafProbabilityLabel = new QLabel();

    leafProbabilityLabel->setText("leaf density: " + QString::number(Common::ivy.leafProbability));

    leafProbabilityLabel->setToolTip("defines the density of the leaves during geometry generation");


    ivyBranchSizeLabel = new QLabel();

    ivyBranchSizeLabel->setText("ivy branch size: " + QString::number(Common::ivy.ivyBranchSize));

    ivyBranchSizeLabel->setToolTip("defines the diameter of the branch geometry relative to the ivy size");

    QSlider *leafProbabilitySlider = new QSlider();

    leafProbabilitySlider->setOrientation(Qt::Horizontal);

    leafProbabilitySlider->setRange(0, 1000);

    leafProbabilitySlider->setValue(Common::ivy.leafProbability * 1000.0f);

    leafProbabilitySlider->setToolTip("defines the density of the leaves during geometry generation");

    connect(leafProbabilitySlider, &QSlider::valueChanged, this, &BirthWidget::setLeafProbability);


    QPushButton *birthButton = new QPushButton("birth");

    connect(birthButton, &QPushButton::clicked, this, &BirthWidget::onBirth);

    birthButton->setToolTip("generate the ivy geometry");


    QVBoxLayout *birthLayout = new QVBoxLayout();

    birthLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    birthLayout->setMargin(10);

    birthLayout->setSpacing(5);

    birthLayout->addWidget(ivyBranchSizeSlider);

    birthLayout->addWidget(ivyBranchSizeLabel);

    birthLayout->addWidget(ivyLeafSizeSlider);

    birthLayout->addWidget(ivyLeafSizeLabel);

    birthLayout->addWidget(leafProbabilitySlider);

    birthLayout->addWidget(leafProbabilityLabel);

    birthLayout->addWidget(birthButton);

    setLayout(birthLayout);
}

void BirthWidget::update()
{
    QWidget::update();

    ivyLeafSizeLabel->setText("ivy leaf size: " + QString::number(Common::ivy.ivyLeafSize));

    ivyBranchSizeLabel->setText("ivy branch size: " + QString::number(Common::ivy.ivyBranchSize));

    leafProbabilityLabel->setText("leaf probability: " + QString::number(Common::ivy.leafProbability));
}

void BirthWidget::setIvyLeafSize(int value)
{
    Common::ivy.ivyLeafSize = (float)value / 500.0f;

    ivyLeafSizeLabel->setText("ivy leaf size: " + QString::number(Common::ivy.ivyLeafSize));
}


void BirthWidget::setIvyBranchSize(int value)
{
    Common::ivy.ivyBranchSize = (float)value / 2000.0f;

    ivyBranchSizeLabel->setText("ivy branch size: " + QString::number(Common::ivy.ivyBranchSize));
}

void BirthWidget::setLeafProbability(int value)
{
    Common::ivy.leafProbability = (float)value / 1000.0f;

    leafProbabilityLabel->setText("leaf probability: " + QString::number(Common::ivy.leafProbability));
}

void BirthWidget::onBirth()
{
    Common::timer->stop();

    Common::ivy.birth();

    Common::renderWidget->updateGL();
}
