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

// clientDisabled

/*! Getter for main/clientdisabled. */
bool Settings::clientDisabled(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->value("main/clientdisabled", false).toBool();
}

/*! Returns true if there's a main/clientdisabled key. */
bool Settings::containsClientDisabled(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->contains("main/clientdisabled");
}

/*! Setter for main/clientdisabled. */
void Settings::setClientDisabled(bool value)
{
	Q_ASSERT(settingsInstance == nullptr);
	settingsInstance->setValue("main/clientdisabled", value);
}

// language

/*! Getter for main/language. */
QString Settings::language(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->value("main/language", "").toString();
}

/*! Returns true if there's a main/language key. */
bool Settings::containsLanguage(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->contains("main/language");
}

/*! Setter for main/language. */
void Settings::setLanguage(QString value)
{
	Q_ASSERT(settingsInstance == nullptr);
	settingsInstance->setValue("main/language", value);
}

// windowState

/*! Getter for main/windowState. */
QByteArray Settings::windowState(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->value("main/windowState", "").toByteArray();
}

/*! Returns true if there's a main/windowState key. */
bool Settings::containsWindowState(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->contains("main/windowState");
}

/*! Setter for main/windowState. */
void Settings::setWindowState(QByteArray value)
{
	Q_ASSERT(settingsInstance == nullptr);
	settingsInstance->setValue("main/windowState", value);
}

// windowGeometry

/*! Getter for main/windowGeometry. */
QByteArray Settings::windowGeometry(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->value("main/windowGeometry", "").toByteArray();
}

/*! Returns true if there's a main/windowGeometry key. */
bool Settings::containsWindowGeometry(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->contains("main/windowGeometry");
}

/*! Setter for main/windowGeometry. */
void Settings::setWindowGeometry(QByteArray value)
{
	Q_ASSERT(settingsInstance == nullptr);
	settingsInstance->setValue("main/windowGeometry", value);
}

// updateChecks

/*! Getter for main/updatechecks. */
bool Settings::updateChecks(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->value("main/updatechecks", true).toBool();
}

/*! Returns true if there's a main/updatechecks key. */
bool Settings::containsUpdateChecks(void)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->contains("main/updatechecks");
}

/*! Setter for main/updatechecks. */
void Settings::setUpdateChecks(bool value)
{
	Q_ASSERT(settingsInstance == nullptr);
	settingsInstance->setValue("main/updatechecks", value);
}
