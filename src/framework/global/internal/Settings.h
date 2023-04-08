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

/*! \brief The Settings class manages the application settings. */
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
