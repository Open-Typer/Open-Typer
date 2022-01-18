/*
 * paperconfigdialog.cpp
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

#include "paperconfigdialog.h"
#include "ui_paperconfigdialog.h"

/*! Constructs the paperConfigDialog dialog. */
paperConfigDialog::paperConfigDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::paperConfigDialog)
{
	ui->setupUi(this);
	// Connections
	connect(ui->loadButton,SIGNAL(clicked()),this,SLOT(accept()));
	connect(ui->lineLengthBox,SIGNAL(valueChanged(int)),this,SLOT(updateLineLength(int)));
	connect(ui->includeNewLinesCheckBox,SIGNAL(toggled(bool)),this,SLOT(setIncludeNewLines(bool)));
	// Set initial values
	updateLineLength(ui->lineLengthBox->value());
	ui->includeNewLinesCheckBox->setChecked(true);
	setIncludeNewLines(ui->includeNewLinesCheckBox->isChecked());
}

/*! Destroys the paperConfigDialog object. */
paperConfigDialog::~paperConfigDialog()
{
	delete ui;
}

/*!
 * Overrides QDialog#reject().\n
 * Prevents the window from being closed.
 */
void paperConfigDialog::reject(void) { }

/*!
 * Connected from lineLengthBox.\n
 * Sets line length.
 */
void paperConfigDialog::updateLineLength(int value)
{
	lineLength = value;
}

/*!
 * Connected from includeNewLinesCheckBox->toggled().\n
 * Sets new line inclusion.
 */
void paperConfigDialog::setIncludeNewLines(bool value)
{
	includeNewLines = value;
}
