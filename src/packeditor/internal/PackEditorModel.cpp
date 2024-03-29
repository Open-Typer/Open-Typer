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

#include <QBuffer>
#include <QFile>
#include "PackEditorModel.h"

PackEditorModel::PackEditorModel(QObject *parent) :
	QObject(parent)
{
	// Create a new instance of ConfigParser
	// This is required for multiple pack editor tabs
	m_parser = configParser()->createInstance();

	// Update lists when language changes
	connect(languageManager().get(), &ILanguageManager::languageChanged, this, &PackEditorModel::updateLists);

	connect(this, &PackEditorModel::lessonChanged, this, &PackEditorModel::updateAbsoluteSublesson);
	connect(this, &PackEditorModel::lessonChanged, this, &PackEditorModel::unusedSublessonsChanged);
	connect(this, &PackEditorModel::lessonChanged, this, &PackEditorModel::currentLessonDescriptionChanged);
	connect(this, &PackEditorModel::sublessonChanged, this, &PackEditorModel::updateAbsoluteSublesson);

	connect(this, &PackEditorModel::exerciseChanged, this, &PackEditorModel::currentRawTextChanged);
	connect(this, &PackEditorModel::exerciseChanged, this, &PackEditorModel::currentRepeatTypeChanged);
	connect(this, &PackEditorModel::exerciseChanged, this, &PackEditorModel::currentLengthLimitChanged);
	connect(this, &PackEditorModel::exerciseChanged, this, &PackEditorModel::currentLineLengthChanged);

	connect(this, &PackEditorModel::currentRawTextChanged, this, &PackEditorModel::currentTextChanged);
	connect(this, &PackEditorModel::currentRepeatTypeChanged, this, &PackEditorModel::currentTextChanged);
	connect(this, &PackEditorModel::currentLengthLimitChanged, this, &PackEditorModel::currentTextChanged);
	connect(this, &PackEditorModel::currentLineLengthChanged, this, &PackEditorModel::currentTextChanged);
	connect(this, &PackEditorModel::currentLessonDescriptionChanged, this, &PackEditorModel::updateLessonList);
}

