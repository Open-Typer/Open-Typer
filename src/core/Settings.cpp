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

// themeFontItalic

/*! Getter for theme/fontitalic. */
bool Settings::themeFontItalic(void) { return get("theme/fontitalic", false).toBool(); }

/*! Returns true if there's a theme/fontitalic key. */
bool Settings::containsThemeFontItalic(void) { return contains("theme/fontitalic"); }

/*! Setter for theme/fontitalic. */
void Settings::setThemeFontItalic(bool value) { set("theme/fontitalic", value); }

// themeFontUnderline

/*! Getter for theme/fontunderline. */
bool Settings::themeFontUnderline(void) { return get("theme/fontunderline", false).toBool(); }

/*! Returns true if there's a theme/fontunderline key. */
bool Settings::containsThemeFontUnderline(void) { return contains("theme/fontunderline"); }

/*! Setter for theme/fontunderline. */
void Settings::setThemeFontUnderline(bool value) { set("theme/fontunderline", value); }

// customExerciseTextColor

/*! Getter for theme/customleveltextcolor. */
bool Settings::customExerciseTextColor(void) { return get("theme/customleveltextcolor", false).toBool(); }

/*! Returns true if there's a theme/customleveltextcolor key. */
bool Settings::containsCustomExerciseTextColor(void) { return contains("theme/customleveltextcolor"); }

/*! Setter for theme/customleveltextcolor. */
void Settings::setCustomExerciseTextColor(bool value) { set("theme/customleveltextcolor", value); }

// exerciseTextColor

/*! Getter for exercise text color (theme/leveltextred, theme/leveltextgreen, theme/leveltextblue). */
QRgb Settings::exerciseTextColor(void)
{
	return QColor(get("theme/leveltextred", 0).toInt(),
		get("theme/leveltextgreen", 0).toInt(),
		get("theme/leveltextblue", 0).toInt())
		.rgb();
}

/*! Returns true if there's are all exercise text color keys. */
bool Settings::containsExerciseTextColor(void)
{
	return (contains("theme/leveltextred") && contains("theme/leveltextgreen") && contains("theme/leveltextblue"));
}

/*! Setter for exercise text color. */
void Settings::setExerciseTextColor(QRgb value)
{
	QColor color(value);
	set("theme/leveltextred", color.red());
	set("theme/leveltextgreen", color.green());
	set("theme/leveltextblue", color.blue());
}

// inputTextColor

/*! Getter for input text color (theme/inputtextred, theme/inputtextgreen, theme/inputtextblue). */
QRgb Settings::inputTextColor(void)
{
	return QColor(get("theme/inputtextred", 0).toInt(),
		get("theme/inputtextgreen", 0).toInt(),
		get("theme/inputtextblue", 0).toInt())
		.rgb();
}

/*! Returns true if there's are all input text color keys. */
bool Settings::containsInputTextColor(void)
{
	return (contains("theme/inputtextred") && contains("theme/inputtextgreen") && contains("theme/inputtextblue"));
}

/*! Setter for input text color. */
void Settings::setInputTextColor(QRgb value)
{
	QColor color(value);
	set("theme/inputtextred", color.red());
	set("theme/inputtextgreen", color.green());
	set("theme/inputtextblue", color.blue());
}

// customInputTextColor

/*! Getter for theme/custominputtextcolor. */
bool Settings::customInputTextColor(void) { return get("theme/custominputtextcolor", false).toBool(); }

/*! Returns true if there's a theme/custominputtextcolor key. */
bool Settings::containsCustomInputTextColor(void) { return contains("theme/custominputtextcolor"); }

/*! Setter for theme/custominputtextcolor. */
void Settings::setCustomInputTextColor(bool value) { set("theme/custominputtextcolor", value); }

// customBgColor

/*! Getter for theme/custombgcolor. */
bool Settings::customBgColor(void) { return get("theme/custombgcolor", false).toBool(); }

/*! Returns true if there's a theme/custombgcolor key. */
bool Settings::containsCustomBgColor(void) { return contains("theme/custombgcolor"); }

/*! Setter for theme/custombgcolor. */
void Settings::setCustomBgColor(bool value) { set("theme/custombgcolor", value); }

