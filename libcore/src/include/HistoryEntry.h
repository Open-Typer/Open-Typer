/*
 * HistoryEntry.h
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

#ifndef HISTORYENTRY_H
#define HISTORYENTRY_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>

/*! \brief The HistoryEntry class is an entry in the exercise history. */
class CORE_LIB_EXPORT HistoryEntry
{
		Q_GADGET
		Q_PROPERTY(int grossHitsPerMinute READ grossHitsPerMinute WRITE setGrossHitsPerMinute)
		Q_PROPERTY(int mistakes READ mistakes WRITE setMistakes)
		Q_PROPERTY(int timeSecs READ timeSecs WRITE setTimeSecs)
	public:
		HistoryEntry();
		HistoryEntry(int grossHitsPerMinute, int mistakes, int timeSecs);
		int grossHitsPerMinute(void);
		void setGrossHitsPerMinute(int value);
		int mistakes(void);
		void setMistakes(int value);
		int timeSecs(void);
		void setTimeSecs(int value);

	private:
		int m_grossHitsPerMinute = 0;
		int m_mistakes = 0;
		int m_timeSecs = 0;
};

#endif // HISTORYENTRY_H
