/*
 * AppMenuBar.h
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

#ifndef APPMENUBAR_H
#define APPMENUBAR_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include "AbstractMenuBar.h"

/*!
 * \brief The AppMenuBar class provides a model for the application menu bar.
 * Note: This model shouldn't be created more than once because the application is intended to have only one menu bar.
 * Use globalMenuBar to access the model.
 */
class CORE_LIB_EXPORT AppMenuBar : public AbstractMenuBar
{
		Q_OBJECT
		Q_PROPERTY(AppMenuItem *openAction READ openAction NOTIFY openActionChanged)
		Q_PROPERTY(AppMenuItem *printAction READ printAction NOTIFY printActionChanged)
		Q_PROPERTY(AppMenuItem *typingTestAction READ typingTestAction NOTIFY typingTestActionChanged)
		Q_PROPERTY(AppMenuItem *exerciseHistoryAction READ exerciseHistoryAction NOTIFY exerciseHistoryActionChanged)
		Q_PROPERTY(AppMenuItem *timedExAction READ timedExAction NOTIFY timedExActionChanged)
		Q_PROPERTY(AppMenuItem *errorWordsAction READ errorWordsAction NOTIFY errorWordsActionChanged)
		Q_PROPERTY(AppMenuItem *reverseTextAction READ reverseTextAction NOTIFY reverseTextActionChanged)
		Q_PROPERTY(AppMenuItem *preferencesAction READ preferencesAction NOTIFY preferencesActionChanged)

	public:
		void updateMenus(void) override;

		AppMenuItem *openAction(void);
		AppMenuItem *printAction(void);
		AppMenuItem *typingTestAction(void);
		AppMenuItem *exerciseHistoryAction(void);
		AppMenuItem *timedExAction(void);
		AppMenuItem *errorWordsAction(void);
		AppMenuItem *reverseTextAction(void);
		AppMenuItem *preferencesAction(void);

	private:
		void createMenus(void) override;
		bool blockDarkThemeActionConnection = false;

		// File
		AppMenuModel fileMenu;
		AppMenuItem m_openAction;
		AppMenuItem fileSeparator1;
		AppMenuItem fileSeparator2;
		AppMenuItem m_printAction;
		AppMenuItem quitAction;

		// View
		AppMenuModel viewMenu;
		AppMenuItem uiMenuAction;
		AppMenuModel uiMenu;
		AppMenuItem darkThemeAction;

		// Tools
		AppMenuModel toolsMenu;
		AppMenuItem m_typingTestAction;

		// Exercise
		AppMenuModel exerciseMenu;
		AppMenuItem m_exerciseHistoryAction;
		AppMenuItem m_timedExAction;
		AppMenuItem m_errorWordsAction;
		AppMenuItem m_reverseTextAction;

		// Options
		AppMenuModel optionsMenu;
		AppMenuItem m_preferencesAction;

	signals:
		void openActionChanged();
		void printActionChanged();
		void typingTestActionChanged();
		void exerciseHistoryActionChanged();
		void timedExActionChanged();
		void errorWordsActionChanged();
		void reverseTextActionChanged();
		void preferencesActionChanged();
};

extern AppMenuBar CORE_LIB_EXPORT globalMenuBar;

#endif // APPMENUBAR_H
