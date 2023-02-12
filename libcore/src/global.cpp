/*
 * global.cpp
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

#include <QTcpSocket>
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
#include <QNetworkInformation>
#else
#include <QNetworkConfigurationManager>
#endif
#include "global.h"

bool internetConnected(void)
{
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
	QNetworkInformation::loadDefaultBackend();
	Q_ASSERT(QNetworkInformation::instance());
	return QNetworkInformation::instance()->reachability() == QNetworkInformation::Reachability::Online;
#else
	QNetworkConfigurationManager mgr;
	if(!mgr.isOnline())
		return false;
	QTcpSocket sock;
	sock.connectToHost("8.8.8.8", 53);
	bool connected = sock.waitForConnected(30000); //ms
	if(!connected)
	{
		sock.abort();
		return false;
	}
	sock.close();
	return true;
#endif
}
