/*
 * utils.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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

#include "core/utils.h"

/*! Returns number of words in the string. */
int stringUtils::wordCount(QString str)
{
	int i, out = 0;
	for(i=0; i < str.count(); i++)
	{
		if(str[i] == ' ')
			out++;
	}
	if(str[i] != ' ')
		out++;
	return out;
}

/*! Returns a word in the string. */
QString stringUtils::word(QString str, int id)
{
	int i, cur = 0;
	QString out = "";
	for(i=0; i < str.count(); i++)
	{
		if((str[i] == ' ') || (i+1 == str.count()))
		{
			cur++;
			if(i+1 == str.count())
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
QString stringUtils::wordAt(QString str, int index)
{
	if(str[index] == ' ')
		return "";
	QString currentWord = "";
	for(int i=0; i < str.count(); i++)
	{
		if((str[i] == ' ') || (i+1 >= str.count()))
		{
			if(i+1 >= str.count())
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

/*! Returns the longest common subsequence of source and target string. */
QString stringUtils::longestCommonSubsequence(QString source, QString target)
{
	/*
	* References:
	* https://www.geeksforgeeks.org/printing-longest-common-subsequence/
	* https://nasauber.de/blog/2019/levenshtein-distance-and-longest-common-subsequence-in-qt/
	*/
	QMap<int, QMap<int, int>> l;
	for (int i = 0; i <= source.count(); i++)
	{
		for (int j = 0; j <= target.count(); j++)
		{
			if (i == 0 || j == 0)
				l[i][j] = 0;
			else if (source.at(i - 1) == target.at(j - 1))
				l[i][j] = l[i - 1][j - 1] + 1;
			else
				l[i][j] = std::max(l[i - 1][j], l[i][j - 1]);
		}
	}
	int i = source.count();
	int j = target.count();
	int index = l[source.count()][target.count()];
	QString longestCommonSubsequence;
	while (i > 0 && j > 0)
	{
		if (source.at(i - 1) == target.at(j - 1))
		{
			longestCommonSubsequence[index - 1] = source.at(i - 1);
			i--;
			j--;
			index--;
		}
		else if (l[i - 1][j] > l[i][j - 1])
			i--;
		else
			j--;
	}
	return longestCommonSubsequence;
}

/*!
 * Returns the path to the program configuration directory.\n
 * For example: <tt>/home/user/.config/Open-Typer</tt>
 */
QString fileUtils::configLocation(void)
{
	return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Open-Typer";
}

/*! Returns the path to the main configuration file, currently \c config.ini in the program configuration directory. */
QString fileUtils::mainSettingsLocation(void)
{
	return configLocation()+"/config.ini";
}

/*!
 * Returns true if the key is a special key.\n
 * Special keys are ignored by OpenTyper#keyPress().
 * \see OpenTyper#keyPress()
 */
bool keyboardUtils::isSpecialKey(QKeyEvent *event)
{
	if((event->text() == "") && (event->key() != Qt::Key_Return) && (event->key() != Qt::Key_Enter))
		return true;
	switch(event->key()) {
		case Qt::Key_Delete:
			return true;
			break;
		case Qt::Key_Backspace:
			return true;
			break;
		case Qt::Key_Backtab:
			return true;
			break;
		case Qt::Key_Escape:
			return true;
			break;
		default:
			return false;
			break;
	}
}

#if QT_VERSION <= QT_VERSION_CHECK(5, 10, 0)
#define LAST_DEAD_KEY Qt::Key_Dead_Horn
#else
#define LAST_DEAD_KEY Qt::Key_Dead_Longsolidusoverlay
#endif

/*! Returns true if the key code belongs to a dead key. */
bool keyboardUtils::isDeadKey(int key)
{
	return ((key >= Qt::Key_Dead_Grave) && (key <= LAST_DEAD_KEY));
}
