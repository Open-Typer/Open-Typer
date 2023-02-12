/*
 * SettingsCategory.cpp
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

#include "SettingsCategory.h"

/*! Constructs SettingsCategory. */
SettingsCategory::SettingsCategory(QObject *parent) :
	QObject(parent)
{
}

/*! Category name. */
QString SettingsCategory::name(void)
{
	return m_name;
}

void SettingsCategory::setName(QString newName)
{
	m_name = newName;
	emit nameChanged(newName);
}

/*! The icon name. */
QString SettingsCategory::iconName(void)
{
	return m_iconName;
}

void SettingsCategory::setIconName(QString newIconName)
{
	m_iconName = newIconName;
	emit iconNameChanged(newIconName);
}

/*! The icon source (URL). */
QString SettingsCategory::iconSource(void)
{
	return m_iconSource;
}

void SettingsCategory::setIconSource(QString newIconSource)
{
	m_iconSource = newIconSource;
	emit iconSourceChanged(newIconSource);
}

/*! The name of the QML file of the settings category. */
QString SettingsCategory::qmlFileName(void)
{
	return m_qmlFileName;
}

void SettingsCategory::setQmlFileName(QString newQmlFileName)
{
	m_qmlFileName = newQmlFileName;
	emit qmlFileNameChanged(newQmlFileName);
}

/*! Whether the settings category is visible. */
bool SettingsCategory::visible(void)
{
	return m_visible;
}

void SettingsCategory::setVisible(bool newVisible)
{
	if(m_visible == newVisible)
		return;
	m_visible = newVisible;
	emit visibleChanged(newVisible);
}
