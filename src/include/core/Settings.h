/*
 * Settings.h
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QApplication>
#include "core/FileUtils.h"

/*!
 * \brief The Settings class contains functions for application settings.
 *
 * You should use this class instead of QSettings.\n
 * If there is a new settings key, create getter and setter functions for it.
 * and don't forget to describe it in the list below.\n
 * Use Settings#init() to initialize settings when the application starts.\n
 * \n
 * <b>List of settings keys:</b>
 *  - Settings#clientDisabled() - If there's a connection failure, MonitorClient sets this to true. Use MonitorClient#enableClient() to enable it again.
 *  - Settings#language() - Application language name. Unset to use the system language.
 *  - Settings#windowState() - Main window state (see QMainWindow#saveState()).
 *  - Settings#windowGeometry() - Main window geometry.
 *  - Settings#updateChecks() - Whether automatic update checks are enabled (on supported platforms).
 *  - Settings#networkEnabled() - Whether to enable network features (client/server).
 *  - Settings#networkMode() - Network mode (1 = server, 2 = client).
 */
class Settings
{
	public:
		static void init(void);
		// clientDisabled
		static bool clientDisabled(void);
		static bool containsClientDisabled(void);
		static void setClientDisabled(bool value);
		// language
		static QString language(void);
		static bool containsLanguage(void);
		static void setLanguage(QString value);
		// windowState
		static QByteArray windowState(void);
		static bool containsWindowState(void);
		static void setWindowState(QByteArray value);
		// windowGeometry
		static QByteArray windowGeometry(void);
		static bool containsWindowGeometry(void);
		static void setWindowGeometry(QByteArray value);
		// updateChecks
		static bool updateChecks(void);
		static bool containsUpdateChecks(void);
		static void setUpdateChecks(bool value);
		// networkEnabled
		static bool networkEnabled(void);
		static bool containsNetworkEnabled(void);
		static void setNetworkEnabled(bool value);
		// networkMode
		static int networkMode(void);
		static bool containsNetworkMode(void);
		static void setNetworkMode(int value);

	private:
		static QSettings *settingsInstance;
};

#endif // SETTINGS_H
