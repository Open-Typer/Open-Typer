/*
 * Settings.cpp
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

#include "core/Settings.h"

QSettings *Settings::settingsInstance = nullptr;

/*! Initializes settings. Run Settings#init() after the application starts. */
void Settings::init(void)
{
	settingsInstance = new QSettings(FileUtils::mainSettingsLocation(), QSettings::IniFormat, qApp);
}

/*! Getter for main/clientdisabled. */
bool Settings::clientDisabled(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->value("main/clientdisabled", false).toBool();
}

/*! Setter for main/clientdisabled. */
void Settings::setClientDisabled(bool value)
{
	Q_ASSERT(settingsInstance == nullptr);
	settingsInstance->setValue("main/clientdisabled", value);
}

/*! Getter for main/language. */
QString Settings::language(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->value("main/language", "").toString();
}

/*! Setter for main/language. */
void Settings::setLanguage(QString value)
{
	Q_ASSERT(settingsInstance == nullptr);
	settingsInstance->setValue("main/language", value);
}

/*! Getter for main/windowState. */
QByteArray Settings::windowState(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->value("main/windowState", "").toByteArray();
}

/*! Setter for main/windowState. */
void Settings::setWindowState(QByteArray value)
{
	Q_ASSERT(settingsInstance == nullptr);
	settingsInstance->setValue("main/windowState", value);
}
