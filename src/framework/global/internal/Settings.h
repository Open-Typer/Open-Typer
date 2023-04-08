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
 *  - Settings#lessonPack() - Selected lesson pack (or keyboard layout in the settings).
 *  - Settings#customLessonPack() - Whether to load lesson pack from a file (if true, the file name is in Settings#lessonPack()).
 *  - Settings#errorPenalty() - Number of hits subtracted from net hits on every mistake.
 *  - Settings#mistakeLimit() - Whether to limit number of mistakes in per characters in words.
 *  - Settings#mistakeChars() - Number of characters in one word with max. 1 mistake.
 *  - Settings#editorGeometry() - Deprecated, do not use in new code. There'll be a new pack editor window soon.
 *  - Settings#initFinished() - If true, initial setup has been finished.
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
		// editorGeometry
		Q_INVOKABLE Q_DECL_DEPRECATED static QByteArray editorGeometry(void);
		Q_INVOKABLE Q_DECL_DEPRECATED static bool containsEditorGeometry(void);
		Q_INVOKABLE Q_DECL_DEPRECATED static void setEditorGeometry(QByteArray value);
		// initFinished
		Q_INVOKABLE static bool initFinished(void);
		Q_INVOKABLE static bool containsInitFinished(void);
		Q_INVOKABLE static void setInitFinished(bool value);

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