PackEditorModel::~PackEditorModel()
{
	m_parser->close();
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
	emit sublessonChanged();
	emit exerciseChanged();
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
	emit exerciseChanged();
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

QString PackEditorModel::currentText() const
{
	QString text = m_parser->exerciseText(m_lesson, m_absoluteSublesson, m_exercise);
	int lineLength = m_parser->exerciseLineLength(m_lesson, m_absoluteSublesson, m_exercise);
	return m_parser->initExercise(text, lineLength);
}

QString PackEditorModel::currentRawText() const
{
	return m_parser->exerciseRawText(m_lesson, m_absoluteSublesson, m_exercise);
}

void PackEditorModel::setCurrentRawText(const QString &newRawText)
{
	bool repeat = m_parser->exerciseRepeatBool(m_lesson, m_absoluteSublesson, m_exercise);
	QString repeatType = m_parser->exerciseRepeatType(m_lesson, m_absoluteSublesson, m_exercise);
	int repeatLimit = m_parser->exerciseRepeatLimit(m_lesson, m_absoluteSublesson, m_exercise);
	int lineLength = m_parser->exerciseLineLength(m_lesson, m_absoluteSublesson, m_exercise);
	QString desc = "";
	if(m_exercise == 1)
		desc = m_parser->lessonDesc(m_lesson);

	editExercise(repeat, repeatType, repeatLimit, lineLength, desc, newRawText);

	m_saved = false;
	emit savedChanged();

	emit currentRawTextChanged();
}

QString PackEditorModel::currentRepeatType() const
{
	bool repeat = m_parser->exerciseRepeatBool(m_lesson, m_absoluteSublesson, m_exercise);
	QString repeatType = m_parser->exerciseRepeatType(m_lesson, m_absoluteSublesson, m_exercise);
	return repeat ? repeatType : "0";
}

void PackEditorModel::setCurrentRepeatType(const QString &newRepeatType)
{
	bool repeat = newRepeatType == "0" ? false : true;
	int repeatLimit = m_parser->exerciseRepeatLimit(m_lesson, m_absoluteSublesson, m_exercise);
	int lineLength = m_parser->exerciseLineLength(m_lesson, m_absoluteSublesson, m_exercise);
	QString desc = "";
	if(m_exercise == 1)
		desc = m_parser->lessonDesc(m_lesson);
	QString rawText = m_parser->exerciseRawText(m_lesson, m_absoluteSublesson, m_exercise);

	editExercise(repeat, newRepeatType, repeatLimit, lineLength, desc, rawText);

	m_saved = false;
	emit savedChanged();

	emit currentRepeatTypeChanged();
}

int PackEditorModel::currentLengthLimit() const
{
	return m_parser->exerciseRepeatLimit(m_lesson, m_absoluteSublesson, m_exercise);
}

void PackEditorModel::setCurrentLengthLimit(int newLengthLimit)
{
	bool repeat = m_parser->exerciseRepeatBool(m_lesson, m_absoluteSublesson, m_exercise);
	QString repeatType = m_parser->exerciseRepeatType(m_lesson, m_absoluteSublesson, m_exercise);
	int lineLength = m_parser->exerciseLineLength(m_lesson, m_absoluteSublesson, m_exercise);
	QString desc = "";
	if(m_exercise == 1)
		desc = m_parser->lessonDesc(m_lesson);
	QString rawText = m_parser->exerciseRawText(m_lesson, m_absoluteSublesson, m_exercise);

	editExercise(repeat, repeatType, newLengthLimit, lineLength, desc, rawText);

	m_saved = false;
	emit savedChanged();

	emit currentLengthLimitChanged();
}

int PackEditorModel::currentLineLength() const
{
	return m_parser->exerciseLineLength(m_lesson, m_absoluteSublesson, m_exercise);
}

void PackEditorModel::setCurrentLineLength(int newLineLength)
{
	bool repeat = m_parser->exerciseRepeatBool(m_lesson, m_absoluteSublesson, m_exercise);
	QString repeatType = m_parser->exerciseRepeatType(m_lesson, m_absoluteSublesson, m_exercise);
	int repeatLimit = m_parser->exerciseRepeatLimit(m_lesson, m_absoluteSublesson, m_exercise);
	QString desc = "";
	if(m_exercise == 1)
		desc = m_parser->lessonDesc(m_lesson);
	QString rawText = m_parser->exerciseRawText(m_lesson, m_absoluteSublesson, m_exercise);

	m_saved = false;
	emit savedChanged();

	editExercise(repeat, repeatType, repeatLimit, newLineLength, desc, rawText);
	emit currentLineLengthChanged();
}

QString PackEditorModel::currentLessonDescription() const
{
	return m_parser->lessonDesc(m_lesson);
}

void PackEditorModel::setCurrentLessonDescription(const QString &newLessonDescription)
{
	bool repeat = m_parser->exerciseRepeatBool(m_lesson, m_absoluteSublesson, m_exercise);
	QString repeatType = m_parser->exerciseRepeatType(m_lesson, m_absoluteSublesson, m_exercise);
	int repeatLimit = m_parser->exerciseRepeatLimit(m_lesson, m_absoluteSublesson, m_exercise);
	int lineLength = m_parser->exerciseLineLength(m_lesson, m_absoluteSublesson, m_exercise);
	QString rawText = m_parser->exerciseRawText(m_lesson, m_absoluteSublesson, m_exercise);

	// TODO: There isn't any API for replacing an exercise yet, so delete it and add a new one
	deleteExerciseLine(m_lesson, m_absoluteSublesson, 1);
	m_parser->addExercise(m_lesson, m_absoluteSublesson, 1, repeat, repeatType, repeatLimit, lineLength, newLessonDescription, rawText);

	m_saved = false;
	emit savedChanged();
	emit currentLessonDescriptionChanged();
}

QList<int> PackEditorModel::unusedSublessons() const
{
	QList<int> ret;
	int count = 4; // loop through touch, words, sentences and text

	for(int i = 1; i <= count; i++)
	{
		if(m_parser->exerciseCount(m_lesson, i) == 0)
			ret.push_back(i);
	}

	return ret;
}

bool PackEditorModel::canRemove() const
{
	if(m_parser->lessonCount() == 0)
		return false;

	if(m_parser->lessonCount() > 1)
		return true;

	if(m_parser->sublessonCount(m_lesson) > 1)
		return true;

	if(m_parser->exerciseCount(m_lesson, m_absoluteSublesson) > 1)
		return true;

	return false;
}

void PackEditorModel::newFile()
{
	m_parser->close();
	m_parser->loadToBuffer("");
	m_readOnly = true;
	m_opened = true;
	emit readOnlyChanged();
	emit openedChanged();
	init();

	addExercise();
	updateLessonList();
	updateSublessonList();
	emit lessonChanged();
	emit sublessonChanged();
	m_fileName = tr("Unnamed", "Unnamed (lesson pack)") + ".typer";
	emit fileNameChanged();
}

void PackEditorModel::open()
{
	m_parser->close();
	m_opened = m_parser->open(m_fileName);

	if(m_opened)
	{
		m_parser->loadToBuffer(m_parser->data());
		m_saved = true;
		init();
	}

	emit openedChanged();
}

bool PackEditorModel::save()
{
	QFile file(m_fileName);
	bool ok = file.open(QFile::WriteOnly | QFile::Text);

	if(ok)
	{
		file.write(m_parser->data());
		m_saved = true;
		emit savedChanged();
		return true;
	}

	return false;
}

bool PackEditorModel::saveAs(const QString &fileName)
{
	QString oldName = m_fileName;
	m_fileName = fileName;

	if(save())
	{
		emit fileNameChanged();
		return true;
	}

	m_fileName = oldName;
	return false;
}

void PackEditorModel::nextExercise()
{
	int count = m_parser->exerciseCount(m_lesson, m_absoluteSublesson);
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
		m_exercise = m_parser->exerciseCount(m_lesson, m_absoluteSublesson);
	}
	emit exerciseChanged();
}

