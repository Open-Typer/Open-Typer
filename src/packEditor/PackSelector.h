/*
 * PackSelector.h
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

#ifndef PACKSELECTOR_H
#define PACKSELECTOR_H

#include <QDialog>
#include <QDirIterator>
#include "StringUtils.h"
#include "BuiltInPacks.h"

namespace Ui {
	class PackSelector;
}

/*!
 * \brief The PackSelector class is a QDialog, which allows the user to select a built-in pack.
 *
 * \image html PackSelector.png
 */
class PackSelector : public QDialog
{
		Q_OBJECT
	public:
		explicit PackSelector(QWidget *parent = nullptr);
		~PackSelector();
		QString selectedConfig(void);
		QStringList rawItems;

	private:
		Ui::PackSelector *ui;
};

#endif // PACKSELECTOR_H
