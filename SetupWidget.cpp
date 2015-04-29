/**************************************************************************************
**
**  Copyright (C) 2006 Thomas Luft, University of Konstanz. All rights reserved.
**
**  This file is part of the Ivy Generator Tool.
**
**  This program is free software; you can redistribute it and/or modify it
**  under the terms of the GNU General Public License as published by the
**  Free Software Foundation; either version 2 of the License, or (at your
**  option) any later version.
**  This program is distributed in the hope that it will be useful, but
**  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
**  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
**  for more details.
**  You should have received a copy of the GNU General Public License along
**  with this program; if not, write to the Free Software Foundation,
**  Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA 
**
***************************************************************************************/


#include "SetupWidget.h"
#include "Common.h"
#include "OBJLoader.h"
#include "OBJWriter.h"


SetupWidget::SetupWidget(QWidget *parent) : QWidget(parent)
{
	QSlider *ivySizeSlider = new QSlider();

	ivySizeSlider->setOrientation(Qt::Horizontal);

	ivySizeSlider->setRange(0, 1000);

	ivySizeSlider->setValue(Common::ivy.ivySize * 20000.0f);

	ivySizeSlider->setToolTip("adapts the ivy growing and geometry to the scene size and content");

	connect(ivySizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIvySize(int)));


	QSlider *ivyLeafSizeSlider = new QSlider();

	ivyLeafSizeSlider->setOrientation(Qt::Horizontal);

	ivyLeafSizeSlider->setRange(0, 1000);

	ivyLeafSizeSlider->setValue(Common::ivy.ivyLeafSize * 500.0f);

	ivyLeafSizeSlider->setToolTip("defines the size of the leaf geometry relative to the ivy size");

	connect(ivyLeafSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIvyLeafSize(int)));


	QSlider *ivyBranchSizeSlider = new QSlider();

	ivyBranchSizeSlider->setOrientation(Qt::Horizontal);

	ivyBranchSizeSlider->setRange(0, 1000);

	ivyBranchSizeSlider->setValue(Common::ivy.ivyBranchSize * 2000.0f);

	ivyBranchSizeSlider->setToolTip("defines the diameter of the branch geometry relative to the ivy size");

	connect(ivyBranchSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIvyBranchSize(int)));


	QSlider *branchingProbabilitySlider = new QSlider();

	branchingProbabilitySlider->setOrientation(Qt::Horizontal);

	branchingProbabilitySlider->setRange(0, 1000);

	branchingProbabilitySlider->setValue(Common::ivy.branchingProbability * 1000.0f);

	branchingProbabilitySlider->setToolTip("defines the density of the branching structure during growing");

	connect(branchingProbabilitySlider, SIGNAL(valueChanged(int)), this, SLOT(setBranchingProbability(int)));


	QSlider *leafProbabilitySlider = new QSlider();

	leafProbabilitySlider->setOrientation(Qt::Horizontal);

	leafProbabilitySlider->setRange(0, 1000);

	leafProbabilitySlider->setValue(Common::ivy.leafProbability * 1000.0f);

	leafProbabilitySlider->setToolTip("defines the density of the leaves during geometry generation");

	connect(leafProbabilitySlider, SIGNAL(valueChanged(int)), this, SLOT(setLeafProbability(int)));


	QSlider *maxFloatLengthSlider = new QSlider();

	maxFloatLengthSlider->setOrientation(Qt::Horizontal);

	maxFloatLengthSlider->setRange(0, 1000);

	maxFloatLengthSlider->setValue(Common::ivy.maxFloatLength * 1000.0f);

	maxFloatLengthSlider->setToolTip("defines the length at which a freely floating branch will die");

	connect(maxFloatLengthSlider, SIGNAL(valueChanged(int)), this, SLOT(setMaxFloatLength(int)));


	QSlider *maxAdhesionDistanceSlider = new QSlider();

	maxAdhesionDistanceSlider->setOrientation(Qt::Horizontal);

	maxAdhesionDistanceSlider->setRange(0, 1000);

	maxAdhesionDistanceSlider->setValue(Common::ivy.maxAdhesionDistance * 1000.0f);

	maxAdhesionDistanceSlider->setToolTip("defines the maximum distance to a surface at which the surface will attract the ivy");

	connect(maxAdhesionDistanceSlider, SIGNAL(valueChanged(int)), this, SLOT(setMaxAdhesionDistance(int)));


	QSlider *primaryWeightSlider = new QSlider();

	primaryWeightSlider->setOrientation(Qt::Horizontal);

	primaryWeightSlider->setRange(0, 1000);

	primaryWeightSlider->setValue(Common::ivy.primaryWeight * 1000.0f);

	primaryWeightSlider->setToolTip("defines the weight of the primary growing direction");

	connect(primaryWeightSlider, SIGNAL(valueChanged(int)), this, SLOT(setPrimaryWeight(int)));


	QSlider *randomWeightSlider = new QSlider();

	randomWeightSlider->setOrientation(Qt::Horizontal);

	randomWeightSlider->setRange(0, 1000);

	randomWeightSlider->setValue(Common::ivy.randomWeight * 1000.0f);

	randomWeightSlider->setToolTip("defines the weight of the a random growing direction");

	connect(randomWeightSlider, SIGNAL(valueChanged(int)), this, SLOT(setRandomWeight(int)));


	QSlider *gravityWeightSlider = new QSlider();

	gravityWeightSlider->setOrientation(Qt::Horizontal);

	gravityWeightSlider->setRange(0, 2000);

	gravityWeightSlider->setValue(Common::ivy.gravityWeight * 1000.0f);

	gravityWeightSlider->setToolTip("defines the weight of gravity");

	connect(gravityWeightSlider, SIGNAL(valueChanged(int)), this, SLOT(setGravityWeight(int)));


	QSlider *adhesionWeightSlider = new QSlider();

	adhesionWeightSlider->setOrientation(Qt::Horizontal);

	adhesionWeightSlider->setRange(0, 1000);

	adhesionWeightSlider->setValue(Common::ivy.adhesionWeight * 1000.0f);

	adhesionWeightSlider->setToolTip("defines the weight of the adhesion towards attracting surfaces");

	connect(adhesionWeightSlider, SIGNAL(valueChanged(int)), this, SLOT(setAdhesionWeight(int)));




	ivySizeLabel = new QLabel();

	ivySizeLabel->setText("ivy size: " + QString::number(Common::ivy.ivySize));

	ivySizeLabel->setToolTip("adapts the ivy growing and geometry to the scene size and content");

	
	ivyLeafSizeLabel = new QLabel();

	ivyLeafSizeLabel->setText("ivy leaf size: " + QString::number(Common::ivy.ivyLeafSize));

	ivyLeafSizeLabel->setToolTip("defines the size of the leaf geometry relative to the ivy size");


	ivyBranchSizeLabel = new QLabel();

	ivyBranchSizeLabel->setText("ivy branch size: " + QString::number(Common::ivy.ivyBranchSize));

	ivyBranchSizeLabel->setToolTip("defines the diameter of the branch geometry relative to the ivy size");

    
	branchingProbabilityLabel = new QLabel();

	branchingProbabilityLabel->setText("branching probability: " + QString::number(Common::ivy.branchingProbability));

	branchingProbabilityLabel->setToolTip("defines the density of the branching structure during growing");


	leafProbabilityLabel = new QLabel();

	leafProbabilityLabel->setText("leaf density: " + QString::number(Common::ivy.leafProbability));

	leafProbabilityLabel->setToolTip("defines the density of the leaves during geometry generation");


	maxFloatLengthLabel = new QLabel();

	maxFloatLengthLabel->setText("max floating length: " + QString::number(Common::ivy.maxFloatLength));

	maxFloatLengthLabel->setToolTip("defines the length at which a freely floating branch will die");


	maxAdhesionDistanceLabel = new QLabel();

	maxAdhesionDistanceLabel->setText("max adhesion distance: " + QString::number(Common::ivy.maxAdhesionDistance));

	maxAdhesionDistanceLabel->setToolTip("defines the maximum distance to a surface at which the surface will attract the ivy");


	primaryWeightLabel = new QLabel();

	primaryWeightLabel->setText("primary weight: " + QString::number(Common::ivy.primaryWeight));

	primaryWeightLabel->setToolTip("defines the weight of the primary growing direction");


	randomWeightLabel = new QLabel();

	randomWeightLabel->setText("random weight: " + QString::number(Common::ivy.randomWeight));

	randomWeightLabel->setToolTip("defines the weight of the a random growing direction");


	gravityWeightLabel = new QLabel();

	gravityWeightLabel->setText("gravity weight: " + QString::number(Common::ivy.gravityWeight));

	gravityWeightLabel->setToolTip("defines the weight of gravity");


	adhesionWeightLabel = new QLabel();

	adhesionWeightLabel->setText("adhesion weight: " + QString::number(Common::ivy.adhesionWeight));

	adhesionWeightLabel->setToolTip("defines the weight of the adhesion towards attracting surfaces");


	aliveRootLabel = new QLabel();

	aliveRootLabel->setText("living branches: " + QString::number(Common::ivy.roots.size()));

	aliveRootLabel->setToolTip("shows the number of ivy branches alive");


	QPushButton *importButton = new QPushButton("import obj+mlt");

	connect(importButton, SIGNAL(clicked()), this, SLOT(onImportObj()));

	importButton->setToolTip("import your scene the ivy should grow in");


	QPushButton *exportButton = new QPushButton("export obj+mlt");

	connect(exportButton, SIGNAL(clicked()), this, SLOT(onExportObj()));

	exportButton->setToolTip("export the ivy geometry");


	growButton = new QPushButton("grow");

	growButton->setCheckable(true);
    
	connect(growButton, SIGNAL(clicked(bool)), this, SLOT(onToggleGrow(bool)));

	growButton->setToolTip("start and stop the growing of the ivy");


	QPushButton *birthButton = new QPushButton("birth");

	connect(birthButton, SIGNAL(clicked()), this, SLOT(onBirth()));

	birthButton->setToolTip("generate the ivy geometry");


	QPushButton *flipNormalsButton = new QPushButton("flip normals");

	connect(flipNormalsButton, SIGNAL(clicked()), this, SLOT(onFlipNormals()));

	flipNormalsButton->setToolTip("flip the surface normals of your scene, if the ivy is growing inside");



	QGroupBox *sceneWidget = new QGroupBox("scene");

	QGridLayout *sceneLayout = new QGridLayout();

	sceneLayout->setMargin(10);

	sceneLayout->setSpacing(5);

	sceneLayout->addWidget(importButton, 0, 0);

	sceneLayout->addWidget(exportButton, 0, 1);

	sceneLayout->addWidget(flipNormalsButton, 1, 0);

	sceneWidget->setLayout(sceneLayout);



	QGroupBox *growWidget = new QGroupBox("growing");

	QVBoxLayout *growLayout = new QVBoxLayout();

	growLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	growLayout->setMargin(10);

	growLayout->setSpacing(5);

	growLayout->addWidget(ivySizeSlider);

	growLayout->addWidget(ivySizeLabel);

	growLayout->addWidget(primaryWeightSlider);

	growLayout->addWidget(primaryWeightLabel);

	growLayout->addWidget(randomWeightSlider);

	growLayout->addWidget(randomWeightLabel);

	growLayout->addWidget(gravityWeightSlider);

	growLayout->addWidget(gravityWeightLabel);

	growLayout->addWidget(adhesionWeightSlider);

	growLayout->addWidget(adhesionWeightLabel);

	growLayout->addWidget(branchingProbabilitySlider);

	growLayout->addWidget(branchingProbabilityLabel);

	growLayout->addWidget(maxFloatLengthSlider);

	growLayout->addWidget(maxFloatLengthLabel);

	growLayout->addWidget(maxAdhesionDistanceSlider);

	growLayout->addWidget(maxAdhesionDistanceLabel);

	growLayout->addWidget(growButton);

	growLayout->addWidget(aliveRootLabel);	

	growWidget->setLayout(growLayout);



	QGroupBox *birthWidget = new QGroupBox("birth");

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

	birthWidget->setLayout(birthLayout);




	QVBoxLayout *layout = new QVBoxLayout();

	layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	layout->setMargin(10);

	layout->setSpacing(5);

	layout->addWidget(sceneWidget);

	layout->addWidget(growWidget);

	layout->addWidget(birthWidget);


	setLayout(layout);


	timer = new QTimer(this);

	timer->setInterval(1);

	connect(timer, SIGNAL(timeout()), this, SLOT(onGrow()));
}


