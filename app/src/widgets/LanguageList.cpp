/*
 * LanguageList.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
 * Copyright (C) 2022 - Roker2
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

#include "widgets/LanguageList.h"

/*! Constructs LanguageList. */
LanguageList::LanguageList(QWidget *parent) :
	QListWidget(parent)
{
	clear();
	addItems(langMgr.getBoxItems());
	if(Settings::language() == "")
		setCurrentRow(0);
	else
		setCurrentRow(langMgr.getBoxItems().indexOf(Settings::language()));
	connect(this, SIGNAL(currentRowChanged(int)), this, SLOT(changeLanguage(int)));
}

/*! Destroys the LanguageList object. */
LanguageList::~LanguageList() { }

/*!
 * Connected from LanguageList#currentRowChanged() (this object).
 * Changes display language in program settings and emits languageChanged().
 * \see languageChanged()
 */
void LanguageList::changeLanguage(int index)
{
	if(index == 0)
		Settings::setLanguage("");
	else
		Settings::setLanguage(langMgr.getBoxItems()[index]);
	langMgr.setLanguage(index - 1);
}
