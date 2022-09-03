/*
 * ConfigParser.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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

#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <QObject>
#include <QFile>
#include <QBuffer>
#include <QString>
#include "utils.h"

namespace publicPos {
	extern int currentLesson, currentSublesson, currentExercise;
}

// TODO: Add a link to pack file format documentation.
/*!
 * \brief The ConfigParser class provides functions for the pack file format.
 *
 * Example usage:
 * \code
 * ConfigParser parser;
 * parser.open("/path/to/pack.typer");
 * printf("There are %d lessons in the opened pack.\n",parser.lessonCount());
 * \endcode
 *
 * Closing the file isn't required most of the time, but there might be
 * some special situations, in which you'll have to close the file.\n
 * For example if you need to open the file again before destroying the ConfigParser object.
 * \code
 * parser.close();
 * \endcode
 */
class ConfigParser : public QObject
{
		Q_OBJECT
	public:
		explicit ConfigParser(QObject *parent = nullptr);
		static const int defaultRepeatLimit = 128;
		static const int defaultLineLength = 60;
		bool open(const QString fileName);
		void loadToBuffer(const QByteArray content);
		QByteArray data(void);
		bool bufferOpened(void);
		void close(void);
		QString fileName(void);
		int lessonCount(void);
		int sublessonCount(int lesson);
		int exerciseCount(int lesson, int sublesson);
		int exerciseLine(int lesson, int sublesson, int exercise);
		bool exerciseRepeatBool(int lesson, int sublesson, int exercise);
		QString exerciseRepeatType(int lesson, int sublesson, int exercise);
		int exerciseRepeatLimit(int lesson, int sublesson, int exercise);
		int exerciseLineLength(int lesson, int sublesson, int exercise);
		QString lessonDesc(int lesson);
		static QString parseDesc(QString desc);
		static QString sublessonName(int id);
		static QString lessonTr(int id);
		static QString sublessonTr(int id);
		static QString exerciseTr(int id);
		QString exerciseRawText(int lesson, int sublesson, int exercise);
		QString exerciseText(int lesson, int sublesson, int exercise);
		static QString initExercise(QString exercise, int lineLength);
		static QString initExercise(QString exercise, int lineLength, bool lineCountLimit, int currentLine);
		static QString initText(QString rawText);
		bool addExercise(int lesson, int sublesson, int exercise, bool repeat, QString repeatType, int repeatLimit, int lineLength, QString desc, QString rawText);

	private:
		QFile configFile;
		QBuffer configBuffer;
		QIODevice *currentDevice;
		bool reopen(QIODevice::OpenMode mode);
		int exerciseID(const QString line, const int part);
		QString lineOf(int lesson, int sublesson, int exercise);
		bool exerciseRepeatBool(const QString config);
		QString exerciseRepeatType(const QString config);
		QString exerciseRepeatConfig(const QString line);
		QString exerciseAttribute(const QString config, const int id);
		QString exerciseAttributes(const QString line);
		QString exerciseRawText(const QString line);
		static QString generateText(QString rawText, bool repeat, QString repeatType, int repeatLimit);
};

#endif // CONFIGPARSER_H
