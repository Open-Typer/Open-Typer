/*
 * FileUtils.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2023 - adazem009
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

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QStandardPaths>
#include <QDir>
#include "../IFileUtils.h"

/*! \brief The FileUtils class contains functions for file or directory paths. */
class Q_DECL_EXPORT FileUtils : public QObject, public IFileUtils
{
		Q_OBJECT
	public:
		static std::shared_ptr<FileUtils> instance();
		Q_INVOKABLE QString configLocation(void) override;
		Q_INVOKABLE QString mainSettingsLocation(void) override;
		Q_INVOKABLE QString classConfigLocation(void) override;
		Q_INVOKABLE QString fileName(QString path) override;

	private:
		static std::shared_ptr<FileUtils> m_instance;
};

#endif // FILEUTILS_H
