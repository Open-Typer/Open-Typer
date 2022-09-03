/*
 * UserManager.cpp
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

#include "server/UserManager.h"
#include "ui_UserManager.h"

/*! Constructs UserManager. */
UserManager::UserManager(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UserManager)
{
	ui->setupUi(this);
	setupList();
	// Connections
	connect(ui->userList, &QListWidget::itemSelectionChanged, this, &UserManager::verify);
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addUser()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeUser()));
	connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editUser()));
}

/*! Destroys the UserManager object. */
UserManager::~UserManager()
{
	delete ui;
}

/*! Loads the users. */
void UserManager::setupList(void)
{
	ui->userList->clear();
	QList<int> users = dbMgr.teacherIDs();
	for(int i = 0; i < users.count(); i++)
	{
		QListWidgetItem *item = new QListWidgetItem(dbMgr.userName(users[i]));
		ui->userList->addItem(item);
	}
	verify();
}

/*!
 * Connected from userList->itemSelectionChanged().\n
 * Checks if there's a selected user and enables the open and remove buttons.
 */
void UserManager::verify(void)
{
	// Check list of users
	bool enable = (ui->userList->currentRow() != -1);
	ui->editButton->setEnabled(enable);
	if((dbMgr.teacherIDs().value(ui->userList->currentRow()) != dbMgr.loginID()) && enable)
		ui->removeButton->setEnabled(true);
	else
		ui->removeButton->setEnabled(false);
}

/*!
 * Connected from addButton->clicked().\n
 * Opens UserEdit and creates a new user.
 *
 * \see UserEdit
 */
void UserManager::addUser(void)
{
	UserEdit dialog(true);
	dialog.exec();
	setupList();
}

/*!
 * Connected from removeButton->clicked().\n
 * Removes selected user.
 */
void UserManager::removeUser(void)
{
	if(ui->userList->currentRow() == -1)
		return;
	int userID = dbMgr.teacherIDs().value(ui->userList->currentRow());
	QMessageBox confirmDialog;
	confirmDialog.setWindowTitle(tr("Confirm"));
	confirmDialog.setText(tr("Are you sure you want to remove user %1?").arg(dbMgr.userName(userID)));
	confirmDialog.setInformativeText(tr("This will remove all classes the user owns!"));
	QPushButton *yesButton = confirmDialog.addButton(tr("Yes"), QMessageBox::YesRole);
	QPushButton *noButton = confirmDialog.addButton(tr("No"), QMessageBox::NoRole);
	confirmDialog.setIcon(QMessageBox::Question);
	confirmDialog.exec();
	if(confirmDialog.clickedButton() == yesButton)
	{
		if(dbMgr.auth(dbMgr.loginID()))
			dbMgr.removeUser(userID);
	}
	else if(confirmDialog.clickedButton() == noButton)
		return;
	setupList();
}

/*!
 * Connected from editButton->clicked().\n
 * Opens UserEdit and edits selected user.
 *
 * \see UserEdit
 */
void UserManager::editUser(void)
{
	UserEdit dialog(false, dbMgr.teacherIDs().value(ui->userList->currentRow()));
	dialog.exec();
	setupList();
}
