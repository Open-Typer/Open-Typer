/*
 * AddonApi.cpp
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

#include "AddonApi.h"

QList<QVariantMap> AddonApi::m_settingsCategories;

/*! Adds settings category with widget of class className. */
bool AddonApi::addSettingsCategory(QString categoryName, QIcon icon, QString className)
{
	if(QMetaType::type(QString(className + "*").toStdString().c_str()) == QMetaType::UnknownType)
	{
		QMessageBox::critical(nullptr, "Error", QString("Could not add settings category \"%1\": meta type \"%2\" not found").arg(categoryName, className));
		return false;
	}
	QVariantMap category;
	category.insert("name", categoryName);
	category.insert("icon", icon);
	category.insert("class", className);
	m_settingsCategories.append(category);
	return true;
}

/*!
 * Returns list of settings categories.\n
 * Every category is defined by a map consisting of "name", "icon" and "class" keys. \see addSettingsCategory
 */
QList<QVariantMap> AddonApi::settingsCategories(void)
{
	return m_settingsCategories;
}

/*! Removes all settings categories. */
void AddonApi::clearSettingsCategories(void)
{
	m_settingsCategories.clear();
}

/*! Sends IAddon#Event_InitSettings to each loaded addon. */
void AddonApi::initSettingsCategories(bool clear)
{
	if(clear)
		clearSettingsCategories();
	sendEvent(IAddon::Event_InitSettings);
}

/*! Sends an event with the given type to each loaded addon. */
void AddonApi::sendEvent(IAddon::Event type)
{
	for(int i = 0; i < loadedAddons.count(); i++)
		loadedAddons[i]->addonEvent(type);
}
