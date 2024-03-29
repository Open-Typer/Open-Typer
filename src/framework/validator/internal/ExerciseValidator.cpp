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

#include <QVector>
#include "ExerciseValidator.h"

static const QString module = "validator";
static const ISettings::Key MISTAKE_LIMIT(module, "mistakeLimit");
static const ISettings::Key MISTAKE_CHARS(module, "mistakeChars");

/*! The exercise text. */
QString ExerciseValidator::exerciseText(void)
{
	return m_exerciseText;
}

void ExerciseValidator::setExerciseText(QString text)
{
	m_exerciseText = text;
	emit exerciseTextChanged();
}

/*! The input text. */
QString ExerciseValidator::inputText(void)
{
	return m_inputText;
}

void ExerciseValidator::setInputText(QString text)
{
	m_inputText = text;
	emit inputTextChanged();
}

/*! Removes all mistakes. */
void ExerciseValidator::clearMistakes(void)
{
	m_mistakes.clear();
	emit mistakesChanged();
}

/*! Adds a mistake. */
void ExerciseValidator::addMistake(MistakeRecord mistake)
{
	m_mistakes.append(mistake);
	emit mistakesChanged();
}

/*! List of mistakes. */
QList<MistakeRecord> ExerciseValidator::mistakes(void)
{
	return m_mistakes;
}

void ExerciseValidator::setMistakes(QList<MistakeRecord> mistakeList)
{
	m_mistakes = mistakeList;
	emit mistakesChanged();
}

/*! Removes all characters. */
void ExerciseValidator::clearCharacters(void)
{
	m_characters.clear();
	emit charactersChanged();
}

/*! Adds a character. */
void ExerciseValidator::addCharacter(CharacterRecord character)
{
	m_characters.append(character);
	emit charactersChanged();
}

/*! List of characters. */
QList<CharacterRecord> ExerciseValidator::characters(void)
{
	return m_characters;
}

void ExerciseValidator::setCharacters(QList<CharacterRecord> characterList)
{
	m_characters = characterList;
	emit charactersChanged();
}

/*! Set this to true if this is a timed exercise. */
bool ExerciseValidator::timed(void)
{
	return m_timed;
}

void ExerciseValidator::setTimed(bool value)
{
	m_timed = value;
	emit timedChanged();
}

/*! Returns exercise time (in seconds). */
qreal ExerciseValidator::time(void)
{
	return m_time;
}

void ExerciseValidator::setTime(qreal value)
{
	m_time = value;
	emit timeChanged();
}

/*! Validates the exercise and calculates the results (list of mistakes, number of hits, etc.). */
void ExerciseValidator::validate(void)
{
	m_mistakeCount = 0;
	m_grossHits = 0;
	m_errorWords.clear();
	setMistakes(validateExercise(m_exerciseText, m_inputText, m_characters.toVector(), &m_grossHits, &m_mistakeCount, &m_errorWords, m_timed, m_time));
	emit validated();
}

/*! Sets a custom result without actually validating the exercise. */
void ExerciseValidator::validate(int grossHits, QStringList errorWords)
{
	m_mistakeCount = m_mistakes.length();
	m_grossHits = grossHits;
	m_errorWords = errorWords;
	emit validated();
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
	return addMistakes(m_inputText, mistakePtrList);
}

