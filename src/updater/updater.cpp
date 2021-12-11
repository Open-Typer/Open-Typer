/*
 * updater.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

Updater::Updater(QObject *parent) :
	QObject(parent)
{
	Net *net = new Net();
	if(net->internetConnected())
	{
		versionHdw = new Downloader(QString(_VERSION_H_URL),this);
		connect(versionHdw,SIGNAL(downloaded()),this,SLOT(initUpdate()));
	}
}

Updater::~Updater() { }

void Updater::initUpdate()
{
	versionH = versionHdw->downloadedData();
	QString newVer = readVersion(versionH);
	if(newVer != _VERSION)
	{
		// New version found
		UpdaterDialog dialog;
		dialog.setCurrentVer("v"+QString(_VERSION));
		dialog.setNewVer("v"+QString(newVer));
		if(dialog.exec() == QDialog::Accepted)
		{
			Net *net = new Net();
			if(!net->internetConnected())
			{
				QMessageBox errBox;
				errBox.setText(tr("Error: No internet connection."));
				errBox.exec();
				return;
			}
			QString executable;
			#ifdef _WIN32
			executable = "Open-Typer.exe";
			#else
			executable = "Open-Typer-linux-amd64";
			#endif
			updatedProgram = new Downloader(_GITHUB_REPO+QString("/releases/download/v")+newVer+"/"+executable,this);
			connect(updatedProgram,SIGNAL(downloaded()),this,SLOT(overwriteExecutable()));
			UpdaterDialog *msgBox = new UpdaterDialog(nullptr,true);
			connect(updatedProgram,SIGNAL(progressChanged(int)),msgBox,SLOT(updateProgress(int)));
			connect(msgBox,SIGNAL(cancelDownload()),updatedProgram,SLOT(cancelDownload()));
			msgBox->exec();
		}
	}
}

QString Updater::readVersion(const QString versionHeader)
{
	int i;
	QString out = "";
	// #define _VERSION "1.0.0"
	//                  ^ 18
	for(i=18; i < versionHeader.count()-2; i++)
		out += versionHeader[i];
	if(out == "")
		return _VERSION;
	return out;
}

void Updater::overwriteExecutable()
{
	QSaveFile file(QCoreApplication::applicationFilePath()+".part");
	file.open(QIODevice::WriteOnly);
	file.write(updatedProgram->downloadedData());
	file.commit();
	QProcess *process = new QProcess(this);
	QStringList args;
	args += QCoreApplication::applicationFilePath();
	process->start(QCoreApplication::applicationFilePath()+".part",args);
	exit(0);
}
