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
QMap<QString, QPair<QString, QMenu *>> AddonApi::m_menus;
QMap<QString, QPair<QPair<QIcon, QString>, QPair<AddonApi::TopBarSection, QPushButton *>>> AddonApi::m_buttons;
QMap<QString, QPair<QPair<AddonApi::TopBarSection, AddonApi::TopBarPos>, QWidget *>> AddonApi::m_topBarWidgets;

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

/*! Deletes all buttons. */
void AddonApi::deleteButtons(void)
{
	QStringList keys = m_buttons.keys();
	for(int i = 0; i < keys.count(); i++)
	{
		QPushButton *button = m_buttons[keys[i]].second.second;
		if(button != nullptr)
			button->deleteLater();
	}
	m_buttons.clear();
}

/*! Adds a button with the given icon and tool tip to the list of buttons. Use registerButton() to assign a QPushButton to the ID. */
void AddonApi::addButton(QString id, QIcon icon, QString toolTip, AddonApi::TopBarSection section)
{
	QPair<QIcon, QString> buttonPair1;
	QPair<TopBarSection, QPushButton *> buttonPair2;
	buttonPair1.first = icon;
	buttonPair1.second = toolTip;
	buttonPair2.first = section;
	buttonPair2.second = nullptr;
	m_buttons[id] = QPair<QPair<QIcon, QString>, QPair<TopBarSection, QPushButton *>>(buttonPair1, buttonPair2);
}

/*! Assigns a QPushButton to a button with the given ID. */
void AddonApi::registerButton(QString id, QPushButton *button)
{
	QPair<QIcon, QString> buttonPair1 = m_buttons[id].first;
	QPair<TopBarSection, QPushButton *> buttonPair2 = m_buttons[id].second;
	buttonPair2.second = button;
	m_buttons[id] = QPair<QPair<QIcon, QString>, QPair<TopBarSection, QPushButton *>>(buttonPair1, buttonPair2);
}

/*! Returns the map of buttons. */
QMap<QString, QPair<QPair<QIcon, QString>, QPair<AddonApi::TopBarSection, QPushButton *>>> AddonApi::buttons(void)
{
	return m_buttons;
}

/*! Convenience function, which returns button with the given ID. */
QPushButton *AddonApi::button(QString id)
{
	return m_buttons[id].second.second;
}

/*! Deletes all top bar widgets. */
void AddonApi::deleteTopBarWidgets(void)
{
	QStringList keys = m_topBarWidgets.keys();
	for(int i = 0; i < keys.count(); i++)
	{
		QWidget *widget = m_topBarWidgets[keys[i]].second;
		if(widget != nullptr)
			widget->deleteLater();
	}
	m_topBarWidgets.clear();
}

/*! Adds a top bar widget with the given position to the list of top bar widgets. Use registerTopBarWidget() to assign a QWidget to the ID. */
void AddonApi::addTopBarWidget(QString id, AddonApi::TopBarSection section, AddonApi::TopBarPos pos)
{
	QPair<TopBarSection, TopBarPos> widgetPos;
	widgetPos.first = section;
	widgetPos.second = pos;
	m_topBarWidgets[id] = QPair<QPair<TopBarSection, TopBarPos>, QWidget *>(widgetPos, nullptr);
}

/*! Assigns a QWidget to top bar widget with the given ID. */
void AddonApi::registerTopBarWidget(QString id, QWidget *widget)
{
	m_topBarWidgets[id] = QPair<QPair<TopBarSection, TopBarPos>, QWidget *>(m_topBarWidgets[id].first, widget);
}

/*! Returns the map of top bar widgets. */
QMap<QString, QPair<QPair<AddonApi::TopBarSection, AddonApi::TopBarPos>, QWidget *>> AddonApi::topBarWidgets(void)
{
	return m_topBarWidgets;
}

/*! Convenience function, which returns top bar widget with the given ID. */
QWidget *AddonApi::topBarWidget(QString id)
{
	return m_topBarWidgets[id].second;
}

/*! Transforms old widget to new widget. Useful if you don't want to use QWidget for custom widgets. */
void AddonApi::recreateWidget(QWidget *oldWidget, QWidget *newWidget)
{
	QWidget *parent = qobject_cast<QWidget *>(oldWidget->parent());
	newWidget->setParent(parent);
	oldWidget->deleteLater();
	parent->layout()->addWidget(newWidget);
}
