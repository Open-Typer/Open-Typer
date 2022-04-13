/*
 * language.cpp
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

#include "core/language.h"

QVector<QTranslator*> translators;

/*! Constructs languageManager. */
languageManager::languageManager(QObject *parent) :
	QObject(parent)
{
	supportedLanguages.clear();
	supportedCountries.clear();
	// Supported languages
	// de_DE
	supportedLanguages += QLocale::German;
	supportedCountries += QLocale::Germany;
	// en_US
	supportedLanguages += QLocale::English;
	supportedCountries += QLocale::UnitedStates;
	// sk_SK
	supportedLanguages += QLocale::Slovak;
	supportedCountries += QLocale::Slovakia;
	// QStringList for combo boxes and list widgets
	boxItems.clear();
	for(int i=0; i < supportedLanguages.count(); i++)
		boxItems += QLocale::languageToString(supportedLanguages[i]) + " (" + QLocale::countryToString(supportedCountries[i]) + ")";
	boxItems.insert(0,tr("System (default)"));
}

/*! Destroys the languageManager object. */
languageManager::~languageManager() { }

/*! Changes the application language. */
void languageManager::setLanguage(int index)
{
	QLocale targetLocale;
	if(index == -1)
		targetLocale = QLocale::system();
	else
		targetLocale = QLocale(supportedLanguages[index], supportedCountries[index]);
	for(int i=0; i < translators.count(); i++)
	{
		QCoreApplication::removeTranslator(translators[i]);
		translators[i]->deleteLater();
	}
	translators.clear();
	QTranslator *translator = new QTranslator;
	if(translator->load(targetLocale, "Open-Typer", "_", ":/res/lang"))
	{
		QCoreApplication::installTranslator(translator);
		translators += translator;
	}
	translator = new QTranslator;
	if(translator->load(targetLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath) + "/qtbase_"))
	{
		QCoreApplication::installTranslator(translator);
		translators += translator;
	}
}
