/*
 * AdminSelector.cpp
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

#include "server/AdminSelector.h"
#include "ui_AdminSelector.h"

/*! Constructs AdminSelector. */
AdminSelector::AdminSelector(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AdminSelector)
{
	ui->setupUi(this);
	// Load administrators
	adminIDs = dbMgr.administratorIDs();
	ui->adminList->addItems(dbMgr.administratorNames());
	ui->adminList->setCurrentRow(0);
	// Connections
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(finish()));
	connect(ui->adminList, &QListWidget::itemDoubleClicked, this, &AdminSelector::finish);
}

/*! Destroys the AdminSelector object. */
AdminSelector::~AdminSelector()
{
	delete ui;
}

/*!
 * Connected from okButton->clicked().\n
 * Sets userID attribute and closes the dialog.
 */
void AdminSelector::finish(void)
{
	userID = adminIDs.value(ui->adminList->currentRow());
	accept();
}
