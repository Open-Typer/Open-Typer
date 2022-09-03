/*
 * UserManager.h
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

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QWidget>
#include <QSettings>
#include <QMessageBox>
#include "UserEdit.h"
#include "core/DatabaseManager.h"

namespace Ui {
	class UserManager;
}

/*!
 * \brief the UserManager class is a widget for user management.
 *
 * \image html UserManager.png
 */
class UserManager : public QDialog
{
		Q_OBJECT
	public:
		explicit UserManager(QWidget *parent = nullptr);
		~UserManager();

	private:
		Ui::UserManager *ui;
		void setupList(void);

	private slots:
		void verify(void);
		void addUser(void);
		void removeUser(void);
		void editUser(void);
};

#endif // USERMANAGER_H
