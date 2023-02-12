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
#include <QFile>
#include "AddonApi.h"
#include "IAddon.h"
#include "AppMenuBar.h"

AddonApi globalAddonApi;
QMap<int, QString> AddonApi::m_loadExTargets;
bool AddonApi::m_blockLoadedEx;
QVector<IAddon *> AddonApi::m_loadedAddons;

/*! List of loaded addon interfaces. Automatically set by AddonManager. */
QVector<IAddon *> AddonApi::loadedAddons(void)
{
	return m_loadedAddons;
}

void AddonApi::setLoadedAddons(QVector<IAddon *> newLoadedAddons)
{
	if(m_loadedAddons == newLoadedAddons)
		return;
	m_loadedAddons = newLoadedAddons;
}

/*! Adds a settings category with QML component with the given URL. */
bool AddonApi::addSettingsCategory(QString categoryName, QString qmlFileName, QString iconName, QString iconSource)
{
	if(!QFile::exists(":/qml/" + qmlFileName))
	{
		QMessageBox::critical(nullptr, "Error", QString("Could not add settings category \"%1\": QML file \"%2\" not found").arg(categoryName, qmlFileName));
		return false;
	}
	AddonSettingsCategory *category = new AddonSettingsCategory(this);
	category->setName(categoryName);
	category->setQmlFileName(qmlFileName);
	category->setIconName(iconName);
	category->setIconSource(iconSource);
	m_settingsCategories.append(category);
	emit settingsCategoriesChanged(m_settingsCategories);
	return true;
}

/*! List of settings categories. */
QList<AddonSettingsCategory *> AddonApi::settingsCategories(void)
{
	return m_settingsCategories;
}

/*! Deletes all settings categories. */
void AddonApi::deleteSettingsCategories(void)
{
	QList<QObject *> objList;
	for(int i = 0; i < m_settingsCategories.length(); i++)
		objList.append(m_settingsCategories[i]);
	deleteObjects(&objList);
	m_settingsCategories.clear();
	emit settingsCategoriesChanged(m_settingsCategories);
}

/*! Sends an event with the given type to each loaded addon. */
void AddonApi::sendEvent(Event type, QVariantMap args)
{
	if(type == Event_InitApp)
	{
		globalAddonApi.deleteSettingsCategories();
		globalAddonApi.deleteMenus();
		globalAddonApi.deleteMainButtons();
		globalAddonApi.deleteExOptionsButtons();
		globalAddonApi.deleteNavigationButtons();
		globalAddonApi.deleteExInfoButtons();
	}
	for(int i = 0; i < m_loadedAddons.count(); i++)
		m_loadedAddons[i]->addonEvent(type, args);
}

/*! Deletes all menus. */
void AddonApi::deleteMenus(void)
{
	globalMenuBar.blockSignals(true);
	for(int i = 0; i < m_menus.length(); i++)
	{
		auto *menu = m_menus[i];
		if(menu)
		{
			globalMenuBar.removeMenu(menu);
			menu->deleteLater();
		}
	}
	globalMenuBar.blockSignals(false);
	globalMenuBar.menusChanged(globalMenuBar.menus());
	m_menus.clear();
	emit menusChanged(m_menus);
}

/*! Adds a menu. */
void AddonApi::addMenu(AppMenuModel *menu)
{
	globalMenuBar.addMenu(menu);
	m_menus.append(menu);
}

/*! List of menus. */
QList<AppMenuModel *> AddonApi::menus(void)
{
	return m_menus;
}

/*! Deletes all buttons in the main section. */
void AddonApi::deleteMainButtons(void)
{
	deleteButtons(&m_mainButtons);
	emit mainButtonsChanged(m_mainButtons);
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

/*! Deletes all buttons in the exercise options section. */
void AddonApi::deleteExOptionsButtons(void)
{
	deleteButtons(&m_exOptionsButtons);
	emit exOptionsButtonsChanged(m_exOptionsButtons);
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

/*! Deletes all buttons in the navigation section. */
void AddonApi::deleteNavigationButtons(void)
{
	deleteButtons(&m_navigationButtons);
	emit navigationButtonsChanged(m_navigationButtons);
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

/*! Deletes all buttons in the exercise info section. */
void AddonApi::deleteExInfoButtons(void)
{
	deleteButtons(&m_exInfoButtons);
	emit exInfoButtonsChanged(m_exInfoButtons);
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

/*! Deletes all buttons in the given list and clears it. */
void AddonApi::deleteButtons(QList<AddonButton *> *buttonList)
{
	QList<QObject *> objList;
	for(int i = 0; i < buttonList->length(); i++)
		objList.append(buttonList->at(i));
	deleteObjects(&objList);
	buttonList->clear();
}

/*! Deletes all objects in the given list and clears it. */
void AddonApi::deleteObjects(QList<QObject *> *objList)
{
	for(int i = 0; i < objList->length(); i++)
	{
		QObject *obj = objList->at(i);
		if(obj)
			obj->deleteLater();
	}
	objList->clear();
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
