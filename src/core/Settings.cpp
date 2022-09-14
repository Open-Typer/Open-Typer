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

/*! Returns the value of the given key. */
QVariant Settings::get(QString key, QVariant defaultValue)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->value(key, defaultValue);
}

/*! Returns true if the given key exists. */
bool Settings::contains(QString key)
{
	Q_ASSERT(settingsInstance == nullptr);
	return settingsInstance->contains(key);
}

/*! Sets the key value. */
void Settings::set(QString key, QVariant value)
{
	Q_ASSERT(settingsInstance == nullptr);
	settingsInstance->setValue(key, value);
}

// clientDisabled

/*! Getter for main/clientdisabled. */
bool Settings::clientDisabled(void) { return get("main/clientdisabled", false).toBool(); }

/*! Returns true if there's a main/clientdisabled key. */
bool Settings::containsClientDisabled(void) { return contains("main/clientdisabled"); }

/*! Setter for main/clientdisabled. */
void Settings::setClientDisabled(bool value) { set("main/clientdisabled", value); }

// language

/*! Getter for main/language. */
QString Settings::language(void) { return get("main/language", "").toString(); }

/*! Returns true if there's a main/language key. */
bool Settings::containsLanguage(void) { return contains("main/language"); }

/*! Setter for main/language. */
void Settings::setLanguage(QString value) { set("main/language", value); }

// windowState

/*! Getter for main/windowState. */
QByteArray Settings::windowState(void) { return get("main/windowState", "").toByteArray(); }

/*! Returns true if there's a main/windowState key. */
bool Settings::containsWindowState(void) { return contains("main/windowState"); }

/*! Setter for main/windowState. */
void Settings::setWindowState(QByteArray value) { set("main/windowState", value); }

// windowGeometry

/*! Getter for main/windowGeometry. */
QByteArray Settings::windowGeometry(void) { return get("main/windowGeometry", "").toByteArray(); }

/*! Returns true if there's a main/windowGeometry key. */
bool Settings::containsWindowGeometry(void) { return contains("main/windowGeometry"); }

/*! Setter for main/windowGeometry. */
void Settings::setWindowGeometry(QByteArray value) { set("main/windowGeometry", value); }

// updateChecks

/*! Getter for main/updatechecks. */
bool Settings::updateChecks(void) { return get("main/updatechecks", true).toBool(); }

/*! Returns true if there's a main/updatechecks key. */
bool Settings::containsUpdateChecks(void) { return contains("main/updatechecks"); }

/*! Setter for main/updatechecks. */
void Settings::setUpdateChecks(bool value) { set("main/updatechecks", value); }

// networkEnabled

/*! Getter for main/networkEnabled. */
bool Settings::networkEnabled(void) { return get("main/networkEnabled", false).toBool(); }

/*! Returns true if there's a main/networkEnabled key. */
bool Settings::containsNetworkEnabled(void) { return contains("main/networkEnabled"); }

/*! Setter for main/networkEnabled. */
void Settings::setNetworkEnabled(bool value) { set("main/networkEnabled", value); }

// networkMode

/*! Getter for server/mode. */
int Settings::networkMode(void) { return get("server/mode", 2).toInt(); }

/*! Returns true if there's a server/mode key. */
bool Settings::containsNetworkMode(void) { return contains("server/mode"); }

/*! Setter for server/mode. */
void Settings::setNetworkMode(int value) { set("server/mode", value); }

// lessonPack

/*! Getter for main/configfile. */
QString Settings::lessonPack(void) { return get("main/configfile", "").toString(); }

/*! Returns true if there's a main/configfile key. */
bool Settings::containsLessonPack(void) { return contains("main/configfile"); }

/*! Setter for main/configfile. */
void Settings::setLessonPack(QString value) { set("main/configfile", value); }

// customLessonPack

/*! Getter for main/customconfig. */
bool Settings::customLessonPack(void) { return get("main/customconfig", false).toBool(); }

/*! Returns true if there's a main/customconfig key. */
bool Settings::containsCustomLessonPack(void) { return contains("main/customconfig"); }