SetupWidget::~SetupWidget()
{
}


void SetupWidget::update()
{
	QWidget::update();

	ivySizeLabel->setText("ivy size: " + QString::number(Common::ivy.ivySize));

	ivyLeafSizeLabel->setText("ivy leaf size: " + QString::number(Common::ivy.ivyLeafSize));

	ivyBranchSizeLabel->setText("ivy branch size: " + QString::number(Common::ivy.ivyBranchSize));

	branchingProbabilityLabel->setText("branching probability: " + QString::number(Common::ivy.branchingProbability));

	leafProbabilityLabel->setText("leaf probability: " + QString::number(Common::ivy.leafProbability));

	maxFloatLengthLabel->setText("max float length: " + QString::number(Common::ivy.maxFloatLength));

	maxAdhesionDistanceLabel->setText("max adhesion distance: " + QString::number(Common::ivy.maxAdhesionDistance));

	primaryWeightLabel->setText("primary weight: " + QString::number(Common::ivy.primaryWeight));

	randomWeightLabel->setText("random weight: " + QString::number(Common::ivy.randomWeight));

	gravityWeightLabel->setText("gravity weight: " + QString::number(Common::ivy.gravityWeight));

	adhesionWeightLabel->setText("adhesion weight: " + QString::number(Common::ivy.adhesionWeight));
}