/*! Compares 2 lists using longest common subsequence. */
QList<MistakeRecord> ExerciseValidator::compareLists(QList<QVariant> source, QList<QVariant> target, QVector<CharacterRecord> *recordedCharacters, int *hits, int *inputPos)
{
	QList<MistakeRecord> out;
	auto lcs = stringUtils()->longestCommonSubsequence(source, target);
	int sourcePos = 0, targetPos = 0;
	int count = std::max(source.count(), target.count());
	for(int i = 0; i < count; i++)
	{
		QVariant subseq, sourceSubseq, targetSubseq;
		bool lcsSrc = false, lcsTarget = false;
		if(i < lcs.count())
		{
			subseq = lcs[i];
			if(sourcePos < source.count())
			{
				sourceSubseq = source[sourcePos];
				lcsSrc = (subseq == sourceSubseq);
			}
			if(targetPos < target.count())
			{
				targetSubseq = target[targetPos];
				lcsTarget = (subseq == targetSubseq);
			}
		}
		else
		{
			lcsSrc = true;
			lcsTarget = true;
			if(sourcePos < source.count())
			{
				sourceSubseq = source[sourcePos];
				lcsSrc = false;
			}
			if(targetPos < target.count())
			{
				targetSubseq = target[targetPos];
				lcsTarget = false;
			}
			if((lcsSrc && !lcsTarget) || (!lcsSrc && lcsTarget))
				i++;
			if(sourcePos >= source.count() && targetPos >= target.count())
			{
				lcsSrc = true;
				lcsTarget = true;
			}
		}
		if(!lcsSrc && !lcsTarget)
		{
			// Changed character
			MistakeRecord diff;
			diff.setPosition(targetPos);
			diff.setType(MistakeRecord::Type_Change);
			diff.setPreviousVariant(sourceSubseq);
			diff.setPreviousPosition(sourcePos);
			out += diff;
			targetPos++;
			if(inputPos)
				*inputPos += 1;
			sourcePos++;
			i--;
		}
		else if(!lcsSrc)
		{
			// Deleted character
			MistakeRecord diff;
			diff.setPosition(targetPos);
			diff.setType(MistakeRecord::Type_Deletion);
			diff.setPreviousVariant(sourceSubseq);
			diff.setPreviousPosition(sourcePos);
			out += diff;
			sourcePos++;
			if(inputPos)
				*inputPos -= 1;
			i--;
		}
		else if(!lcsTarget)
		{
			// Added character
			MistakeRecord diff;
			diff.setPosition(targetPos);
			diff.setType(MistakeRecord::Type_Addition);
			out += diff;
			targetPos++;
			if(inputPos)
				*inputPos += 1;
			i--;
		}
		else
		{
			if(recordedCharacters && (*inputPos < recordedCharacters->count()) && hits && inputPos)
			{
				if(*inputPos < 0)
					*inputPos = 0;
				auto character = recordedCharacters->at(*inputPos);
				*hits += character.hits();
				*inputPos += 1;
			}
			sourcePos++;
			targetPos++;
		}
	}
	return out;
}

/*! Compares 2 strings using longest common subsequence. */
QList<MistakeRecord> ExerciseValidator::compareStrings(QString source, QString target, QVector<CharacterRecord> *recordedCharacters, int *hits, int *inputPos)
{
	QList<QVariant> sourceList, targetList;
	int i;
	for(i = 0; i < source.length(); i++)
		sourceList += QString(source[i]);
	for(i = 0; i < target.length(); i++)
		targetList += QString(target[i]);
	return compareLists(sourceList, targetList, recordedCharacters, hits, inputPos);
}

