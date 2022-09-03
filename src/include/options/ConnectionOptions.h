/*
 * ConnectionOptions.h
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
#ifndef Q_OS_WASM
#include "core/server.h"
#endif // Q_OS_WASM

namespace Ui {
	class ConnectionOptions;
}

/*!
 * \brief The ConnectionOptions class is the connection category in the options.
 * \image html ConnectionOptions.png
 * \see optionsWindow
 * \see monitorClient
 */
class ConnectionOptions : public QWidget
{
		Q_OBJECT

	public:
		explicit ConnectionOptions(QWidget *parent = nullptr);
		~ConnectionOptions();

	private:
		Ui::ConnectionOptions *ui;
		monitorClient client;
		QSettings settings;

	private slots:
		void refresh(void);
		void toggleNetworkOptions(bool checked);
		void changeMode(void);
		void setFullMode(bool enable);
		void changeAddress(void);
		void testConnection(bool silent = false);
};

#endif // CONNECTION_H
