/*
 * Class.cpp
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

#include "Class.h"

/*! Constructs Class. */
Class::Class(QObject *parent) :
	QObject(parent)
{
	// Required for all properties!
	connect(this, &Class::nameChanged, this, &Class::modified);
	connect(this, &Class::descriptionChanged, this, &Class::modified);
	connect(this, &Class::targetHitsPerMinuteChanged, this, &Class::modified);
	connect(this, &Class::gradeConfigChanged, this, &Class::modified);
}

/*! The name of the class. */
const QString &Class::name(void) const
{
	return m_name;
}

void Class::setName(const QString &newName)
{
	if(m_name == newName)
		return;
	m_name = newName;
	emit nameChanged();
}

/*! The description of the class. */
const QString &Class::description(void) const
{
	return m_description;
}

void Class::setDescription(const QString &newDescription)
{
	if(m_description == newDescription)
		return;
	m_description = newDescription;
	emit descriptionChanged();
}

/*! Number of hits per minute for the best grade. */
int Class::targetHitsPerMinute(void) const
{
	return m_targetHitsPerMinute;
}

void Class::setTargetHitsPerMinute(int newTargetHitsPerMinute)
{
	if(m_targetHitsPerMinute == newTargetHitsPerMinute)
		return;
	m_targetHitsPerMinute = newTargetHitsPerMinute;
	emit targetHitsPerMinuteChanged();
}

/*! Target number of hits per minute for each month. */
const QMap<int, int> &Class::gradeConfig(void) const
{
	return m_gradeConfig;
}

void Class::setGradeConfig(const QMap<int, int> &newGradeConfig)
{
	if(m_gradeConfig == newGradeConfig)
		return;
	m_gradeConfig = newGradeConfig;
	emit gradeConfigChanged();
}

/*! Returns target hits pre minute for the given month. */
int Class::targetHitsForMonth(int month)
{
	return m_gradeConfig[month];
}

/*! Sets target hits per minute for the given month. */
void Class::setTargetHitsForMonth(int month, int targetHits)
{
	m_gradeConfig[month] = targetHits;
	emit gradeConfigChanged();
}
