/*
 * FileUtils.cpp
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

#include "FileUtils.h"

/*!
 * Returns the path to the program configuration directory.\n
 * For example: <tt>/home/user/.config/Open-Typer</tt>
 */
QString FileUtils::configLocation(void)
{
#ifdef Q_OS_WINDOWS
	QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
#else
	QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Open-Typer";
#endif
	QDir dir;
	dir.mkpath(path);
	return path;
}

/*! Returns the path to the main configuration file, currently \c config.ini in the program configuration directory. */
QString FileUtils::mainSettingsLocation(void)
{
	return configLocation() + "/config.ini";
}

/*! Uses QFileInfo to get the file name of a file. */
QString FileUtils::fileName(QString path)
{
	QFileInfo fileInfo(path);
	return fileInfo.fileName();
}
