/*
 * AddonsModule.cpp
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
#include "AddonsModule.h"
#include "internal/AddonButton.h"
#include "internal/AddonApi.h"
#include "internal/AddonManager.h"

std::string AddonsModule::moduleName() const
{
	return "addons";
}

void AddonsModule::registerExports()
{
	modularity::ioc()->registerExport<IAddonManager>(AddonManager::instance());
	modularity::ioc()->registerExport<IAddonApi>(AddonApi::instance());
}

void AddonsModule::registerResources()
{
	Q_INIT_RESOURCE(addons);
}

void AddonsModule::registerUiTypes()
{
	QQmlEngine::setObjectOwnership(AddonApi::instance().get(), QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<AddonApi>("OpenTyper.Addons", 1, 0, "AddonApi", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return AddonApi::instance().get();
	});
	QQmlEngine::setObjectOwnership(AddonManager::instance().get(), QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<AddonManager>("OpenTyper.Addons", 1, 0, "AddonManager", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return AddonManager::instance().get();
	});
	qmlRegisterType<AddonItemModel>("OpenTyper.Addons", 1, 0, "AddonItemModel");
	qmlRegisterType<AddonListModel>("OpenTyper.Addons", 1, 0, "AddonListModel");
	qmlRegisterType<AddonModel>("OpenTyper.Addons", 1, 0, "AddonModel");
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qmlRegisterUncreatableMetaObject(AddonButton::staticMetaObject, "OpenTyper.Addons", 1, 0, "AddonButton", "Please use AddonApi to create buttons");
#else
	qmlRegisterUncreatableType<AddonButton>("OpenTyper.Addons", 1, 0, "AddonButton", "Please use AddonApi to create buttons");
#endif
	qRegisterMetaType<QList<AddonButton *>>();
}

void AddonsModule::onPreInit()
{
	AddonManager::instance()->init();
	AddonManager::instance()->loadAddons();
	AddonManager::instance()->getAddonUpdates();
	addonsLoaded = true;
}

void AddonsModule::onInit()
{
	if(!addonsLoaded)
		AddonManager::instance()->loadAddons();
}

void AddonsModule::onDeinit()
{
	AddonManager::instance()->unloadAddons();
	addonsLoaded = false;
}
