/*
 * languagelist.h
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

#ifndef LANGUAGECOMBOBOX_H
#define LANGUAGECOMBOBOX_H

#include <QWidget>
#include <QListWidget>
#include <QSettings>
#include "core/utils.h"
#include "core/language.h"

class languageList : public QListWidget
{
	Q_OBJECT
	public:
		explicit languageList(QWidget *parent = nullptr);
		~languageList();
		QLocale::Language language(int index);
		QLocale::Country country(int index);
		
	private:
		languageManager *langMgr;
		QSettings *settings;

	signals:
		void languageChanged(int index);

	private slots:
		void changeLanguage(int index);
};

#endif // LANGUAGECOMBOBOX_H
