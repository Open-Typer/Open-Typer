/*
 * Updater.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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

#include "updater/Updater.h"

/*! Checks for updates and returns true if there's an update available (only supports Windows). */
bool Updater::updateAvailable(void)
{
#ifdef Q_OS_WINDOWS
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
	process->startDetached(maintenancetoolFile.fileName(), args);
	exit(0);
#endif // Q_OS_WINDOWS
}