QString PackEditorModel::sublessonName(int id)
{
	return m_parser->sublessonName(id);
}

void PackEditorModel::addLesson()
{
	m_lesson = m_parser->lessonCount() + 1;

	addSublesson(1);
	updateLessonList();
	emit lessonChanged();
}

void PackEditorModel::addSublesson(int id)
{
	Q_ASSERT(unusedSublessons().contains(id));
	m_absoluteSublesson = id;

	addExercise();
	updateSublessonList();
	emit unusedSublessonsChanged();
	setAbsoluteSublesson(id);
}

void PackEditorModel::addExercise()
{
	m_exercise = m_parser->exerciseCount(m_lesson, m_absoluteSublesson) + 1;

	int repeatLimit = m_parser->defaultRepeatLimit();
	int lineLength = m_parser->defaultLineLength();
	QString rawText = tr("New exercise");
	m_parser->addExercise(m_lesson, m_absoluteSublesson, m_exercise, false, "0", repeatLimit, lineLength, "", rawText);
	updateExerciseList();

	m_saved = false;
	emit savedChanged();

	emit exerciseChanged();
	emit canRemoveChanged();
}

void PackEditorModel::removeCurrentExercise()
{
	int lessonCount = m_parser->lessonCount();
	int sublessonCount = m_parser->sublessonCount(m_lesson);
	int exerciseCount = m_parser->exerciseCount(m_lesson, m_absoluteSublesson);

	// Remove the exercise
	QString lessonDesc;
	if(m_sublesson == 1 && m_exercise == 1)
		lessonDesc = m_parser->lessonDesc(m_lesson);
	deleteExerciseLine(m_lesson, m_absoluteSublesson, m_exercise);

	if(exerciseCount == 1)
	{
		if(sublessonCount == 1)
		{
			// Move lessons after the removed lesson
			for(int i = m_lesson; i <= lessonCount - 1; i++)
				moveLesson(i + 1, i);

			// Select last lesson if the previous last lesson was removed
			if(m_lesson == lessonCount)
				m_lesson = lessonCount - 1;

			updateLessonList();
			emit lessonChanged();
		}
		else
		{
			// Select last sublesson if the previous last sublesson was removed
			if(m_sublesson == sublessonCount)
				m_sublesson = sublessonCount - 1;
		}

		updateSublessonList();
		emit sublessonChanged();
	}
	else
	{
		// Move exercises after the removed exercise
		for(int i = m_exercise; i <= exerciseCount - 1; i++)
			moveExercise(m_lesson, m_absoluteSublesson, i + 1, m_lesson, m_absoluteSublesson, i);

		// Select last exercise if the previous last exercise was removed
		if(m_exercise == exerciseCount)
			m_exercise = exerciseCount - 1;
	}

	// Restore lesson description
	if(!lessonDesc.isEmpty())
	{
		bool repeat = m_parser->exerciseRepeatBool(m_lesson, m_absoluteSublesson, m_exercise);
		QString repeatType = m_parser->exerciseRepeatType(m_lesson, m_absoluteSublesson, m_exercise);
		int repeatLimit = m_parser->exerciseRepeatLimit(m_lesson, m_absoluteSublesson, m_exercise);
		int lineLength = m_parser->exerciseLineLength(m_lesson, m_absoluteSublesson, m_exercise);
		QString rawText = m_parser->exerciseRawText(m_lesson, m_absoluteSublesson, m_exercise);

		editExercise(repeat, repeatType, repeatLimit, lineLength, lessonDesc, rawText);
	}

	updateExerciseList();

	m_saved = false;
	emit savedChanged();

	emit exerciseChanged();
	emit canRemoveChanged();
	emit unusedSublessonsChanged();
}

