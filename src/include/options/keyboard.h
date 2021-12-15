/*
 * keyboard.h
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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QListWidget>
#include <QDirIterator>
#include <QSettings>
#include "options/optionswindow.h"
#include "packEditor/packeditor.h"
#include "core/utils.h"
#include "core/packs.h"

namespace Ui {
	class keyboardOptions;
}

/*!
 * \brief The keyboardOptions class is the keyboard category in the options.
 * \image html keyboardOptions.png
 * \see optionsWindow
 */
class keyboardOptions : public QWidget
{
	Q_OBJECT
	public:
		explicit keyboardOptions(QWidget *parent = nullptr);
		~keyboardOptions();

	private:
		Ui::keyboardOptions *ui;
		QSettings *settings;
		QStringList rawItems;

	private slots:
		void openEditor(void);
		void changePack(int index);
};

#endif // KEYBOARD_H
