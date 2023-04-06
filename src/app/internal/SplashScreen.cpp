/*
 * SplashScreen.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

#include <QCoreApplication>
#include "SplashScreen.h"

/*! Constructs SplashScreen. */
SplashScreen::SplashScreen(QObject *parent) :
	QObject(parent)
{
	QPixmap pixmap(":/res/images/splash.png");
	m_splash = new QSplashScreen(pixmap);
}

/*! Destroys the SplashScreen object. */
SplashScreen::~SplashScreen()
{
	m_splash->deleteLater();
}

/*! Shows the splash screen. */
void SplashScreen::show()
{
	setText(tr("Loading..."));
	m_splash->show();
}

/*! Hides the splash screen. */
void SplashScreen::hide()
{
	if(m_splash->isVisible())
		m_splash->close();
}

/*! Changes the splash screen message. */
void SplashScreen::setText(const QString &text)
{
	// Add application version (if defined)
	QString versionStr = "";
	if(!QCoreApplication::applicationVersion().isEmpty())
		versionStr = QObject::tr("Version: %1").arg(QCoreApplication::applicationVersion());
	m_splash->showMessage(versionStr + "\n" + text, Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
	QCoreApplication::processEvents();
}
