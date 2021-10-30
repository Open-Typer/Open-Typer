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
	versionH = (char*) malloc(1024);
	strcpy(versionH,QString(versionHdw->downloadedData()).toStdString().c_str());
	char *newVer = readVersion(versionH);
	if(strcmp(newVer,_VERSION) != 0)
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
			const char *executable;
			#ifdef _WIN32
			executable = "Open-Typer.exe";
			#else
			executable = "Open-Typer-linux-amd64";
			#endif
			updatedProgram = new Downloader(QString(_GITHUB_REPO)+"/releases/download/v"+QString(newVer)+"/"+QString(executable),this);
			connect(updatedProgram,SIGNAL(downloaded()),this,SLOT(overwriteExecutable()));
			QMessageBox msgBox;
			msgBox.setText(tr("Downloading update, please wait..."));
			msgBox.setStandardButtons(QMessageBox::NoButton);
			msgBox.exec();
		}
	}
}

char *Updater::readVersion(const char *versionHeader)
{
	unsigned int i;
	char *out = (char*) malloc(strlen(versionHeader));
	strcpy(out,"");
	// #define _VERSION "1.0.0"
	//                  ^ 18
	for(i=18; i < strlen(versionHeader)-2; i++)
		strncat(out,&versionHeader[i],1);
	if(strcmp(out,"") == 0)
		return (char*) _VERSION;
	return out;
}

void Updater::overwriteExecutable()
{
	QSaveFile file(QCoreApplication::applicationFilePath()+".part");
	file.open(QIODevice::WriteOnly);
	file.write(updatedProgram->downloadedData());
	file.commit();
	chmod(QString(QCoreApplication::applicationFilePath()+".part").toStdString().c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH);
	QProcess *process = new QProcess(this);
	QStringList args;
	args += QCoreApplication::applicationFilePath();
	process->start(QCoreApplication::applicationFilePath()+".part",args);
	exit(0);
}
