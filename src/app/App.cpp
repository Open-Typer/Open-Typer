/*
 * App.cpp
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

#include <QApplication>
#include <QSplashScreen>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#ifdef Q_OS_MACOS
#include <QtSvg>
#endif
#include "App.h"
#include "translations/LanguageManager.h"
#include "addons/AddonApi.h"
#include "addons/AddonButton.h"
#include "utils/ExerciseTimer.h"
#include "validator/CharacterRecord.h"
#include "validator/MistakeRecord.h"
#include "validator/ExerciseValidator.h"
#include "uicomponents/QmlFileDialog.h"
#include "BuiltInPacks.h"
#include "history/HistoryParser.h"
#include "uicomponents/QmlWidget.h"
#include "export/ExportProvider.h"
#include "export/ExportTable.h"
#include "AppMenuBar.h"
#include "grades/ClassManager.h"
#ifndef Q_OS_WASM
#include "addons/AddonListModel.h"
#include "addons/AddonManager.h"
#endif
#include "global/global.h"
#include "global/GlobalModule.h"
#include "updater/Updater.h"

static GlobalModule globalModule;

App::App()
{
	addModule(&globalModule);
}

int App::run(int argc, char **argv)
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
	for(IModuleSetup *module : m_modules)
	{
		module->registerResources();
		module->registerUiTypes();
	}
	for(IModuleSetup *module : m_modules)
		module->onPreInit();
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
	QQmlEngine::setObjectOwnership(&globalMenuBar, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<AppMenuBar>("OpenTyper", 1, 0, "AppMenuBar", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalMenuBar;
	});
#ifndef Q_OS_WASM
	QQmlEngine::setObjectOwnership(&globalAddonApi, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<AddonApi>("OpenTyper", 1, 0, "AddonApi", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalAddonApi;
	});
	QQmlEngine::setObjectOwnership(&globalAddonManager, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<AddonManager>("OpenTyper", 1, 0, "AddonManager", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalAddonManager;
	});
#endif
	QQmlEngine::setObjectOwnership(&globalClassManager, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<ClassManager>("OpenTyper", 1, 0, "ClassManager", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalClassManager;
	});
	qmlRegisterType<ExerciseTimer>("OpenTyper", 1, 0, "ExerciseTimer");
	qmlRegisterType<ExerciseValidator>("OpenTyper", 1, 0, "ExerciseValidator");
	qmlRegisterType<QmlFileDialog>("OpenTyper", 1, 0, "QmlFileDialog");
	qmlRegisterType<HistoryParser>("OpenTyper", 1, 0, "HistoryParser");
	qmlRegisterType<QmlWidget>("OpenTyper", 1, 0, "Widget");
	qmlRegisterType<QWidget>("OpenTyper", 1, 0, "QWidget");
	qmlRegisterType<ExportProvider>("OpenTyper", 1, 0, "ExportProvider");
	qmlRegisterType<SettingsCategory>("OpenTyper", 1, 0, "SettingsCategory");
	qmlRegisterType<Class>("OpenTyper", 1, 0, "Class");
#ifndef Q_OS_WASM
	qmlRegisterType<AddonItemModel>("OpenTyper", 1, 0, "AddonItemModel");
	qmlRegisterType<AddonListModel>("OpenTyper", 1, 0, "AddonListModel");
	qmlRegisterType<AddonModel>("OpenTyper", 1, 0, "AddonModel");
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qmlRegisterUncreatableMetaObject(AddonButton::staticMetaObject, "OpenTyper", 1, 0, "AddonButton", "Please use AddonApi to create buttons");
#else
	qmlRegisterUncreatableType<AddonButton>("OpenTyper", 1, 0, "AddonButton", "Please use AddonApi to create buttons");
#endif
	qRegisterMetaType<QList<AddonButton *>>();
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qmlRegisterUncreatableMetaObject(MistakeRecord::staticMetaObject, "OpenTyper", 1, 0, "MistakeRecord", "Please use QmlUtils.createMistakeRecord()");
#else
	qmlRegisterUncreatableType<MistakeRecord>("OpenTyper", 1, 0, "MistakeRecord", "Please use QmlUtils.createMistakeRecord()");
#endif
	qRegisterMetaType<QList<MistakeRecord>>();
	qRegisterMetaType<QMap<int, int>>();
	qRegisterMetaType<CharacterRecord>();
	qRegisterMetaType<HistoryEntry>();
	qRegisterMetaType<ClassManager::GradingMethod>();
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
		for(IModuleSetup *module : m_modules)
			module->onInit();
#ifndef Q_OS_WASM
		if(!addonsLoaded)
			globalAddonManager.loadAddons();
#endif
		QQmlApplicationEngine engine;
		for(IModuleSetup *module : m_modules)
			module->setRootContextProperties(engine.rootContext());
		engine.rootContext()->setContextProperty("rootItem", &globalLanguageManager);
		QObject::connect(&globalLanguageManager, &LanguageManager::languageChanged, &engine, &QQmlApplicationEngine::retranslate);
		Updater updater;
		engine.rootContext()->setContextProperty("Updater", &updater);
		BuiltInPacks builtInPacks;
		engine.rootContext()->setContextProperty("BuiltInPacks", &builtInPacks);
		ExportTable table;
		engine.rootContext()->setContextProperty("exportTable", &table);
		engine.load("qrc:/qml/MainWindow.qml");
		for(IModuleSetup *module : m_modules)
			module->onStartApp();
		if(splash.isVisible())
			splash.close();
		currentExitCode = a.exec();
		for(IModuleSetup *module : m_modules)
			module->onDeinit();
#ifndef Q_OS_WASM
		globalAddonManager.unloadAddons();
		addonsLoaded = false;
#endif
	} while(currentExitCode == EXIT_CODE_REBOOT);

	for(IModuleSetup *module : m_modules)
		module->onDestroy();
	qDeleteAll(m_modules);
	m_modules.clear();
	return currentExitCode;
}

void App::addModule(IModuleSetup *module)
{
	m_modules.append(module);
}

void App::changeSplashMessage(QSplashScreen *splash, QString message)
{
	// Add application version (if defined)
	QString versionStr = "";
	if(!QCoreApplication::applicationVersion().isEmpty())
		versionStr = QObject::tr("Version: %1").arg(QCoreApplication::applicationVersion());
	splash->showMessage(versionStr + "\n" + message, Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
}