/*! Recursively generates a diff list from source and target word list. */
QMap<int, MistakeRecord> ExerciseValidator::generateDiffList(QStringList *sourceWords, QStringList *targetWords, QList<int> *mergeList)
{
	// Compare word lists
	QList<QVariant> sourceList, targetList;
	for(int i = 0; i < sourceWords->count(); i++)
		sourceList += sourceWords->at(i);
	for(int i = 0; i < targetWords->count(); i++)
		targetList += targetWords->at(i);
	QList<MistakeRecord> wordDiff = compareLists(sourceList, targetList);
	QMap<int, MistakeRecord> differences;
	QList<int> newMergeList;
	if(mergeList == nullptr || !mergeList)
		mergeList = &newMergeList;
	for(int i = 0; i < wordDiff.count(); i++)
	{
		if(mergeList->contains(wordDiff[i].position()))
			wordDiff[i].setMerged(true);
		// If current diff is a change and the next one is a deletion or a change,
		// there might be a "deleted space" between the 2 words.
		if((i < wordDiff.count() - 1) && (wordDiff[i + 1].position() == wordDiff[i].position() + 1) && !mergeList->contains(wordDiff[i].position()))
		{
			auto currentDiff = &wordDiff[i];
			auto nextDiff = &wordDiff[i + 1];
			auto type1 = currentDiff->type();
			auto type2 = nextDiff->type();
			QString newWord = currentDiff->previousText() + " " + nextDiff->previousText();
			int count1 = compareStrings(newWord, targetWords->at(currentDiff->position())).count();
			int count2 = compareStrings(currentDiff->previousText(), targetWords->at(currentDiff->position())).count();
			if((type1 == MistakeRecord::Type_Change) && ((type2 == MistakeRecord::Type_Deletion) || ((type2 == MistakeRecord::Type_Change) && (count1 < count2))))
			{
				// old_word1[space]old_word2
				currentDiff->setPreviousText(newWord);
				currentDiff->setMerged(true);
				sourceWords->replace(currentDiff->previousPosition(), newWord);
				sourceWords->removeAt(currentDiff->previousPosition() + 1);
				mergeList->append(currentDiff->position());
				return generateDiffList(sourceWords, targetWords, mergeList);
			}
		}
		if(differences.contains(wordDiff[i].position()))
		{
			auto currentMistake = differences[wordDiff[i].position()];
			if(!currentMistake.previousText().isNull())
			{
				QString previous = wordDiff[i].previousText();
				QString tmpPrevious = previous;
				if((previous == " ") || previous[0].isPunct() || (tmpPrevious == " "))
					currentMistake.setPreviousText(currentMistake.previousText() + previous);
				else
					currentMistake.setPreviousText(currentMistake.previousText() + " " + previous);
			}
			differences[wordDiff[i].position()] = currentMistake;
		}
		else
			differences[wordDiff[i].position()] = wordDiff[i];
	}
	return differences;
}

