/*
 * StudentEdit.h
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

#ifndef STUDENTEDIT_H
#define STUDENTEDIT_H

#include <QDialog>
#include "DatabaseManager.h"

namespace Ui {
	class StudentEdit;
}

/*!
 * \brief The StudentEdit class is a dialog used to add or edit students.
 *
 * \image html StudentEdit.png
 */
class StudentEdit : public QDialog
{
		Q_OBJECT
	public:
		explicit StudentEdit(bool newStudent = false, int class_id = 0, int id = 1, QWidget *parent = nullptr);
		~StudentEdit();

	private:
		Ui::StudentEdit *ui;
		bool creatingNewStudent, passwordReset;
		int classID, userID;
		QList<int> classes, students;

	private slots:
		void verify(void);
		void resetPassword(void);
		void finish(void);
		void switchMode(void);
		void openClass(void);
};

#endif // STUDENTEDIT_H
