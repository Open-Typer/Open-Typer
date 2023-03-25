/*
 * QmlFileDialog.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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

#ifndef QMLFILEDIALOG_H
#define QMLFILEDIALOG_H

#include <QObject>

/*!
 * \brief The QmlFileDialog class provides a file dialog for QML.
 * This file dialog supports file uploads on WebAssembly.
 */
class Q_DECL_EXPORT QmlFileDialog : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QStringList nameFilters READ nameFilters WRITE setNameFilters NOTIFY nameFiltersChanged)
		Q_PROPERTY(bool showAllFiles READ showAllFiles WRITE setShowAllFiles NOTIFY showAllFilesChanged)
		Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
		Q_PROPERTY(QString shortFileName READ shortFileName NOTIFY shortFileNameChanged)
	public:
		QStringList nameFilters(void);
		void setNameFilters(QStringList filters);
		bool showAllFiles(void);
		void setShowAllFiles(bool value);
		QString fileName(void);
		QString shortFileName(void);
		Q_INVOKABLE void getOpenFileContent(void);

	private:
		QStringList m_nameFilters;
		bool m_showAllFiles = true;
		QString m_fileName;

	signals:
		void nameFiltersChanged(QStringList filters);
		void showAllFilesChanged(bool value);
		void fileNameChanged(QString name);
		void shortFileNameChanged(QString name);
		void fileContentReady(QString content);
};

#endif // QMLFILEDIALOG_H
