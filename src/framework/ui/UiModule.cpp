/*
 * UiModule.cpp
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
#include "UiModule.h"
#include "QmlUtils.h"
#include "ThemeEngine.h"
#include "menubar/AppMenuModel.h"
#include "translations/LanguageManager.h"
#include "global/modularity/ioc.h"
#include "internal/UiEngine.h"

std::string UiModule::moduleName() const
{
	return "ui";
}

void UiModule::registerExports()
{
	modularity::ioc()->registerExport<IUiEngine>(UiEngine::instance());
}

void UiModule::registerUiTypes()
{
	qmlRegisterSingletonType<QmlUtils>("OpenTyper.Ui", 1, 0, "QmlUtils", [](QQmlEngine *, QJSEngine *) -> QObject * {
		QmlUtils *qmlUtils = new QmlUtils;
		QObject::connect(&globalLanguageManager, &LanguageManager::languageChanged, qmlUtils, &QmlUtils::reloadMenuBar);
		QObject::connect(&globalThemeEngine, &ThemeEngine::themeChanged, qmlUtils, &QmlUtils::reloadMenuBar);
		return qmlUtils;
	});
	QQmlEngine::setObjectOwnership(&globalThemeEngine, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<ThemeEngine>("OpenTyper.Ui", 1, 0, "ThemeEngine", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalThemeEngine;
	});
	qmlRegisterType<AppMenuModel>("OpenTyper.Ui", 1, 0, "AppMenuModel");
	qmlRegisterType<AppMenuItem>("OpenTyper.Ui", 1, 0, "AppMenuItem");
	// TODO: Remove this after fully switching to Qt 6
	qmlRegisterModule("Qt5Compat.GraphicalEffects", 1, 0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qmlRegisterModule("QtGraphicalEffects", 1, 0);
#endif
}
