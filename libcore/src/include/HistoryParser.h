/*
 * HistoryParser.h
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

#ifndef HISTORYPARSER_H
#define HISTORYPARSER_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include "FileUtils.h"
#include "HistoryEntry.h"

/*! \brief The HistoryParser class provides functions for exercise history and statistics. */
class CORE_LIB_EXPORT HistoryParser : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString lessonPack READ lessonPack WRITE setLessonPack NOTIFY lessonPackChanged)
		Q_PROPERTY(int lesson READ lesson WRITE setLesson NOTIFY lessonChanged)
		Q_PROPERTY(int sublesson READ sublesson WRITE setSublesson NOTIFY sublessonChanged)
		Q_PROPERTY(int exercise READ exercise WRITE setExercise NOTIFY exerciseChanged)
		Q_PROPERTY(int count READ count NOTIFY countChanged)
	public:
		QString lessonPack(void);
		void setLessonPack(QString value);
		int lesson(void);
		void setLesson(int value);
		int sublesson(void);
		void setSublesson(int value);
		int exercise(void);
		void setExercise(int value);
		int count(void);
		Q_INVOKABLE HistoryEntry at(int index);
		Q_INVOKABLE void append(HistoryEntry entry);
		Q_INVOKABLE void append(int grossHitsPerMinute, int mistakes, int timeSecs);

	private:
		static QJsonDocument historyDocument(void);
		static QJsonValue historyPackValue(QString pack);
		QString m_lessonPack;
		int m_lesson;
		int m_sublesson;
		int m_exercise;

	signals:
		void lessonPackChanged(QString value);
		void lessonChanged(int value);
		void sublessonChanged(int value);
		void exerciseChanged(int value);
		void countChanged(int value);
};

#endif // HISTORYPARSER_H
