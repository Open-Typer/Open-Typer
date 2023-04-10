/*
 * WindowsUpdater.cpp
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

#include <QSysInfo>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDateTime>
#include "WindowsUpdater.h"

std::shared_ptr<WindowsUpdater> WindowsUpdater::m_instance = std::make_shared<WindowsUpdater>();

std::shared_ptr<WindowsUpdater> WindowsUpdater::instance()
{
	return m_instance;
}

bool WindowsUpdater::updateAvailable()
{
	if(!updatesEnabled())
		return false;
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
	return false;
}

void WindowsUpdater::installUpdate()
{
	QProcess *process = new QProcess(qApp);
	QStringList args;
	args += "--updater";
	args += "updateSource=" + QFileInfo(QCoreApplication::applicationFilePath()).fileName();
	process->startDetached(QCoreApplication::applicationDirPath() + "/../maintenancetool", args);
	exit(0);
}