void PackEditorModel::init()
{
	m_lesson = 1;
	m_sublesson = 1;
	m_exercise = 1;
	emit savedChanged();
	emit lessonChanged();
	emit sublessonChanged();
	emit exerciseChanged();
	updateLists();
	emit unusedSublessonsChanged();
	emit canRemoveChanged();
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
	int j = 0;
	int count = m_parser->sublessonCount(m_lesson);

	for(int i = 1; i <= count + j; i++)
	{
		if(m_parser->exerciseCount(m_lesson, i) > 0)
			m_sublessonList.push_back(m_parser->sublessonName(i));
		else
			j++;
	}

	emit sublessonListChanged();
}

void PackEditorModel::updateExerciseList()
{
	updateAbsoluteSublesson();
	m_exerciseList.clear();
	int count = m_parser->exerciseCount(m_lesson, m_absoluteSublesson);

	for(int i = 1; i <= count; i++)
		m_exerciseList.push_back(m_parser->exerciseTr(i));

	emit exerciseListChanged();
}

void PackEditorModel::updateAbsoluteSublesson()
{
	int count = m_parser->sublessonCount(m_lesson);
	int sublessonListStart = 0;
	int i, j = 0;

	for(i = 1; i <= count + j; i++)
	{
		if(m_parser->exerciseCount(m_lesson, i) == 0)
		{
			j++;
			if(m_sublesson + j > i)
				sublessonListStart++;
		}
	}

	m_absoluteSublesson = m_sublesson + sublessonListStart;
}

void PackEditorModel::setAbsoluteSublesson(int absoluteSublesson)
{
	int count = m_parser->sublessonCount(m_lesson);
	int i, j = 0;

	for(i = 1; i <= count + j; i++)
	{
		if(m_parser->exerciseCount(m_lesson, i) == 0)
			j++;
		else if(i == absoluteSublesson)
		{
			m_sublesson = i - j;
			m_absoluteSublesson = absoluteSublesson;
		}
	}

	emit sublessonChanged();
}

void PackEditorModel::editExercise(bool repeat, const QString &repeatType, int repeatLimit, int lineLength, const QString &desc, const QString &rawText)
{
	// TODO: There isn't any API for replacing an exercise yet, so delete it and add a new one
	deleteExerciseLine(m_lesson, m_absoluteSublesson, m_exercise);
	m_parser->addExercise(m_lesson, m_absoluteSublesson, m_exercise, repeat, repeatType, repeatLimit, lineLength, desc, rawText);
}

void PackEditorModel::moveExercise(int lesson, int sublesson, int exercise, int newLesson, int newSublesson, int newExercise)
{
	int lengthLimit = m_parser->exerciseRepeatLimit(lesson, sublesson, exercise);
	int lineLength = m_parser->exerciseLineLength(lesson, sublesson, exercise);
	QString repeatType = m_parser->exerciseRepeatType(lesson, sublesson, exercise);
	bool repeat = (repeatType != "0");
	QString lessonDesc = "";
	if(exercise == 1) // the first exercise of a lesson holds the description
		lessonDesc = m_parser->lessonDesc(lesson);
	else if(newExercise == 1)
		lessonDesc = m_parser->lessonDesc(newLesson);
	QString targetText = m_parser->exerciseRawText(lesson, sublesson, exercise);

	deleteExerciseLine(lesson, sublesson, exercise);
	m_parser->addExercise(newLesson, newSublesson, newExercise, repeat, repeatType, lengthLimit, lineLength, lessonDesc, targetText);
}

void PackEditorModel::moveLesson(int lesson, int newLesson)
{
	int sublessonCount = m_parser->sublessonCount(lesson);
	int j = 0;

	for(int i = 1; i <= sublessonCount + j; i++)
	{
		int exerciseCount = m_parser->exerciseCount(lesson, i);

		if(exerciseCount == 0)
			j++;
		else
		{
			for(int k = 1; k <= exerciseCount; k++)
				moveExercise(lesson, i, k, newLesson, i, k);
		}
	}
}

void PackEditorModel::deleteExerciseLine(int lesson, int sublesson, int exercise)
{
	// TODO: Add removeExercise() and replaceExercise() to ConfigParser
	int targetLine, curLine;
	targetLine = m_parser->exerciseLine(lesson, sublesson, exercise);
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write(m_parser->data());
	QByteArray out;
	buffer.seek(0);
	curLine = 0;
	while(!buffer.atEnd())
	{
		curLine++;
		QByteArray line = buffer.readLine();
		if(curLine != targetLine)
			out += line;
	}
	buffer.close();
	m_parser->loadToBuffer(out);
}
