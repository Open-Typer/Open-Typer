/*
 * main.cpp
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

#include <QApplication>
#include <QLabel>
#include <QTranslator>
#include "opentyper.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	// Load translations
	QTranslator* translator = new QTranslator();
	if(translator->load(QLocale(), QLatin1String("Open-Typer"), QLatin1String("_"), QLatin1String(":/res/lang")))
		a.installTranslator(translator);
	else
		printf("D: Failed to load language\n");
	OpenTyper w;
	w.setWindowState(Qt::WindowMaximized);
	w.show();
	return a.exec();
}
