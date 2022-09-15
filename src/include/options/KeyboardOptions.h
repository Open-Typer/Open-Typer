/*
 * KeyboardOptions.h
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

#ifndef KEYBOARDOPTIONS_H
#define KEYBOARDOPTIONS_H

#include <QWidget>
#include <QListWidget>
#include <QDirIterator>
#include "options/OptionsWindow.h"
#include "packEditor/PackEditor.h"
#include "core/StringUtils.h"
#include "core/BuiltInPacks.h"

namespace Ui {
	class KeyboardOptions;
}

/*!
 * \brief The KeyboardOptions class is the keyboard category in the options.
 * \image html KeyboardOptions.png
 * \see OptionsWindow
 */
class KeyboardOptions : public QWidget
{
		Q_OBJECT
	public:
		explicit KeyboardOptions(QWidget *parent = nullptr);
		~KeyboardOptions();

	private:
		Ui::KeyboardOptions *ui;
		QStringList rawItems;

	private slots:
		void openEditor(void);
		void changePack(int index);
};

#endif // KEYBOARDOPTIONS_H
