/*
 * HistoryParser.cpp
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

#include "core/HistoryParser.h"

/*! Returns number of entries in the exercise history. */
int HistoryParser::historySize(QString pack, int lesson, int sublesson, int exercise)
{
	QJsonValue packValue = historyPackValue(pack);
	QJsonArray exerciseArray = packValue.toObject().value(QString::number(lesson)).toObject().value(QString::number(sublesson)).toObject().value(QString::number(exercise)).toArray();
	return exerciseArray.count();
}

/*! Returns single row of the exercise history. */
QStringList HistoryParser::historyEntry(QString pack, int lesson, int sublesson, int exercise, int entry)
{
	QJsonValue packValue = historyPackValue(pack);
	QStringList out;
	out.clear();
	QJsonArray exerciseArray = packValue.toObject().value(QString::number(lesson)).toObject().value(QString::number(sublesson)).toObject().value(QString::number(exercise)).toArray();
	QJsonObject rowObject = exerciseArray[entry].toObject();
	out += QString::number(rowObject.value("speed").toInt());
	out += QString::number(rowObject.value("mistakes").toInt());
	out += QString::number(rowObject.value("time").toInt());
	return out;
}

/*! Adds a new entry to the exercise history. */
void HistoryParser::addHistoryEntry(QString pack, int lesson, int sublesson, int exercise, QList<QVariant> entry)
{
	// Get objects, values and entry array
	QJsonValue packValue = historyPackValue(pack);
	QJsonObject lessonObject = packValue.toObject().value(QString::number(lesson)).toObject();
	QJsonObject sublessonObject = lessonObject.value(QString::number(sublesson)).toObject();
	QJsonValue exerciseValue = sublessonObject.value(QString::number(exercise));
	QJsonArray exerciseArray = exerciseValue.toArray();
	QJsonObject entryObject;
	// Write entry
	entryObject.insert("speed", entry[0].toInt());
	entryObject.insert("mistakes", entry[1].toInt());
	entryObject.insert("time", entry[2].toInt());
	// Save objects
	exerciseArray.append(entryObject);
	exerciseValue = exerciseArray;
	sublessonObject.insert(QString::number(exercise), exerciseValue);
	lessonObject.insert(QString::number(sublesson), sublessonObject);
	QJsonObject packObject = packValue.toObject();
	packObject.insert(QString::number(lesson), lessonObject);
	QJsonObject docObject = historyDocument().object();
	docObject.insert(pack, packObject);
	// Save JSON document
	QJsonDocument document(docObject);
	QFile historyFile(FileUtils::configLocation() + "/history.json");
	if(historyFile.open(QIODevice::WriteOnly | QIODevice::Text))
		historyFile.write(document.toJson(QJsonDocument::Indented));
}

/*! Returns JSON document of the history JSON file. */
QJsonDocument HistoryParser::historyDocument(void)
{
	QFile historyFile(FileUtils::configLocation() + "/history.json");
	if(historyFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return QJsonDocument::fromJson(historyFile.readAll());
	else if(historyFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		historyFile.write("{ }\n");
		historyFile.close();
		return historyDocument();
	}
	else
		return QJsonDocument();
}

/*! Returns JSON value of the pack history. */
QJsonValue HistoryParser::historyPackValue(QString pack)
{
	return historyDocument().object().value(pack);
}
