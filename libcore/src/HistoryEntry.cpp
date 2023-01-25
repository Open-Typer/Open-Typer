/*
 * HistoryEntry.cpp
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

#include "HistoryEntry.h"

/*! Constructs HistoryEntry. */
HistoryEntry::HistoryEntry() { }

/*! Constructs HistoryEntry with values. */
HistoryEntry::HistoryEntry(int grossHitsPerMinute, int mistakes, int timeSecs)
{
	setGrossHitsPerMinute(grossHitsPerMinute);
	setMistakes(mistakes);
	setTimeSecs(timeSecs);
}

/*! Number of gross (total) hits per minute. */
int HistoryEntry::grossHitsPerMinute(void)
{
	return m_grossHitsPerMinute;
}

void HistoryEntry::setGrossHitsPerMinute(int value)
{
	m_grossHitsPerMinute = value;
}

/*! Number of mistakes. */
int HistoryEntry::mistakes(void)
{
	return m_mistakes;
}

void HistoryEntry::setMistakes(int value)
{
	m_mistakes = value;
}

/*! Time in seconds. */
int HistoryEntry::timeSecs(void)
{
	return m_timeSecs;
}

void HistoryEntry::setTimeSecs(int value)
{
	m_timeSecs = value;
}
