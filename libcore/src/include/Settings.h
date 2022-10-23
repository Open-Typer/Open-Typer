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

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QSettings>
#include <QApplication>
#include <QRgb>
#include <QColor>
#ifdef Q_OS_WASM
#include <qwasmsettings.h>
#endif // Q_OS_WASM
#include "FileUtils.h"
#include "ThemeEngine.h"

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
 *  - Settings#lessonPack() - Selected lesson pack (or keyboard layout in the settings).
 *  - Settings#customLessonPack() - Whether to load lesson pack from a file (if true, the file name is in Settings#lessonPack()).
 *  - Settings#spaceNewLine() - Whether to allow space bar to be used to break lines.
 *  - Settings#errorPenalty() - Number of hits subtracted from net hits on every mistake.
 *  - Settings#serverFullMode() - Whether to enable server full mode (teacher and student accounts, database, etc.).
 *  - Settings#mistakeLimit() - Whether to limit number of mistakes in per characters in words.
 *  - Settings#mistakeChars() - Number of characters in one word with max. 1 mistake.
 *  - Settings#serverAddress() - The IPv4 address of the server (used by client).
 *  - Settings#serverPort() - Server port (used by client or server).
 *  - Settings#themeFont() - Text font family name.
 *  - Settings#themeFontSize() - Text font size.
 *  - Settings#themeFontBold() - Whether to use a bold font in exercise and input text.
 *  - Settings#themeFontItalic() - Whether to use an italic font in exercise and input text.
 *  - Settings#themeFontUnderline() - Whether to use an underlined font in exercise and input text.
 *  - Settings#customExerciseTextColor() - If false, the exercise text color is based on the theme.
 *  - Settings#exerciseTextColor() - Exercise text color.
 *  - Settings#inputTextColor() - Input text color.
 *  - Settings#customInputTextColor() - If false, the default input text color is used.
 *  - Settings#customBgColor() - If false, the background color is based on the theme.
 *  - Settings#bgColor() - Background color.
 *  - Settings#customPaperColor() - If false, the paper color is based on the theme.
 *  - Settings#paperColor() - Paper color.
 *  - Settings#customPanelColor() - If false, the panel color is based on the theme.
 *  - Settings#panelColor() - Top and bottom panel color.
 *  - Settings#applicationStyle() - The application style (see ThemeEngine#Style).
 *  - Settings#applicationTheme() - The application theme (light, dark, light blue, etc.).
 *  - Settings#settingsLockEnabled() - If true, settings will ask for a password before they can be accessed.
 *  - Settings#settingsLockPasswd() - Hashed password (SHA256) for settings lock.
 *  - Settings#advancedTheme() - Whether to allow the user to select advanced themes (dark, light, light blue, etc.).
 *  - Settings#simpleThemeId() - Simple theme (0 = light, 1 = dark).
 *  - Settings#editorGeometry() - Pack editor window geometry.
 *  - Settings#keyboardVisible() - Whether to show the virtual keyboard.
 */
