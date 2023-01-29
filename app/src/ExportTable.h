/*
 * ExportTable.h
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

#ifndef EXPORTTABLE_H
#define EXPORTTABLE_H

#include <QTableView>
#include "ExerciseValidator.h"

/*! \brief The ExportTableModel class provides a table for typing test results. */
class ExportTable : public QTableView
{
		Q_OBJECT
		Q_PROPERTY(int contentWidth READ contentWidth NOTIFY contentWidthChanged)
		Q_PROPERTY(int contentHeight READ contentHeight NOTIFY contentHeightChanged)
		Q_PROPERTY(QString studentName READ studentName WRITE setStudentName NOTIFY studentNameChanged)
		Q_PROPERTY(QString className READ className WRITE setClassName NOTIFY classNameChanged)
		Q_PROPERTY(QString testNumber READ testNumber WRITE setTestNumber NOTIFY testNumberChanged)
		Q_PROPERTY(QString grade READ grade WRITE setGrade NOTIFY gradeChanged)
		Q_PROPERTY(ExerciseValidator *validator READ validator WRITE setValidator NOTIFY validatorChanged)
	public:
		ExportTable(QWidget *parent = nullptr);
		void setModel(QAbstractItemModel *model) override;
		Q_INVOKABLE void updateModel(void);
		Q_INVOKABLE void updateStyle(bool forceLight = false);
		int contentWidth(void);
		int contentHeight(void);
		QString studentName(void);
		void setStudentName(QString name);
		QString className(void);
		void setClassName(QString name);
		QString testNumber(void);
		void setTestNumber(QString number);
		QString grade(void);
		void setGrade(QString grade);
		ExerciseValidator *validator(void);
		void setValidator(ExerciseValidator *validator);

	protected:
		int sizeHintForColumn(int column) const override;
		int sizeHintForRow(int row) const override;

	private:
		QString m_studentName;
		QString m_className;
		QString m_testNumber;
		QString m_grade;
		ExerciseValidator *m_validator = nullptr;

	signals:
		void contentWidthChanged(int width);
		void contentHeightChanged(int height);
		void studentNameChanged(QString name);
		void classNameChanged(QString name);
		void testNumberChanged(QString number);
		void gradeChanged(QString grade);
		void validatorChanged(ExerciseValidator *validator);
		void modelChanged();
};

#endif // EXPORTTABLE_H
