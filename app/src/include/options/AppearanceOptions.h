/*
 * AppearanceOptions.h
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

#ifndef APPEARANCEOPTIONS_H
#define APPEARANCEOPTIONS_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QPropertyAnimation>
#include <QColorDialog>
#include "options/OptionsWindow.h"
#include "StringUtils.h"
#include "ThemeEngine.h"

namespace Ui {
	class AppearanceOptions;
}

/*!
 * \brief The AppearanceOptions class is the customization category in the options.
 * \image html AppearanceOptions.png
 * \n
 * \image html AppearanceOptions2.png
 * \see OptionsWindow
 */
class AppearanceOptions : public QWidget
{
		Q_OBJECT
	public:
		Q_INVOKABLE AppearanceOptions(QWidget *parent = nullptr);
		~AppearanceOptions();
		void init(void);
		void hideModeSelector(void);
		void setSimpleTheme(int theme);

	private:
		Ui::AppearanceOptions *ui;
		QListWidgetItem *lastItem = nullptr;
		void updateSimpleTheme(void);
		void selectCurrentFullTheme(void);
		void updateFont(void);
		void setColors();

	private slots:
		void changeThemeMode(bool advanced);
		void changeFullTheme(QListWidgetItem *item);
		void goBack(void);
		void changeFont(QFont font);
		void changeFontSize(int size);
		void setBoldText(void);
		void setItalicText(void);
		void setUnderlineText(void);
		void changeLevelTextColor(void);
		void changeInputTextColor(void);
		void resetTextColors(void);
		void changeBgColor(void);
		void changePaperColor(void);
		void changePanelColor(void);
		void resetBgPaperColors(void);
		void changeTheme(int index);
};

#endif // APPEARANCEOPTIONS_H
