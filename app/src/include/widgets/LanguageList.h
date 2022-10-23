/*
 * LanguageList.h
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

#ifndef LANGUAGELIST_H
#define LANGUAGELIST_H

#include <QWidget>
#include <QListWidget>
#include "StringUtils.h"
#include "LanguageManager.h"

/*!
 * \brief The LanguageList class is a QListWidget, which provides a language selector.
 *
 * \image html LanguageList.png
 * \see LanguageManager
 * \see OptionsWindow
 */
class LanguageList : public QListWidget
{
		Q_OBJECT
	public:
		explicit LanguageList(QWidget *parent = nullptr);
		~LanguageList();
		QLocale::Language language(int index);
		QLocale::Country country(int index);

	private:
		LanguageManager langMgr;

	private slots:
		void changeLanguage(int index);
};

#endif // LANGUAGELIST_H
