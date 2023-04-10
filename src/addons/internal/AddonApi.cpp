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
#include "app/AppMenuBar.h"

std::shared_ptr<AddonApi> AddonApi::m_instance = std::make_shared<AddonApi>();

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
	SettingsCategory *category = new SettingsCategory(this);
	category->setName(categoryName);
	category->setQmlFileName(qmlFileName);
	category->setIconName(iconName);
	category->setIconSource(iconSource);
	m_settingsCategories.append(category);
	emit settingsCategoriesChanged();
	return true;
}

/*! List of settings categories. */
QQmlListProperty<SettingsCategory> AddonApi::settingsCategories(void)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	return QQmlListProperty<SettingsCategory>(this, &m_settingsCategories);
#else
	return QQmlListProperty<SettingsCategory>(this, m_settingsCategories);
#endif
}

/*! Deletes all settings categories. */
void AddonApi::deleteSettingsCategories(void)
{
	QList<QObject *> objList;
	for(int i = 0; i < m_settingsCategories.length(); i++)
		objList.append(m_settingsCategories[i]);
	deleteObjects(&objList);
	m_settingsCategories.clear();
	emit settingsCategoriesChanged();
}

/*! Returns the static instance of AddonApi. */
std::shared_ptr<AddonApi> AddonApi::instance()
{
	return m_instance;
}

/*! Sends an event with the given type to each loaded addon. */
void AddonApi::sendEvent(Event type, QVariantMap args)
{
	if(type == Event_InitApp)
	{
		deleteSettingsCategories();
		deleteMenus();
		deleteMainButtons();
		deleteExOptionsButtons();
		deleteNavigationButtons();
		deleteExInfoButtons();
	}
	for(int i = 0; i < m_loadedAddons.count(); i++)
		m_loadedAddons[i]->addonEvent(type, args);
}

/*! Deletes all menus. */
void AddonApi::deleteMenus(void)
{
	AppMenuBar::instance()->blockSignals(true);
	for(int i = 0; i < m_menus.length(); i++)
	{
		auto *menu = m_menus[i];
		if(menu)
		{
			AppMenuBar::instance()->removeMenu(menu);
			menu->deleteLater();
		}
	}
	AppMenuBar::instance()->blockSignals(false);
	emit AppMenuBar::instance()->menusChanged();
	m_menus.clear();
	emit menusChanged();
}

/*! Adds a menu. */
void AddonApi::addMenu(AppMenuModel *menu)
{
	AppMenuBar::instance()->addMenu(menu);
	m_menus.append(menu);
}

/*! List of menus. */
QQmlListProperty<AppMenuModel> AddonApi::menus(void)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	return QQmlListProperty<AppMenuModel>(this, &m_menus);
#else
	return QQmlListProperty<AppMenuModel>(this, m_menus);
#endif
}

/*! Deletes all buttons in the main section. */
void AddonApi::deleteMainButtons(void)
{
	deleteButtons(&m_mainButtons);
	emit mainButtonsChanged();
}

/*! Adds a new button to the main section. */
AddonButton *AddonApi::addMainButton(QString text, QString toolTip, QString iconName, QString iconSource)
{
	AddonButton *button = createButton(text, toolTip, iconName, iconSource);
	m_mainButtons.append(button);
	emit mainButtonsChanged();
	return button;
}

/*! The list of buttons in the main section. */
QQmlListProperty<AddonButton> AddonApi::mainButtons(void)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	return QQmlListProperty<AddonButton>(this, &m_mainButtons);
#else
	return QQmlListProperty<AddonButton>(this, m_mainButtons);
#endif
}

/*! Deletes all buttons in the exercise options section. */
void AddonApi::deleteExOptionsButtons(void)
{
	deleteButtons(&m_exOptionsButtons);
	emit exOptionsButtonsChanged();
}

/*! Adds a new button to the exercise options section. */
AddonButton *AddonApi::addExOptionsButton(QString text, QString toolTip, QString iconName, QString iconSource)
{
	AddonButton *button = createButton(text, toolTip, iconName, iconSource);
	m_exOptionsButtons.append(button);
	emit exOptionsButtonsChanged();
	return button;
}

/*! The list of buttons in the exercise options section. */
QQmlListProperty<AddonButton> AddonApi::exOptionsButtons(void)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	return QQmlListProperty<AddonButton>(this, &m_exOptionsButtons);
#else
	return QQmlListProperty<AddonButton>(this, m_exOptionsButtons);
#endif
}

/*! Deletes all buttons in the navigation section. */
void AddonApi::deleteNavigationButtons(void)
{
	deleteButtons(&m_navigationButtons);
	emit navigationButtonsChanged();
}

/*! Adds a new button to the navigation section. */
AddonButton *AddonApi::addNavigationButton(QString text, QString toolTip, QString iconName, QString iconSource)
{
	AddonButton *button = createButton(text, toolTip, iconName, iconSource);
	m_navigationButtons.append(button);
	emit navigationButtonsChanged();
	return button;
}

/*! The list of buttons in the navigation section. */
QQmlListProperty<AddonButton> AddonApi::navigationButtons(void)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	return QQmlListProperty<AddonButton>(this, &m_navigationButtons);
#else
	return QQmlListProperty<AddonButton>(this, m_navigationButtons);
#endif
}

/*! Deletes all buttons in the exercise info section. */
void AddonApi::deleteExInfoButtons(void)
{
	deleteButtons(&m_exInfoButtons);
	emit exInfoButtonsChanged();
}

/*! Adds a new button to the exercise info section. */
AddonButton *AddonApi::addExInfoButton(QString text, QString toolTip, QString iconName, QString iconSource)
{
	AddonButton *button = createButton(text, toolTip, iconName, iconSource);
	m_exInfoButtons.append(button);
	emit exInfoButtonsChanged();
	return button;
}

/*! The list of buttons in the exercise info section. */
QQmlListProperty<AddonButton> AddonApi::exInfoButtons(void)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	return QQmlListProperty<AddonButton>(this, &m_exInfoButtons);
#else
	return QQmlListProperty<AddonButton>(this, m_exInfoButtons);
#endif
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
