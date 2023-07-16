/*
 * ConfigParser.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2023 - adazem009
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
#include "IConfigParser.h"
#include "global/IStringUtils.h"

namespace publicPos {
	Q_DECL_EXPORT Q_NAMESPACE;
	extern int Q_DECL_EXPORT currentLesson, currentSublesson, currentExercise;
}

// TODO: Add a link to pack file format documentation.
/*!
 * \brief The ConfigParser class provides functions for the pack file format.
 *
 * Example usage:
 * \code
 * class MyClass {
 *	INJECT(IConfigParser, configParser)
 *	...
 *
 * configParser()->open("/path/to/pack.typer") override;
 * printf("There are %d lessons in the opened pack.\n", configParser()->lessonCount()) override;
 * \endcode
 */
class Q_DECL_EXPORT ConfigParser : public QObject, public IConfigParser
{
		Q_OBJECT
		INJECT(IStringUtils, stringUtils)
	public:
		explicit ConfigParser(QObject *parent = nullptr);
		static std::shared_ptr<ConfigParser> instance();
		std::shared_ptr<IConfigParser> createInstance() const override;
		Q_INVOKABLE bool open(const QString fileName) override;
		Q_INVOKABLE void loadToBuffer(const QByteArray content) override;
		Q_INVOKABLE QByteArray data(void) override;
		Q_INVOKABLE bool bufferOpened(void) override;
		Q_INVOKABLE void close(void) override;
		QString fileName(void) override;
		Q_INVOKABLE int lessonCount(void) override;
		Q_INVOKABLE int sublessonCount(int lesson) override;
		Q_INVOKABLE int exerciseCount(int lesson, int sublesson) override;
		Q_INVOKABLE int exerciseLine(int lesson, int sublesson, int exercise) override;
		Q_INVOKABLE bool exerciseRepeatBool(int lesson, int sublesson, int exercise) override;
		Q_INVOKABLE QString exerciseRepeatType(int lesson, int sublesson, int exercise) override;
		Q_INVOKABLE int exerciseRepeatLimit(int lesson, int sublesson, int exercise) override;
		Q_INVOKABLE int exerciseLineLength(int lesson, int sublesson, int exercise) override;
		Q_INVOKABLE QString lessonDesc(int lesson) override;
		Q_INVOKABLE QString parseDesc(QString desc) override;
		Q_INVOKABLE QString sublessonName(int id) override;
		Q_INVOKABLE QString lessonTr(int id) override;
		Q_INVOKABLE QString sublessonTr(int id) override;
		Q_INVOKABLE QString exerciseTr(int id) override;
		Q_INVOKABLE QString exerciseRawText(int lesson, int sublesson, int exercise) override;
		Q_INVOKABLE QString exerciseText(int lesson, int sublesson, int exercise) override;
		Q_INVOKABLE QString initExercise(QString exercise, int lineLength) override;
		Q_INVOKABLE QString initExercise(QString exercise, int lineLength, bool lineCountLimit, int currentLine) override;
		Q_INVOKABLE QString initText(QString rawText) override;
		Q_INVOKABLE bool addExercise(int lesson, int sublesson, int exercise, bool repeat, QString repeatType, int repeatLimit, int lineLength, QString desc, QString rawText) override;
		Q_INVOKABLE int defaultRepeatLimit(void) override;
		Q_INVOKABLE int defaultLineLength(void) override;
		Q_INVOKABLE void setCurrentLesson(int value) override;
		Q_INVOKABLE int currentLesson(void) override;
		Q_INVOKABLE void setCurrentSublesson(int value) override;
		Q_INVOKABLE int currentSublesson(void) override;
		Q_INVOKABLE void setCurrentExercise(int value) override;
		Q_INVOKABLE int currentExercise(void) override;

	private:
		static std::shared_ptr<ConfigParser> m_instance;
		QFile configFile;
		QBuffer configBuffer;
		QIODevice *currentDevice;
		static const int m_defaultRepeatLimit = 128;
		static const int m_defaultLineLength = 100;
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
