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


#include "IvyGenerator.h"
#include "Common.h"


IvyGenerator::IvyGenerator(QWidget *parent) : QWidget(parent)
{
	setGeometry(100, 100, 1024, 768);

	setWindowTitle("IvyGenerator  - Copyright (c) 2007 Thomas Luft");


	Common::renderWidget = new RenderWidget();

	Common::setupWidget = new SetupWidget();


	QSplitter *splitter = new QSplitter(Qt::Horizontal);

	splitter->addWidget(Common::renderWidget);

	splitter->addWidget(Common::setupWidget);

	splitter->setFrameShadow(QFrame::Shadow::Sunken);

	splitter->setFrameShape(QFrame::Shape::Panel);

	QList<int> sizeList;

	sizeList.push_back(width()-300);

	sizeList.push_back(300);

	splitter->setSizes(sizeList);

	
	QGridLayout *layout = new QGridLayout();

	layout->setMargin(0);

	layout->addWidget(splitter);

	setLayout(layout);
}


IvyGenerator::~IvyGenerator()
{
}

