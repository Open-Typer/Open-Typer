/*
 * languagecombobox.cpp
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

#include "widgets/languagecombobox.h"

languageComboBox::languageComboBox(QWidget *parent) :
	QComboBox(parent)
{
	clear();
	supportedLanguages.clear();
	supportedCountries.clear();
	// Supported languages
	// en_US
	supportedLanguages += QLocale::English;
	supportedCountries += QLocale::UnitedStates;
	// sk_SK
	supportedLanguages += QLocale::Slovak;
	supportedCountries += QLocale::Slovakia;
	// Combobox list
	QStringList boxItems;
	for(int i=0; i < supportedLanguages.count(); i++)
	{
		boxItems += QLocale::languageToString(supportedLanguages[i]) + " (" + QLocale::countryToString(supportedCountries[i]) + ")";
	}
	boxItems.sort();
	boxItems.insert(0,tr("System (default)"));
	addItems(boxItems);
}

languageComboBox::~languageComboBox() { }

QLocale::Language languageComboBox::language(int index)
{
	return supportedLanguages[index-1];
}

QLocale::Country languageComboBox::country(int index)
{
	return supportedCountries[index-1];
}
