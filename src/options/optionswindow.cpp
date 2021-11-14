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
	setupList();
	// Connections
	connect(ui->list,SIGNAL(currentRowChanged(int)),this,SLOT(changeOptionWidget(int)));
}

void optionsWindow::setupList(void)
{
	ui->list->clear();
	QStringList optionLabels;
	// List of options
	optionLabels += tr("Language");
	optionLabels += tr("Keyboard");
	optionLabels += tr("Customization");
	ui->list->addItems(optionLabels);
}

optionsWindow::~optionsWindow()
{
	delete ui;
}

void optionsWindow::changeOptionWidget(int index)
{
	switch(index) {
		case 0:
			{
				// Language
				languageList *options = new languageList;
				options->setStyleSheet(styleSheet());
				connect(options,SIGNAL(languageChanged(int)),this,SLOT(changeLanguage(int)));
				ui->currentOptions->setWidget(options);
			}
			break;
		case 1:
			{
				// Keyboard
				keyboardOptions *options = new keyboardOptions;
				options->setStyleSheet(styleSheet());
				ui->currentOptions->setWidget(options);
			}
			break;
		case 2:
			{
				// Customization
				customizationOptions *options = new customizationOptions;
				options->setStyleSheet(styleSheet());
				ui->currentOptions->setWidget(options);
				options->init();
			}
			break;
	}
}

void optionsWindow::init(void)
{
	ui->list->setCurrentRow(0);
}

void optionsWindow::changeLanguage(int index)
{
	emit languageChanged(index);
}

void optionsWindow::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::LanguageChange)
	{
		ui->retranslateUi(this);
		setupList();
	}
	else
		QWidget::changeEvent(event);
}
