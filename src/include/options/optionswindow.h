/*
 * optionswindow.h
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

#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QDialog>
#include "options/behavior.h"
#include "options/keyboard.h"
#include "options/customization.h"
#include "options/connection.h"
#include "widgets/languagelist.h"

namespace Ui {
	class optionsWindow;
}

/*!
 * \brief The options window class shows program options.
 *
 * The list of categories is can be changed in the setupList() function.\n
 * New categories can be added in the changeOptionWidget() function. Make sure to follow the other categories' code structure.
 *
 * \image html optionsWindow.png
 */
class optionsWindow : public QDialog
{
	Q_OBJECT
	public:
		explicit optionsWindow(QWidget *parent = nullptr);
		~optionsWindow();
		void init(void);

	private:
		Ui::optionsWindow *ui;
		QWidget *currentWidget = nullptr;
		void setupList(void);

	protected:
		void changeEvent(QEvent *event);

	signals:
		/*!
		 * A signal, which is emitted on language change.
		 * \param[out] index The index in languageList.
		 * \see languageList
		 */
		void languageChanged(int index);

	private slots:
		void changeOptionWidget(int index);
		void changeLanguage(int);
};

#endif // OPTIONSWINDOW_H
