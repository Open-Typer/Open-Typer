/*
 * ExerciseTimer.cpp
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

#include <QTime>
#include "ExerciseTimer.h"

/*! Returns formatted elapsed time (m:s or h:m:s). */
QString ExerciseTimer::formattedTime(void)
{
	QTime time(0, 0);
	int msecs = m_timer.elapsed();
	QTime newTime = time.addMSecs(msecs);
	if(msecs > 3600000)
		return newTime.toString("hh:mm:ss");
	else
		return newTime.toString("mm:ss");
}

/*! Returns formatted remaining time (m:s or h:m:s). */
QString ExerciseTimer::formattedRemainingTime(int limitMSecs)
{
	QTime time(0, 0);
	int msecs = limitMSecs - m_timer.elapsed();
	QTime newTime = time.addMSecs(msecs);
	if(msecs > 3600000)
		return newTime.toString("hh:mm:ss");
	else
		return newTime.toString("mm:ss");
}

/*! Updates the elapsed property. */
void ExerciseTimer::update(void)
{
	m_elapsed = m_timer.elapsed();
	emit elapsedChanged();
}

/*! Starts the timer. */
void ExerciseTimer::start()
{
	m_elapsed = 0;
	m_running = true;
	m_timer.start();
	emit runningChanged();
}

/*! Stops the timer. */
void ExerciseTimer::stop()
{
	m_elapsed = m_timer.elapsed();
	m_running = false;
	emit elapsedChanged();
	emit runningChanged();
}
