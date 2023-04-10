/*
 * IConfigParser.h
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

#ifndef ICONFIGPARSER_H
#define ICONFIGPARSER_H

#include "global/modularity/ioc.h"

class IConfigParser : MODULE_EXPORT_INTERFACE
{
	public:
		virtual ~IConfigParser() { }

		virtual bool open(const QString fileName) = 0;
		virtual void loadToBuffer(const QByteArray content) = 0;
		virtual QByteArray data(void) = 0;
		virtual bool bufferOpened(void) = 0;
		virtual void close(void) = 0;
		virtual QString fileName(void) = 0;
		virtual int lessonCount(void) = 0;
		virtual int sublessonCount(int lesson) = 0;
		virtual int exerciseCount(int lesson, int sublesson) = 0;
		virtual int exerciseLine(int lesson, int sublesson, int exercise) = 0;
		virtual bool exerciseRepeatBool(int lesson, int sublesson, int exercise) = 0;
		virtual QString exerciseRepeatType(int lesson, int sublesson, int exercise) = 0;
		virtual int exerciseRepeatLimit(int lesson, int sublesson, int exercise) = 0;
		virtual int exerciseLineLength(int lesson, int sublesson, int exercise) = 0;
		virtual QString lessonDesc(int lesson) = 0;
		virtual QString parseDesc(QString desc) = 0;
		virtual QString sublessonName(int id) = 0;
		virtual QString lessonTr(int id) = 0;
		virtual QString sublessonTr(int id) = 0;
		virtual QString exerciseTr(int id) = 0;
		virtual QString exerciseRawText(int lesson, int sublesson, int exercise) = 0;
		virtual QString exerciseText(int lesson, int sublesson, int exercise) = 0;
		virtual QString initExercise(QString exercise, int lineLength) = 0;
		virtual QString initExercise(QString exercise, int lineLength, bool lineCountLimit, int currentLine) = 0;
		virtual QString initText(QString rawText) = 0;
		virtual bool addExercise(int lesson, int sublesson, int exercise, bool repeat, QString repeatType, int repeatLimit, int lineLength, QString desc, QString rawText) = 0;
		virtual int defaultRepeatLimit(void) = 0;
		virtual int defaultLineLength(void) = 0;
		virtual void setCurrentLesson(int value) = 0;
		virtual int currentLesson(void) = 0;
		virtual void setCurrentSublesson(int value) = 0;
		virtual int currentSublesson(void) = 0;
		virtual void setCurrentExercise(int value) = 0;
		virtual int currentExercise(void) = 0;
};

#endif // ICONFIGPARSER_H
