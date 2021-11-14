/*
 * optionswindow.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QDialog>
#include <QTranslator>
#include "options/keyboard.h"
#include "options/customization.h"
#include "widgets/languagelist.h"

namespace Ui {
	class optionsWindow;
}

class optionsWindow : public QDialog
{
	Q_OBJECT
	public:
		explicit optionsWindow(QWidget *parent = nullptr);
		~optionsWindow();
		void init(void);

	private:
		Ui::optionsWindow *ui;

	signals:
		void languageChanged(int index);

	private slots:
		void changeOptionWidget(int index);
		void changeLanguage(int);
};

#endif // OPTIONSWINDOW_H
