/*
 * ServerSetup.cpp
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

#include "server/ServerSetup.h"
#include "ui_ServerSetup.h"

/*! Constructs ServerSetup. */
ServerSetup::ServerSetup(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ServerSetup)
{
	ui->setupUi(this);
	ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);
	verify();
	// Connections
	connect(ui->adminNameEdit, &QLineEdit::textChanged, this, &ServerSetup::verify);
	connect(ui->adminPasswordEdit, &QLineEdit::textChanged, this, &ServerSetup::verify);
	connect(ui->repeatPasswordEdit, &QLineEdit::textChanged, this, &ServerSetup::verify);
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(save()));
}

/*! Destroys the ServerSetup object. */
ServerSetup::~ServerSetup()
{
	delete ui;
}

/*! Connected from all line edits' %textChanged() signal.\n
 * Checks if everything is correct and enables the OK button.
 */
void ServerSetup::verify(void)
{
	ui->okButton->setEnabled(false);
	// Check administrator name
	if(ui->adminNameEdit->text() == "")
		return;
	// Check administrator password
	if(ui->adminPasswordEdit->text() == "")
		return;
	if(ui->adminPasswordEdit->text() != ui->repeatPasswordEdit->text())
		return;
	// Everything is correct
	ui->okButton->setEnabled(true);
}

/*!
 * Connected from okButton->clicked().\n
 * Saves settings and closes the dialog.
 */
void ServerSetup::save(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	dbMgr.addUser(ui->adminNameEdit->text(), databaseManager::Role_Administrator, ui->adminPasswordEdit->text());
	accept();
}
