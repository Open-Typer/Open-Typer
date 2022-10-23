/*
 * DeviceEdit.h
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

#ifndef DEVICEEDIT_H
#define DEVICEEDIT_H

#include <QDialog>
#include <QPushButton>
#include "DatabaseManager.h"

namespace Ui {
	class DeviceEdit;
}

/*!
 * \brief The DeviceEdit class is a dialog used to add or edit devices.
 *
 * \image html DeviceEdit.png
 */
class DeviceEdit : public QDialog
{
		Q_OBJECT
	public:
		explicit DeviceEdit(int deviceID = 0, QWidget *parent = nullptr);
		~DeviceEdit();

	private:
		Ui::DeviceEdit *ui;
		bool newDevice;
		int m_deviceID;
		QHostAddress oldAddress;

	private slots:
		void verify(void);
		void finish(void);
};

#endif // DEVICEEDIT_H
