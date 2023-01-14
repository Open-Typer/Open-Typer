/*
 * StatsDialog.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
 * Copyright (C) 2022 - Roker2
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

#include "StatsDialog.h"
#include "ui_StatsDialog.h"

/*! Constructs StatsDialog. */
StatsDialog::StatsDialog(bool offline, QList<QStringList> data, QPair<int, int> studentComparison, QString configName, int lesson, int sublesson, int exercise, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::StatsDialog),
	m_packName(configName),
	m_lesson(lesson),
	m_sublesson(sublesson),
	m_exercise(exercise),
	m_offline(offline),
	m_data(data),
	m_comparison(studentComparison)
{
	ui->setupUi(this);
	ui->statsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->statsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	// Load data
	loadData();
	// Connections
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

/*! Destroys the StatsDialog object. */
StatsDialog::~StatsDialog()
{
	delete ui;
}

/*! Loads history data. */
void StatsDialog::loadData(void)
{
	if(chartsCreated)
	{
		speedSeries->deleteLater();
		speedChartView->close();
		speedChart->deleteLater();
		mistakesSeries->deleteLater();
		mistakesChartView->close();
		mistakesChart->deleteLater();
		timeSeries->deleteLater();
		timeChartView->close();
		timeChart->deleteLater();
		chartsCreated = 0;
	}
	QStringList response;
	if(m_offline)
		response = QStringList({ QString("ok"), QString::number(HistoryParser::historySize(m_packName, m_lesson, m_sublesson, m_exercise)) });
	else
		response = QStringList({ QString("ok"), QString::number(m_data.count()) });
	if(response[0] != "ok")
	{
		QMetaObject::invokeMethod(this, "reject", Qt::QueuedConnection);
		return;
	}
	ui->statsTable->clear();
	ui->statsTable->setColumnCount(3);
	ui->statsTable->setHorizontalHeaderLabels({ tr("Speed"), tr("Mistakes"), tr("Time") });
	int i, count = response[1].toInt();
	ui->statsTable->setRowCount(count);
	speedSeries = new QLineSeries;
	mistakesSeries = new QLineSeries;
	timeSeries = new QLineSeries;
	QTableWidgetItem *item;
	for(i = 0; i < count; i++)
	{
		if(m_offline)
		{
			response = QStringList({ "ok" });
			QStringList entry = HistoryParser::historyEntry(m_packName, m_lesson, m_sublesson, m_exercise, i);
			for(int i2 = 0; i2 < entry.count(); i2++)
				response += entry[i2].toUtf8();
		}
		else
			response = m_data[i];
		if(response[0] != "ok")
		{
			QMetaObject::invokeMethod(this, "reject", Qt::QueuedConnection);
			return;
		}
		// Speed
		item = new QTableWidgetItem(QString(response[1]));
		ui->statsTable->setItem(i, 0, item);
		speedSeries->append(i, response[1].toInt());
		// Mistakes
		item = new QTableWidgetItem(QString(response[2]));
		ui->statsTable->setItem(i, 1, item);
		mistakesSeries->append(i, response[2].toInt());
		// Time
		item = new QTableWidgetItem(QString(response[3]));
		ui->statsTable->setItem(i, 2, item);
		timeSeries->append(i, response[3].toInt());
	}
	// Set up charts
	// Speed
	speedChart = new QChart;
	speedChartView = new QChartView(speedChart, ui->chartTab);
	speedChartView->setAttribute(Qt::WA_DeleteOnClose);
	ui->chartTabLayout->addWidget(speedChartView);
	speedChart->addSeries(speedSeries);
	speedChart->legend()->hide();
	speedChart->createDefaultAxes();
	speedChart->axes(Qt::Vertical).value(0)->setMin(0);
	speedChart->setTitle(tr("Speed"));
	// Mistakes
	mistakesChart = new QChart;
	mistakesChartView = new QChartView(mistakesChart, ui->chartTab);
	mistakesChartView->setAttribute(Qt::WA_DeleteOnClose);
	ui->chartTabLayout->addWidget(mistakesChartView);
	mistakesChart->addSeries(mistakesSeries);
	mistakesChart->legend()->hide();
	mistakesChart->createDefaultAxes();
	mistakesChart->axes(Qt::Vertical).value(0)->setMin(0);
	mistakesChart->setTitle(tr("Mistakes"));
	// Time
	timeChart = new QChart;
	timeChartView = new QChartView(timeChart, ui->chartTab);
	timeChartView->setAttribute(Qt::WA_DeleteOnClose);
	ui->chartTabLayout->addWidget(timeChartView);
	timeChart->addSeries(timeSeries);
	timeChart->legend()->hide();
	timeChart->createDefaultAxes();
	timeChart->axes(Qt::Vertical).value(0)->setMin(0);
	timeChart->setTitle(tr("Time"));
	// Set charts theme
	QChart::ChartTheme theme;
	if(globalThemeEngine.theme() == ThemeEngine::Theme::DarkTheme)
		theme = QChart::ChartThemeDark;
	else
		theme = QChart::ChartThemeLight;
	speedChart->setTheme(theme);
	mistakesChart->setTheme(theme);
	timeChart->setTheme(theme);
	chartsCreated = 1;
	// Load comparison data
	if(m_offline)
	{
		ui->betterStudentsLabel->hide();
		ui->worseStudentsLabel->hide();
	}
	else
	{
		ui->betterStudentsLabel->setText(tr("Better students: %1").arg(m_comparison.first));
		ui->worseStudentsLabel->setText(tr("Worse students: %1").arg(m_comparison.second));
	}
}

/*! Sets the pack name. */
void StatsDialog::setPackName(QString name)
{
	m_packName = name;
	emit packNameChanged(name);
}

/*! Returns the pack name. */
QString StatsDialog::packName(void)
{
	return m_packName;
}

/*! Sets the lesson ID. */
void StatsDialog::setLesson(int lesson)
{
	m_lesson = lesson;
	emit lessonChanged(lesson);
}

/*! Returns the lesson ID. */
int StatsDialog::lesson(void)
{
	return m_lesson;
}

/*! Sets the sublesson ID. */
void StatsDialog::setSublesson(int sublesson)
{
	m_sublesson = sublesson;
	emit sublessonChanged(sublesson);
}

/*! Returns the sublesson ID. */
int StatsDialog::sublesson(void)
{
	return m_sublesson;
}

/*! Sets the exercise ID. */
void StatsDialog::setExercise(int exercise)
{
	m_exercise = exercise;
	emit exerciseChanged(exercise);
}

/*! Returns the exercise ID. */
int StatsDialog::exercise(void)
{
	return m_exercise;
}
