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
#include "ThemeEngine.h"

AppMenuBar globalMenuBar;

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

	// View
	darkThemeAction.setCheckable(true);
	darkThemeAction.setChecked(globalThemeEngine.theme() == ThemeEngine::Theme::DarkTheme);
	connect(&darkThemeAction, &AppMenuItem::checkedChanged, [this](bool checked) {
		if(blockDarkThemeActionConnection)
			return;
		globalThemeEngine.setTheme(checked ? ThemeEngine::Theme::DarkTheme : ThemeEngine::Theme::LightTheme);
	});
	connect(&globalThemeEngine, &ThemeEngine::themeChanged, [this]() {
		blockDarkThemeActionConnection = true;
		darkThemeAction.setChecked(globalThemeEngine.theme() == ThemeEngine::Theme::DarkTheme);
		blockDarkThemeActionConnection = false;
	});
	uiMenu.addItem(&darkThemeAction);
	uiMenuAction.setSubmenu(&uiMenu);
	viewMenu.addItem(&uiMenuAction);
	m_menus.append(&viewMenu);

	// Tools
	toolsMenu.addItem(&m_typingTestAction);
	m_menus.append(&toolsMenu);

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

	// View
	viewMenu.setTitle(tr("&View"));
	uiMenuAction.setText(tr("User interface"));
	darkThemeAction.setText(tr("Dark theme"));

	// Tools
	toolsMenu.setTitle(tr("&Tools"));
	m_typingTestAction.setText(tr("Typing test"));

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

AppMenuItem *AppMenuBar::typingTestAction(void)
{
	return &m_typingTestAction;
}
