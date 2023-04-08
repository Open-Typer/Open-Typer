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

#include <QSettings>
#include <QRgb>
#include <QColor>
#include "../ISettings.h"
#include "global/FileUtils.h"
#include "ui/ThemeEngine.h"
#include "grades/ClassManager.h"

/*!
 * \brief The Settings class contains functions for application settings.
 *
 * You should use this class instead of QSettings.\n
 * If there is a new settings key, create getter and setter functions for it.
 * and don't forget to describe it in the list below.\n
 * Use Settings#init() to initialize settings when the application starts.\n
 * \n
 * <b>List of settings keys:</b>
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
 *  - Settings#initFinished() - If true, initial setup has been finished.
 *  - Settings#targetHitsPerMinute() - Number of hits per minute for the best grade.
 *  - Settings#gradeNetHits() - Whether to use net hits per minute for grading.
 *  - Settings#selectedClass() - Index of selected class.
 *  - Settings#gradingMethod() - The type of grades, for example letters or numbers.
 *  - Settings#gradeStartNumber() - The number to start with during grading.
 *  - Settings#gradeEndNumber() - The number to end with during grading.
 *  - Settings#gradeStartLetter() - The letter to start with during grading.
 *  - Settings#gradeEndLetter() - The letter to end with during grading.
 */
class Q_DECL_EXPORT Settings : public ISettings
{
		Q_OBJECT
	public:
		static std::shared_ptr<Settings> instance();
		static void init(void);
		void addKey(QString moduleName, QString keyName, QString key, QVariant defaultValue) override;
		Q_INVOKABLE void setValue(QString moduleName, QString keyName, QVariant value) override;
		void setValue(Key key, QVariant value) override;
		Q_INVOKABLE QVariant getValue(QString moduleName, QString keyName) override;
		QVariant getValue(Key key) override;
		Q_INVOKABLE bool containsKey(QString moduleName, QString keyName) override;
		bool containsKey(Key key) override;
		Q_INVOKABLE void freeze(void) override;
		Q_INVOKABLE void saveChanges(void) override;
		Q_INVOKABLE void discardChanges(void) override;
		Q_INVOKABLE bool isFrozen(void) override;
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
		Q_INVOKABLE Q_DECL_DEPRECATED static bool themeFontBold(void);
		Q_INVOKABLE Q_DECL_DEPRECATED static bool containsThemeFontBold(void);
		Q_INVOKABLE Q_DECL_DEPRECATED static void setThemeFontBold(bool value);
		// themeFontItalic
		Q_INVOKABLE Q_DECL_DEPRECATED static bool themeFontItalic(void);
		Q_INVOKABLE Q_DECL_DEPRECATED static bool containsThemeFontItalic(void);
		Q_INVOKABLE Q_DECL_DEPRECATED static void setThemeFontItalic(bool value);
		// themeFontUnderline
		Q_INVOKABLE Q_DECL_DEPRECATED static bool themeFontUnderline(void);
		Q_INVOKABLE Q_DECL_DEPRECATED static bool containsThemeFontUnderline(void);
		Q_INVOKABLE Q_DECL_DEPRECATED static void setThemeFontUnderline(bool value);
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
		// initFinished
		Q_INVOKABLE static bool initFinished(void);
		Q_INVOKABLE static bool containsInitFinished(void);
		Q_INVOKABLE static void setInitFinished(bool value);
		// targetHitsPerMinute
		Q_INVOKABLE static int targetHitsPerMinute(void);
		Q_INVOKABLE static bool containsTargetHitsPerMinute(void);
		Q_INVOKABLE static void setTargetHitsPerMinute(int value);
		// gradeNetHits
		Q_INVOKABLE static bool gradeNetHits(void);
		Q_INVOKABLE static bool containsGradeNetHits(void);
		Q_INVOKABLE static void setGradeNetHits(bool value);
		// selectedClass
		Q_INVOKABLE static int selectedClass(void);
		Q_INVOKABLE static bool containsSelectedClass(void);
		Q_INVOKABLE static void setSelectedClass(int value);
		// gradingMethod
		Q_INVOKABLE static ClassManager::GradingMethod gradingMethod(void);
		Q_INVOKABLE static bool containsGradingMethod(void);
		Q_INVOKABLE static void setGradingMethod(ClassManager::GradingMethod value);
		// gradeStartNumber
		Q_INVOKABLE static int gradeStartNumber(void);
		Q_INVOKABLE static bool containsGradeStartNumber(void);
		Q_INVOKABLE static void setGradeStartNumber(int value);
		// gradeEndNumber
		Q_INVOKABLE static int gradeEndNumber(void);
		Q_INVOKABLE static bool containsGradeEndNumber(void);
		Q_INVOKABLE static void setGradeEndNumber(int value);
		// gradeStartLetter
		Q_INVOKABLE static QChar gradeStartLetter(void);
		Q_INVOKABLE static bool containsGradeStartLetter(void);
		Q_INVOKABLE static void setGradeStartLetter(QChar value);
		// gradeEndLetter
		Q_INVOKABLE static QChar gradeEndLetter(void);
		Q_INVOKABLE static bool containsGradeEndLetter(void);
		Q_INVOKABLE static void setGradeEndLetter(QChar value);

	protected:
		static QVariant get(QString key, QVariant defaultValue);
		static bool contains(QString key);
		static void set(QString key, QVariant value);

	private:
		static std::shared_ptr<Settings> m_instance;
		static QSettings *settingsInstance;
		static QSettings *mainSettingsInstance;
		static bool frozen;
		static QMap<QPair<QString, QString>, Key> m_keys;
		static void copySettings(QSettings *source, QSettings *target);
#ifdef Q_OS_WASM
		static bool tempSettingsCopied;
		static void copyTempSettings(void);
#endif // Q_OS_WASM
};

#endif // SETTINGS_H