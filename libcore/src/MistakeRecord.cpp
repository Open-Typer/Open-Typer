/*
 * MistakeRecord.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#include "MistakeRecord.h"

/*! Sets mistake position. */
void MistakeRecord::setPosition(int pos)
{
	m_position = pos;
	emit positionChanged(pos);
}

/*! Returns mistake position. */
int MistakeRecord::position(void)
{
	return m_position;
}

/*! Sets mistake type. */
void MistakeRecord::setType(MistakeRecord::Type type)
{
	m_type = type;
	emit typeChanged(type);
}

/*! Returns mistake type. */
MistakeRecord::Type MistakeRecord::type(void)
{
	return m_type;
}

/*! Sets previous text. */
void MistakeRecord::setPreviousText(QString text)
{
	m_previousText = text;
	emit previousTextChanged(text);
}

/*! Returns previous text. */
QString MistakeRecord::previousText(void)
{
	return m_previousText;
}