/*! Compares input text with exercise text and finds mistakes. */
QList<MistakeRecord> ExerciseValidator::findMistakes(QString exerciseText, QString input, QVector<CharacterRecord> recordedCharacters, int *totalHits, QStringList *errorWords)
{
	QList<MistakeRecord> out;
	int i;
	// Split lines
	QStringList exerciseLines = exerciseText.split('\n');
	QStringList inputLines = input.split('\n');
	// Split words in each line
	QStringList exerciseWords, inputWords;
	for(i = 0; i < exerciseLines.count(); i++)
	{
		if(i > 0)
			exerciseWords += "\n";
		exerciseWords += exerciseLines[i].split(' ');
	}
	exerciseWords = splitWordsByPunct(exerciseWords);
	for(i = 0; i < inputLines.count(); i++)
	{
		if(i > 0)
			inputWords += "\n";
		inputWords += inputLines[i].split(' ');
	}
	inputWords = splitWordsByPunct(inputWords);
	auto differences = generateDiffList(&exerciseWords, &inputWords);
	if(errorWords)
		errorWords->clear();
	int pos = 0, hits = 0;
	for(i = 0; i < inputWords.count(); i++)
	{
		if((i > 0) && (inputWords[i] != "\n"))
		{
			if((inputWords[i] != " ") && (pos < recordedCharacters.count()))
				hits += recordedCharacters[pos].hits();
			pos++;
		}
		if(inputWords[i][0].isPunct())
			pos--;
		else if((i > 0) && (inputWords[i - 1] == " "))
			pos--;
		if(inputWords[i] == " ")
			pos--;
		else if(((i > 0) && inputWords[i - 1][0].isPunct()) && !inputWords[i][0].isPunct())
			pos--;
		if(differences.contains(i))
		{
			if(errorWords)
				errorWords->append(differences[i].previousText());
			if(differences[i].type() == MistakeRecord::Type_Change)
			{
				int wordStart = pos;
				auto diff = compareStrings(differences[i].previousText(), inputWords[i], &recordedCharacters, &hits, &pos);
				// Ensure there's max. one mistake per n characters (depends on settings)
				if(settings()->getValue(MISTAKE_LIMIT).toBool())
				{
					int charCount = settings()->getValue(MISTAKE_CHARS).toInt();
					int lastMistakePos = -1;
					for(int i2 = 0; i2 < diff.count(); i2++)
					{
						if((lastMistakePos != -1) && (diff[i2].position() / charCount == lastMistakePos / charCount))
							diff[i2].setEnabled(false);
						else
							lastMistakePos = diff[i2].position();
					}
				}
				bool merged = differences[i].merged();
				// Translate mistake position
				for(int i2 = 0; i2 < diff.count(); i2++)
				{
					auto currentMistake = diff[i2];
					if((currentMistake.type() == MistakeRecord::Type_Deletion) && (currentMistake.previousText() == " ") && !merged)
						currentMistake.setPosition(currentMistake.position() - 1);
					currentMistake.setPosition(wordStart + currentMistake.position());
					diff[i2] = currentMistake;
				}
				out += diff;
				pos = wordStart + inputWords[i].length();
			}
			else if(differences[i].type() == MistakeRecord::Type_Deletion)
			{
				auto currentMistake = differences[i];
				currentMistake.setPosition(pos > 0 && !inputWords[i][0].isPunct() ? pos - 1 : pos);
				QString previous = currentMistake.previousText();
				if((i > 0) && (previous != " ") && !previous[0].isPunct())
				{
					if(inputWords[i - 1] == "\n")
						currentMistake.setPreviousText(previous.prepend("\n"));
					else
						currentMistake.setPreviousText(previous.prepend(" "));
				}
				out += currentMistake;
				pos += inputWords[i].length();
			}
			else if(differences[i].type() == MistakeRecord::Type_Addition)
			{
				int k = i, previousPos = -1;
				bool disable = false;
				do
				{
					if(k > i)
					{
						pos++;
						if(inputWords[k][0].isPunct())
							pos--;
						else if((k > 0) && (inputWords[k - 1] == " "))
							pos--;
						if(inputWords[k] == " ")
							pos--;
						else if(((k > 0) && inputWords[k - 1][0].isPunct()) && !inputWords[k][0].isPunct())
							pos--;
						for(int l = 0; l < pos - previousPos; l++)
						{
							auto currentMistake = differences[k];
							currentMistake.setPosition(previousPos + l);
							currentMistake.setEnabled(!disable);
							disable = true;
							out += currentMistake;
						}
					}
					if(inputWords[k].length() == 0)
					{
						auto currentMistake = differences[k];
						currentMistake.setPosition(pos);
						currentMistake.setEnabled(!disable);
						disable = true;
						out += currentMistake;
					}
					else
					{
						for(int j = 0; j < inputWords[k].length(); j++)
						{
							auto currentMistake = differences[k];
							currentMistake.setPosition(pos);
							currentMistake.setEnabled(!disable);
							disable = true;
							out += currentMistake;
							pos++;
						}
					}
					previousPos = pos;
					k++;
				} while((differences.contains(k)) && (differences[k].type() == MistakeRecord::Type_Addition));
				i = k - 1;
				// TODO: Should we count hits in added words?
			}
		}
		else if(inputWords[i] != "\n")
		{
			for(int i2 = 0; i2 < inputWords[i].length(); i2++)
			{
				if(pos < recordedCharacters.count())
					hits += recordedCharacters[pos].hits();
				pos++;
			}
		}
	}
	if(totalHits)
		*totalHits = hits;
	// Merge mistakes with the same position
	QMap<int, MistakeRecord *> mistakesMap;
	for(int i = 0; i < out.count(); i++)
	{
		int pos = out[i].position();
		if(mistakesMap.contains(pos))
			mistakesMap[pos]->setPreviousText(mistakesMap[pos]->previousText() + out[i].previousText());
		else
			mistakesMap[pos] = &out[i];
	}
	QList<MistakeRecord> finalList;
	for(int i = 0; i < mistakesMap.keys().count(); i++)
		finalList.append(*mistakesMap[mistakesMap.keys().at(i)]);
	return finalList;
}

