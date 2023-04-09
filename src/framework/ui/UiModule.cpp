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
#include "internal/QmlUtils.h"
#include "internal/ThemeEngine.h"
#include "menubar/AppMenuModel.h"
#include "internal/UiEngine.h"

std::string UiModule::moduleName() const
{
	return "ui";
}

void UiModule::registerExports()
{
	modularity::ioc()->registerExport<IUiEngine>(UiEngine::instance());
	modularity::ioc()->registerExport<IThemeEngine>(ThemeEngine::instance());
}

void UiModule::initSettings()
{
	INIT_SETTINGS_KEY("fontFamily", "theme/font", "");
	INIT_SETTINGS_KEY("fontSize", "theme/fontsize", 20);
	INIT_SETTINGS_KEY("customExerciseTextColor", "theme/customleveltextcolor", false);
	INIT_SETTINGS_KEY("exerciseTextColorR", "theme/leveltextred", 0);
	INIT_SETTINGS_KEY("exerciseTextColorG", "theme/leveltextgreen", 0);
	INIT_SETTINGS_KEY("exerciseTextColorB", "theme/leveltextblue", 0);
	INIT_SETTINGS_KEY("customInputTextColor", "theme/custominputtextcolor", false);
	INIT_SETTINGS_KEY("inputTextColorR", "theme/inputtextred", 0);
	INIT_SETTINGS_KEY("inputTextColorG", "theme/inputtextgreen", 0);
	INIT_SETTINGS_KEY("inputTextColorB", "theme/inputtextblue", 0);
	INIT_SETTINGS_KEY("customBgColor", "theme/custombgcolor", false);
	INIT_SETTINGS_KEY("bgColorR", "theme/bgred", 0);
	INIT_SETTINGS_KEY("bgColorG", "theme/bggreen", 0);
	INIT_SETTINGS_KEY("bgColorB", "theme/bgblue", 0);
	INIT_SETTINGS_KEY("customPaperColor", "theme/custompapercolor", false);
	INIT_SETTINGS_KEY("paperColorR", "theme/paperred", 0);
	INIT_SETTINGS_KEY("paperColorG", "theme/papergreen", 0);
	INIT_SETTINGS_KEY("paperColorB", "theme/paperblue", 0);
	INIT_SETTINGS_KEY("customPanelColor", "theme/custompanelcolor", false);
	INIT_SETTINGS_KEY("panelColorR", "theme/panelred", 0);
	INIT_SETTINGS_KEY("panelColorG", "theme/panelgreen", 0);
	INIT_SETTINGS_KEY("panelColorB", "theme/panelblue", 0);
	INIT_SETTINGS_KEY("accentColorId", "theme/accentcolorid", ThemeEngine::Accent_LightBlue);
	INIT_SETTINGS_KEY("appTheme", "theme/apptheme", static_cast<int>(ThemeEngine::Theme::LightTheme));
}

void UiModule::registerUiTypes()
{
	qmlRegisterSingletonType<QmlUtils>("OpenTyper.Ui", 1, 0, "QmlUtils", [](QQmlEngine *, QJSEngine *) -> QObject * {
		QmlUtils *qmlUtils = new QmlUtils;
		QObject::connect(languageManager().get(), &ILanguageManager::languageChanged, qmlUtils, &QmlUtils::reloadMenuBar);
		QObject::connect(ThemeEngine::instance().get(), &ThemeEngine::themeChanged, qmlUtils, &QmlUtils::reloadMenuBar);
		return qmlUtils;
	});
	QQmlEngine::setObjectOwnership(ThemeEngine::instance().get(), QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<ThemeEngine>("OpenTyper.Ui", 1, 0, "ThemeEngine", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return ThemeEngine::instance().get();
	});
	qmlRegisterType<AppMenuModel>("OpenTyper.Ui", 1, 0, "AppMenuModel");
	qmlRegisterType<AppMenuItem>("OpenTyper.Ui", 1, 0, "AppMenuItem");
	// TODO: Remove this after fully switching to Qt 6
	qmlRegisterModule("Qt5Compat.GraphicalEffects", 1, 0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qmlRegisterModule("QtGraphicalEffects", 1, 0);
#endif
	QObject::connect(settings().get(), &ISettings::discarded, ThemeEngine::instance().get(), &ThemeEngine::themeChanged);
	QObject::connect(settings().get(), &ISettings::discarded, ThemeEngine::instance().get(), &ThemeEngine::panelColorChanged);
}