void SetupWidget::setIvySize(int value)
{
	Common::ivy.ivySize = (float)value / 20000.0f;

	ivySizeLabel->setText("ivy Size: " + QString::number(Common::ivy.ivySize));
}


void SetupWidget::setIvyLeafSize(int value)
{
	Common::ivy.ivyLeafSize = (float)value / 500.0f;

	ivyLeafSizeLabel->setText("ivy leaf size: " + QString::number(Common::ivy.ivyLeafSize));
}


void SetupWidget::setIvyBranchSize(int value)
{
	Common::ivy.ivyBranchSize = (float)value / 2000.0f;

	ivyBranchSizeLabel->setText("ivy branch size: " + QString::number(Common::ivy.ivyBranchSize));
}


void SetupWidget::setBranchingProbability(int value)
{
	Common::ivy.branchingProbability = (float)value / 1000.0f;

	branchingProbabilityLabel->setText("branching probability: " + QString::number(Common::ivy.branchingProbability));
}


void SetupWidget::setLeafProbability(int value)
{
	Common::ivy.leafProbability = (float)value / 1000.0f;

	leafProbabilityLabel->setText("leaf probability: " + QString::number(Common::ivy.leafProbability));
}


void SetupWidget::setMaxFloatLength(int value)
{
	Common::ivy.maxFloatLength = (float)value / 1000.0f;

	maxFloatLengthLabel->setText("max float length: " + QString::number(Common::ivy.maxFloatLength));
}


