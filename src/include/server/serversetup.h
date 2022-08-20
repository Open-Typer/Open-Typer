/*
 * serversetup.h
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

#ifndef SERVERSETUP_H
#define SERVERSETUP_H

#include <QDialog>
#include <QSettings>
#include "core/database.h"
#include "core/utils.h"

namespace Ui {
	class serverSetup;
}

/*!
 * \brief The serverSetup class is a dialog for initial server setup.
 * \image html serverSetup.png
 */
class serverSetup : public QDialog
{
		Q_OBJECT
	public:
		explicit serverSetup(QWidget *parent = nullptr);
		~serverSetup();

	private:
		Ui::serverSetup *ui;

	private slots:
		void verify(void);
		void save(void);
};

#endif // SERVERSETUP_H
