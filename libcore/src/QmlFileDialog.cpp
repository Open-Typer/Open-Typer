/*
 * QmlFileDialog.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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
#include <QFileDialog>
#include "QmlFileDialog.h"

/*!
 * Sets name filters, for example: [ "Image files (*.jpg *.png)", "Text files (*.txt)" ]\n
 * "All files" will be included by default. To disable it, use setShowAllFiles(false).
 */
void QmlFileDialog::setNameFilters(QStringList filters)
{
	m_nameFilters = filters;
	emit nameFiltersChanged(filters);
}

/*! Returns list of name filters. */
QStringList QmlFileDialog::nameFilters(void)
{
	return m_nameFilters;
}

/*! Toggles "All files" name filter. */
void QmlFileDialog::setShowAllFiles(bool value)
{
	m_showAllFiles = value;
	emit showAllFilesChanged(value);
}

/*! Returns true if "All files" name filter is enabled. */
bool QmlFileDialog::showAllFiles(void)
{
	return m_showAllFiles;
}

/*! Shows a file dialog and emits fileContentReady() after a file is selected. */
void QmlFileDialog::getOpenFileContent(void)
{
	auto fileContentReadyLambda = [this](const QString &fileName, const QByteArray &fileContent) {
		if(!fileName.isEmpty())
		{
			emit fileContentReady(fileContent);
		}
	};
#ifdef Q_OS_WASM
	QFileDialog::getOpenFileContent(QString(), fileContentReadyLambda);
#else
	QString filtersStr = m_nameFilters.join(";;");
	if(m_showAllFiles)
		filtersStr += tr("All files") + " (*)";
	QString fileName = QFileDialog::getOpenFileName(nullptr, QString(), QString(), filtersStr);
	if(fileName != "")
	{
		QFile file(fileName);
		if(file.open(QIODevice::ReadOnly | QIODevice::Text))
			fileContentReadyLambda(fileName, file.readAll());
	}
#endif
}
