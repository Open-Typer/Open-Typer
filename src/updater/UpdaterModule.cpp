/*
 * UpdaterModule.cpp
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
#include "UpdaterModule.h"
#include "global/ISettings.h"
#ifdef Q_OS_WINDOWS
#include "platform/windows/WindowsUpdater.h"
using PlatformUpdater = WindowsUpdater;
#else
#include "platform/stub/StubUpdater.h"
using PlatformUpdater = StubUpdater;
#endif

std::string UpdaterModule::moduleName() const
{
	return "updater";
}

void UpdaterModule::registerExports()
{
	modularity::ioc()->registerExport<IUpdater>(PlatformUpdater::instance());
}

void UpdaterModule::registerResources()
{
	Q_INIT_RESOURCE(updater);
}

void UpdaterModule::initSettings()
{
	INIT_SETTINGS_KEY("updateChecks", "main/updatechecks", true);
}

void UpdaterModule::registerUiTypes()
{
	QQmlEngine::setObjectOwnership(PlatformUpdater::instance().get(), QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<PlatformUpdater>("OpenTyper.Updater", 1, 0, "Updater", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return PlatformUpdater::instance().get();
	});
}