class CORE_LIB_EXPORT Settings
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
		// lessonPack
		static QString lessonPack(void);
		static bool containsLessonPack(void);
		static void setLessonPack(QString value);
		// customLessonPack
		static bool customLessonPack(void);
		static bool containsCustomLessonPack(void);
		static void setCustomLessonPack(bool value);
		// spaceNewLine
		static bool spaceNewLine(void);
		static bool containsSpaceNewLine(void);
		static void setSpaceNewLine(bool value);
		// errorPenalty
		static int errorPenalty(void);
		static bool containsErrorPenalty(void);
		static void setErrorPenalty(int value);
		// serverFullMode
		static bool serverFullMode(void);
		static bool containsServerFullMode(void);
		static void setServerFullMode(bool value);
		// mistakeLimit
		static bool mistakeLimit(void);
		static bool containsMistakeLimit(void);
		static void setMistakeLimit(bool value);
		// mistakeChars
		static int mistakeChars(void);
		static bool containsMistakeChars(void);
		static void setMistakeChars(int value);
		// serverAddress
		static QString serverAddress(void);
		static bool containsServerAddress(void);
		static void setServerAddress(QString value);
		// serverPort
		static uint serverPort(void);
		static bool containsServerPort(void);
		static void setServerPort(uint value);
		// themeFont
		static QString themeFont(void);
		static bool containsThemeFont(void);
		static void setThemeFont(QString value);
		// themeFontSize
		static int themeFontSize(void);
		static bool containsThemeFontSize(void);
		static void setThemeFontSize(int value);
		// themeFontBold
		static bool themeFontBold(void);
		static bool containsThemeFontBold(void);
		static void setThemeFontBold(bool value);
		// themeFontItalic
		static bool themeFontItalic(void);
		static bool containsThemeFontItalic(void);
		static void setThemeFontItalic(bool value);
		// themeFontUnderline
		static bool themeFontUnderline(void);
		static bool containsThemeFontUnderline(void);
		static void setThemeFontUnderline(bool value);
		// customExerciseTextColor
		static bool customExerciseTextColor(void);
		static bool containsCustomExerciseTextColor(void);
		static void setCustomExerciseTextColor(bool value);
		// exerciseTextColor
		static QRgb exerciseTextColor(void);
		static bool containsExerciseTextColor(void);
		static void setExerciseTextColor(QRgb value);
		// inputTextColor
		static QRgb inputTextColor(void);
		static bool containsInputTextColor(void);
		static void setInputTextColor(QRgb value);
		// customInputTextColor
		static bool customInputTextColor(void);
		static bool containsCustomInputTextColor(void);
		static void setCustomInputTextColor(bool value);
		// customBgColor
		static bool customBgColor(void);
		static bool containsCustomBgColor(void);
		static void setCustomBgColor(bool value);
		// bgColor
		static QRgb bgColor(void);
		static bool containsBgColor(void);
		static void setBgColor(QRgb value);
		// customPaperColor
		static bool customPaperColor(void);
		static bool containsCustomPaperColor(void);
		static void setCustomPaperColor(bool value);
		// paperColor
		static QRgb paperColor(void);
		static bool containsPaperColor(void);
		static void setPaperColor(QRgb value);
		// customPanelColor
		static bool customPanelColor(void);
		static bool containsCustomPanelColor(void);
		static void setCustomPanelColor(bool value);
		// panelColor
		static QRgb panelColor(void);
		static bool containsPanelColor(void);
		static void setPanelColor(QRgb value);
		// applicationStyle
		static ThemeEngine::Style applicationStyle(void);
		static bool containsApplicationStyle(void);
		static void setApplicationStyle(ThemeEngine::Style value);
		// applicationTheme
		static QString applicationTheme(void);
		static bool containsApplicationTheme(void);
		static void setApplicationTheme(QString value);
		// settingsLockEnabled
		static bool settingsLockEnabled(void);
		static bool containsSettingsLockEnabled(void);
		static void setSettingsLockEnabled(bool value);
		// settingsLockPasswd
		static QByteArray settingsLockPasswd(void);
		static bool containsSettingsLockPasswd(void);
		static void setSettingsLockPasswd(QByteArray value);
		// advancedTheme
		static bool advancedTheme(void);
		static bool containsAdvancedTheme(void);
		static void setAdvancedTheme(bool value);
		// simpleThemeId
		static int simpleThemeId(void);
		static bool containsSimpleThemeId(void);
		static void setSimpleThemeId(int value);
		// editorGeometry
		static QByteArray editorGeometry(void);
		static bool containsEditorGeometry(void);
		static void setEditorGeometry(QByteArray value);
		// keyboardVisible
		static bool keyboardVisible(void);
		static bool containsKeyboardVisible(void);
		static void setKeyboardVisible(bool value);

	private:
		static QSettings *settingsInstance;
#ifdef Q_OS_WASM
		static bool tempSettingsCopied;
		static void copyTempSettings(void);
#endif // Q_OS_WASM
		static QVariant get(QString key, QVariant defaultValue);
		static bool contains(QString key);
		static void set(QString key, QVariant value);
};

#endif // SETTINGS_H
