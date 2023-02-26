/*
 * main.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2023 - adazem009
 * Copyright (C) 2022-2023 - Roker2
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

#include <QApplication>
#include <QSettings>
#include <QSplashScreen>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#ifdef Q_OS_MACOS
#include <QtSvg>
#endif
#include "Settings.h"
#include "LanguageManager.h"
#include "AddonApi.h"
#include "AddonButton.h"
#include "ConfigParser.h"
#include "QmlKeyboardHandler.h"
#include "ExerciseTimer.h"
#include "CharacterRecord.h"
#include "MistakeRecord.h"
#include "QmlUtils.h"
#include "ExerciseValidator.h"
#include "QmlFileDialog.h"
#include "BuiltInPacks.h"
#include "HistoryParser.h"
#include "KeyboardLayout.h"
#include "Key.h"
#include "QmlWidget.h"
#include "ExportProvider.h"
#include "ExportTable.h"
#include "AppMenuBar.h"
#ifndef Q_OS_WASM
#include "AddonListModel.h"
#include "AddonManager.h"
#endif
#include "global.h"
#include "updater/Updater.h"

void changeSplashMessage(QSplashScreen *splash, QString message)
{
	// Add application version (if defined)
	QString versionStr = "";
	if(!QCoreApplication::applicationVersion().isEmpty())
		versionStr = QObject::tr("Version: %1").arg(QCoreApplication::applicationVersion());
	splash->showMessage(versionStr + "\n" + message, Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifndef Q_OS_WINDOWS
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#endif
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationDomain("open-typer.sourceforge.io");
	QCoreApplication::setOrganizationName("Open-Typer");
	QCoreApplication::setApplicationName("Open-Typer " + QString::number(QVersionNumber::fromString(BUILD_VERSION).majorVersion()));
#ifdef BUILD_VERSION
	QCoreApplication::setApplicationVersion(BUILD_VERSION);
#endif // BUILD_VERSION
	// Initialize settings
	Settings::init();
	// Set language
	globalLanguageManager.init();
	if(Settings::language() == "")
		globalLanguageManager.setLanguage(-1);
	else
		globalLanguageManager.setLanguage(globalLanguageManager.getBoxItems().indexOf(Settings::language()) - 1);
	QPixmap pixmap(":/res/images/splash.png");
	QSplashScreen splash(pixmap);
	splash.show();
#ifndef Q_OS_WASM
	changeSplashMessage(&splash, QObject::tr("Loading addons..."));
	a.processEvents();
	globalAddonManager.loadAddons();
	Updater::getAddonUpdates();
	bool addonsLoaded = true;
#endif
	changeSplashMessage(&splash, QObject::tr("Opening main window..."));
	a.processEvents();
	// Register QML types
	QQmlEngine::setObjectOwnership(&globalThemeEngine, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<ThemeEngine>("OpenTyper", 1, 0, "ThemeEngine", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalThemeEngine;
	});
	qmlRegisterSingletonType<KeyboardUtils>("OpenTyper", 1, 0, "KeyboardUtils", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return new KeyboardUtils;
	});
	qmlRegisterSingletonType<QmlUtils>("OpenTyper", 1, 0, "QmlUtils", [](QQmlEngine *, QJSEngine *) -> QObject * {
		QmlUtils *qmlUtils = new QmlUtils;
		QObject::connect(&globalLanguageManager, &LanguageManager::languageChanged, qmlUtils, &QmlUtils::reloadMenuBar);
		QObject::connect(&globalThemeEngine, &ThemeEngine::themeChanged, qmlUtils, &QmlUtils::reloadMenuBar);
		return qmlUtils;
	});
	QQmlEngine::setObjectOwnership(&globalLanguageManager, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<LanguageManager>("OpenTyper", 1, 0, "LanguageManager", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalLanguageManager;
	});
	QQmlEngine::setObjectOwnership(&globalMenuBar, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<AppMenuBar>("OpenTyper", 1, 0, "AppMenuBar", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalMenuBar;
	});
	QQmlEngine::setObjectOwnership(&globalAddonApi, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<AddonApi>("OpenTyper", 1, 0, "AddonApi", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalAddonApi;
	});
#ifndef Q_OS_WASM
	QQmlEngine::setObjectOwnership(&globalAddonManager, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<AddonManager>("OpenTyper", 1, 0, "AddonManager", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalAddonManager;
	});
#endif
	qmlRegisterType<ConfigParser>("OpenTyper", 1, 0, "ConfigParser");
	qmlRegisterType<QmlKeyboardHandler>("OpenTyper", 1, 0, "KeyboardHandler");
	qmlRegisterType<ExerciseTimer>("OpenTyper", 1, 0, "ExerciseTimer");
	qmlRegisterType<ExerciseValidator>("OpenTyper", 1, 0, "ExerciseValidator");
	qmlRegisterType<QmlFileDialog>("OpenTyper", 1, 0, "QmlFileDialog");
	qmlRegisterType<KeyboardLayout>("OpenTyper", 1, 0, "KeyboardLayout");
	qmlRegisterType<HistoryParser>("OpenTyper", 1, 0, "HistoryParser");
	qmlRegisterType<QmlWidget>("OpenTyper", 1, 0, "Widget");
	qmlRegisterType<QWidget>("OpenTyper", 1, 0, "QWidget");
	qmlRegisterType<ExportProvider>("OpenTyper", 1, 0, "ExportProvider");
	qmlRegisterType<AppMenuModel>("OpenTyper", 1, 0, "AppMenuModel");
	qmlRegisterType<AppMenuItem>("OpenTyper", 1, 0, "AppMenuItem");
	qmlRegisterType<SettingsCategory>("OpenTyper", 1, 0, "SettingsCategory");
#ifndef Q_OS_WASM
	qmlRegisterType<AddonItemModel>("OpenTyper", 1, 0, "AddonItemModel");
	qmlRegisterType<AddonListModel>("OpenTyper", 1, 0, "AddonListModel");
	qmlRegisterType<AddonModel>("OpenTyper", 1, 0, "AddonModel");
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qmlRegisterUncreatableMetaObject(MistakeRecord::staticMetaObject, "OpenTyper", 1, 0, "MistakeRecord", "Please use QmlUtils.createMistakeRecord()");
	qmlRegisterUncreatableMetaObject(AddonButton::staticMetaObject, "OpenTyper", 1, 0, "AddonButton", "Please use AddonApi to create buttons");
#else
	qmlRegisterUncreatableType<MistakeRecord>("OpenTyper", 1, 0, "MistakeRecord", "Please use QmlUtils.createMistakeRecord()");
	qmlRegisterUncreatableType<AddonButton>("OpenTyper", 1, 0, "AddonButton", "Please use AddonApi to create buttons");
#endif
	qRegisterMetaType<QList<MistakeRecord>>();
	qRegisterMetaType<QList<AddonButton *>>();
	qRegisterMetaType<CharacterRecord>();
	qRegisterMetaType<HistoryEntry>();
	qRegisterMetaType<Key>();
	qRegisterMetaType<KeyboardRow>();
	qmlRegisterUncreatableMetaObject(publicPos::staticMetaObject, "OpenTyper", 1, 0, "PublicPos", "Error: PublicPos is uncreatable");
	// TODO: Remove this after fully switching to Qt 6
	qmlRegisterModule("Qt5Compat.GraphicalEffects", 1, 0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qmlRegisterModule("QtGraphicalEffects", 1, 0);
#endif
	// Set icon theme
	QIcon::setThemeName("open-typer");
	// Set icon
	a.setWindowIcon(QIcon(":/res/images/icon.ico"));
	QQuickStyle::setStyle("Material");

	int currentExitCode;
	do
	{
#ifndef Q_OS_WASM
		if(!addonsLoaded)
			globalAddonManager.loadAddons();
#endif
		QQmlApplicationEngine engine;
		engine.rootContext()->setContextProperty("rootItem", &globalLanguageManager);
		QObject::connect(&globalLanguageManager, &LanguageManager::languageChanged, &engine, &QQmlApplicationEngine::retranslate);
		Settings settings;
		engine.rootContext()->setContextProperty("Settings", &settings);
		Updater updater;
		engine.rootContext()->setContextProperty("Updater", &updater);
		FileUtils fileUtils;
		engine.rootContext()->setContextProperty("FileUtils", &fileUtils);
		BuiltInPacks builtInPacks;
		engine.rootContext()->setContextProperty("BuiltInPacks", &builtInPacks);
		StringUtils stringUtils;
		engine.rootContext()->setContextProperty("StringUtils", &stringUtils);
		ExportTable table;
		engine.rootContext()->setContextProperty("exportTable", &table);
		engine.load("qrc:/qml/MainWindow.qml");
		if(splash.isVisible())
			splash.close();
		currentExitCode = a.exec();
#ifndef Q_OS_WASM
		globalAddonManager.unloadAddons();
		addonsLoaded = false;
#endif
		if(Settings::isFrozen())
			Settings::saveChanges();
	} while(currentExitCode == EXIT_CODE_REBOOT);
	return 0;
}
