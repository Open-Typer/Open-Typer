/*
 * utils.h
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

#include <iostream>
#include <QObject>
#include <QString>
#include <QStandardPaths>
#ifdef Q_OS_WINDOWS
#include <windows.h>
#endif

class stringUtils
{
	public:
		static int wordCount(QString str);
};

bool int_array_contains(int arr[], int target, int count);
int _get_word_count(char *str) __attribute__ ((deprecated));
char *_get_word(char *str, int id);
QString getConfigLoc(void);
QString parseDesc(QString desc);
QString _sublesson_text(int id);
QString _init_level(QString level, int levelLengthExtension);
#ifdef Q_OS_WINDOWS
WCHAR* str_to_wcs(const char* str);
#endif
