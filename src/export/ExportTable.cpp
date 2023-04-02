/*
 * ExportTable.cpp
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

#include <QHeaderView>
#include <QLabel>
#include "ExportTable.h"
#include "ExportTableModel.h"
#include "framework/ui/ThemeEngine.h"

/*! Constructs ExportTable. */
ExportTable::ExportTable(QWidget *parent) :
	QTableView(parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	horizontalHeader()->hide();
	verticalHeader()->hide();
	setSizeAdjustPolicy(AdjustToContents);
	updateStyle();
	QMetaObject::invokeMethod(this, "updateModel", Qt::QueuedConnection);
	// Connections
	connect(this, &ExportTable::studentNameChanged, this, &ExportTable::updateModel);
	connect(this, &ExportTable::classNameChanged, this, &ExportTable::updateModel);
	connect(this, &ExportTable::testNumberChanged, this, &ExportTable::updateModel);
	connect(this, &ExportTable::gradeChanged, this, &ExportTable::updateModel);
	connect(this, &ExportTable::validatorChanged, this, &ExportTable::updateModel);
}

void ExportTable::setModel(QAbstractItemModel *model)
{
	QTableView::setModel(model);
	for(int row = 0; row < this->model()->rowCount(); row++)
	{
		for(int col = 0; col < this->model()->columnCount(); col++)
		{
			QSize span = this->model()->span(this->model()->index(row, col));
			if(span.width() + span.height() != 2)
				this->setSpan(row, col, span.height(), span.width());
		}
	}
}

/*! Reloads the table model. */
void ExportTable::updateModel(void)
{
	QItemSelectionModel *m = selectionModel();
	ExportTableModel *model = new ExportTableModel;
	model->setStudentName(m_studentName);
	model->setClassName(m_className);
	model->setTestNumber(m_testNumber);
	model->setGrade(m_grade);
	model->setValidator(m_validator);
	setModel(model);
	m->deleteLater();

	for(int i = 0; i < model->rowCount(); i++)
	{
		for(int j = 0; j < model->columnCount(); j++)
		{
			QModelIndex index = model->index(i, j);
			QLabel *label = new QLabel(model->data(index, Qt::DisplayRole).toString(), this);
			label->setGeometry(0, 0, 0, 0);
			setIndexWidget(index, label);
			model->setData(index, "", Qt::DisplayRole);
		}
	}
	resizeColumnsToContents();
	resizeRowsToContents();
	adjustSize();
	emit modelChanged();
	emit contentWidthChanged(contentWidth());
	emit contentHeightChanged(contentHeight());
}

/*! Updates the style sheet. */
void ExportTable::updateStyle(bool forceLight)
{
	bool dark;
	if(forceLight)
		dark = false;
	else
		dark = globalThemeEngine.theme() == ThemeEngine::Theme::DarkTheme;
	QString style;
	style += "QWidget { background-color: transparent; border: none; color: %1; }";
	style += "QTableView::item { border: 1px solid %1; padding: 5px; }";
	style += "QWidget::item:hover { background-color: transparent; color: %1; }";
	setStyleSheet(style.arg(dark ? "white" : "black"));
}

/*! Table width. */
int ExportTable::contentWidth(void)
{
	if(viewport()->width() > size().width())
		return viewport()->width();
	else
		return size().width();
}

/*! Table height. */
int ExportTable::contentHeight(void)
{
	if(viewport()->height() > size().height())
		return viewport()->height();
	else
		return size().height();
}

/*! The name of the student. */
QString ExportTable::studentName(void)
{
	return m_studentName;
}

void ExportTable::setStudentName(QString name)
{
	m_studentName = name;
	emit studentNameChanged(name);
}

/*! The name of the class. */
QString ExportTable::className(void)
{
	return m_className;
}

void ExportTable::setClassName(QString name)
{
	m_className = name;
	emit classNameChanged(name);
}

/*! The typing test number. */
QString ExportTable::testNumber(void)
{
	return m_testNumber;
}

void ExportTable::setTestNumber(QString number)
{
	m_testNumber = number;
	emit testNumberChanged(number);
}

/*! The grade. */
QString ExportTable::grade(void)
{
	return m_grade;
}

void ExportTable::setGrade(QString grade)
{
	m_grade = grade;
	emit gradeChanged(grade);
}

/*! The ExerciseValidator used to get the typing results. */
ExerciseValidator *ExportTable::validator(void)
{
	return m_validator;
}

void ExportTable::setValidator(ExerciseValidator *validator)
{
	m_validator = validator;
	emit validatorChanged(validator);
}

int ExportTable::sizeHintForColumn(int column) const
{
	int maxValue = 0;
	// Ignore column 0 because the title has a large size hint
	for(int i = 1; i < model()->rowCount(); i++)
		maxValue = std::max(indexWidget(model()->index(i, column))->sizeHint().width() + 20, maxValue);
	return maxValue;
}

int ExportTable::sizeHintForRow(int row) const
{
	int maxValue = 0;
	for(int i = 0; i < model()->columnCount(); i++)
		maxValue = std::max(indexWidget(model()->index(row, i))->sizeHint().height() + 20, maxValue);
	return maxValue;
}
