/*
 * AddonApi.cpp
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
#include "AddonApi.h"
#include "AddonButton.h"

AddonApi AddonApi::m_instance;
QMap<int, QString> AddonApi::m_loadExTargets;
bool AddonApi::m_blockLoadedEx;
QList<QVariantMap> AddonApi::m_settingsCategories;
QMap<QString, QPair<QString, QMenu *>> AddonApi::m_menus;

/*! Returns pointer to the global instance of AddonApi. Can be used to emit signals. */
AddonApi *AddonApi::instance(void)
{
	return &m_instance;
}

/*! Adds a load exercise target. */
void AddonApi::addLoadExTarget(int id, QString name)
{
	m_loadExTargets[id] = name;
}

/*! Clears map of load exercise targets. */
void AddonApi::clearLoadExTargets(void)
{
	m_loadExTargets.clear();
}

/*! Returns map of load exercise targets. */
QMap<int, QString> AddonApi::loadExTargets(void)
{
	return m_loadExTargets;
}

/*! Returns true if an addon blocked starting the exercise loaded by LoadExerciseDialog. */
bool AddonApi::blockLoadedEx(void)
{
	return m_blockLoadedEx;
}

/*! If set to true, the exercise loaded by LoadExerciseDialog will be ignored. */
void AddonApi::setBlockLoadedEx(bool value)
{
	m_blockLoadedEx = value;
}

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
void AddonApi::sendEvent(IAddon::Event type, QVariantMap args)
{
	for(int i = 0; i < loadedAddons.count(); i++)
		loadedAddons[i]->addonEvent(type, args);
}

/*! Deletes all menus. */
void AddonApi::deleteMenus(void)
{
	QStringList keys = m_menus.keys();
	for(int i = 0; i < keys.count(); i++)
	{
		QMenu *menu = m_menus[keys[i]].second;
		if(menu != nullptr)
			menu->deleteLater();
	}
	m_menus.clear();
}

/*! Adds a menu with the given name to the list of menus. Use registerMenu() to assign a QMenu to the ID. */
void AddonApi::addMenu(QString id, QString name)
{
	m_menus[id] = QPair<QString, QMenu *>(name, nullptr);
}

/*! Assigns a QMenu to menu with the given ID. */
void AddonApi::registerMenu(QString id, QMenu *menu)
{
	m_menus[id] = QPair<QString, QMenu *>(m_menus[id].first, menu);
}

/*! Returns the map of menus. */
QMap<QString, QPair<QString, QMenu *>> AddonApi::menus(void)
{
	return m_menus;
}

/*! Convenience function, which returns menu with the given ID. */
QMenu *AddonApi::menu(QString id)
{
	return m_menus[id].second;
}

/*! Adds a new button to the main section. */
AddonButton *AddonApi::addMainButton(QString text, QString toolTip, QString iconName, QString iconSource)
{
	AddonButton *button = createButton(text, toolTip, iconName, iconSource);
	m_mainButtons.append(button);
	emit mainButtonsChanged(m_mainButtons);
	return button;
}

/*! The list of buttons in the main section. */
QList<AddonButton *> AddonApi::mainButtons(void)
{
	return m_mainButtons;
}

void AddonApi::setMainButtons(QList<AddonButton *> buttons)
{
	m_mainButtons = buttons;
	emit mainButtonsChanged(buttons);
}

/*! Adds a new button to the exercise options section. */
AddonButton *AddonApi::addExOptionsButton(QString text, QString toolTip, QString iconName, QString iconSource)
{
	AddonButton *button = createButton(text, toolTip, iconName, iconSource);
	m_exOptionsButtons.append(button);
	emit exOptionsButtonsChanged(m_exOptionsButtons);
	return button;
}

/*! The list of buttons in the exercise options section. */
QList<AddonButton *> AddonApi::exOptionsButtons(void)
{
	return m_exOptionsButtons;
}

void AddonApi::setExOptionsButtons(QList<AddonButton *> buttons)
{
	m_exOptionsButtons = buttons;
	emit exOptionsButtonsChanged(buttons);
}

/*! Adds a new button to the navigation section. */
AddonButton *AddonApi::addNavigationButton(QString text, QString toolTip, QString iconName, QString iconSource)
{
	AddonButton *button = createButton(text, toolTip, iconName, iconSource);
	m_navigationButtons.append(button);
	emit navigationButtonsChanged(m_navigationButtons);
	return button;
}

/*! The list of buttons in the navigation section. */
QList<AddonButton *> AddonApi::navigationButtons(void)
{
	return m_navigationButtons;
}

void AddonApi::setNavigationButtons(QList<AddonButton *> buttons)
{
	m_navigationButtons = buttons;
	emit navigationButtonsChanged(buttons);
}

/*! Adds a new button to the exercise info section. */
AddonButton *AddonApi::addExInfoButton(QString text, QString toolTip, QString iconName, QString iconSource)
{
	AddonButton *button = createButton(text, toolTip, iconName, iconSource);
	m_exInfoButtons.append(button);
	emit exInfoButtonsChanged(m_exInfoButtons);
	return button;
}

/*! The list of buttons in the exercise info section. */
QList<AddonButton *> AddonApi::exInfoButtons(void)
{
	return m_exInfoButtons;
}

void AddonApi::setExInfoButtons(QList<AddonButton *> buttons)
{
	m_exInfoButtons = buttons;
	emit exInfoButtonsChanged(buttons);
}

/*! Creates a new button. */
AddonButton *AddonApi::createButton(QString text, QString toolTip, QString iconName, QString iconSource)
{
	AddonButton *button = new AddonButton(qApp);
	button->setText(text);
	button->setToolTip(toolTip);
	button->setIconName(iconName);
	button->setIconSource(iconSource);
	return button;
}
