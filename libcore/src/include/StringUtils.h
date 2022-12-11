/*
 * StringUtils.h
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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QString>
#include <QVector>
#include "FileUtils.h"
#include "Settings.h"

/*! \brief The StringUtils class contains functions related to strings. */
class CORE_LIB_EXPORT StringUtils : public QObject
{
		Q_OBJECT
	public:
		Q_INVOKABLE static int wordCount(QString str);
		Q_INVOKABLE static QString word(QString str, int id);
		Q_INVOKABLE static QString wordAt(QString str, int index);
		Q_INVOKABLE static QList<QVariant> longestCommonSubsequence(QList<QVariant> source, QList<QVariant> target);
		Q_INVOKABLE static QString longestCommonSubsequence(QString source, QString target);
		Q_INVOKABLE static QList<QVariantMap> compareLists(QList<QVariant> source, QList<QVariant> target, QVector<QPair<QString, int>> *recordedCharacters = nullptr, int *hits = nullptr, int *inputPos = nullptr);
		Q_INVOKABLE static QList<QVariantMap> compareStrings(QString source, QString target, QVector<QPair<QString, int>> *recordedCharacters = nullptr, int *hits = nullptr, int *inputPos = nullptr);
		Q_INVOKABLE static QList<QVariantMap> findMistakes(QString exerciseText, QString input, QVector<QPair<QString, int>> recordedCharacters, int *totalHits = nullptr, QStringList *errorWords = nullptr);
		Q_INVOKABLE static QList<QVariantMap> validateExercise(QString exerciseText, QString inputText, QVector<QPair<QString, int>> recordedCharacters, int *totalHits, int *mistakeCount, QStringList *errorWords = nullptr, bool timed = false, int timeSecs = 0);
		Q_INVOKABLE static QString addMistakes(QString exerciseText, QList<QVariantMap> *recordedMistakes);

	private:
		static int lcsLen(QList<QVariant> source, QList<QVariant> target);
		static int lcsLen(QList<QVariant> source, QList<QVariant> target, QMap<int, QMap<int, int>> *lcsTable);
		static QStringList splitWordsByPunct(QStringList source);
		static QMap<int, QVariantMap> generateDiffList(QStringList *sourceWords, QStringList *targetWords, QList<int> *mergeList = nullptr);
};

#endif // STRINGUTILS_H
