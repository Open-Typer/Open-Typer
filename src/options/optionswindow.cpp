/*
 * optionswindow.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
 * Copyright (C) 2022 - yeti0904
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
	setAttribute(Qt::WA_DeleteOnClose, true);
	setupList();
	// Connections
	connect(ui->list, SIGNAL(currentRowChanged(int)), this, SLOT(changeOptionWidget(int)));
	connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &optionsWindow::accept);
}

/*! Sets up list of categories. */
void optionsWindow::setupList(void)
{
	int oldIndex = ui->list->currentRow();
	ui->list->clear();
	ui->list->setIconSize(QSize(36, 36));
	// List of options
	ui->list->addItem(new QListWidgetItem(QIcon(":res/images/languageOptions.svg"), tr("Language")));
	ui->list->addItem(new QListWidgetItem(QIcon(":res/images/behaviorOptions.svg"), tr("Behavior")));
	ui->list->addItem(new QListWidgetItem(QIcon(":res/images/keyboardOptions.svg"), tr("Keyboard")));
	ui->list->addItem(new QListWidgetItem(QIcon(":res/images/appearanceOptions.svg"), tr("Appearance")));
#ifndef Q_OS_WASM
	ui->list->addItem(new QListWidgetItem(QIcon(":res/images/connectionOptions.svg"), tr("Connection")));
#endif
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
	QWidget *options = nullptr;
	switch(index)
	{
	case 0:
		// Language
		options = new languageList;
		break;
	case 1:
		// Behavior
		options = new behaviorOptions;
		break;
	case 2:
		// Keyboard
		options = new keyboardOptions;
		break;
	case 3:
		// Appearance
		options = new appearanceOptions;
		break;
	case 4:
		// Connection
		options = new connectionOptions;
		break;
	}
	if(options == nullptr)
		return;
	options->setParent(ui->currentOptions);
	if(currentWidget != nullptr)
	{
		ui->currentOptionsLayout->removeWidget(currentWidget);
		currentWidget->deleteLater();
	}
	currentWidget = options;
	ui->currentOptionsLayout->addWidget(currentWidget);
}

/*! Selects default category. */
void optionsWindow::init(void)
{
	ui->list->setCurrentRow(0);
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
