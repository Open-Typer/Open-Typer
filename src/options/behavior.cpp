/*
 * behavior.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

behaviorOptions::behaviorOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::behaviorOptions)
{
	ui->setupUi(this);
	// Load settings
	settings = new QSettings(fileUtils::mainSettingsLocation(),QSettings::IniFormat);
	// Space bar newline
	if(settings->value("main/spacenewline","true").toBool())
		ui->spaceNewlineCheckBox->setCheckState(Qt::Checked);
	else
		ui->spaceNewlineCheckBox->setCheckState(Qt::Unchecked);
	// Error penalty
	ui->errorPenaltyBox->setValue(settings->value("main/errorpenalty","10").toInt());
	// Typing mode
	QStringList typingModes;
	typingModes += tr("Paper, whole text (default)","Shows the entire text on a paper");
	typingModes += tr("Paper, 2 lines","Shows current 2 lines of the text on a paper");
	typingModes += tr("Full screen, whole text","Shows the entire text");
	typingModes += tr("Full screen, 2 lines","Shows current 2 lines");
	ui->typingModeComboBox->addItems(typingModes);
	ui->typingModeComboBox->setCurrentIndex(settings->value("main/typingmode","0").toInt());
	// Connect
	// Space bar newline checkbox
	connect(ui->spaceNewlineCheckBox,SIGNAL(clicked(bool)),this,SLOT(setSpaceNewline(bool)));
	// Error penalty box
	connect(ui->errorPenaltyBox,SIGNAL(valueChanged(int)),this,SLOT(setErrorPenalty(int)));
	// Typing mode combobox
	connect(ui->typingModeComboBox,SIGNAL(activated(int)),this,SLOT(setTypingMode(int)));
}

behaviorOptions::~behaviorOptions()
{
	delete ui;
}

void behaviorOptions::setSpaceNewline(bool value)
{
	if(value)
		settings->setValue("main/spacenewline","true");
	else
		settings->setValue("main/spacenewline","false");
}

void behaviorOptions::setErrorPenalty(int value)
{
	settings->setValue("main/errorpenalty",value);
}

void behaviorOptions::setTypingMode(int mode)
{
	settings->setValue("main/typingmode",mode);
}
