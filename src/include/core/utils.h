/*
 * utils.h
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

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QString>
#include <QStandardPaths>
#include <QKeyEvent>
#include <QDir>

/*! \brief The stringUtils class contains functions related to strings. */
class stringUtils
{
	public:
		static int wordCount(QString str);
		static QString word(QString str, int id);
		static QString wordAt(QString str, int index);
		static QList<QVariant> longestCommonSubsequence(QList<QVariant> source, QList<QVariant> target);
		static QString longestCommonSubsequence(QString source, QString target);
		static QList<QVariantMap> compareLists(QList<QVariant> source, QList<QVariant> target, QVector<QPair<QString,int>> *recordedCharacters = nullptr, int *hits = nullptr, int *inputPos = nullptr);
		static QList<QVariantMap> compareStrings(QString source, QString target, QVector<QPair<QString,int>> *recordedCharacters = nullptr, int *hits = nullptr, int *inputPos = nullptr);
		static QList<QVariantMap> findMistakes(QString exerciseText, QString input, QVector<QPair<QString,int>> recordedCharacters, int *totalHits = nullptr, QStringList *errorWords = nullptr);
};

/*! \brief The fileUtils class contains functions for file or directory paths. */
class fileUtils
{
	public:
		static QString configLocation(void);
		static QString mainSettingsLocation(void);
};

/*! \brief The keyboardUtils class provides functions related to keyboard and input method. */
class keyboardUtils
{
	public:
		static bool isSpecialKey(QKeyEvent *event);
		static bool isDeadKey(int key);
};

#endif // UTILS_H
