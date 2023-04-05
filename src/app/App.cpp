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
#include "translations/LanguageManager.h"
#include "global/global.h"
#include "global/GlobalModule.h"

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
	changeSplashMessage(&splash, QObject::tr("Opening main window..."));
	a.processEvents();
	// Register QML types
	qRegisterMetaType<QMap<int, int>>();
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
		QQmlApplicationEngine engine;
		for(IModuleSetup *module : m_modules)
			module->setRootContextProperties(engine.rootContext());
		engine.rootContext()->setContextProperty("rootItem", &globalLanguageManager);
		QObject::connect(&globalLanguageManager, &LanguageManager::languageChanged, &engine, &QQmlApplicationEngine::retranslate);
		engine.load("qrc:/qml/MainWindow.qml");
		for(IModuleSetup *module : m_modules)
			module->onStartApp();
		if(splash.isVisible())
			splash.close();
		currentExitCode = a.exec();
		for(IModuleSetup *module : m_modules)
			module->onDeinit();
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
