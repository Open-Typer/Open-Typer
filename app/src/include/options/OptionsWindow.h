/*
 * OptionsWindow.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
 * Copyright (C) 2022 - yeti0904
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
#include <QPushButton>
#include "options/BehaviorOptions.h"
#include "options/KeyboardOptions.h"
#include "options/AppearanceOptions.h"
#include "options/ConnectionOptions.h"
#include "widgets/LanguageList.h"

namespace Ui {
	class OptionsWindow;
}

/*!
 * \brief The options window class shows program options.
 *
 * The list of categories is can be changed in the setupList() function.\n
 * New categories can be added in the changeOptionWidget() function. Make sure to follow the other categories' code structure.
 *
 * \image html OptionsWindow.png
 */
class OptionsWindow : public QDialog
{
		Q_OBJECT
	public:
		explicit OptionsWindow(QWidget *parent = nullptr);
		~OptionsWindow();
		void init(void);

	private:
		Ui::OptionsWindow *ui;
		QWidget *currentWidget = nullptr;
		void setupList(void);

	protected:
		void changeEvent(QEvent *event);

	private slots:
		void changeOptionWidget(int index);
		void unlockSettings(void);
};

#endif // OPTIONSWINDOW_H
