/*
 * AppMenuModel.cpp
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

#include "AppMenuModel.h"

/*! Constructs AppMenuModel. */
AppMenuModel::AppMenuModel(QObject *parent) :
	QObject(parent) { }

/*! The title of the menu. */
QString AppMenuModel::title(void)
{
	return m_title;
}

void AppMenuModel::setTitle(QString newTitle)
{
	m_title = newTitle;
	emit titleChanged(newTitle);
}

/*! The items in the menu. */
QList<AppMenuItem *> AppMenuModel::items(void)
{
	return m_items;
}

void AppMenuModel::setItems(QList<AppMenuItem *> newItems)
{
	m_items = newItems;
	emit itemsChanged(newItems);
}

/*! Adds an item to the menu. */
void AppMenuModel::addItem(AppMenuItem *item)
{
	m_items.append(item);
	emit itemsChanged(m_items);
}

/*! Removes an item from the menu. */
void AppMenuModel::removeItem(AppMenuItem *item)
{
	m_items.removeAll(item);
	emit itemsChanged(m_items);
}
