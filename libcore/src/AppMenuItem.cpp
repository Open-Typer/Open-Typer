/*
 * AppMenuItem.cpp
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

#include "AppMenuItem.h"
#include "AppMenuModel.h"

/*! Constructs AppMenuItem. */
AppMenuItem::AppMenuItem(QObject *parent) :
	QObject(parent) { }

/*! The text of the item. */
QString AppMenuItem::text(void)
{
	return m_text;
}

void AppMenuItem::setText(QString newText)
{
	m_text = newText;
	emit textChanged(newText);
}

/*! The submenu of the item. */
AppMenuModel *AppMenuItem::submenu(void)
{
	return m_submenu;
}

void AppMenuItem::setSubmenu(AppMenuModel *newSubmenu)
{
	m_submenu = newSubmenu;
	emit submenuChanged(newSubmenu);
}

/*! If true, the item doesn't display any text and is a menu separator. */
bool AppMenuItem::isSeparator(void)
{
	return m_isSeparator;
}

void AppMenuItem::setIsSeparator(bool newIsSeparator)
{
	m_isSeparator = newIsSeparator;
	emit isSeparatorChanged(newIsSeparator);
}
