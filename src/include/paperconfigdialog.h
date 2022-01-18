/*
 * paperconfigdialog.h
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

#ifndef PAPERCONFIGDIALOG_H
#define PAPERCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
	class paperConfigDialog;
}

/*!
 * \brief The paperConfigDialog class is a dialog, which asks for exercise configuration.
 *
 * The dialog asks for line length and new line inclusion, but it might be extended with more\n
 * features in the future.
 *
 * \image html paperConfigDialog.png
 *
 * Line length can be accessed using the lineLength attribute.\n
 * \code
 * paperConfigDialog myDialog;
 * myDialog.exec();
 * int length = myDialog.lineLength; // Line length
 * bool includeNewLines = myDialog.includeNewLines; // New line inclusion
 * \endcode
 */
class paperConfigDialog : public QDialog
{
Q_OBJECT
	public:
		explicit paperConfigDialog(QWidget *parent = nullptr);
		~paperConfigDialog();
		int lineLength;
		bool includeNewLines;
	
	private:
		Ui::paperConfigDialog *ui;
	
	private slots:
		void reject(void);
		void updateLineLength(int value);
		void setIncludeNewLines(bool value);
};

#endif // PAPERCONFIGDIALOG_H
