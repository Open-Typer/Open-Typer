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
	char *out = (char*) malloc(strlen(str)+1);
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
	return (char*) "";
}
QString getConfigLoc(void)
{
	return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Open-Typer";
}
QString parseDesc(QString desc)
{
	QString out = "";
	int i;
	bool bracket=false;
	for(i=0; i < desc.count(); i++)
	{
		if(desc[i] == '%')
		{
			i++;
			if(bracket)
				out += '}';
			if(desc[i] == 'r')
				out += QObject::tr("Revision");
			else if(desc[i] == '%')
				out += '%';
			// %b is reserved (it's used to separate 2 sets)
			if(desc[i] == 's')
			{
				if(!bracket)
				{
					out += '{';
					bracket=true;
				}
				out += QObject::tr("Shift","Shift key");
			}
			else
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
QString _sublesson_text(int id)
{
	switch(id) {
		case 1:
			return QObject::tr("Touch");
			break;
		case 2:
			return QObject::tr("Words");
			break;
		case 3:
			return QObject::tr("Sentences");
			break;
		case 4:
			return QObject::tr("Text");
			break;
		default:
			return QObject::tr("Sublesson") + " " + QString::number(id);
			break;
	}
}
QString _init_level(QString level, int levelLengthExtension)
{
	int len, len2, i, line_pos=0;
	QString out = "";
	len = level.count();
	QString word = "";
	bool first_word=true;
	for(i=0; i < len; i++)
	{
		if((level[i] == ' ') || (i+1 >= len))
		{
			if(i+1 >= len)
			{
				word += level[i];
			}
			len2 = word.count();
			if(line_pos+len2 > levelLengthExtension)
			{
				out += '\n';
				line_pos=0;
			}
			if(!first_word && (line_pos > 0))
			{
				out += ' ';
			}
			first_word=false;
			out += word;
			word = "";
			if(!first_word)
				line_pos++;
			line_pos=line_pos+len2;
		}
		else
		{
			word += level[i];
		}
	}
	return out;
}
#ifdef Q_OS_WINDOWS
WCHAR* str_to_wcs(const char* str)
{
	size_t size = MultiByteToWideChar(CP_UTF8,0,str,-1,NULL,0)*sizeof(WCHAR)+2;
	WCHAR* out = (WCHAR*) malloc(size);
	wcscpy(out,L"");
	MultiByteToWideChar(CP_UTF8,0,&str[0],-1,out,size);
	return out;
}
#endif
