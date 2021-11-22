/*
 * configfile.h
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

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include "utils.h"

#define _REPEAT_LIMIT 128

class configParser : public QObject
{
	Q_OBJECT
	public:
		explicit configParser(QObject *parent = nullptr);
		bool open(const QString fileName);
		int lessonCount(void);

	private:
		QFile *configFile;
		int exerciseID(const QString line, const int part);
};

int _lesson_count(FILE *cr) __attribute__ ((deprecated));
int _lesson_sublesson_count(FILE *cr, int tlesson);
int _lesson_sublesson_level_count(FILE *cr, int tlesson, int tsublesson);
int _lesson_sublesson_level_line(FILE *cr, int tlesson, int tsublesson, int tlevel);
char* _lesson_sublesson_level_repeat_type(FILE *cr, int tlesson, int tsublesson, int tlevel);
char *_lesson_desc(FILE *cr, int tlesson);
int _lesson_sublesson_level_limit_extension(FILE *cr, int tlesson, int tsublesson, int tlevel);
int _lesson_sublesson_level_length_extension(FILE *cr, int tlesson, int tsublesson, int tlevel);
char* _lesson_sublesson_level_raw_text(FILE *cr, int tlesson, int tsublesson, int tlevel);
char *_lesson_sublesson_level_text(FILE *cr, int tlesson, int tsublesson, int tlevel);
// Write functions
void _add_level(FILE *cr, int tlesson, int tsublesson, int tlevel, bool repeat, char *repeat_type, int limit_extension, int length_extension, char *desc, const char *level_text);

#endif // CONFIGFILE_H
