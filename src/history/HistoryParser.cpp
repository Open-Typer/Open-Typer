/*
 * HistoryParser.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
 * Copyright (C) 2023 - Roker2
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

#include "HistoryParser.h"
#include "global/FileUtils.h"

const QString HistoryParser::historyFile = "/history.json";
const QString HistoryParser::speedProperty = "speed";
const QString HistoryParser::mistakesProperty = "mistakes";
const QString HistoryParser::timeProperty = "time";

/*! Current lesson pack raw name. \see BuiltInPacks */
QString HistoryParser::lessonPack(void)
{
	return m_lessonPack;
}

void HistoryParser::setLessonPack(QString value)
{
	m_lessonPack = value;
	emit lessonPackChanged(value);
	emit countChanged(count());
}

/*! Current lesson */
int HistoryParser::lesson(void)
{
	return m_lesson;
}

void HistoryParser::setLesson(int value)
{
	m_lesson = value;
	emit lessonChanged(value);
	emit countChanged(count());
}

/*! Current sublesson */
int HistoryParser::sublesson(void)
{
	return m_sublesson;
}

void HistoryParser::setSublesson(int value)
{
	m_sublesson = value;
	emit sublessonChanged(value);
	emit countChanged(count());
}

/*! Current exercise */
int HistoryParser::exercise(void)
{
	return m_exercise;
}

void HistoryParser::setExercise(int value)
{
	m_exercise = value;
	emit exerciseChanged(value);
	emit countChanged(count());
}

/*! Number of entries in the exercise history. */
int HistoryParser::count(void)
{
	QJsonValue packValue = historyPackValue(m_lessonPack);
	QJsonArray exerciseArray = packValue.toObject().value(QString::number(m_lesson)).toObject().value(QString::number(m_sublesson)).toObject().value(QString::number(m_exercise)).toArray();
	return exerciseArray.count();
}

/*! Returns an entry of the exercise history. */
HistoryEntry HistoryParser::at(int index)
{
	QJsonValue packValue = historyPackValue(m_lessonPack);
	HistoryEntry out;
	QJsonArray exerciseArray = packValue.toObject().value(QString::number(m_lesson)).toObject().value(QString::number(m_sublesson)).toObject().value(QString::number(m_exercise)).toArray();
	QJsonObject rowObject = exerciseArray[index].toObject();
	out.setGrossHitsPerMinute(rowObject.value(speedProperty).toInt());
	out.setMistakes(rowObject.value(mistakesProperty).toInt());
	out.setTimeSecs(rowObject.value(timeProperty).toInt());
	return out;
}

void HistoryParser::append(HistoryEntry entry)
{
	// Get objects, values and entry array
	QJsonValue packValue = historyPackValue(m_lessonPack);
	QJsonObject lessonObject = packValue.toObject().value(QString::number(m_lesson)).toObject();
	QJsonObject sublessonObject = lessonObject.value(QString::number(m_sublesson)).toObject();
	QJsonValue exerciseValue = sublessonObject.value(QString::number(m_exercise));
	QJsonArray exerciseArray = exerciseValue.toArray();
	QJsonObject entryObject;
	// Write entry
	entryObject.insert(speedProperty, entry.grossHitsPerMinute());
	entryObject.insert(mistakesProperty, entry.mistakes());
	entryObject.insert(timeProperty, entry.timeSecs());
	// Save objects
	exerciseArray.append(entryObject);
	exerciseValue = exerciseArray;
	sublessonObject.insert(QString::number(m_exercise), exerciseValue);
	lessonObject.insert(QString::number(m_sublesson), sublessonObject);
	QJsonObject packObject = packValue.toObject();
	packObject.insert(QString::number(m_lesson), lessonObject);
	QJsonObject docObject = historyDocument().object();
	docObject.insert(m_lessonPack, packObject);
	// Save JSON document
	QJsonDocument document(docObject);
	QString pathToHistoryFile = FileUtils::configLocation() + historyFile;
	QFile historyFile(pathToHistoryFile);
	if(historyFile.open(QIODevice::WriteOnly | QIODevice::Text))
		historyFile.write(document.toJson(QJsonDocument::Indented));
	emit countChanged(count());
}

/*! Adds a new entry to the exercise history. */
void HistoryParser::append(int grossHitsPerMinute, int mistakes, int timeSecs)
{
	append(HistoryEntry(grossHitsPerMinute, mistakes, timeSecs));
}

/*! Returns JSON document of the history JSON file. */
QJsonDocument HistoryParser::historyDocument(void)
{
	QString pathToHistoryFile = FileUtils::configLocation() + historyFile;
	QFile historyFile(pathToHistoryFile);
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
