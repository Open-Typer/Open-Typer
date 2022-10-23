/*
 * LanguageManager.h
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

#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QLocale>
#include <QVector>
#include <QTranslator>
#include <QCoreApplication>
#include <QLibraryInfo>

extern QTranslator *translator1;
extern QTranslator *translator2;

/*!
 * \brief The LanguageManager class provides a list of supported languages.
 *
 * \code
 * LanguageManager langMgr;
 * QList<QLocale::Language> languages = langMgr.supportedLanguages; // list of supported languages
 * QList<QLocale::Country> countries = langMgr.supportedCountries; // list of supported countries (every language has a country, e. g. en_US and en_GB)
 * QStringList items = langMgr.boxItems; // list of languages, which can be used in selector widgets
 * \endcode
 *
 * \see LanguageList
 */
class CORE_LIB_EXPORT LanguageManager : public QObject
{
		Q_OBJECT
	public:
		explicit LanguageManager(QObject *parent = nullptr);
		virtual ~LanguageManager();
		void setLanguage(int index);
		QList<QLocale::Language> supportedLanguages;
		QList<QLocale::Country> supportedCountries;
		QStringList boxItems;
};

#endif // LANGUAGEMANAGER_H