/*! Setter for main/customconfig. */
void Settings::setCustomLessonPack(bool value) { set("main/customconfig", value); }

// spaceNewLine

/*! Getter for main/spacenewline. */
bool Settings::spaceNewLine(void) { return get("main/spacenewline", true).toBool(); }

/*! Returns true if there's a main/spacenewline key. */
bool Settings::containsSpaceNewLine(void) { return contains("main/spacenewline"); }

/*! Setter for main/spacenewline. */
void Settings::setSpaceNewLine(bool value) { set("main/spacenewline", value); }

// errorPenalty

/*! Getter for main/errorpenalty. */
int Settings::errorPenalty(void) { return get("main/errorpenalty", 10).toInt(); }

/*! Returns true if there's a main/errorpenalty key. */
bool Settings::containsErrorPenalty(void) { return contains("main/errorpenalty"); }

/*! Setter for main/errorpenalty. */
void Settings::setErrorPenalty(int value) { set("main/errorpenalty", value); }

// serverFullMode

/*! Getter for server/fullmode. */
bool Settings::serverFullMode(void) { return get("server/fullmode", false).toBool(); }

/*! Returns true if there's a server/fullmode key. */
bool Settings::containsServerFullMode(void) { return contains("server/fullmode"); }

/*! Setter for server/fullmode. */
void Settings::setServerFullMode(bool value) { set("server/fullmode", value); }

// mistakeLimit

/*! Getter for main/mistakelimit. */
bool Settings::mistakeLimit(void) { return get("main/mistakelimit", true).toBool(); }

/*! Returns true if there's a main/mistakelimit key. */
bool Settings::containsMistakeLimit(void) { return contains("main/mistakelimit"); }

/*! Setter for main/mistakelimit. */
void Settings::setMistakeLimit(bool value) { set("main/mistakelimit", value); }

// mistakeChars

/*! Getter for main/mistakechars. */
int Settings::mistakeChars(void) { return get("main/mistakechars", 6).toInt(); }

/*! Returns true if there's a main/mistakechars key. */
bool Settings::containsMistakeChars(void) { return contains("main/mistakechars"); }

/*! Setter for main/mistakechars. */
void Settings::setMistakeChars(int value) { set("main/mistakechars", value); }

// serverAddress

/*! Getter for server/address. */
QString Settings::serverAddress(void) { return get("server/address", "127.0.0.1").toString(); }

/*! Returns true if there's a server/address key. */
bool Settings::containsServerAddress(void) { return contains("server/address"); }

/*! Setter for server/address. */
void Settings::setServerAddress(QString value) { set("server/address", value); }

// serverPort

/*! Getter for server/port. */
uint Settings::serverPort(void) { return get("server/port", 57100).toUInt(); }

/*! Returns true if there's a server/port key. */
bool Settings::containsServerPort(void) { return contains("server/port"); }

/*! Setter for server/port. */
void Settings::setServerPort(uint value) { set("server/port", value); }

// themeFont

/*! Getter for theme/font. */
QString Settings::themeFont(void) { return get("theme/font", "").toString(); }

/*! Returns true if there's a theme/font key. */
bool Settings::containsThemeFont(void) { return contains("theme/font"); }

/*! Setter for theme/font. */
void Settings::setThemeFont(QString value) { set("theme/font", value); }

// themeFontSize

/*! Getter for theme/fontsize. */
int Settings::themeFontSize(void) { return get("theme/fontsize", 20).toInt(); }

/*! Returns true if there's a theme/fontsize key. */
bool Settings::containsThemeFontSize(void) { return contains("theme/fontsize"); }

/*! Setter for theme/fontsize. */
void Settings::setThemeFontSize(int value) { set("theme/fontsize", value); }

// themeFontBold

/*! Getter for theme/fontbold. */
bool Settings::themeFontBold(void) { return get("theme/fontbold", true).toBool(); }

/*! Returns true if there's a theme/fontbold key. */
bool Settings::containsThemeFontBold(void) { return contains("theme/fontbold"); }

/*! Setter for theme/fontbold. */
void Settings::setThemeFontBold(bool value) { set("theme/fontbold", value); }
