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
	versionHdw = new Downloader(QString(_VERSION_H_URL),this);
	connect(versionHdw,SIGNAL(downloaded()),this,SLOT(initUpdate()));
}

Updater::~Updater() { }

void Updater::initUpdate()
{
	versionH = (char*) malloc(1024);
	strcpy(versionH,qPrintable(QString(versionHdw->downloadedData())));
	char *newVer = readVersion(versionH);
	printf("latest version: %s\n",newVer);
	if(strcmp(newVer,_VERSION) != 0)
		printf("New version found!\n");
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
