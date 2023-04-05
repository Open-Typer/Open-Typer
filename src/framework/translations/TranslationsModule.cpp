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
#include "LanguageManager.h"
#include "global/Settings.h"

std::string TranslationsModule::moduleName() const
{
	return "translations";
}

void TranslationsModule::registerUiTypes()
{
	QQmlEngine::setObjectOwnership(&globalLanguageManager, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<LanguageManager>("OpenTyper", 1, 0, "LanguageManager", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalLanguageManager;
	});
}

void TranslationsModule::onPreInit()
{
	globalLanguageManager.init();
	if(Settings::language() == "")
		globalLanguageManager.setLanguage(-1);
	else
		globalLanguageManager.setLanguage(globalLanguageManager.getBoxItems().indexOf(Settings::language()) - 1);
}
