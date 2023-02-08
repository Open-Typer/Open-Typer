/*
 * AppMenuBar.cpp
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
#include "AppMenuBar.h"

AppMenuBar globalMenuBar;

/*! Constructs AppMenuBar. */
AppMenuBar::AppMenuBar(QObject *parent) :
	QObject(parent)
{
	QMetaObject::invokeMethod(this, "createMenus", Qt::QueuedConnection);
}

/*! Creates default menus. */
void AppMenuBar::createMenus(void)
{
	m_menus.clear();
	// File
	fileMenu.addItem(&m_openAction);
	fileSeparator1.setIsSeparator(true);
	fileMenu.addItem(&fileSeparator1);
#ifndef Q_OS_WASM
	fileMenu.addItem(&m_printAction);
	fileSeparator2.setIsSeparator(true);
	fileMenu.addItem(&fileSeparator2);
#endif
	fileMenu.addItem(&quitAction);
	connect(&quitAction, &AppMenuItem::clicked, this, []() {
		qApp->quit();
	});
	m_menus.append(&fileMenu);
	updateMenus();
}

/*! Updates default menus. */
void AppMenuBar::updateMenus(void)
{
	// File
	fileMenu.setTitle(tr("&File"));
	m_openAction.setText(tr("Open..."));
	m_printAction.setText(tr("Print"));
	quitAction.setText(tr("Quit"));
	emit menusChanged(m_menus);
}

/*! List of menus in the menu bar. */
QList<AppMenuModel *> AppMenuBar::menus(void)
{
	return m_menus;
}

void AppMenuBar::setMenus(QList<AppMenuModel *> newMenus)
{
	m_menus = newMenus;
	emit menusChanged(newMenus);
}

/*! Adds a menu. */
void AppMenuBar::addMenu(AppMenuModel *menu)
{
	m_menus.append(menu);
	emit menusChanged(m_menus);
}

/*! Removes a menu. */
void AppMenuBar::removeMenu(AppMenuModel *menu)
{
	m_menus.removeAll(menu);
	emit menusChanged(m_menus);
}

AppMenuItem *AppMenuBar::openAction(void)
{
	return &m_openAction;
}

AppMenuItem *AppMenuBar::printAction(void)
{
	return &m_printAction;
}
