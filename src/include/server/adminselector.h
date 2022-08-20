/*
 * adminselector.h
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
#include "core/database.h"

namespace Ui {
	class adminSelector;
}

/*!
 * \brief The adminSelector class is a dialog used to select an administrator account.
 * \image html adminSelector.png
 */
class adminSelector : public QDialog
{
		Q_OBJECT
	public:
		explicit adminSelector(QWidget *parent = nullptr);
		~adminSelector();
		int userID;

	private:
		Ui::adminSelector *ui;
		QList<int> adminIDs;

	private slots:
		void finish(void);
};

#endif // ADMINSELECTOR_H
