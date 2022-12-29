/*
 * HistoryParser.h
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

/*! \brief The HistoryParser class provides functions for exercise history and statistics. */
class CORE_LIB_EXPORT HistoryParser : public QObject
{
		Q_OBJECT
	public:
		Q_INVOKABLE static int historySize(QString pack, int lesson, int sublesson, int exercise);
		Q_INVOKABLE static QStringList historyEntry(QString pack, int lesson, int sublesson, int exercise, int entry);
		Q_INVOKABLE static void addHistoryEntry(QString pack, int lesson, int sublesson, int exercise, QList<QVariant> entry);

	private:
		static QJsonDocument historyDocument(void);
		static QJsonValue historyPackValue(QString pack);
};

#endif // HISTORYPARSER_H
