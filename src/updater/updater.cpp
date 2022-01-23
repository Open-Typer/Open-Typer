/*
 * updater.cpp
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

#include "updater/updater.h"

/*!
 * Constructs Updater and starts maintenance tool to check for updates.\n
 * Version header file URL is stored in the _VERSION_H_URL macro.
 *
 * \see initUpdate()
 */
Updater::Updater(QObject *parent) :
	QObject(parent)
{
	QFile maintenancetoolFile;
	maintenancetoolFile.setFileName(QCoreApplication::applicationDirPath() + "/maintenancetool");
	if(!maintenancetoolFile.exists())
	{
		maintenancetoolFile.setFileName(QCoreApplication::applicationDirPath() + "/maintenancetool.exe");
		if(!maintenancetoolFile.exists())
		{
			deleteLater();
			return;
		}
	}
	QProcess *process = new QProcess(this);
	process->start(maintenancetoolFile.fileName(),{"ch"});
	process->waitForFinished();
	if(process->readAllStandardOutput().contains("<update"))
	{
		UpdaterDialog dialog;
		if(dialog.exec() == QDialog::Accepted)
		{
			process->kill();
			QStringList args;
			args += "--updater";
			args += "updateSource=" + QFileInfo(QCoreApplication::applicationFilePath()).fileName();
			process->startDetached(maintenancetoolFile.fileName(),args);
			exit(0);
		}
	}
	deleteLater();
}

/*! Destroys the Updater object. */
Updater::~Updater() { }
