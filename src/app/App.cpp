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
#include <QIcon>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#ifdef Q_OS_MACOS
#include <QtSvg>
#endif
#include "App.h"
#include "internal/SplashScreen.h"
#include "translations/internal/LanguageManager.h"
#include "global/global.h"
#include "global/GlobalModule.h"
#include "ui/internal/UiEngine.h"

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
	SplashScreen splash;
	splash.show();
	globalModule.onPreInit();
	for(IModuleSetup *module : m_modules)
		module->registerExports();
	for(IModuleSetup *module : m_modules)
		module->initSettings();
	for(IModuleSetup *module : m_modules)
	{
		if(module != &globalModule)
			module->onPreInit();
	}
	for(IModuleSetup *module : m_modules)
	{
		module->registerResources();
		module->registerUiTypes();
	}
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
		QQmlApplicationEngine engine;
		engine.rootContext()->setContextProperty("rootItem", LanguageManager::instance().get());
		QObject::connect(LanguageManager::instance().get(), &LanguageManager::languageChanged, &engine, &QQmlApplicationEngine::retranslate);
		UiEngine::instance()->setQmlEngine(&engine);
		UiEngine::instance()->addSourceImportPath(":/qml");
		engine.load("qrc:/qml/MainWindow.qml");
		for(IModuleSetup *module : m_modules)
			module->onStartApp();
		splash.hide();
		currentExitCode = a.exec();
		for(IModuleSetup *module : m_modules)
			module->onDeinit();
	} while(currentExitCode == EXIT_CODE_REBOOT);

	for(IModuleSetup *module : m_modules)
		module->onDestroy();
	m_modules.removeAll(&globalModule);
	qDeleteAll(m_modules);
	m_modules.clear();
	return currentExitCode;
}

void App::addModule(IModuleSetup *module)
{
	m_modules.append(module);
}
