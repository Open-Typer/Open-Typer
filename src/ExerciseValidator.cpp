/*
 * ExerciseValidator.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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

#include "ExerciseValidator.h"
#include "StringUtils.h"

/*! Constructs ExerciseValidator. */
ExerciseValidator::ExerciseValidator(QObject *parent) :
	QObject(parent)
{
	m_useNetHitsForGrading = Settings::gradeNetHits();
	m_gradingMethod = Settings::gradingMethod();
	connect(this, &ExerciseValidator::targetHitsPerMinuteChanged, this, &ExerciseValidator::updateGrade);
	connect(this, &ExerciseValidator::useNetHitsForGradingChanged, this, &ExerciseValidator::updateGrade);
	connect(this, &ExerciseValidator::gradingMethodChanged, this, &ExerciseValidator::updateGrade);
}

/*! The exercise text. */
QString ExerciseValidator::exerciseText(void)
{
	return m_exerciseText;
}

void ExerciseValidator::setExerciseText(QString text)
{
	m_exerciseText = text;
	emit exerciseTextChanged(text);
}

/*! The input text. */
QString ExerciseValidator::inputText(void)
{
	return m_inputText;
}

void ExerciseValidator::setInputText(QString text)
{
	m_inputText = text;
	emit inputTextChanged(text);
}

/*! Removes all mistakes. */
void ExerciseValidator::clearMistakes(void)
{
	m_mistakes.clear();
	emit mistakesChanged(m_mistakes);
}

/*! Adds a mistake. */
void ExerciseValidator::addMistake(MistakeRecord mistake)
{
	m_mistakes.append(mistake);
	emit mistakesChanged(m_mistakes);
}

/*! List of mistakes. */
QList<MistakeRecord> ExerciseValidator::mistakes(void)
{
	return m_mistakes;
}

void ExerciseValidator::setMistakes(QList<MistakeRecord> mistakeList)
{
	m_mistakes = mistakeList;
	emit mistakesChanged(mistakeList);
}

/*! Removes all characters. */
void ExerciseValidator::clearCharacters(void)
{
	m_characters.clear();
	emit charactersChanged(m_characters);
}

/*! Adds a character. */
void ExerciseValidator::addCharacter(CharacterRecord character)
{
	m_characters.append(character);
	emit charactersChanged(m_characters);
}

/*! List of characters. */
QList<CharacterRecord> ExerciseValidator::characters(void)
{
	return m_characters;
}

void ExerciseValidator::setCharacters(QList<CharacterRecord> characterList)
{
	m_characters = characterList;
	emit charactersChanged(characterList);
}

/*! Set this to true if this is a timed exercise. */
bool ExerciseValidator::timed(void)
{
	return m_timed;
}

/*!
 * Returns true if this is a timed exercise.
 * \deprecated Use timed() instead.
 */
bool ExerciseValidator::isTimed(void)
{
	return m_timed;
}

void ExerciseValidator::setTimed(bool value)
{
	m_timed = value;
	emit timedChanged(value);
}

/*! Returns exercise time (in seconds). */
qreal ExerciseValidator::time(void)
{
	return m_time;
}

void ExerciseValidator::setTime(qreal value)
{
	m_time = value;
	emit timeChanged(value);
}

/*! Validates the exercise and calculates the results (list of mistakes, number of hits, etc.). */
void ExerciseValidator::validate(void)
{
	m_mistakeCount = 0;
	m_grossHits = 0;
	m_errorWords.clear();
	setMistakes(StringUtils::validateExercise(m_exerciseText, m_inputText, m_characters.toVector(), &m_grossHits, &m_mistakeCount, &m_errorWords, m_timed, m_time));
	updateGrade();
}

/*! Sets a custom result without actually validating the exercise. */
void ExerciseValidator::validate(int grossHits, QStringList errorWords)
{
	m_mistakeCount = m_mistakes.length();
	m_grossHits = grossHits;
	m_errorWords = errorWords;
	updateGrade();
}

/*! Returns number of gross hits. */
int ExerciseValidator::grossHits(void)
{
	return m_grossHits;
}

/*! Returns number of mistakes. */
int ExerciseValidator::mistakeCount(void)
{
	return m_mistakeCount;
}

/*! Returns list of error words. */
QStringList ExerciseValidator::errorWords(void)
{
	return m_errorWords;
}

