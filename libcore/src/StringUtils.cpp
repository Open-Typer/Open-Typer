/*
 * StringUtils.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2023 - adazem009
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

#include "StringUtils.h"

/*! Returns number of words in the string. */
int StringUtils::wordCount(QString str)
{
	int i, out = 0;
	for(i = 0; i < str.count(); i++)
	{
		if(str[i] == ' ')
			out++;
	}
	if(str[i - 1] != ' ')
		out++;
	return out;
}

/*! Returns count of the given character in the string. */
int StringUtils::charCount(QString str, QChar ch)
{
	return str.count(ch);
}

/*! Returns a word in the string. */
QString StringUtils::word(QString str, int id)
{
	int i, cur = 0;
	QString out = "";
	for(i = 0; i < str.count(); i++)
	{
		if((str[i] == ' ') || (i + 1 == str.count()))
		{
			cur++;
			if(i + 1 == str.count())
				out += str[i];
			if(cur == id)
				return out;
			out = "";
		}
		else
			out += str[i];
	}
	return "";
}

/*! Returns the word at index. */
QString StringUtils::wordAt(QString str, int index)
{
	if(str[index] == ' ')
		return "";
	QString currentWord = "";
	for(int i = 0; i < str.count(); i++)
	{
		if((str[i] == ' ') || (i + 1 >= str.count()))
		{
			if(i + 1 >= str.count())
				currentWord += str[i];
			if(i > index)
				return currentWord;
			currentWord = "";
		}
		else
			currentWord += str[i];
	}
	return "";
}

/*! Returns repeated string. Same as string.repeated(n). */
QString StringUtils::repeatString(QString string, int n)
{
	return string.repeated(n);
}

/*! Returns the length of the longest common subsequence of source and target list. */
int StringUtils::lcsLen(QList<QVariant> source, QList<QVariant> target)
{
	QMap<int, QMap<int, int>> table;
	return lcsLen(source, target, &table);
}

int StringUtils::lcsLen(QList<QVariant> source, QList<QVariant> target, QMap<int, QMap<int, int>> *lcsTable)
{
	/*
	* References:
	* https://www.geeksforgeeks.org/printing-longest-common-subsequence/
	* https://nasauber.de/blog/2019/levenshtein-distance-and-longest-common-subsequence-in-qt/
	*/
	QMap<int, QMap<int, int>> l;
	for(int i = 0; i <= source.count(); i++)
	{
		for(int j = 0; j <= target.count(); j++)
		{
			if(i == 0 || j == 0)
				l[i][j] = 0;
			else if(source.at(i - 1) == target.at(j - 1))
				l[i][j] = l[i - 1][j - 1] + 1;
			else
				l[i][j] = std::max(l[i - 1][j], l[i][j - 1]);
		}
	}
	*lcsTable = l;
	return l[source.count()][target.count()];
}

/*! Returns the longest common subsequence of source and target list. */
QList<QVariant> StringUtils::longestCommonSubsequence(QList<QVariant> source, QList<QVariant> target)
{
	QMap<int, QMap<int, int>> l;
	lcsLen(source, target, &l);
	int i = source.count();
	int j = target.count();
	QList<QVariant> longestCommonSubsequence;
	while(i > 0 && j > 0)
	{
		if(i == 0)
			j--;
		else if(j == 0)
			i--;
		else if(source.at(i - 1) == target.at(j - 1))
		{
			longestCommonSubsequence.prepend(source.at(i - 1));
			i--;
			j--;
		}
		//else if (l[i][j] == l[i-1][j])
		else if(l[i - 1][j] > l[i][j - 1]) // This method works better for switched characters (e. g. abcd -> acbd)
			i--;
		else
			j--;
	}
	return longestCommonSubsequence;
}

/*! Returns the longest common subsequence of source and target string. */
QString StringUtils::longestCommonSubsequence(QString source, QString target)
{
	QList<QVariant> sourceList, targetList;
	int i;
	for(i = 0; i < source.count(); i++)
		sourceList += QString(source[i]);
	for(i = 0; i < target.count(); i++)
		targetList += QString(target[i]);
	QList<QVariant> lcs = longestCommonSubsequence(sourceList, targetList);
	QString out = "";
	for(i = 0; i < lcs.count(); i++)
		out += lcs[i].toString();
	return out;
}

