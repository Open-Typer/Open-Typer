/*
 * ExerciseValidator.cpp
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

#include "ExerciseValidator.h"
#include "StringUtils.h"

/*! Sets exercise text. */
void ExerciseValidator::setExerciseText(QString text)
{
	m_exerciseText = text;
	emit exerciseTextChanged(text);
}

/*! Returns exercise text. */
QString ExerciseValidator::exerciseText(void)
{
	return m_exerciseText;
}

/*! Sets input text. */
void ExerciseValidator::setInputText(QString text)
{
	m_inputText = text;
	emit inputTextChanged(text);
}

/*! Returns input text. */
QString ExerciseValidator::inputText(void)
{
	return m_inputText;
}

/*! Sets list of mistakes. */
void ExerciseValidator::setMistakes(QList<MistakeRecord *> mistakeList)
{
	m_mistakes = mistakeList;
	emit mistakesChanged(mistakeList);
}

/*! Removes all mistakes. */
void ExerciseValidator::clearMistakes(void)
{
	for(int i = 0; i < m_mistakes.size(); i++)
		m_mistakes[i]->deleteLater();
	m_mistakes.clear();
	emit mistakesChanged(m_mistakes);
}

/*! Adds a mistake. */
void ExerciseValidator::addMistake(MistakeRecord *mistake)
{
	m_mistakes.append(mistake);
	emit mistakesChanged(m_mistakes);
}

/*! Returns list of mistakes. */
QList<MistakeRecord *> ExerciseValidator::mistakes(void)
{
	return m_mistakes;
}

/*! Sets list of characters. */
void ExerciseValidator::setCharacters(QList<CharacterRecord *> characterList)
{
	m_characters = characterList;
	emit charactersChanged(characterList);
}

/*! Removes all characters. */
void ExerciseValidator::clearCharacters(void)
{
	for(int i = 0; i < m_characters.size(); i++)
		m_characters[i]->deleteLater();
	m_characters.clear();
	emit charactersChanged(m_characters);
}

/*! Adds a character. */
void ExerciseValidator::addCharacter(CharacterRecord *character)
{
	m_characters.append(character);
	emit charactersChanged(m_characters);
}

/*! Returns list of characters. */
QList<CharacterRecord *> ExerciseValidator::characters(void)
{
	return m_characters;
}

/*! Toggles timed exercise. */
void ExerciseValidator::setTimed(bool value)
{
	m_isTimed = value;
	emit timedChanged(value);
}

/*! Returns true if this is a timed exercise. */
bool ExerciseValidator::isTimed(void)
{
	return m_isTimed;
}

/*! Sets exercise time (in seconds). */
void ExerciseValidator::setTime(qreal value)
{
	m_time = value;
	emit timeChanged(value);
}

/*! Returns exercise time (in seconds). */
qreal ExerciseValidator::time(void)
{
	return m_time;
}

/*! Validates the exercise and calculates the results (list of mistakes, number of hits, etc.). */
void ExerciseValidator::validate(void)
{
	m_mistakeCount = 0;
	m_grossHits = 0;
	m_errorWords.clear();
	setMistakes(StringUtils::validateExercise(m_exerciseText, m_inputText, m_characters.toVector(), &m_grossHits, &m_mistakeCount, &m_errorWords, m_isTimed, m_time));
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
		mistakesMap[m_mistakes[i]->position()] = m_mistakes[i];
	QString inputTxt = "";
	QStringList lines = m_inputText.split("\n");
	int pos = 0, delta = 0;
	QString mistakeTxt = "";
	for(int i = 0; i < lines.count(); i++)
	{
		QString addLine = lines[i];
		inputTxt += QString(" ").repeated(addLine.count()) + "\n";
		// Add line with correct characters
		int oldPos = pos;
		int count = lines[i].count();
		for(int j = 0; j <= lines[i].count(); j++)
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
					correct = m_exerciseText[(pos - delta) % (m_exerciseText.count() - 1)];
				else
					correct = mistakesMap[pos]->previousText();
				auto type = mistakesMap[pos]->type();
				if(type == MistakeRecord::Type_Deletion)
				{
					mistakeTxt += correct.split("\n").at(0);
					inputTxt += QString(" ").repeated(correct.split("\n").at(0).count());
				}
				else
					mistakeTxt += correct;
				if(type == MistakeRecord::Type_Change)
				{
					if(correct == "\n")
						delta++;
				}
				else
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
					mistakeTxt += QString("_").repeated(std::max(1LL, (long long int) correct.count()));
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
	return StringUtils::addMistakes(m_inputText, m_mistakes);
}
