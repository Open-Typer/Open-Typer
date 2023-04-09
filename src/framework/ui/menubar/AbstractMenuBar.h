/*
 * AbstractMenuBar.h
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

#ifndef ABSTRACTMENUBAR_H
#define ABSTRACTMENUBAR_H

#include <QObject>
#include <QQmlListProperty>
#include "AppMenuModel.h"
#include "translations/ILanguageManager.h"

/*! \brief The AbstractMenuBar class provides a model for a menu bar. */
class Q_DECL_EXPORT AbstractMenuBar : public QObject
{
		Q_OBJECT
		INJECT(ILanguageManager, languageManager)
		Q_PROPERTY(QQmlListProperty<AppMenuModel> menus READ menus NOTIFY menusChanged)

	public:
		AbstractMenuBar(QObject *parent = nullptr);

		void init();
		QQmlListProperty<AppMenuModel> menus(void);
		QList<AppMenuModel *> getMenus(void);
		void setMenus(QList<AppMenuModel *> newMenus);
		Q_INVOKABLE void addMenu(AppMenuModel *menu);
		Q_INVOKABLE void removeMenu(AppMenuModel *menu);
		Q_INVOKABLE virtual void updateMenus(void);

	protected:
		Q_INVOKABLE virtual void createMenus(void);
		QList<AppMenuModel *> m_menus;

	signals:
		void menusChanged();
};

#endif // ABSTRACTMENUBAR_H