/*! Validates a typing test. */
QList<MistakeRecord> ExerciseValidator::validateExercise(QString exerciseText, QString inputText, QVector<CharacterRecord> recordedCharacters, int *totalHits, int *mistakeCount, QStringList *errorWords, bool timed, int timeSecs)
{
	QList<MistakeRecord> recordedMistakes;
	if(timed)
	{
		QMap<int, QList<MistakeRecord>> attempts;
		QString newText = "";
		int minValue = -1, minId = -1;
		if(exerciseText.length() > 0)
		{
			int pos = 0, count = timeSecs * 10;
			for(int i = 0; i < count; i++)
			{
				newText += exerciseText[pos];
				pos++;
				if(pos >= exerciseText.length())
				{
					newText.remove(newText.length() - 1, 1);
					pos = 0;
				}
				if((i % std::max(exerciseText.length(), inputText.length()) == 0) || (i == exerciseText.length() - 1) || (i + 1 >= count))
				{
					attempts[i] = findMistakes(newText, inputText, recordedCharacters, totalHits, errorWords);
					if((minValue == -1) || (attempts[i].count() < minValue))
					{
						minValue = attempts[i].count();
						minId = i;
					}
				}
			}
			if(minId == -1)
				recordedMistakes = QList<MistakeRecord>();
			else
				recordedMistakes = attempts[minId];
		}
		else
			recordedMistakes = QList<MistakeRecord>();
	}
	else
		recordedMistakes = findMistakes(exerciseText, inputText, recordedCharacters, totalHits, errorWords);
	QList<MistakeRecord> mistakesToRemove;
	*mistakeCount = 0;
	for(int i = 0; i < recordedMistakes.count(); i++)
	{
		if(recordedMistakes[i].position() >= inputText.length())
			mistakesToRemove += recordedMistakes[i];
		else
		{
			if(recordedMistakes[i].enabled())
				*mistakeCount = (*mistakeCount) + 1;
		}
	}
	for(int i = 0; i < mistakesToRemove.count(); i++)
		recordedMistakes.removeAll(mistakesToRemove[i]);
	return recordedMistakes;
}

/*! Adds mistakes to an exercise with mistake correction enabled. */
QString ExerciseValidator::addMistakes(QString exerciseText, QList<MistakeRecord *> recordedMistakes)
{
	QMap<int, int> mistakesMap;
	for(int i = 0; i < recordedMistakes.count(); i++)
		mistakesMap[recordedMistakes.at(i)->position()] = i;
	int delta = 0;
	QString out;
	for(int i = 0; i <= exerciseText.length(); i++)
	{
		if(mistakesMap.contains(i))
		{
			MistakeRecord *currentMistake = recordedMistakes.at(mistakesMap[i]);
			out += currentMistake->previousText();
			currentMistake->setPosition(currentMistake->position() + delta);
			if((i < exerciseText.length()) && (exerciseText[i] == '\n'))
			{
				out += "\n";
				delta++;
			}
		}
		else
			out += i < exerciseText.length() ? QString(exerciseText[i]) : QString();
	}
	return out;
}

/*! Splits each word by punctuation marks. */
QStringList ExerciseValidator::splitWordsByPunct(QStringList source)
{
	QStringList out;
	for(int i = 0; i < source.count(); i++)
	{
		QString part = "";
		bool ignore = (source[i].length() > 0);
		for(int j = 0; j < source[i].length(); j++)
		{
			if(source[i][j].isPunct())
			{
				if(!ignore)
					out += part;
				ignore = true;
				part = "";
				if(j == 0)
					out += " ";
				out += QString(source[i][j]);
				if(j + 1 == source[i].length())
					out += " ";
			}
			else
			{
				part += source[i][j];
				ignore = false;
			}
		}
		if(ignore)
			ignore = false;
		else
			out += part;
	}
	return out;
}
