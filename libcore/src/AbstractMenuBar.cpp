/*
 * AbstractMenuBar.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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
#include "AbstractMenuBar.h"

/*! Constructs AbstractMenuBar. */
AbstractMenuBar::AbstractMenuBar(QObject *parent) :
	QObject(parent)
{
	m_menus.clear();
	QMetaObject::invokeMethod(this, "createMenus", Qt::QueuedConnection);
	QMetaObject::invokeMethod(this, "updateMenus", Qt::QueuedConnection);
}

/*! Override this method to create menus. */
void AbstractMenuBar::createMenus(void)
{
}

/*! Override this method to set titles and menu item text. */
void AbstractMenuBar::updateMenus(void)
{
}

/*! List of menus in the menu bar. */
QList<AppMenuModel *> AbstractMenuBar::menus(void)
{
	return m_menus;
}

void AbstractMenuBar::setMenus(QList<AppMenuModel *> newMenus)
{
	m_menus = newMenus;
	emit menusChanged(newMenus);
}

/*! Adds a menu. */
void AbstractMenuBar::addMenu(AppMenuModel *menu)
{
	m_menus.append(menu);
	emit menusChanged(m_menus);
}

/*! Removes a menu. */
void AbstractMenuBar::removeMenu(AppMenuModel *menu)
{
	m_menus.removeAll(menu);
	emit menusChanged(m_menus);
}
