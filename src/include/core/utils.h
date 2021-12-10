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

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QString>
#include <QStandardPaths>
#include <QKeyEvent>
#include <QSettings>

class stringUtils
{
	public:
		static int wordCount(QString str);
		static QString word(QString str, int id);
};

/*! \brief The fileUtils class contains functions for file or directory paths. */
class fileUtils
{
	public:
		static QString configLocation(void);
		static QString mainSettingsLocation(void);
};

class keyboardUtils
{
	public:
		static bool isSpecialKey(QKeyEvent *event);
		static bool isDeadKey(int key);
};

#endif // UTILS_H
