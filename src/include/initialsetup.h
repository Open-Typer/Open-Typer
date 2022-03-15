/*
 * initialsetup.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#ifndef INITIALSETUP_H
#define INITIALSETUP_H

#include <QDialog>
#include <QDirIterator>
#include "options/customization.h"
#include "core/packs.h"
#include "core/utils.h"

namespace Ui {
	class initialSetup;
}

/*!
 * \brief The initialSetup class is a dialog for initial program configuration.
 *
 * \image html initialSetup.png
 */
class initialSetup : public QDialog
{
	Q_OBJECT
	public:
		explicit initialSetup(QWidget *parent = nullptr);
		~initialSetup();

	private:
		Ui::initialSetup *ui;
		QStringList rawItems;
		QSettings settings;

	private slots:
		void finish(void);
		void loadList(void);
};

#endif // INITIALSETUP_H
