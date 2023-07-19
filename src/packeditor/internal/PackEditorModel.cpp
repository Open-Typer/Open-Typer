/*
 * PackEditorModel.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

#include "PackEditorModel.h"

PackEditorModel::PackEditorModel(QObject *parent) :
	QObject(parent)
{
	// Create a new instance of ConfigParser
	// This is required for multiple pack editor tabs
	m_parser = configParser()->createInstance();

	// Update lists when language changes
	connect(languageManager().get(), &ILanguageManager::languageChanged, this, &PackEditorModel::updateLists);
}

PackEditorModel::~PackEditorModel()
{
	m_parser->close();
}

IConfigParser *PackEditorModel::parser() const
{
	return m_parser.get();
}

const QString &PackEditorModel::fileName() const
{
	return m_fileName;
}

void PackEditorModel::setFileName(const QString &newFileName)
{
	if(m_fileName == newFileName)
		return;
	m_fileName = newFileName;
	open();
	emit fileNameChanged();
}

bool PackEditorModel::saved() const
{
	return m_saved;
}

bool PackEditorModel::readOnly() const
{
	return m_readOnly;
}

int PackEditorModel::lesson() const
{
	return m_lesson;
}

void PackEditorModel::setLesson(int newLesson)
{
	if(m_lesson == newLesson)
		return;
	m_lesson = newLesson;
	m_sublesson = 1;
	m_exercise = 1;
	updateSublessonList();
	updateExerciseList();
	emit lessonChanged();
}

int PackEditorModel::sublesson() const
{
	return m_sublesson;
}

void PackEditorModel::setSublesson(int newSublesson)
{
	if(m_sublesson == newSublesson)
		return;
	m_sublesson = newSublesson;
	m_exercise = 1;
	updateExerciseList();
	emit sublessonChanged();
}

int PackEditorModel::exercise() const
{
	return m_exercise;
}

void PackEditorModel::setExercise(int newExercise)
{
	if(m_exercise == newExercise)
		return;
	m_exercise = newExercise;
	emit exerciseChanged();
}

bool PackEditorModel::opened() const
{
	return m_opened;
}

const QStringList &PackEditorModel::lessonList() const
{
	return m_lessonList;
}

const QStringList &PackEditorModel::sublessonList() const
{
	return m_sublessonList;
}

const QStringList &PackEditorModel::exerciseList() const
{
	return m_exerciseList;
}

void PackEditorModel::open()
{
	m_parser->close();
	m_opened = m_parser->open(m_fileName);

	if(m_opened)
	{
		m_lesson = 1;
		m_sublesson = 1;
		m_exercise = 1;
		emit lessonChanged();
		emit sublessonChanged();
		emit exerciseChanged();
		updateLists();
	}

	emit openedChanged();
}

void PackEditorModel::nextExercise()
{
	int count = m_parser->exerciseCount(m_lesson, m_sublesson);
	Q_ASSERT(m_exercise <= count);

	if(m_exercise >= count)
	{
		nextSublesson();
		m_exercise = 1;
	}
	else
		m_exercise++;
	emit exerciseChanged();
}

void PackEditorModel::previousExercise()
{
	m_exercise--;
	Q_ASSERT(m_exercise >= 0);

	if(m_exercise == 0)
	{
		previousSublesson();
		m_exercise = m_parser->exerciseCount(m_lesson, m_sublesson);
	}
	emit exerciseChanged();
}

void PackEditorModel::nextSublesson()
{
	int count = m_parser->sublessonCount(m_lesson);
	Q_ASSERT(m_sublesson <= count);

	if(m_sublesson >= count)
	{
		nextLesson(false);
		m_sublesson = 1;
	}
	else
		m_sublesson++;
	updateExerciseList();
	emit sublessonChanged();
}

void PackEditorModel::previousSublesson()
{
	m_sublesson--;
	Q_ASSERT(m_sublesson >= 0);

	if(m_sublesson == 0)
	{
		previousLesson(false);
		m_sublesson = m_parser->sublessonCount(m_lesson);
	}
	updateExerciseList();
	emit sublessonChanged();
}

void PackEditorModel::nextLesson(bool updateExList)
{
	int count = m_parser->lessonCount();
	Q_ASSERT(m_lesson <= count);

	if(m_lesson >= count)
		m_lesson = 1;
	else
		m_lesson++;
	updateSublessonList();
	if(updateExList)
		updateExerciseList();
	emit lessonChanged();
}

void PackEditorModel::previousLesson(bool updateExList)
{
	m_lesson--;
	Q_ASSERT(m_lesson >= 0);

	if(m_lesson == 0)
		m_lesson = m_parser->lessonCount();
	updateSublessonList();
	if(updateExList)
		updateExerciseList();
	emit lessonChanged();
}

void PackEditorModel::updateLists()
{
	updateLessonList();
	updateSublessonList();
	updateExerciseList();
}

void PackEditorModel::updateLessonList()
{
	m_lessonList.clear();
	QString lessonDesc;
	int count = m_parser->lessonCount();

	for(int i = 1; i <= count; i++)
	{
		lessonDesc = m_parser->parseDesc(m_parser->lessonDesc(i));
		if(lessonDesc == "")
			m_lessonList.push_back(m_parser->lessonTr(i));
		else
			m_lessonList.push_back(m_parser->lessonTr(i) + " " + lessonDesc);
	}

	emit lessonListChanged();
}

void PackEditorModel::updateSublessonList()
{
	m_sublessonList.clear();
	int i2 = 0;
	int count = m_parser->sublessonCount(m_lesson);

	for(int i = 1; i <= count + i2; i++)
	{
		if(m_parser->exerciseCount(m_lesson, i) > 0)
			m_sublessonList.push_back(m_parser->sublessonName(i));
	}

	emit sublessonListChanged();
}

void PackEditorModel::updateExerciseList()
{
	m_exerciseList.clear();
	int count = m_parser->exerciseCount(m_lesson, m_sublesson);

	for(int i = 1; i <= count; i++)
		m_exerciseList.push_back(m_parser->exerciseTr(i));

	emit exerciseListChanged();
}
