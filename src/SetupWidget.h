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


#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H


#include <QtWidgets>


/** setup widget */
class SetupWidget : public QWidget
{
    Q_OBJECT

public:

    SetupWidget(QWidget *parent = 0);

    ~SetupWidget();

public slots:

	void update();

private slots:

	void setIvySize(int value);

	void setBranchingProbability(int value);

    void setMaxFloatLength(int value);

	void setMaxAdhesionDistance(int value);

	void setPrimaryWeight(int value);

	void setRandomWeight(int value);

	void setGravityWeight(int value);

	void setAdhesionWeight(int value);

	void onGrow();

	void onToggleGrow(bool checked);

private:

	QTimer *timer;
	
	QLabel *ivySizeLabel;

	QLabel *branchingProbabilityLabel;

	QLabel *maxFloatLengthLabel;

	QLabel *maxAdhesionDistanceLabel;

	QLabel *primaryWeightLabel;

	QLabel *randomWeightLabel;

	QLabel *gravityWeightLabel;

	QLabel *adhesionWeightLabel;

	QLabel *aliveRootLabel;

	QPushButton *growButton;

};

#endif