/*! Compares 2 lists using longest common subsequence. */
QList<MistakeRecord> StringUtils::compareLists(QList<QVariant> source, QList<QVariant> target, QVector<CharacterRecord> *recordedCharacters, int *hits, int *inputPos)
{
	QList<MistakeRecord> out;
	auto lcs = longestCommonSubsequence(source, target);
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
QList<MistakeRecord> StringUtils::compareStrings(QString source, QString target, QVector<CharacterRecord> *recordedCharacters, int *hits, int *inputPos)
{
	QList<QVariant> sourceList, targetList;
	int i;
	for(i = 0; i < source.count(); i++)
		sourceList += QString(source[i]);
	for(i = 0; i < target.count(); i++)
		targetList += QString(target[i]);
	return compareLists(sourceList, targetList, recordedCharacters, hits, inputPos);
}

/*! Splits each word by punctuation marks. */
QStringList StringUtils::splitWordsByPunct(QStringList source)
{
	QStringList out;
	for(int i = 0; i < source.count(); i++)
	{
		QString part = "";
		bool ignore = (source[i].count() > 0);
		for(int j = 0; j < source[i].count(); j++)
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
				if(j + 1 == source[i].count())
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

/*! Recursively generates a diff list from source and target word list. */
QMap<int, MistakeRecord> StringUtils::generateDiffList(QStringList *sourceWords, QStringList *targetWords, QList<int> *mergeList)
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
QList<MistakeRecord> StringUtils::findMistakes(QString exerciseText, QString input, QVector<CharacterRecord> recordedCharacters, int *totalHits, QStringList *errorWords)
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
				if(Settings::mistakeLimit())
				{
					int charCount = Settings::mistakeChars();
					int lastMistakePos = -1;
					for(int i2 = 0; i2 < diff.count(); i2++)
					{
						if((lastMistakePos != -1) && (diff[i2].position() / charCount == lastMistakePos / charCount))
							diff[i2].setEnabled(false);
						else
							lastMistakePos = diff[i2].position();
					}
				}
				bool merged = differences[i].isMerged();
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
				pos = wordStart + inputWords[i].count();
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
				pos += inputWords[i].count();
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
					if(inputWords[k].count() == 0)
					{
						auto currentMistake = differences[k];
						currentMistake.setPosition(pos);
						currentMistake.setEnabled(!disable);
						disable = true;
						out += currentMistake;
					}
					else
					{
						for(int j = 0; j < inputWords[k].count(); j++)
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
			for(int i2 = 0; i2 < inputWords[i].count(); i2++)
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
QList<MistakeRecord> StringUtils::validateExercise(QString exerciseText, QString inputText, QVector<CharacterRecord> recordedCharacters, int *totalHits, int *mistakeCount, QStringList *errorWords, bool timed, int timeSecs)
{
	QList<MistakeRecord> recordedMistakes;
	if(timed)
	{
		QMap<int, QList<MistakeRecord>> attempts;
		QString newText = "";
		int minValue = -1, minId = -1;
		if(exerciseText.count() > 0)
		{
			int pos = 0, count = timeSecs * 10;
			for(int i = 0; i < count; i++)
			{
				newText += exerciseText[pos];
				pos++;
				if(pos >= exerciseText.count())
				{
					newText.remove(newText.count() - 1, 1);
					pos = 0;
				}
				if((i % std::max(exerciseText.count(), inputText.count()) == 0) || (i == exerciseText.count() - 1) || (i + 1 >= count))
				{
					attempts[i] = StringUtils::findMistakes(newText, inputText, recordedCharacters, totalHits, errorWords);
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
		recordedMistakes = StringUtils::findMistakes(exerciseText, inputText, recordedCharacters, totalHits, errorWords);
	QList<MistakeRecord> mistakesToRemove;
	*mistakeCount = 0;
	for(int i = 0; i < recordedMistakes.count(); i++)
	{
		if(recordedMistakes[i].position() >= inputText.count())
			mistakesToRemove += recordedMistakes[i];
		else
		{
			if(recordedMistakes[i].isEnabled())
				*mistakeCount = (*mistakeCount) + 1;
		}
	}
	for(int i = 0; i < mistakesToRemove.count(); i++)
		recordedMistakes.removeAll(mistakesToRemove[i]);
	return recordedMistakes;
}

/*! Adds mistakes to an exercise with mistake correction enabled. */
QString StringUtils::addMistakes(QString exerciseText, QList<MistakeRecord *> recordedMistakes)
{
	QMap<int, int> mistakesMap;
	for(int i = 0; i < recordedMistakes.count(); i++)
		mistakesMap[recordedMistakes.at(i)->position()] = i;
	int delta = 0;
	QString out;
	for(int i = 0; i <= exerciseText.count(); i++)
	{
		if(mistakesMap.contains(i))
		{
			MistakeRecord *currentMistake = recordedMistakes.at(mistakesMap[i]);
			out += currentMistake->previousText();
			currentMistake->setPosition(currentMistake->position() + delta);
			if(exerciseText[i] == '\n')
			{
				out += "\n";
				delta++;
			}
		}
		else
			out += i < exerciseText.count() ? QString(exerciseText[i]) : QString();
	}
	return out;
}

/*! Normalizes the given string. */
QString StringUtils::normalizeString(QString str)
{
	return str.normalized(QString::NormalizationForm_D);
}
