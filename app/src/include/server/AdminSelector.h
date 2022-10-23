/*
 * AdminSelector.h
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

#ifndef ADMINSELECTOR_H
#define ADMINSELECTOR_H

#include <QDialog>
#include "DatabaseManager.h"

namespace Ui {
	class AdminSelector;
}

/*!
 * \brief The AdminSelector class is a dialog used to select an administrator account.
 * \image html AdminSelector.png
 */
class AdminSelector : public QDialog
{
		Q_OBJECT
	public:
		explicit AdminSelector(QWidget *parent = nullptr);
		~AdminSelector();
		int userID;

	private:
		Ui::AdminSelector *ui;
		QList<int> adminIDs;

	private slots:
		void finish(void);
};

#endif // ADMINSELECTOR_H
