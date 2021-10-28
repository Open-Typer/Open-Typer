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

#include "utils.h"

int QStringLen(QString str)
{
	int i=0;
	while(str[i] != '\0')
		i++;
	return i;
}
bool int_array_contains(int arr[], int target, int count)
{
	int i;
	for(i=0; i < count; i++)
	{
		if(arr[i] == target)
			return true;
	}
	return false;
}
int _get_word_count(char *str)
{
	unsigned int i;
	int out=0;
	for(i=0; i < strlen(str); i++)
	{
		if(str[i] == ' ')
			out++;
	}
	if(str[i] != ' ')
		out++;
	return out;
}
char *_get_word(char *str, int id)
{
	unsigned int i;
	int cur=0;
	char *out = (char*) malloc(strlen(str));
	strcpy(out,"");
	for(i=0; i < strlen(str); i++)
	{
		if((str[i] == ' ') || (i+1 == strlen(str)))
		{
			cur++;
			if(i+1 == strlen(str))
				strncat(out,&str[i],1);
			if(cur == id)
				return out;
			strcpy(out,"");
		}
		else
			strncat(out,&str[i],1);
	}
	return NULL;
}
QString getConfigLoc(void)
{
	return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Open-Typer";
}
QString parseDesc(QString desc, QString trRevision)
{
	QString out = "";
	int i;
	bool bracket=false;
	for(i=0; i < QStringLen(desc); i++)
	{
		if(desc[i] == '%')
		{
			i++;
			if(bracket)
				out += '}';
			if(desc[i] == 'r')
				out += trRevision;
			else if(desc[i] == '%')
				out += '%';
			// %b is reserved (it's used to separate 2 sets)
			bracket=false;
		}
		else
		{
			if(!bracket)
			{
				out += '{';
				bracket=true;
			}
			out += desc[i];
		}
	}
	if(bracket)
		out += '}';
	return out;
}
