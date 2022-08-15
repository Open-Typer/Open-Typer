/*
 * main.cpp
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

#include <QApplication>
#include <QSettings>
#include "opentyper.h"
#include "core/database.h"

void changeSplashMessage(QSplashScreen *splash, QString message)
{
	// Add application version (if defined)
	QString versionStr = "";
#ifdef BUILD_VERSION
	versionStr = QObject::tr("Version: %1").arg(BUILD_VERSION);
#endif // BUILD_VERSION
	splash->showMessage(versionStr + "\n" + message, Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
	QApplication a(argc, argv);
	QCoreApplication::setApplicationName("Open-Typer");
#ifdef BUILD_VERSION
	QCoreApplication::setApplicationVersion(BUILD_VERSION);
#endif // BUILD_VERSION
	QPixmap pixmap(":/res/images/splash.png");
	QSplashScreen splash(pixmap);
	splash.show();
	changeSplashMessage(&splash, QObject::tr("Opening database..."));
	a.processEvents();
	if(!dbMgr.status())
		dbMgr.open();
	changeSplashMessage(&splash, QObject::tr("Opening main window..."));
	a.processEvents();
	// Set icon
	a.setWindowIcon(QIcon(":/res/images/icon.ico"));
	OpenTyper w;
	// Main window will get shown by itself
	splash.finish(&w);
	return a.exec();
}
