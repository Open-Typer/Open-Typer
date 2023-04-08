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

#include <QMap>
#include "StringUtils.h"

/*! Returns number of words in the string. */
int StringUtils::wordCount(QString str)
{
	int i, out = 0;
	for(i = 0; i < str.length(); i++)
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
	for(i = 0; i < str.length(); i++)
	{
		if((str[i] == ' ') || (i + 1 == str.length()))
		{
			cur++;
			if(i + 1 == str.length())
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
	for(int i = 0; i < str.length(); i++)
	{
		if((str[i] == ' ') || (i + 1 >= str.length()))
		{
			if(i + 1 >= str.length())
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
	for(i = 0; i < source.length(); i++)
		sourceList += QString(source[i]);
	for(i = 0; i < target.length(); i++)
		targetList += QString(target[i]);
	QList<QVariant> lcs = longestCommonSubsequence(sourceList, targetList);
	QString out = "";
	for(i = 0; i < lcs.count(); i++)
		out += lcs[i].toString();
	return out;
}

/*! Normalizes the given string. */
QString StringUtils::normalizeString(QString str)
{
	return str.normalized(QString::NormalizationForm_D);
}
