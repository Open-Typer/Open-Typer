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
#include <QDesktopServices>
#include <QUrl>
#include "AppMenuBar.h"

std::shared_ptr<AppMenuBar> AppMenuBar::m_instance = std::make_shared<AppMenuBar>();

/*! Creates default menus. */
void AppMenuBar::createMenus(void)
{
	m_menus.clear();

	// File
	fileMenu.addItem(&openMenuAction);
	openMenu.addItem(&m_openExerciseAction);
	openMenu.addItem(&m_openPackAction);
	openMenuAction.setSubmenu(&openMenu);
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
	darkThemeAction.setChecked(themeEngine()->theme() == IThemeEngine::Theme::DarkTheme);
	connect(&darkThemeAction, &AppMenuItem::checkedChanged, [this](bool checked) {
		if(blockDarkThemeActionConnection)
			return;
		themeEngine()->setTheme(checked ? IThemeEngine::Theme::DarkTheme : IThemeEngine::Theme::LightTheme);
	});
	connect(themeEngine().get(), &IThemeEngine::themeChanged, [this]() {
		blockDarkThemeActionConnection = true;
		darkThemeAction.setChecked(themeEngine()->theme() == IThemeEngine::Theme::DarkTheme);
		blockDarkThemeActionConnection = false;
	});
	uiMenu.addItem(&darkThemeAction);
	uiMenuAction.setSubmenu(&uiMenu);
	viewMenu.addItem(&uiMenuAction);
	m_menus.append(&viewMenu);

	// Tools
	toolsMenu.addItem(&m_typingTestAction);
	m_menus.append(&toolsMenu);

	// Exercise
	exerciseMenu.addItem(&m_exerciseHistoryAction);
	exerciseMenu.addItem(&m_timedExAction);
	exerciseMenu.addItem(&m_errorWordsAction);
	exerciseMenu.addItem(&m_reverseTextAction);
	m_menus.append(&exerciseMenu);

	// Options
	optionsMenu.addItem(&m_preferencesAction);
	m_menus.append(&optionsMenu);

	// Help
	helpMenu.addItem(&docsAction);
	connect(&docsAction, &AppMenuItem::clicked, []() {
		QDesktopServices::openUrl(QUrl("https://open-typer.github.io/docs"));
	});
	helpMenu.addItem(&m_aboutProgramAction);
	helpMenu.addItem(&aboutQtAction);
	connect(&aboutQtAction, &AppMenuItem::clicked, qApp, &QApplication::aboutQt);
	m_menus.append(&helpMenu);

	updateMenus();
}

/*! Returns the static instance of AppMenuBar. */
std::shared_ptr<AppMenuBar> AppMenuBar::instance()
{
	return m_instance;
}

/*! Updates default menus. */
void AppMenuBar::updateMenus(void)
{
	// File
	fileMenu.setTitle(tr("&File"));
	openMenuAction.setText(tr("Open..."));
	m_openExerciseAction.setText(tr("Exercise", "To open \"Exercise\""));
	m_openPackAction.setText(tr("Lesson pack", "To open \"Lesson pack\""));
	m_printAction.setText(tr("Print"));
	quitAction.setText(tr("Quit"));

	// View
	viewMenu.setTitle(tr("&View"));
	uiMenuAction.setText(tr("User interface"));
	darkThemeAction.setText(tr("Dark theme"));

	// Tools
	toolsMenu.setTitle(tr("&Tools"));
	m_typingTestAction.setText(tr("Typing test"));

	// Exercise
	exerciseMenu.setTitle(tr("&Exercise"));
	m_exerciseHistoryAction.setText(tr("Exercise history"));
	m_timedExAction.setText(tr("Timed exercise"));
	m_errorWordsAction.setText(tr("Text from error words"));
	m_reverseTextAction.setText(tr("Reverse text"));

	// Options
	optionsMenu.setTitle(tr("&Options"));
	m_preferencesAction.setText(tr("Preferences"));

	// Help
	helpMenu.setTitle(tr("&Help"));
	docsAction.setText(tr("&Online documentation"));
	m_aboutProgramAction.setText(tr("About program..."));
	aboutQtAction.setText(tr("About Qt..."));

	emit menusChanged();
}

AppMenuItem *AppMenuBar::openExerciseAction(void)
{
	return &m_openExerciseAction;
}

AppMenuItem *AppMenuBar::openPackAction(void)
{
	return &m_openPackAction;
}

AppMenuItem *AppMenuBar::printAction(void)
{
	return &m_printAction;
}

AppMenuItem *AppMenuBar::typingTestAction(void)
{
	return &m_typingTestAction;
}

AppMenuItem *AppMenuBar::exerciseHistoryAction(void)
{
	return &m_exerciseHistoryAction;
}

AppMenuItem *AppMenuBar::timedExAction(void)
{
	return &m_timedExAction;
}

AppMenuItem *AppMenuBar::errorWordsAction(void)
{
	return &m_errorWordsAction;
}

AppMenuItem *AppMenuBar::reverseTextAction(void)
{
	return &m_reverseTextAction;
}

AppMenuItem *AppMenuBar::preferencesAction(void)
{
	return &m_preferencesAction;
}

AppMenuItem *AppMenuBar::aboutProgramAction(void)
{
	return &m_aboutProgramAction;
}
