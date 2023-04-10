/*
 * Settings.cpp
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

#include <QApplication>
#include "Settings.h"

std::shared_ptr<Settings> Settings::m_instance = std::make_shared<Settings>();
QSettings *Settings::settingsInstance = nullptr;
QSettings *Settings::mainSettingsInstance = nullptr;
bool Settings::frozen = false;
QMap<QPair<QString, QString>, Settings::Key> Settings::m_keys;
#ifdef Q_OS_WASM
bool Settings::tempSettingsCopied = false;
#endif // Q_OS_WASM

/*! Returns the static instance of Settings. */
std::shared_ptr<Settings> Settings::instance()
{
	return m_instance;
}

/*! Initializes settings. Run Settings#init() after the application starts. */
void Settings::init(void)
{
#ifdef Q_OS_WASM
	settingsInstance = new QSettings(qApp);
#else
	settingsInstance = new QSettings(FileUtils::instance()->mainSettingsLocation(), QSettings::IniFormat, qApp);
#endif
}

/*! Registers a key. */
void Settings::addKey(QString moduleName, QString keyName, QString key, QVariant defaultValue)
{
	Key k(moduleName, key, defaultValue);
	m_keys.insert({ moduleName, keyName }, k);
}

/*! Sets the key value. */
void Settings::setValue(QString moduleName, QString keyName, QVariant value)
{
	QPair<QString, QString> key = { moduleName, keyName };
	Q_ASSERT(m_keys.contains(key));
	if(m_keys.contains(key))
		set(m_keys[key].key, value);
}

/*! Sets the key value (module name and key name are stored in the Key object). */
void Settings::setValue(Key key, QVariant value)
{
	setValue(key.moduleName, key.key, value);
}

/*! Returns the value of the given key. */
QVariant Settings::getValue(QString moduleName, QString keyName)
{
	QPair<QString, QString> key = { moduleName, keyName };
	Q_ASSERT(m_keys.contains(key));
	if(m_keys.contains(key))
	{
		Key k = m_keys[key];
		QVariant value = get(k.key, k.defaultValue);
		if((value == "true") || (value == "false"))
			value = value.toBool();
		else if(value.toReal() == value)
			value = value.toReal();
		return value;
	}
	else
		return QVariant();
}

/*! Returns the value of the given key (module name and key name are stored in the Key object). */
QVariant Settings::getValue(Key key)
{
	return getValue(key.moduleName, key.key);
}

/*! Returns true if the given key is defined. */
bool Settings::containsKey(QString moduleName, QString keyName)
{
	QPair<QString, QString> key = { moduleName, keyName };
	Q_ASSERT(m_keys.contains(key));
	if(m_keys.contains(key))
	{
		return contains(m_keys[key].key);
	}
	else
		return false;
}

/*! Returns true if the given key is defined (module name and key name are stored in the Key object). */
bool Settings::containsKey(Key key)
{
	return containsKey(key.moduleName, key.key);
}

/*!
 * Switches to temporary settings. You can decide to saveChanges() or discardChanges() later.\n
 * This is useful for settings dialogs with a discard button.
 */
void Settings::freeze(void)
{
	Q_ASSERT(!frozen);
	mainSettingsInstance = settingsInstance;
	settingsInstance = new QSettings(FileUtils::instance()->mainSettingsLocation() + ".tmp", QSettings::IniFormat, qApp);
	copySettings(mainSettingsInstance, settingsInstance);
	frozen = true;
	emit stateChanged();
}

/*! Saves changes to real settings and switches back to them. \see freeze() */
void Settings::saveChanges(void)
{
	Q_ASSERT(frozen);
	copySettings(settingsInstance, mainSettingsInstance);
	mainSettingsInstance->sync();
	settingsInstance->clear();
	settingsInstance->deleteLater();
	settingsInstance = mainSettingsInstance;
	frozen = false;
	emit stateChanged();
	emit saved();
}

/*! Discards changes and switches back to real settings. \see freeze() */
void Settings::discardChanges(void)
{
	Q_ASSERT(frozen);
	settingsInstance->clear();
	settingsInstance->deleteLater();
	settingsInstance = mainSettingsInstance;
	frozen = false;
	emit stateChanged();
	emit discarded();
}

/*! Returns true if settings are frozen. \see freeze(). */
bool Settings::isFrozen(void)
{
	return frozen;
}

/*! Returns the value of the given key. */
QVariant Settings::get(QString key, QVariant defaultValue)
{
	Q_ASSERT(settingsInstance != nullptr);
#ifdef Q_OS_WASM
	if(settingsInstance->isWritable())
	{
		if(!tempSettingsCopied)
			copyTempSettings();
		return settingsInstance->value(key, defaultValue);
	}
	else
	{
		// Use temporary settings until sandbox is initialized
		QSettings settings(FileUtils::instance()->mainSettingsLocation(), QSettings::IniFormat);
		return settings.value(key, defaultValue);
	}
#else
	return settingsInstance->value(key, defaultValue);
#endif // Q_OS_WASM
}

/*! Returns true if the given key exists. */
bool Settings::contains(QString key)
{
	Q_ASSERT(settingsInstance != nullptr);
#ifdef Q_OS_WASM
	if(settingsInstance->isWritable())
	{
		if(!tempSettingsCopied)
			copyTempSettings();
		return settingsInstance->contains(key);
	}
	else
	{
		// Use temporary settings until sandbox is initialized
		QSettings settings(FileUtils::instance()->mainSettingsLocation(), QSettings::IniFormat);
		return settings.contains(key);
	}
#else
	return settingsInstance->contains(key);
#endif // Q_OS_WASM
}

/*! Sets the key value. */
void Settings::set(QString key, QVariant value)
{
	Q_ASSERT(settingsInstance != nullptr);
#ifdef Q_OS_WASM
	if(settingsInstance->isWritable())
	{
		if(!tempSettingsCopied)
			copyTempSettings();
		settingsInstance->setValue(key, value);
		settingsInstance->sync();
	}
	else
	{
		// Use temporary settings until sandbox is initialized
		QSettings settings(FileUtils::instance()->mainSettingsLocation(), QSettings::IniFormat);
		settings.setValue(key, value);
	}
#else
	settingsInstance->setValue(key, value);
#endif // Q_OS_WASM
}

/*! Copies values from source settings to target settings. */
void Settings::copySettings(QSettings *source, QSettings *target)
{
#ifndef Q_OS_WASM
	target->clear();
#endif
	QStringList keys = source->allKeys();
	for(int i = 0; i < keys.count(); i++)
		target->setValue(keys[i], source->value(keys[i]));
}

#ifdef Q_OS_WASM
/*! Copies temporary settings to real settings after the WebAssembly sandbox gets initialized. */
void Settings::copyTempSettings(void)
{
	QSettings settings(FileUtils::instance()->mainSettingsLocation(), QSettings::IniFormat);
	copySettings(&settings, settingsInstance);
	settingsInstance->sync();
	tempSettingsCopied = true;
}
#endif // Q_OS_WASM
