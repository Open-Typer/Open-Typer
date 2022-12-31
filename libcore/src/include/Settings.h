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
 *  - Settings#language() - Application language name. Unset to use the system language.
 *  - Settings#windowState() - Main window state (see QMainWindow#saveState()).
 *  - Settings#windowGeometry() - Main window geometry.
 *  - Settings#updateChecks() - Whether automatic update checks are enabled (on supported platforms).
 *  - Settings#lessonPack() - Selected lesson pack (or keyboard layout in the settings).
 *  - Settings#customLessonPack() - Whether to load lesson pack from a file (if true, the file name is in Settings#lessonPack()).
 *  - Settings#errorPenalty() - Number of hits subtracted from net hits on every mistake.
 *  - Settings#mistakeLimit() - Whether to limit number of mistakes in per characters in words.
 *  - Settings#mistakeChars() - Number of characters in one word with max. 1 mistake.
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
 *  - Settings#initFinished() - If true, initial setup has been finished.
 */
class CORE_LIB_EXPORT Settings : public QObject
{
		Q_OBJECT
	public:
		static void init(void);
		// language
		Q_INVOKABLE static QString language(void);
		Q_INVOKABLE static bool containsLanguage(void);
		Q_INVOKABLE static void setLanguage(QString value);
		// windowState
		Q_INVOKABLE static QByteArray windowState(void);
		Q_INVOKABLE static bool containsWindowState(void);
		Q_INVOKABLE static void setWindowState(QByteArray value);
		// windowGeometry
		Q_INVOKABLE static QByteArray windowGeometry(void);
		Q_INVOKABLE static bool containsWindowGeometry(void);
		Q_INVOKABLE static void setWindowGeometry(QByteArray value);
		// updateChecks
		Q_INVOKABLE static bool updateChecks(void);
		Q_INVOKABLE static bool containsUpdateChecks(void);
		Q_INVOKABLE static void setUpdateChecks(bool value);
		// lessonPack
		Q_INVOKABLE static QString lessonPack(void);
		Q_INVOKABLE static bool containsLessonPack(void);
		Q_INVOKABLE static void setLessonPack(QString value);
		// customLessonPack
		Q_INVOKABLE static bool customLessonPack(void);
		Q_INVOKABLE static bool containsCustomLessonPack(void);
		Q_INVOKABLE static void setCustomLessonPack(bool value);
		// errorPenalty
		Q_INVOKABLE static int errorPenalty(void);
		Q_INVOKABLE static bool containsErrorPenalty(void);
		Q_INVOKABLE static void setErrorPenalty(int value);
		// mistakeLimit
		Q_INVOKABLE static bool mistakeLimit(void);
		Q_INVOKABLE static bool containsMistakeLimit(void);
		Q_INVOKABLE static void setMistakeLimit(bool value);
		// mistakeChars
		Q_INVOKABLE static int mistakeChars(void);
		Q_INVOKABLE static bool containsMistakeChars(void);
		Q_INVOKABLE static void setMistakeChars(int value);
		// themeFont
		Q_INVOKABLE static QString themeFont(void);
		Q_INVOKABLE static bool containsThemeFont(void);
		Q_INVOKABLE static void setThemeFont(QString value);
		// themeFontSize
		Q_INVOKABLE static int themeFontSize(void);
		Q_INVOKABLE static bool containsThemeFontSize(void);
		Q_INVOKABLE static void setThemeFontSize(int value);
		// themeFontBold
		Q_INVOKABLE static bool themeFontBold(void);
		Q_INVOKABLE static bool containsThemeFontBold(void);
		Q_INVOKABLE static void setThemeFontBold(bool value);
		// themeFontItalic
		Q_INVOKABLE static bool themeFontItalic(void);
		Q_INVOKABLE static bool containsThemeFontItalic(void);
		Q_INVOKABLE static void setThemeFontItalic(bool value);
		// themeFontUnderline
		Q_INVOKABLE static bool themeFontUnderline(void);
		Q_INVOKABLE static bool containsThemeFontUnderline(void);
		Q_INVOKABLE static void setThemeFontUnderline(bool value);
		// customExerciseTextColor
		Q_INVOKABLE static bool customExerciseTextColor(void);
		Q_INVOKABLE static bool containsCustomExerciseTextColor(void);
		Q_INVOKABLE static void setCustomExerciseTextColor(bool value);
		// exerciseTextColor
		Q_INVOKABLE static QRgb exerciseTextColor(void);
		Q_INVOKABLE static bool containsExerciseTextColor(void);
		Q_INVOKABLE static void setExerciseTextColor(QRgb value);
		// inputTextColor
		Q_INVOKABLE static QRgb inputTextColor(void);
		Q_INVOKABLE static bool containsInputTextColor(void);
		Q_INVOKABLE static void setInputTextColor(QRgb value);
		// customInputTextColor
		Q_INVOKABLE static bool customInputTextColor(void);
		Q_INVOKABLE static bool containsCustomInputTextColor(void);
		Q_INVOKABLE static void setCustomInputTextColor(bool value);
		// customBgColor
		Q_INVOKABLE static bool customBgColor(void);
		Q_INVOKABLE static bool containsCustomBgColor(void);
		Q_INVOKABLE static void setCustomBgColor(bool value);
		// bgColor
		Q_INVOKABLE static QRgb bgColor(void);
		Q_INVOKABLE static bool containsBgColor(void);
		Q_INVOKABLE static void setBgColor(QRgb value);
		// customPaperColor
		Q_INVOKABLE static bool customPaperColor(void);
		Q_INVOKABLE static bool containsCustomPaperColor(void);
		Q_INVOKABLE static void setCustomPaperColor(bool value);
		// paperColor
		Q_INVOKABLE static QRgb paperColor(void);
		Q_INVOKABLE static bool containsPaperColor(void);
		Q_INVOKABLE static void setPaperColor(QRgb value);
		// customPanelColor
		Q_INVOKABLE static bool customPanelColor(void);
		Q_INVOKABLE static bool containsCustomPanelColor(void);
		Q_INVOKABLE static void setCustomPanelColor(bool value);
		// panelColor
		Q_INVOKABLE static QRgb panelColor(void);
		Q_INVOKABLE static bool containsPanelColor(void);
		Q_INVOKABLE static void setPanelColor(QRgb value);
		// applicationStyle
		Q_INVOKABLE static ThemeEngine::Style applicationStyle(void);
		Q_INVOKABLE static bool containsApplicationStyle(void);
		Q_INVOKABLE static void setApplicationStyle(ThemeEngine::Style value);
		// applicationTheme
		Q_INVOKABLE static QString applicationTheme(void);
		Q_INVOKABLE static bool containsApplicationTheme(void);
		Q_INVOKABLE static void setApplicationTheme(QString value);
		// settingsLockEnabled
		Q_INVOKABLE static bool settingsLockEnabled(void);
		Q_INVOKABLE static bool containsSettingsLockEnabled(void);
		Q_INVOKABLE static void setSettingsLockEnabled(bool value);
		// settingsLockPasswd
		Q_INVOKABLE static QByteArray settingsLockPasswd(void);
		Q_INVOKABLE static bool containsSettingsLockPasswd(void);
		Q_INVOKABLE static void setSettingsLockPasswd(QByteArray value);
		// advancedTheme
		Q_INVOKABLE static bool advancedTheme(void);
		Q_INVOKABLE static bool containsAdvancedTheme(void);
		Q_INVOKABLE static void setAdvancedTheme(bool value);
		// simpleThemeId
		Q_INVOKABLE static ThemeEngine::SimpleTheme simpleThemeId(void);
		Q_INVOKABLE static bool containsSimpleThemeId(void);
		Q_INVOKABLE static void setSimpleThemeId(ThemeEngine::SimpleTheme value);
		// editorGeometry
		Q_INVOKABLE static QByteArray editorGeometry(void);
		Q_INVOKABLE static bool containsEditorGeometry(void);
		Q_INVOKABLE static void setEditorGeometry(QByteArray value);
		// keyboardVisible
		Q_INVOKABLE static bool keyboardVisible(void);
		Q_INVOKABLE static bool containsKeyboardVisible(void);
		Q_INVOKABLE static void setKeyboardVisible(bool value);
		// initFinished
		Q_INVOKABLE static bool initFinished(void);
		Q_INVOKABLE static bool containsInitFinished(void);
		Q_INVOKABLE static void setInitFinished(bool value);

	protected:
		static QVariant get(QString key, QVariant defaultValue);
		static bool contains(QString key);
		static void set(QString key, QVariant value);

	private:
		static QSettings *settingsInstance;
#ifdef Q_OS_WASM
		static bool tempSettingsCopied;
		static void copyTempSettings(void);
#endif // Q_OS_WASM
};

#endif // SETTINGS_H
