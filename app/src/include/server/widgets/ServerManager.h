/*
 * ServerManager.h
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

#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QWidget>
#include "server/UserManager.h"
#include "server/AdminSelector.h"
#include "server/ServerSetup.h"
#include "server/ClassEdit.h"
#include "server/DeviceEdit.h"
#include "server/widgets/ClassControls.h"
#include "server/widgets/StudentDetails.h"
#include "Settings.h"

namespace Ui {
	class ServerManager;
}

/*!
 * \brief The ServerManager class is a widget that can be used as a server control panel.
 *
 * \image html ServerManager.png
 */
class ServerManager : public QWidget
{
		Q_OBJECT
	public:
		explicit ServerManager(QWidget *parent = nullptr);
		~ServerManager();

	private:
		Ui::ServerManager *ui;
		bool expanded = true;
		QList<int> classes;
		bool disableClassOpening = false;
		int lastClassBoxIndex = -1;
		bool fullMode;

	protected:
		void changeEvent(QEvent *event);

	signals:
		/*! A signal, which is emitted when the widget is collapsed. */
		void widgetCollapsed();
		/*! A signal, which is emitted when the widget is expanded. */
		void widgetExpanded();

	public slots:
		bool init(void);
		void collapse(void);
		void expand(void);

	private slots:
		void loadDevices(void);
		void openUserManager(void);
		void addClass(void);
		void openClass(bool auth = true);
		void openDetails(int studentID);
		void removeClass(void);
		void editClass(void);
		void showCloseExercisesMessage(void);
		void addDevice(void);
		void editDevice(void);
		void removeDevice(void);
};

#endif // SERVERMANAGER_H
