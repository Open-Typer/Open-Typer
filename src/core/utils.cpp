/*
 * utils.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

QString fileUtils::configLocation(void)
{
	return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Open-Typer";
}

QString fileUtils::mainSettingsLocation(void)
{
	return configLocation()+"/config.ini";
}

bool keyboardUtils::isSpecialKey(QKeyEvent *event)
{
	if(event->text() == "")
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

bool keyboardUtils::isDeadKey(int key)
{
	return ((key >= Qt::Key_Dead_Grave) && (key <= Qt::Key_Dead_Longsolidusoverlay));
}
