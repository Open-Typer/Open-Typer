/*
 * ClassEdit.h
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

#ifndef CLASSEDIT_H
#define CLASSEDIT_H

#include <QDialog>
#include <QMessageBox>
#include <QDirIterator>
#include "core/database.h"

namespace Ui {
	class ClassEdit;
}

/*!
 * \brief The ClassEdit class is a dialog for class adding and editing.
 *
 * \image html ClassEdit.png
 */
class ClassEdit : public QDialog
{
		Q_OBJECT
	public:
		explicit ClassEdit(bool newClass = false, int id = 1, QWidget *parent = nullptr);
		~ClassEdit();

	private:
		Ui::ClassEdit *ui;
		bool creatingNewClass;
		int classID;

	private slots:
		void verify(void);
		void updateOwner(const QString name);
		void finish(void);
};

#endif // CLASSEDIT_H
