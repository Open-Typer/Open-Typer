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

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QFile>
#include <QBuffer>
#include <QString>
#include "StringUtils.h"

namespace publicPos {
	CORE_LIB_EXPORT Q_NAMESPACE;
	extern int CORE_LIB_EXPORT currentLesson, currentSublesson, currentExercise;
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
class CORE_LIB_EXPORT ConfigParser : public QObject
{
		Q_OBJECT
	public:
		explicit ConfigParser(QObject *parent = nullptr);
		static const int defaultRepeatLimit = 128;
		static const int defaultLineLength = 60;
		Q_INVOKABLE bool open(const QString fileName);
		Q_INVOKABLE void loadToBuffer(const QByteArray content);
		Q_INVOKABLE QByteArray data(void);
		Q_INVOKABLE bool bufferOpened(void);
		Q_INVOKABLE void close(void);
		QString fileName(void);
		Q_INVOKABLE int lessonCount(void);
		Q_INVOKABLE int sublessonCount(int lesson);
		Q_INVOKABLE int exerciseCount(int lesson, int sublesson);
		Q_INVOKABLE int exerciseLine(int lesson, int sublesson, int exercise);
		Q_INVOKABLE bool exerciseRepeatBool(int lesson, int sublesson, int exercise);
		Q_INVOKABLE QString exerciseRepeatType(int lesson, int sublesson, int exercise);
		Q_INVOKABLE int exerciseRepeatLimit(int lesson, int sublesson, int exercise);
		Q_INVOKABLE int exerciseLineLength(int lesson, int sublesson, int exercise);
		Q_INVOKABLE QString lessonDesc(int lesson);
		Q_INVOKABLE static QString parseDesc(QString desc);
		Q_INVOKABLE static QString sublessonName(int id);
		Q_INVOKABLE static QString lessonTr(int id);
		Q_INVOKABLE static QString sublessonTr(int id);
		Q_INVOKABLE static QString exerciseTr(int id);
		Q_INVOKABLE QString exerciseRawText(int lesson, int sublesson, int exercise);
		Q_INVOKABLE QString exerciseText(int lesson, int sublesson, int exercise);
		Q_INVOKABLE static QString initExercise(QString exercise, int lineLength);
		Q_INVOKABLE static QString initExercise(QString exercise, int lineLength, bool lineCountLimit, int currentLine);
		Q_INVOKABLE static QString initText(QString rawText);
		Q_INVOKABLE bool addExercise(int lesson, int sublesson, int exercise, bool repeat, QString repeatType, int repeatLimit, int lineLength, QString desc, QString rawText);

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