void SetupWidget::setMaxAdhesionDistance(int value)
{
	Common::ivy.maxAdhesionDistance = (float)value / 1000.0f;

	maxAdhesionDistanceLabel->setText("max adhesion distance: " + QString::number(Common::ivy.maxAdhesionDistance));
}


void SetupWidget::setPrimaryWeight(int value)
{
	Common::ivy.primaryWeight = (float)value / 1000.0f;

	primaryWeightLabel->setText("primary weight: " + QString::number(Common::ivy.primaryWeight));
}


void SetupWidget::setRandomWeight(int value)
{
	Common::ivy.randomWeight = (float)value / 1000.0f;

	randomWeightLabel->setText("random weight: " + QString::number(Common::ivy.randomWeight));
}


void SetupWidget::setGravityWeight(int value)
{
	Common::ivy.gravityWeight = (float)value / 1000.0f;

	gravityWeightLabel->setText("gravity weight: " + QString::number(Common::ivy.gravityWeight));
}


void SetupWidget::setAdhesionWeight(int value)
{
	Common::ivy.adhesionWeight = (float)value / 1000.0f;

	adhesionWeightLabel->setText("adhesion weight: " + QString::number(Common::ivy.adhesionWeight));
}	


void SetupWidget::onImportObj()
{
	QString fileString = QFileDialog::getOpenFileName(NULL, "open a 3d object", "", "3d objects (*.obj)");

	if (fileString != "")
	{
		QFileInfo fileInfo(fileString);
		
		QString path = fileInfo.path() + "/";

		QString file = fileInfo.completeBaseName() + ".obj";


		Common::mesh.reset();


		OBJLoader::loadOBJ( path.toStdString(), file.toStdString(), Common::mesh );


		Common::mesh.loadTextures();

		Common::mesh.prepareData();

		Common::mesh.calculateVertexNormals();

		Common::mesh.prepareData();

		Common::mesh.createDisplayList();

		Common::camera.placeNicely( Common::mesh );

		update();
	}
}


void SetupWidget::onExportObj()
{
	QString fileString = QFileDialog::getSaveFileName(NULL, "save a 3d object", "", "3d objects (*.obj)");

	if (fileString != "")
	{
		QFileInfo fileInfo(fileString);
		
		QString path = fileInfo.path() + "/";

		QString file = fileInfo.completeBaseName() + ".obj";


		OBJWriter::writeOBJ( path.toStdString(), file.toStdString(), Common::ivy );
	}
}


void SetupWidget::onGrow()
{
	Common::ivy.grow();

	//compute alive roots
	unsigned int aliveCount = 0;

	for (std::vector<IvyRoot>::iterator root = Common::ivy.roots.begin(); root != Common::ivy.roots.end();  ++root)
	{
		if (root->alive) ++aliveCount;
	}

	aliveRootLabel->setText("living branches: " + QString::number(aliveCount) + " of " + QString::number(Common::ivy.roots.size()));

	if (aliveCount == 0)
	{
		timer->stop();

		growButton->setChecked(false);
	}



	//test
	//Common::ivy.birth();
	//Common::renderWidget->writeScreen();

	Common::renderWidget->updateGL();
}


void SetupWidget::onToggleGrow(bool checked)
{
	if (checked)
	{
		Common::ivy.reset();

		timer->start();
	}
	else
	{
		timer->stop();
	}
}


void SetupWidget::onBirth()
{
	timer->stop();

	Common::ivy.birth();

	Common::renderWidget->updateGL();
}


void SetupWidget::onFlipNormals()
{
	Common::mesh.flipNormals();

	Common::mesh.createDisplayList();

	Common::renderWidget->updateGL();
}
	

	


