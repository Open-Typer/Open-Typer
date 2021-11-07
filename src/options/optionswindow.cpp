/*
 * optionswindow.cpp
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

#include "options/optionswindow.h"
#include "ui_optionswindow.h"

optionsWindow::optionsWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::optionsWindow)
{
	ui->setupUi(this);
	QStringList optionLabels;
	// List of options
	optionLabels += tr("Keyboard");
	ui->list->addItems(optionLabels);
	// Connections
	connect(ui->list,SIGNAL(currentRowChanged(int)),this,SLOT(changeOptionWidget(int)));
}

optionsWindow::~optionsWindow()
{
	delete ui;
}

void optionsWindow::changeOptionWidget(int index)
{
	switch(index) {
		case 0:
			// Keyboard
			keyboardOptions *options = new keyboardOptions;
			options->setStyleSheet(styleSheet());
			ui->currentOptions->setWidget(options);
			break;
	}
}

void optionsWindow::init(void)
{
	ui->list->setCurrentRow(0);
}
