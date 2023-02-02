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

#include <QObject>
#include "AppMenuModel.h"

/*!
 * \brief The AppMenuBar class provides a model for the application menu bar.
 * Note: This model shouldn't be created more than once because the application is intended to have only one menu bar.
 * Use globalMenuBar to access the model.
 */
class AppMenuBar : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QList<AppMenuModel *> menus READ menus WRITE setMenus NOTIFY menusChanged)
		Q_PROPERTY(AppMenuItem *openAction READ openAction NOTIFY openActionChanged)

	public:
		explicit AppMenuBar(QObject *parent = nullptr);

		QList<AppMenuModel *> menus(void);
		void setMenus(QList<AppMenuModel *> newMenus);
		Q_INVOKABLE void addMenu(AppMenuModel *menu);
		Q_INVOKABLE void removeMenu(AppMenuModel *menu);
		void updateMenus(void);

		AppMenuItem *openAction(void);

	private:
		void createMenus(void);
		QList<AppMenuModel *> m_menus;

		AppMenuModel fileMenu;
		AppMenuItem m_openAction;
		AppMenuItem fileSeparator;
		AppMenuItem quitAction;

	signals:
		void menusChanged(QList<AppMenuModel *> menus);
		void openActionChanged();
};

extern AppMenuBar CORE_LIB_EXPORT globalMenuBar;

#endif // APPMENUBAR_H
