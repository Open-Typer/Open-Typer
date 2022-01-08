/*
 * optionswindow.cpp
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

#include "options/optionswindow.h"
#include "ui_optionswindow.h"

/*! Constructs optionsWindow. */
optionsWindow::optionsWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::optionsWindow)
{
	ui->setupUi(this);
	setupList();
	// Connections
	connect(ui->list,SIGNAL(currentRowChanged(int)),this,SLOT(changeOptionWidget(int)));
}

/*! Sets up list of categories. */
void optionsWindow::setupList(void)
{
	int oldIndex = ui->list->currentRow();
	ui->list->clear();
	QStringList optionLabels;
	// List of options
	optionLabels += tr("Language");
	optionLabels += tr("Behavior");
	optionLabels += tr("Keyboard");
	optionLabels += tr("Customization");
#ifndef Q_OS_WASM
	optionLabels += tr("Connection");
#endif
	ui->list->addItems(optionLabels);
	ui->list->setCurrentRow(oldIndex);
}

/*! Destroys the optionsWindow object. */
optionsWindow::~optionsWindow()
{
	delete ui;
}

/*! Opens selected category. */
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
				// Behavior
				behaviorOptions *options = new behaviorOptions;
				options->setStyleSheet(styleSheet());
				ui->currentOptions->setWidget(options);
			}
			break;
		case 2:
			{
				// Keyboard
				keyboardOptions *options = new keyboardOptions;
				options->setStyleSheet(styleSheet());
				ui->currentOptions->setWidget(options);
			}
			break;
		case 3:
			{
				// Customization
				customizationOptions *options = new customizationOptions;
				options->setStyleSheet(styleSheet());
				ui->currentOptions->setWidget(options);
				options->init();
			}
			break;
		case 4:
			{
				// Connection
				connectionOptions *options = new connectionOptions;
				options->setStyleSheet(styleSheet());
				ui->currentOptions->setWidget(options);
			}
			break;
	}
}

/*! Selects default category. */
void optionsWindow::init(void)
{
	ui->list->setCurrentRow(0);
}

/*!
 * Connected from languageList#languageChanged().\n
 * Emits languageChanged() signal so that parent windows can retranslate the UI.
 */
void optionsWindow::changeLanguage(int index)
{
	emit languageChanged(index);
}

/*! Overrides QWidget#changeEvent().
 * Retranslates UI when the display language changes.
 */
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
