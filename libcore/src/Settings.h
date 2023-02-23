/*
 * Settings.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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
 *  - Settings#windowX - Main window X position.
 *  - Settings#windowY - Main window Y position.
 *  - Settings#windowWidth - Main window width.
 *  - Settings#windowHeight - Main window height.
 *  - Settings#windowMaximized - Whether the main window was maximized when closed.
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
 *  - Settings#accentColorId() - Current accent color (accent colors are defined in ThemeEngine).
 *  - Settings#appTheme() - The application theme (0 = light, 1 = dark).
 *  - Settings#advancedTheme() - Whether to allow the user to select advanced themes (dark, light, light blue, etc.).
 *  - Settings#editorGeometry() - Deprecated, do not use in new code. There'll be a new pack editor window soon.
 *  - Settings#keyboardVisible() - Whether to show the virtual keyboard.
 *  - Settings#initFinished() - If true, initial setup has been finished.
 *  - Settings#keyboardFingerColors() - If true, the keys on the screen keyboard use a different color for fingers.
 */
class CORE_LIB_EXPORT Settings : public QObject
{
		Q_OBJECT
	public:
		static void init(void);
		Q_INVOKABLE static void freeze(void);
		Q_INVOKABLE static void saveChanges(void);
		Q_INVOKABLE static void discardChanges(void);
		Q_INVOKABLE static bool isFrozen(void);
		// language
		Q_INVOKABLE static QString language(void);
		Q_INVOKABLE static bool containsLanguage(void);
		Q_INVOKABLE static void setLanguage(QString value);
		// windowX
		Q_INVOKABLE static int windowX(void);
		Q_INVOKABLE static bool containsWindowX(void);
		Q_INVOKABLE static void setWindowX(int value);
		// windowY
		Q_INVOKABLE static int windowY(void);
		Q_INVOKABLE static bool containsWindowY(void);
		Q_INVOKABLE static void setWindowY(int value);
		// windowWidth
		Q_INVOKABLE static int windowWidth(void);
		Q_INVOKABLE static bool containsWindowWidth(void);
		Q_INVOKABLE static void setWindowWidth(int value);
		// windowHeight
		Q_INVOKABLE static int windowHeight(void);
		Q_INVOKABLE static bool containsWindowHeight(void);
		Q_INVOKABLE static void setWindowHeight(int value);
		// windowMaximized
		Q_INVOKABLE static bool windowMaximized(void);
		Q_INVOKABLE static bool containsWindowMaximized(void);
		Q_INVOKABLE static void setWindowMaximized(bool value);
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
		// accentColorId
		Q_INVOKABLE static ThemeEngine::AccentColor accentColorId(void);
		Q_INVOKABLE static bool containsAccentColorId(void);
		Q_INVOKABLE static void setAccentColorId(ThemeEngine::AccentColor value);
		// appTheme
		Q_INVOKABLE static ThemeEngine::Theme appTheme(void);
		Q_INVOKABLE static bool containsAppTheme(void);
		Q_INVOKABLE static void setAppTheme(ThemeEngine::Theme value);
		// advancedTheme
		Q_INVOKABLE static bool advancedTheme(void);
		Q_INVOKABLE static bool containsAdvancedTheme(void);
		Q_INVOKABLE static void setAdvancedTheme(bool value);
		// editorGeometry
		Q_INVOKABLE Q_DECL_DEPRECATED static QByteArray editorGeometry(void);
		Q_INVOKABLE Q_DECL_DEPRECATED static bool containsEditorGeometry(void);
		Q_INVOKABLE Q_DECL_DEPRECATED static void setEditorGeometry(QByteArray value);
		// keyboardVisible
		Q_INVOKABLE static bool keyboardVisible(void);
		Q_INVOKABLE static bool containsKeyboardVisible(void);
		Q_INVOKABLE static void setKeyboardVisible(bool value);
		// initFinished
		Q_INVOKABLE static bool initFinished(void);
		Q_INVOKABLE static bool containsInitFinished(void);
		Q_INVOKABLE static void setInitFinished(bool value);
		// keyboardFingerColors
		Q_INVOKABLE static bool keyboardFingerColors(void);
		Q_INVOKABLE static bool containsKeyboardFingerColors(void);
		Q_INVOKABLE static void setKeyboardFingerColors(bool value);

	protected:
		static QVariant get(QString key, QVariant defaultValue);
		static bool contains(QString key);
		static void set(QString key, QVariant value);

	private:
		static QSettings *settingsInstance;
		static QSettings *mainSettingsInstance;
		static bool frozen;
		static void copySettings(QSettings *source, QSettings *target);
#ifdef Q_OS_WASM
		static bool tempSettingsCopied;
		static void copyTempSettings(void);
#endif // Q_OS_WASM
};

#endif // SETTINGS_H
