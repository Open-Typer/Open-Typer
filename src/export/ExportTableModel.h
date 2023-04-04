/*
 * ExportTableModel.h
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

#ifndef EXPORTTABLEMODEL_H
#define EXPORTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QSize>
#include "validator/ExerciseValidator.h"

/*!
 * \brief The ExportTableModel class provides a table model for typing test results.
 *
 * Note: QTableView doesn't support rich text, so you need to replace the cells with widgets that support it.
 */
class Q_DECL_EXPORT ExportTableModel : public QAbstractTableModel
{
		Q_OBJECT
		Q_PROPERTY(QString studentName READ studentName WRITE setStudentName NOTIFY studentNameChanged)
		Q_PROPERTY(QString className READ className WRITE setClassName NOTIFY classNameChanged)
		Q_PROPERTY(QString testNumber READ testNumber WRITE setTestNumber NOTIFY testNumberChanged)
		Q_PROPERTY(QString grade READ grade WRITE setGrade NOTIFY gradeChanged)
		Q_PROPERTY(ExerciseValidator *validator READ validator WRITE setValidator NOTIFY validatorChanged)
	public:
		explicit ExportTableModel(QObject *parent = nullptr);
		int rowCount(const QModelIndex & = QModelIndex()) const override;
		int columnCount(const QModelIndex & = QModelIndex()) const override;
		QVariant data(const QModelIndex &index, int role) const override;
		bool setData(const QModelIndex &index, const QVariant &value, int role) override;
		void loadData(void);
		QHash<int, QByteArray> roleNames() const override;
		QSize span(const QModelIndex &index) const override;
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

	private:
		static const QMap<QPair<int, int>, QPair<int, int>> spans;
		QMap<QPair<int, int>, QVariant> tableData;
		QString m_studentName;
		QString m_className;
		QString m_testNumber;
		QString m_grade;
		ExerciseValidator *m_validator = nullptr;

	signals:
		void studentNameChanged(QString name);
		void classNameChanged(QString name);
		void testNumberChanged(QString number);
		void gradeChanged(QString grade);
		void validatorChanged(ExerciseValidator *validator);
};

#endif // EXPORTTABLEMODEL_H
