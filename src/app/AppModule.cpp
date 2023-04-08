/*
 * AppModule.cpp
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
#include "AppModule.h"
#include "AppMenuBar.h"
#include "settings/SettingsCategory.h"
#include "global/ISettings.h"

std::string AppModule::moduleName() const
{
	return "app";
}

void AppModule::initSettings()
{
	INIT_SETTINGS_KEY("windowX", "main/windowX", 0);
	INIT_SETTINGS_KEY("windowY", "main/windowY", 0);
	INIT_SETTINGS_KEY("windowWidth", "main/windowWidth", 1200);
	INIT_SETTINGS_KEY("windowHeight", "main/windowHeight", 800);
	INIT_SETTINGS_KEY("windowMaximized", "main/windowMaximized", true);
	INIT_SETTINGS_KEY("lessonPack", "main/configfile", "");
	INIT_SETTINGS_KEY("customLessonPack", "main/customconfig", false);
	INIT_SETTINGS_KEY("errorPenalty", "main/errorpenalty", 10);
	INIT_SETTINGS_KEY("initFinished", "main/initfinished", false);
}

void AppModule::registerUiTypes()
{
	QQmlEngine::setObjectOwnership(&globalMenuBar, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<AppMenuBar>("OpenTyper", 1, 0, "AppMenuBar", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalMenuBar;
	});
	qmlRegisterType<SettingsCategory>("OpenTyper", 1, 0, "SettingsCategory");
}

void AppModule::setRootContextProperties(QQmlContext *context)
{
	context->setContextProperty("BuiltInPacks", &m_builtInPacks);
}

void AppModule::onPreInit()
{
	globalMenuBar.init();
}
