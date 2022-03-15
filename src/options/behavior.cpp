/*
 * behavior.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
 *
 * Open-Typer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Open-Typer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open-Typer. If not, see <http://www.gnu.org/licenses/>.
 */

#include "options/behavior.h"
#include "ui_behavior.h"

/*! Constructs behaviorOptions. */
behaviorOptions::behaviorOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::behaviorOptions),
	settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat)
{
	ui->setupUi(this);
	// Load settings
	// Space bar newline
	if(settings.value("main/spacenewline","true").toBool())
		ui->spaceNewlineCheckBox->setCheckState(Qt::Checked);
	else
		ui->spaceNewlineCheckBox->setCheckState(Qt::Unchecked);
	// Error penalty
	ui->errorPenaltyBox->setValue(settings.value("main/errorpenalty","10").toInt());
	// Connect
	// Space bar newline checkbox
	connect(ui->spaceNewlineCheckBox,SIGNAL(clicked(bool)),this,SLOT(setSpaceNewline(bool)));
	// Error penalty box
	connect(ui->errorPenaltyBox,SIGNAL(valueChanged(int)),this,SLOT(setErrorPenalty(int)));
}

/*! Destroys the behaviorOptions object. */
behaviorOptions::~behaviorOptions()
{
	delete ui;
}

/*!
 * Connected from spaceNewlineCheckBox#clicked().
 * Enables or disables space bar at the end of line.
 */
void behaviorOptions::setSpaceNewline(bool value)
{
	if(value)
		settings.setValue("main/spacenewline","true");
	else
		settings.setValue("main/spacenewline","false");
}

/*!
 * Connected from errorPenaltyBox#valueChanged().
 * Sets error penalty value.
 */
void behaviorOptions::setErrorPenalty(int value)
{
	settings.setValue("main/errorpenalty",value);
}
