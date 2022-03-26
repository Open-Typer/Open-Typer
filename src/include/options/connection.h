/*
 * connection.h
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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>
#include "core/utils.h"
#include "core/net.h"

namespace Ui {
	class connectionOptions;
}

/*!
 * \brief The connectionOptions class is the connection category in the options.
 * \image html connectionOptions.png
 * \see optionsWindow
 * \see monitorClient
 */
class connectionOptions : public QWidget
{
		Q_OBJECT

	public:
		explicit connectionOptions(QWidget *parent = nullptr);
		~connectionOptions();

	private:
		Ui::connectionOptions *ui;
		monitorClient client;
		QSettings settings;

	private slots:
		void changeAddress(void);
		void changeServerState(bool enable);
		void testConnection(void);
};

#endif // CONNECTION_H
