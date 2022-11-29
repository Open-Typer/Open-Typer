/*
 * LanguageManager.cpp
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

#include "LanguageManager.h"

QTranslator *translator1 = nullptr;
QTranslator *translator2 = nullptr;
QTranslator *translator3 = nullptr;

/*! Constructs LanguageManager. */
LanguageManager::LanguageManager(QObject *parent) :
	QObject(parent)
{
	for (const auto& [lang, country] : supportedLanguagesList)
	{
		boxItems += boxLangItemTemplate.arg(QLocale::languageToString(lang), QLocale::countryToString(country));
	}
	boxItems.insert(0, tr("System (default)"));
}

/*! Destroys the LanguageManager object. */
LanguageManager::~LanguageManager() { }

/*! Changes the application language. */
void LanguageManager::setLanguage(int index)
{
	QLocale targetLocale;
	if(index < 0)
		targetLocale = QLocale::system();
	else
	{
		const auto [lang, country] = supportedLanguagesList.at(index);
		targetLocale = QLocale(lang, country);
	}
	if(!translator1)
	{
		translator1 = new QTranslator(qApp);
		QCoreApplication::installTranslator(translator1);
	}
	if(!translator2)
	{
		translator2 = new QTranslator(qApp);
		QCoreApplication::installTranslator(translator2);
	}
	if(!translator3)
	{
		translator3 = new QTranslator(qApp);
		QCoreApplication::installTranslator(translator3);
	}
	translator1->load(targetLocale, "Open-Typer", "_", ":/res/lang");
	translator2->load(targetLocale, "libcore", "_", ":/res/lang");
	translator3->load(targetLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath) + "/qtbase_");
	globalThemeEngine.updateThemeList();
}

const QString LanguageManager::boxLangItemTemplate = QString("%1 (%2)");

const QList<LanguageManager::LanguageCountry> LanguageManager::supportedLanguagesList =
{
	{ QLocale::German,  QLocale::Germany      },
	{ QLocale::English, QLocale::UnitedStates },
	{ QLocale::Slovak,  QLocale::Slovakia     },
	{ QLocale::Russian, QLocale::Russia       },
};
