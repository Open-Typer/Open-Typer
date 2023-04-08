/*
 * IStringUtils.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

#ifndef ISTRINGUTILS_H
#define ISTRINGUTILS_H

#include <QtGlobal>
#include "modularity/ioc.h"

class IStringUtils : MODULE_EXPORT_INTERFACE
{
	public:
		virtual int wordCount(QString str) = 0;
		virtual int charCount(QString str, QChar ch) = 0;
		virtual QString word(QString str, int id) = 0;
		virtual QString wordAt(QString str, int index) = 0;
		virtual QString repeatString(QString string, int n) = 0;
		virtual QList<QVariant> longestCommonSubsequence(QList<QVariant> source, QList<QVariant> target) = 0;
		virtual QString longestCommonSubsequence(QString source, QString target) = 0;
		virtual QString normalizeString(QString str) = 0;
};

#endif // ISTRINGUTILS_H