/*! Generates text with correct characters above mistakes. */
void ExerciseValidator::generateMistakeText(bool correctMistakes)
{
	QMap<int, MistakeRecord *> mistakesMap;
	for(int i = 0; i < m_mistakes.count(); i++)
		mistakesMap[m_mistakes[i].position()] = &m_mistakes[i];
	QString inputTxt = "";
	QStringList lines = m_inputText.split("\n");
	int pos = 0, delta = 0;
	QString mistakeTxt = "";
	for(int i = 0; i < lines.count(); i++)
	{
		QString addLine = lines[i];
		inputTxt += QString(" ").repeated(addLine.length()) + "\n";
		// Add line with correct characters
		int oldPos = pos;
		int count = lines[i].length();
		for(int j = 0; j <= count; j++)
		{
			QString inputChar;
			if(j < count)
				inputChar = m_inputText[pos];
			else
				inputChar = " ";
			if(mistakesMap.contains(pos))
			{
				QString correct;
				if(correctMistakes)
					correct = m_exerciseText[(pos - delta) % (m_exerciseText.length() - 1)];
				else
					correct = mistakesMap[pos]->previousText();
				if(correct == "\n")
					correct = "↵";
				auto type = mistakesMap[pos]->type();
				if(type == MistakeRecord::Type_Deletion)
				{
					mistakeTxt += correct.split("\n").at(0);
					inputTxt += QString(" ").repeated(correct.split("\n").at(0).length());
				}
				else
					mistakeTxt += correct;
				if(type != MistakeRecord::Type_Change)
					mistakeTxt += " ";
				inputTxt += inputChar;
			}
			else if(j < count)
			{
				mistakeTxt += " ";
				inputTxt += inputChar;
			}
			if(j < count)
				pos++;
		}
		inputTxt += "\n";
		// Add line with underlined mistakes
		mistakeTxt += "\n";
		pos = oldPos;
		for(int j = 0; j <= count; j++)
		{
			if(mistakesMap.contains(pos))
			{
				QString correct;
				if(correctMistakes)
					correct = m_exerciseText[pos];
				else
					correct = mistakesMap[pos]->previousText();
				if((mistakesMap[pos]->type() == MistakeRecord::Type_Deletion) && correct.contains("\n"))
					mistakeTxt += "_";
				else
					mistakeTxt += QString("_").repeated(std::max(1LL, (long long int) correct.length()));
			}
			else if(j < count)
				mistakeTxt += " ";
			if(mistakesMap.contains(pos))
			{
				if(mistakesMap[pos]->type() == MistakeRecord::Type_Deletion)
					mistakeTxt += " ";
			}
			if(j < count)
				pos++;
		}
		mistakeTxt += "\n";
		pos++;
	}
	m_generatedInputText = inputTxt;
	m_generatedMistakeText = mistakeTxt;
}

/*! Returns input text generated by generateMistakeText(). */
QString ExerciseValidator::generatedInputText(void)
{
	return m_generatedInputText;
}

/*! Returns mistake text generated by generateMistakeText(). */
QString ExerciseValidator::generatedMistakeText(void)
{
	return m_generatedMistakeText;
}

/*! Adds mistakes to the exercise text. */
QString ExerciseValidator::textWithMistakes(void)
{
	QList<MistakeRecord *> mistakePtrList;
	for(int i = 0; i < m_mistakes.length(); i++)
		mistakePtrList.append(&m_mistakes[i]);
	return StringUtils::addMistakes(m_inputText, mistakePtrList);
}

/*! Number of hits per minute for the best grade. \since Open-Typer 5.1.0 */
int ExerciseValidator::targetHitsPerMinute(void) const
{
	return m_targetHitsPerMinute;
}

void ExerciseValidator::setTargetHitsPerMinute(int newTargetHitsPerMinute)
{
	if(m_targetHitsPerMinute == newTargetHitsPerMinute)
		return;
	m_targetHitsPerMinute = newTargetHitsPerMinute;
	emit targetHitsPerMinuteChanged();
}

/*!
 * Whether to use net hits per minute instead of gross hits for grading.\n
 * Default value: Settings#gradeNetHits()
 * \since Open-Typer 5.1.0
 */
bool ExerciseValidator::useNetHitsForGrading(void) const
{
	return m_useNetHitsForGrading;
}

void ExerciseValidator::setUseNetHitsForGrading(bool newUseNetHitsForGrading)
{
	if(m_useNetHitsForGrading == newUseNetHitsForGrading)
		return;
	m_useNetHitsForGrading = newUseNetHitsForGrading;
	emit useNetHitsForGradingChanged();
}

/*!
 * The grading method, for example numbers or letters.\n
 * Default value: Settings#gradingMethod
 * \since Open-Typer 5.1.0
 */
const ClassManager::GradingMethod &ExerciseValidator::gradingMethod(void) const
{
	return m_gradingMethod;
}

void ExerciseValidator::setGradingMethod(const ClassManager::GradingMethod &newGradingMethod)
{
	if(m_gradingMethod == newGradingMethod)
		return;
	m_gradingMethod = newGradingMethod;
	emit gradingMethodChanged();
}

/*! The grade. \since Open-Typer 5.1.0 */
const QString &ExerciseValidator::grade(void) const
{
	return m_grade;
}

/*! Calculates the grade and updates the grade property. \since Open-Typer 5.1.0 */
void ExerciseValidator::updateGrade(void)
{
	qreal hits = m_useNetHitsForGrading ? std::max(0, m_grossHits - m_mistakeCount * Settings::errorPenalty()) : m_grossHits;
	qreal hpm = hits * (60 / m_time);
	QVariantList grades;
	if(m_gradingMethod == ClassManager::GradingMethod_Numbers)
	{
		int start = Settings::gradeStartNumber();
		int end = Settings::gradeEndNumber();
		if(start < end)
		{
			for(int i = start; i <= end; i++)
				grades.append(i);
		}
		else
		{
			for(int i = start; i >= end; i--)
				grades.append(i);
		}
	}
	else if(m_gradingMethod == ClassManager::GradingMethod_Letters)
	{
		QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		int start = alphabet.indexOf(Settings::gradeStartLetter());
		int end = alphabet.indexOf(Settings::gradeEndLetter());
		Q_ASSERT(start >= 0);
		Q_ASSERT(end >= 0);
		if(start < end)
		{
			for(int i = start; i <= end; i++)
				grades.append(QChar(alphabet[i]));
		}
		else
		{
			for(int i = start; i >= end; i--)
				grades.append(QChar(alphabet[i]));
		}
	}
	Q_ASSERT(grades.length() != 0);
	int index = std::min(grades.length() - 1.0, hpm / m_targetHitsPerMinute * grades.length() - 1.0);
	if(index < 0)
		index = 0;
	QVariant grade = grades[index];
	if(m_gradingMethod == ClassManager::GradingMethod_Numbers)
		m_grade = QString::number(grade.toInt());
	else if(m_gradingMethod == ClassManager::GradingMethod_Letters)
		m_grade = grade.toChar();
	emit gradeChanged();
}