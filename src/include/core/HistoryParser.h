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

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include "core/FileUtils.h"

/*! \brief The HistoryParser class provides functions for exercise history and statistics. */
class HistoryParser : public QObject
{
		Q_OBJECT
	public:
		static int historySize(QString pack, int lesson, int sublesson, int exercise);
		static QStringList historyEntry(QString pack, int lesson, int sublesson, int exercise, int entry);
		static void addHistoryEntry(QString pack, int lesson, int sublesson, int exercise, QList<QVariant> entry);

	private:
		static QJsonDocument historyDocument(void);
		static QJsonValue historyPackValue(QString pack);
};

#endif // HISTORYPARSER_H
