/*
 * ExerciseTimer.h
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

#ifndef EXERCISETIMER_H
#define EXERCISETIMER_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QElapsedTimer>

/*! \brief The ExerciseTimer class provides a simple elapsed timer for QML. */
class CORE_LIB_EXPORT ExerciseTimer : public QObject
{
		Q_OBJECT
		Q_PROPERTY(int elapsed MEMBER m_elapsed NOTIFY elapsedChanged)
		Q_PROPERTY(bool running MEMBER m_running NOTIFY runningChanged)
	public:
		Q_INVOKABLE QString formattedTime(void);
		Q_INVOKABLE QString formattedRemainingTime(int limitMSecs);

	private:
		QElapsedTimer m_timer;
		bool m_running;
		int m_elapsed;

	public slots:
		void start(void);
		void stop(void);

	signals:
		void runningChanged(void);
		void elapsedChanged(void);
};

#endif // EXERCISETIMER_H
