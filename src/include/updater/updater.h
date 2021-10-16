/*
 * updater.h
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

#ifndef UPDATER_H
#define UPDATER_H

#include <QCoreApplication>
#include <QMessageBox>
#include <QSaveFile>
#include <QFile>
#include <QProcess>
#include "net.h"
#include "version.h"
#include "updaterdialog.h"

#define _VERSION_H_URL "https://raw.githubusercontent.com/adazem009/Open-Typer/master/src/include/version.h"
#define _GITHUB_REPO "https://github.com/adazem009/Open-Typer"

class Updater : public QObject
{
	Q_OBJECT
	public:
		explicit Updater(QObject *parent = nullptr);
		virtual ~Updater();
	
	private:
		Downloader *versionHdw;
		char *versionH;
		char *readVersion(const char *versionHeader);
		Downloader *updatedProgram;
	
	private slots:
		void initUpdate();
		void overwriteExecutable(void);
};

#endif // UPDATER_H
