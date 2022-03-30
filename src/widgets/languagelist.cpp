/*
 * languagelist.cpp
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

#include "widgets/languagelist.h"

/*! Constructs languageList. */
languageList::languageList(QWidget *parent) :
	QListWidget(parent),
	settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat)
{
	clear();
	addItems(langMgr.boxItems);
	if(settings.value("main/language","").toString() == "")
		setCurrentRow(0);
	else
		setCurrentRow(langMgr.boxItems.indexOf(settings.value("main/language","").toString()));
	connect(this,SIGNAL(currentRowChanged(int)),this,SLOT(changeLanguage(int)));
}

/*! Destroys the languageList object. */
languageList::~languageList() { }

/*!
 * Connected from languageList#currentRowChanged() (this object).
 * Changes display language in program settings and emits languageChanged().
 * \see languageChanged()
 */
void languageList::changeLanguage(int index)
{
	if(index == 0)
		settings.setValue("main/language","");
	else
		settings.setValue("main/language",langMgr.boxItems[index]);
	langMgr.setLanguage(index-1);
}
