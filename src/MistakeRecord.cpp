/*
 * MistakeRecord.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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

#include <QQmlEngine>
#include "MistakeRecord.h"

/*! Mistake position. */
int MistakeRecord::position(void)
{
	return m_position;
}

void MistakeRecord::setPosition(int pos)
{
	m_position = pos;
}

/*! Mistake type. */
MistakeRecord::Type MistakeRecord::type(void)
{
	return m_type;
}

void MistakeRecord::setType(MistakeRecord::Type type)
{
	m_type = type;
}

/*! Previous text. */
QString MistakeRecord::previousText(void)
{
	return m_previousText;
}

void MistakeRecord::setPreviousText(QString text)
{
	m_previousText = text;
	setPreviousVariant(text);
}

/*! Previous value (useful if you need to support non-QString values). */
QVariant MistakeRecord::previousVariant(void)
{
	return m_previousVariant;
}

void MistakeRecord::setPreviousVariant(QVariant value)
{
	m_previousVariant = value;
	if(strcmp(value.typeName(), "QString") == 0)
		m_previousText = value.toString();
}

/*! Previous position. */
int MistakeRecord::previousPosition(void)
{
	return m_previousPosition;
}

void MistakeRecord::setPreviousPosition(int pos)
{
	m_previousPosition = pos;
}

/*! Whether the mistake is enabled. */
bool MistakeRecord::enabled(void)
{
	return m_enabled;
}

/*!
 * Returns true if this mistake is enabled.
 * \deprecated Use enabled() instead.
 */
bool MistakeRecord::isEnabled(void)
{
	return m_enabled;
}

void MistakeRecord::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

/*! Whether the mistake is merged. */
bool MistakeRecord::merged(void)
{
	return m_merged;
}

/*!
 * Returns true if this mistake is merged.
 * \deprecated Use merged() instead.
 */
bool MistakeRecord::isMerged(void)
{
	return m_merged;
}

void MistakeRecord::setMerged(bool merged)
{
	m_merged = merged;
}
