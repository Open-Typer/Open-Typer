/*
 * AddonButton.cpp
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

#include "AddonButton.h"

/*! Whether the button is visible. */
bool AddonButton::visible() const
{
	return m_visible;
}

void AddonButton::setVisible(bool newVisible)
{
	m_visible = newVisible;
	emit visibleChanged();
}

/*! The button text. */
QString AddonButton::text()
{
	return m_text;
}

void AddonButton::setText(QString newText)
{
	m_text = newText;
	emit textChanged();
}

/*! The tool tip text. */
QString AddonButton::toolTip()
{
	return m_toolTip;
}

void AddonButton::setToolTip(QString newToolTip)
{
	m_toolTip = newToolTip;
	emit toolTipChanged();
}

/*! The icon name. */
QString AddonButton::iconName()
{
	return m_iconName;
}

void AddonButton::setIconName(QString newIconName)
{
	m_iconName = newIconName;
	emit iconNameChanged();
}

/*! The icon source (URL). */
QString AddonButton::iconSource()
{
	return m_iconSource;
}

void AddonButton::setIconSource(QString newIconSource)
{
	m_iconSource = newIconSource;
	emit iconSourceChanged();
}
