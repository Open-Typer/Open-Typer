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

#include <QEventLoop>
#include <QTimer>
#include <QDnsLookup>
#include "global.h"

bool internetConnected(void)
{
	QTimer timer;
	timer.setInterval(3000);
	timer.setSingleShot(true);
	QEventLoop eventLoop;
	QObject::connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
	QDnsLookup lookup;
	QObject::connect(&lookup, &QDnsLookup::finished, &eventLoop, &QEventLoop::quit);
	lookup.setType(QDnsLookup::A);
	lookup.setName("dns.google.com");
	lookup.lookup();
	timer.start();
	eventLoop.exec();
	if(timer.remainingTime() == 0)
	{
		lookup.abort();
		return false;
	}
	if(lookup.error() != QDnsLookup::NoError)
		return false;
	return true;
}
