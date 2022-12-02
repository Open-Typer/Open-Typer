/*
 * main.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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
#include "MainWindow.h"
#include "Settings.h"
#include "LanguageManager.h"
#include "IAddon.h"
#include "AddonApi.h"

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
		QFileInfo fileInfo(fileName);
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
	LanguageManager langMgr;
	if(Settings::language() == "")
		langMgr.setLanguage(-1);
	else
		langMgr.setLanguage(langMgr.getBoxItems().indexOf(Settings::language()) - 1);
	QPixmap pixmap(":/res/images/splash.png");
	QSplashScreen splash(pixmap);
	splash.show();
	changeSplashMessage(&splash, QObject::tr("Loading addons..."));
	a.processEvents();
	loadAddons();
	AddonApi::initSettingsCategories();
	changeSplashMessage(&splash, QObject::tr("Opening main window..."));
	a.processEvents();
	// Set icon
	a.setWindowIcon(QIcon(":/res/images/icon.ico"));
	MainWindow w;
	// Main window will get shown by itself
	splash.finish(&w);
	return a.exec();
}
