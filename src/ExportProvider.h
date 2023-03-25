/*
 * ExportWidget.h
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

#ifndef EXPORTPROVIDER_H
#define EXPORTPROVIDER_H

#include <QWidget>
#include <QTextEdit>
#include "ExerciseValidator.h"
#include "ExportTable.h"

/*! \brief The ExportProvider class provides methods for the export dialog. */
class Q_DECL_EXPORT ExportProvider : public QObject
{
		Q_OBJECT
		Q_PROPERTY(ExerciseValidator *validator READ validator WRITE setValidator NOTIFY validatorChanged)
		Q_PROPERTY(QString exportText READ exportText NOTIFY exportTextChanged)
		Q_PROPERTY(ExportTable *table READ table WRITE setTable NOTIFY tableChanged)
	public:
		explicit ExportProvider(QObject *parent = nullptr);
		ExerciseValidator *validator(void);
		void setValidator(ExerciseValidator *validator);
		QString exportText(void);
		ExportTable *table(void);
		void setTable(ExportTable *table);
		Q_INVOKABLE void print(void);

	private:
		ExerciseValidator *m_validator;
		QString m_exportText;
		ExportTable *m_table;

	signals:
		void validatorChanged(ExerciseValidator *validator);
		void exportTextChanged(QString text);
		void tableChanged(ExportTable *table);
};

#endif // EXPORTPROVIDER_H
