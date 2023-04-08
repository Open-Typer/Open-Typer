/*
 * ExportTableModel.cpp
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

#include <QDate>
#include "ExportTableModel.h"
#include "global/internal/Settings.h"

const QMap<QPair<int, int>, QPair<int, int>> ExportTableModel::spans = {
	{ { 0, 0 }, { 1, 4 } },
	{ { 2, 2 }, { 2, 1 } },
	{ { 2, 3 }, { 2, 1 } },
	{ { 4, 2 }, { 2, 1 } },
	{ { 4, 3 }, { 2, 1 } },
	{ { 6, 2 }, { 3, 1 } },
	{ { 6, 3 }, { 3, 1 } }
};

/*! Constructs ExportTableModel. */
ExportTableModel::ExportTableModel(QObject *parent) :
	QAbstractTableModel(parent)
{
	connect(this, &ExportTableModel::studentNameChanged, this, &ExportTableModel::loadData);
	connect(this, &ExportTableModel::classNameChanged, this, &ExportTableModel::loadData);
	connect(this, &ExportTableModel::testNumberChanged, this, &ExportTableModel::loadData);
	connect(this, &ExportTableModel::gradeChanged, this, &ExportTableModel::loadData);
	connect(this, &ExportTableModel::validatorChanged, this, &ExportTableModel::loadData);
}

int ExportTableModel::rowCount(const QModelIndex &) const
{
	return 9;
}

int ExportTableModel::columnCount(const QModelIndex &) const
{
	return 4;
}

QVariant ExportTableModel::data(const QModelIndex &index, int role) const
{
	auto keys = tableData.keys();
	switch(role)
	{
		case Qt::DisplayRole:
			for(int i = 0; i < keys.length(); i++)
			{
				if((keys[i].first == index.row()) && (keys[i].second == index.column()))
					return tableData[keys[i]];
			}
			break;
		default:
			break;
	}
	return QVariant();
}

bool ExportTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	switch(role)
	{
		case Qt::DisplayRole:
			tableData.insert({ index.row(), index.column() }, value);
			return true;
		default:
			return false;
	}
}

/*! Loads the table data. */
void ExportTableModel::loadData(void)
{
	tableData.clear();
	if(!m_validator)
		return;
	int penalty = Settings::errorPenalty();
	int netHits = std::max(0, m_validator->grossHits() - m_validator->mistakeCount() * penalty);
	qreal timeMins = m_validator->time() / 60.0;
	qreal netHitsPerMinute = netHits / timeMins;
	qreal inaccuracy = (m_validator->mistakeCount() * 100) / (double) m_validator->grossHits();
	// Caption
	tableData.insert({ 0, 0 }, "<h1><b><center>" + tr("Typewriting performance result") + "</center></b></h1>");
	// Name
	tableData.insert({ 1, 0 }, "<b>" + tr("Name: %1").arg(m_studentName) + "</b>");
	// Class
	tableData.insert({ 1, 1 }, "<b>" + tr("Class: %1").arg(m_className) + "</b>");
	// Date
	tableData.insert({ 1, 2 }, "<b>" + tr("Date: %1").arg(QDate::currentDate().toString()) + "</b>");
	// Test number
	tableData.insert({ 1, 3 }, "<b>" + tr("Number: %1").arg(m_testNumber) + "</b>");
	// Number of gross hits
	tableData.insert({ 2, 0 }, "<b>" + tr("Number of gross hits") + "</b>");
	tableData.insert({ 2, 1 }, m_validator->grossHits());
	// Number of mistakes
	tableData.insert({ 3, 0 }, tr("Number of mistakes"));
	tableData.insert({ 3, 1 }, m_validator->mistakeCount());
	// Mistake penalty
	tableData.insert({ 4, 0 }, tr("Mistake penalty"));
	tableData.insert({ 4, 1 }, penalty);
	// Number of penalty hits
	tableData.insert({ 5, 0 }, tr("Number of penalty hits"));
	tableData.insert({ 5, 1 }, penalty * m_validator->mistakeCount());
	// Number of net hits
	tableData.insert({ 6, 0 }, tr("Number of net hits"));
	tableData.insert({ 6, 1 }, netHits);
	// Time
	tableData.insert({ 7, 0 }, tr("Time (min)"));
	tableData.insert({ 7, 1 }, QString::number(timeMins, 'g', 2));
	// Number of net hits per minute
	tableData.insert({ 8, 0 }, "<b>" + tr("Number of net hits per minute") + "</b>");
	tableData.insert({ 8, 1 }, ((int) (netHitsPerMinute * 100)) / 100.0);
	// Inaccuracy
	tableData.insert({ 2, 2 }, tr("Inaccuracy"));
	tableData.insert({ 2, 3 }, ((int) (inaccuracy * 100)) / 100.0);
	// Achieved performance
	tableData.insert({ 4, 2 }, tr("Achieved performance"));
	tableData.insert({ 4, 3 }, (int) netHitsPerMinute);
	// Grade
	tableData.insert({ 6, 2 }, "<b>" + tr("Grade") + "</b>");
	tableData.insert({ 6, 3 }, m_grade);
}

QHash<int, QByteArray> ExportTableModel::roleNames() const
{
	return { { Qt::DisplayRole, "display" } };
}

QSize ExportTableModel::span(const QModelIndex &index) const
{
	auto keys = spans.keys();
	for(int i = 0; i < keys.length(); i++)
	{
		if((keys[i].first == index.row()) && (keys[i].second == index.column()))
		{
			auto out = spans[keys[i]];
			return QSize(out.second, out.first);
		}
	}
	return QAbstractItemModel::span(index);
}

/*! The name of the student. */
QString ExportTableModel::studentName(void)
{
	return m_studentName;
}

void ExportTableModel::setStudentName(QString name)
{
	m_studentName = name;
	emit studentNameChanged(name);
}

/*! The name of the class. */
QString ExportTableModel::className(void)
{
	return m_className;
}

void ExportTableModel::setClassName(QString name)
{
	m_className = name;
	emit classNameChanged(name);
}

/*! The typing test number. */
QString ExportTableModel::testNumber(void)
{
	return m_testNumber;
}

void ExportTableModel::setTestNumber(QString number)
{
	m_testNumber = number;
	emit testNumberChanged(number);
}

/*! The grade. */
QString ExportTableModel::grade(void)
{
	return m_grade;
}

void ExportTableModel::setGrade(QString grade)
{
	m_grade = grade;
	emit gradeChanged(grade);
}

/*! The ExerciseValidator used to get the typing results. */
ExerciseValidator *ExportTableModel::validator(void)
{
	return m_validator;
}

void ExportTableModel::setValidator(ExerciseValidator *validator)
{
	m_validator = validator;
	emit validatorChanged(validator);
}
