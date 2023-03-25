/*
 * AppMenuModel.h
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

#ifndef APPMENUMODEL_H
#define APPMENUMODEL_H

#include <QObject>
#include <QQmlListProperty>
#include "AppMenuItem.h"

/*! \brief The AppMenuModel class provides a menu model for the application menu bar. */
class Q_DECL_EXPORT AppMenuModel : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
		Q_PROPERTY(QQmlListProperty<AppMenuItem> items READ items NOTIFY itemsChanged)

	public:
		explicit AppMenuModel(QObject *parent = nullptr);

		QString title(void);
		void setTitle(QString newTitle);

		QQmlListProperty<AppMenuItem> items(void);
		QList<AppMenuItem *> getItems(void);
		void setItems(QList<AppMenuItem *> newItems);
		void addItem(AppMenuItem *item);
		void removeItem(AppMenuItem *item);

	private:
		QString m_title;
		QList<AppMenuItem *> m_items;

	signals:
		void titleChanged(QString title);
		void itemsChanged(QList<AppMenuItem *> items);
};

#endif // APPMENUMODEL_H
