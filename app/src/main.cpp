/*
 * main.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2023 - adazem009
 * Copyright (C) 2022 - Roker2
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
#include <QPluginLoader>
#include <QProcessEnvironment>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#ifdef Q_OS_MACOS
#include <QtSvg>
#endif
#include "Settings.h"
#include "LanguageManager.h"
#include "IAddon.h"
#include "AddonApi.h"
#include "QmlKeyboardHandler.h"
#include "ExerciseTimer.h"
#include "CharacterRecord.h"
#include "MistakeRecord.h"
#include "QmlUtils.h"
#include "ExerciseValidator.h"
#include "QmlFileDialog.h"
#include "BuiltInPacks.h"
#include "HistoryParser.h"
#include "StatsDialog.h"
#include "updater/Updater.h"

void changeSplashMessage(QSplashScreen *splash, QString message)
{
	// Add application version (if defined)
	QString versionStr = "";
	if(!QCoreApplication::applicationVersion().isEmpty())
		versionStr = QObject::tr("Version: %1").arg(QCoreApplication::applicationVersion());
	splash->showMessage(versionStr + "\n" + message, Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
}

void loadAddons(QString path)
{
	QDir pluginsDir(path);
	const QStringList entries = pluginsDir.entryList(QDir::Files);
	for(const QString &fileName : entries)
	{
		QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = pluginLoader.instance();
		if(plugin)
		{
			IAddon *addonInterface = qobject_cast<IAddon *>(plugin);
			if(addonInterface)
			{
				QString className = plugin->metaObject()->className();
				if(loadedAddonsClasses.contains(className))
					continue;
				loadedAddons.append(addonInterface);
				loadedAddonsClasses += className;
			}
			else
				pluginLoader.unload();
		}
	}
}

void loadAddons(void)
{
	QDir pluginsDir(QCoreApplication::applicationDirPath());
#if defined(Q_OS_WIN)
	if(pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
		pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
	if(pluginsDir.dirName() == "MacOS")
	{
		pluginsDir.cdUp();
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#elif defined(Q_OS_UNIX)
	QString AppImageDir = QProcessEnvironment::systemEnvironment().value(QStringLiteral("APPIMAGE"));
	if(!AppImageDir.isEmpty())
		pluginsDir.cd(AppImageDir + "/..");
#endif
	loadAddons(pluginsDir.path());
	pluginsDir.cd("plugins");
	loadAddons(pluginsDir.path());
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationDomain("open-typer.sourceforge.io");
	QCoreApplication::setOrganizationName("Open-Typer");
	QCoreApplication::setApplicationName("Open-Typer");
#ifdef BUILD_VERSION
	QCoreApplication::setApplicationVersion(QString(BUILD_VERSION).remove(0, 1));
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
	changeSplashMessage(&splash, QObject::tr("Loading addons..."));
	a.processEvents();
	loadAddons();
	AddonApi::initSettingsCategories();
	changeSplashMessage(&splash, QObject::tr("Opening main window..."));
	a.processEvents();
	// Register QML types
	QQmlEngine::setObjectOwnership(&globalThemeEngine, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<ThemeEngine>("OpenTyper", 1, 0, "ThemeEngine", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalThemeEngine;
	});
	qmlRegisterSingletonType<QmlUtils>("OpenTyper", 1, 0, "QmlUtils", [](QQmlEngine *, QJSEngine *) -> QObject * {
		QmlUtils *qmlUtils = new QmlUtils;
		QObject::connect(&globalLanguageManager, &LanguageManager::languageChanged, qmlUtils, &QmlUtils::reloadMenuBar);
		return qmlUtils;
	});
	QQmlEngine::setObjectOwnership(&globalLanguageManager, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<LanguageManager>("OpenTyper", 1, 0, "LanguageManager", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalLanguageManager;
	});
	qmlRegisterType<ConfigParser>("OpenTyper", 1, 0, "ConfigParser");
	qmlRegisterType<QmlKeyboardHandler>("OpenTyper", 1, 0, "KeyboardHandler");
	qmlRegisterType<ExerciseTimer>("OpenTyper", 1, 0, "ExerciseTimer");
	qmlRegisterType<CharacterRecord>("OpenTyper", 1, 0, "CharacterRecord");
	qmlRegisterType<MistakeRecord>("OpenTyper", 1, 0, "MistakeRecord");
	qmlRegisterType<ExerciseValidator>("OpenTyper", 1, 0, "ExerciseValidator");
	qmlRegisterType<QmlFileDialog>("OpenTyper", 1, 0, "QmlFileDialog");
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
	KeyboardUtils keyboardUtils;
	engine.rootContext()->setContextProperty("KeyboardUtils", &keyboardUtils);
	HistoryParser historyParser;
	engine.rootContext()->setContextProperty("HistoryParser", &historyParser);
	StatsDialog statsDialog(true, {}, QPair<int, int>(), QString(), 0, 0, 0);
	engine.rootContext()->setContextProperty("statsDialog", &statsDialog);
	engine.load("qrc:/qml/MainWindow.qml");
	splash.finish(nullptr);
	return a.exec();
}