// bgColor

/*! Getter for background color (theme/bgred, theme/bggreen, theme/bgblue). */
QRgb Settings::bgColor(void)
{
	return QColor(get("theme/bgred", 0).toInt(),
		get("theme/bggreen", 0).toInt(),
		get("theme/bgblue", 0).toInt())
		.rgb();
}

/*! Returns true if there's are all background color keys. */
bool Settings::containsBgColor(void)
{
	return (contains("theme/bgred") && contains("theme/bggreen") && contains("theme/bgblue"));
}

/*! Setter for background color. */
void Settings::setBgColor(QRgb value)
{
	QColor color(value);
	set("theme/bgred", color.red());
	set("theme/bggreen", color.green());
	set("theme/bgblue", color.blue());
}

// customPaperColor

/*! Getter for theme/custompapercolor. */
bool Settings::customPaperColor(void) { return get("theme/custompapercolor", false).toBool(); }

/*! Returns true if there's a theme/custompapercolor key. */
bool Settings::containsCustomPaperColor(void) { return contains("theme/custompapercolor"); }

/*! Setter for theme/custompapercolor. */
void Settings::setCustomPaperColor(bool value) { set("theme/custompapercolor", value); }

// paperColor

/*! Getter for paper color (theme/paperred, theme/papergreen, theme/paperblue). */
QRgb Settings::paperColor(void)
{
	return QColor(get("theme/paperred", 0).toInt(),
		get("theme/papergreen", 0).toInt(),
		get("theme/paperblue", 0).toInt())
		.rgb();
}

/*! Returns true if there's are all paper color keys. */
bool Settings::containsPaperColor(void)
{
	return (contains("theme/paperred") && contains("theme/papergreen") && contains("theme/paperblue"));
}

/*! Setter for paper color. */
void Settings::setPaperColor(QRgb value)
{
	QColor color(value);
	set("theme/paperred", color.red());
	set("theme/papergreen", color.green());
	set("theme/paperblue", color.blue());
}

// customPanelColor

/*! Getter for theme/custompanelcolor. */
bool Settings::customPanelColor(void) { return get("theme/custompanelcolor", false).toBool(); }

/*! Returns true if there's a theme/custompanelcolor key. */
bool Settings::containsCustomPanelColor(void) { return contains("theme/custompanelcolor"); }

/*! Setter for theme/custompanelcolor. */
void Settings::setCustomPanelColor(bool value) { set("theme/custompanelcolor", value); }

// panelColor

/*! Getter for panel color (theme/panelred, theme/panelgreen, theme/panelblue). */
QRgb Settings::panelColor(void)
{
	return QColor(get("theme/panelred", 0).toInt(),
		get("theme/panelgreen", 0).toInt(),
		get("theme/panelblue", 0).toInt())
		.rgb();
}

/*! Returns true if there are all panel color keys. */
bool Settings::containsPanelColor(void)
{
	return (contains("theme/panelred") && contains("theme/panelgreen") && contains("theme/panelblue"));
}

/*! Setter for panel color. */
void Settings::setPanelColor(QRgb value)
{
	QColor color(value);
	set("theme/panelred", color.red());
	set("theme/panelgreen", color.green());
	set("theme/panelblue", color.blue());
}

// applicationStyle

/*! Getter for theme/theme. */
ThemeEngine::Style Settings::applicationStyle(void) { return (ThemeEngine::Style) get("theme/theme", 0).toInt(); }

/*! Returns true if there's a theme/theme key. */
bool Settings::containsApplicationStyle(void) { return contains("theme/theme"); }

/*! Setter for theme/theme. */
void Settings::setApplicationStyle(ThemeEngine::Style value) { set("theme/theme", (int) value); }

// applicationTheme

/*! Getter for theme/fulltheme. */
QString Settings::applicationTheme(void) { return get("theme/fulltheme", "default").toString(); }

/*! Returns true if there's a theme/fulltheme key. */
bool Settings::containsApplicationTheme(void) { return contains("theme/fulltheme"); }

/*! Setter for theme/fulltheme. */
void Settings::setApplicationTheme(QString value) { set("theme/fulltheme", value); }
