/*
 * UserEdit.cpp
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

#include "server/UserEdit.h"
#include "ui_UserEdit.h"

/*! Constructs UserEdit. */
UserEdit::UserEdit(bool newUser, int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UserEdit)
{
	ui->setupUi(this);
	// Roles
	ui->roleBox->addItems(DatabaseManager::roles(false));
	ui->roleBox->setCurrentIndex(0);
	if(newUser)
	{
		setWindowTitle(tr("New user"));
		ui->resetPasswordButton->hide();
	}
	else
	{
		setWindowTitle(tr("Edit user"));
		ui->passwordEdit->hide();
		ui->repeatPasswordLabel->hide();
		ui->repeatPasswordEdit->hide();
		ui->userNameEdit->setText(dbMgr.userName(id));
		ui->roleBox->setCurrentIndex(dbMgr.userRole(id));
	}
	creatingNewUser = newUser;
	passwordReset = false;
	userID = id;
	verify();
	// Connections
	connect(ui->userNameEdit, &QLineEdit::textChanged, this, &UserEdit::verify);
	connect(ui->passwordEdit, &QLineEdit::textChanged, this, &UserEdit::verify);
	connect(ui->repeatPasswordEdit, &QLineEdit::textChanged, this, &UserEdit::verify);
	connect(ui->resetPasswordButton, SIGNAL(clicked()), this, SLOT(resetPassword()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(finish()));
}

/*! Destroys the UserEdit object. */
UserEdit::~UserEdit()
{
	delete ui;
}

/*!
 * Connected from all line edits.\n
 * Checks if everything is correct and enables the OK button.
 */
void UserEdit::verify(void)
{
	ui->okButton->setEnabled(false);
	// Check user name
	if(ui->userNameEdit->text() == "")
		return;
	QStringList users = dbMgr.teacherNames();
	for(int i = 0; i < users.count(); i++)
	{
		if(users[i] == ui->userNameEdit->text())
		{
			if(dbMgr.teacherIDs().value(i) != userID)
				return;
		}
	}
	if(creatingNewUser || passwordReset)
	{
		// Check password
		if(ui->passwordEdit->text() == "")
			return;
		if(ui->passwordEdit->text() != ui->repeatPasswordEdit->text())
			return;
	}
	// Everything is correct
	ui->okButton->setEnabled(true);
}

/*!
 * Connected from resetPasswordButton->clicked().\n
 * Shows password boxes.
 */
void UserEdit::resetPassword(void)
{
	ui->resetPasswordButton->hide();
	ui->passwordEdit->show();
	ui->repeatPasswordLabel->show();
	ui->repeatPasswordEdit->show();
	passwordReset = true;
	verify();
}

/*!
 * Connected from okButton->clicked().\n
 * Edits or creates the user and closes the window.
 */
void UserEdit::finish(void)
{
	DatabaseManager::Role role = (DatabaseManager::Role) ui->roleBox->currentIndex();
	if(creatingNewUser)
		dbMgr.addUser(ui->userNameEdit->text(), role, ui->passwordEdit->text());
	else
	{
		if(passwordReset)
			dbMgr.editUser(userID, ui->userNameEdit->text(), role, ui->passwordEdit->text());
		else
			dbMgr.editUser(userID, ui->userNameEdit->text(), role);
	}
	accept();
}
