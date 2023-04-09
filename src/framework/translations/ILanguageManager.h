/*
 * ILanguageManager.h
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

#ifndef ILANGUAGEMANAGER_H
#define ILANGUAGEMANAGER_H

#include <QLocale>
#include "global/modularity/ioc.h"

class ILanguageManager : public QObject, MODULE_EXPORT_INTERFACE
{
		Q_OBJECT
	public:
		virtual ~ILanguageManager() { }

		virtual void setLanguage(int index) = 0;
		virtual QStringList getBoxItems() = 0;
		virtual QLocale getLocale(int index) = 0;

		virtual QString languageStr(void) = 0;

	signals:
		void languageChanged(void);
		void languageStrChanged();
};

#endif // ILANGUAGEMANAGER_H
