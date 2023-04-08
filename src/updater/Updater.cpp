/*
 * Updater.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2023 - adazem009
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

#ifdef Q_OS_WINDOWS
#include <QSysInfo>
#endif
#include "updater/Updater.h"
#ifndef Q_OS_WASM
#include "addons/AddonManager.h"
#include "global/global.h"

AddonListModel Updater::listModel;
QList<AddonItemModel *> Updater::updatableAddons;
#endif

static const QString module = "updater";
static const ISettings::Key UPDATE_CHECKS(module, "updateChecks");

/*! Checks for updates and returns true if there's an update available (only supports Windows). */
bool Updater::updateAvailable(void)
{
	if(!settings()->getValue(UPDATE_CHECKS).toBool())
		return false;
#ifdef Q_OS_WINDOWS
	auto currentDate = QDateTime::currentDateTimeUtc().date();
	// Disable updates on Windows 7 and 8 after May 2023
	if((QSysInfo::productVersion().split(' ').at(0).toInt() < 10) && ((currentDate.year() > 2023) || ((currentDate.year() == 2023) && (currentDate.month() >= 6))))
		return false;
	QFile maintenancetoolFile;
	maintenancetoolFile.setFileName(QCoreApplication::applicationDirPath() + "/../maintenancetool");
	if(!maintenancetoolFile.exists())
	{
		maintenancetoolFile.setFileName(QCoreApplication::applicationDirPath() + "/../maintenancetool.exe");
		if(!maintenancetoolFile.exists())
			return false;
	}
	QProcess *process = new QProcess(qApp);
	process->start(maintenancetoolFile.fileName(), { "ch" });
	process->waitForFinished();
	if(process->readAllStandardOutput().contains("<update"))
		return true;
#endif // Q_OS_WINDOWS
	return false;
}

/*! Starts maintenance tool and installs the update (only supports Windows). */
void Updater::installUpdate(void)
{
#ifdef Q_OS_WINDOWS
	QProcess *process = new QProcess(qApp);
	QStringList args;
	args += "--updater";
	args += "updateSource=" + QFileInfo(QCoreApplication::applicationFilePath()).fileName();
	process->startDetached(QCoreApplication::applicationDirPath() + "/../maintenancetool", args);
	exit(0);
#endif // Q_OS_WINDOWS
}

#ifndef Q_OS_WASM
/*! Checks for addon updates. */
void Updater::getAddonUpdates(void)
{
	if(!settings()->getValue(UPDATE_CHECKS).toBool() || !internetConnected())
		return;
	// Get installed addons
	listModel.setLocalAddons(true);
	listModel.load(); // load() is synchronous with local addons
	auto addons = listModel.getItems();

	// Get online addons
	listModel.setLocalAddons(false);
	QEventLoop eventLoop;
	connect(&listModel, &AddonListModel::loaded, &eventLoop, &QEventLoop::quit);
	listModel.load();
	eventLoop.exec();
	auto onlineAddons = listModel.getItems();

	// Compare versions
	for(int i = 0; i < addons.length(); i++)
	{
		int index = -1;
		for(int j = 0; j < onlineAddons.length(); j++)
		{
			if(onlineAddons[j]->downloadUrls().isEmpty())
				continue;
			if(onlineAddons[j]->id() == addons[i]->id())
			{
				index = j;
				break;
			}
		}
		if(index != -1)
		{
			QVersionNumber currentVersion = addons[i]->version();
			QVersionNumber newVersion = onlineAddons[index]->version();
			if(newVersion > currentVersion)
				updatableAddons.append(onlineAddons[index]);
		}
	}
}

/*! Returns true if there are addon updates available. Run getAddonUpdates() before using this function. */
bool Updater::addonUpdateAvailable(void)
{
	return !updatableAddons.isEmpty();
}

/*! Updates installed addons. Run getAddonUpdates() before using this function. */
void Updater::updateAddons()
{
	globalAddonManager.unloadAddons();
	for(int i = 0; i < updatableAddons.length(); i++)
	{
		globalAddonManager.uninstallAddon(updatableAddons[i]->id());
		auto model = globalAddonManager.installAddon(updatableAddons[i]);
		QEventLoop eventLoop;
		connect(model, &AddonModel::installedChanged, [model, &eventLoop]() {
			if(model->installed())
				eventLoop.quit();
		});
		eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
	}
	updatableAddons.clear();
	qApp->exit(EXIT_CODE_REBOOT);
}
#endif
