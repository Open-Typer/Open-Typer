/*
 * TranslationsModule.cpp
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

#include <QQmlEngine>
#include "TranslationsModule.h"
#include "internal/LanguageManager.h"

static const QString module = "translations";
static const ISettings::Key LANGUAGE(module, "language");

std::string TranslationsModule::moduleName() const
{
	return "translations";
}

void TranslationsModule::registerExports()
{
	modularity::ioc()->registerExport<ILanguageManager>(LanguageManager::instance());
}

void TranslationsModule::initSettings()
{
	INIT_SETTINGS_KEY("language", "main/language", "");
}

void TranslationsModule::registerUiTypes()
{
	QQmlEngine::setObjectOwnership(LanguageManager::instance().get(), QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<LanguageManager>("OpenTyper.Translations", 1, 0, "LanguageManager", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return LanguageManager::instance().get();
	});
}

void TranslationsModule::onPreInit()
{
	auto langMgr = LanguageManager::instance();
	langMgr->init();
	QString language = settings()->getValue(LANGUAGE).toString();
	if(language == "")
		langMgr->setLanguage(-1);
	else
		langMgr->setLanguage(langMgr->getBoxItems().indexOf(language) - 1);
	settings()->connect(settings().get(), &ISettings::discarded, [langMgr]() {
		QString language = settings()->getValue(LANGUAGE).toString();
		langMgr->setLanguage(langMgr->getBoxItems().indexOf(language) - 1);
	});
}
