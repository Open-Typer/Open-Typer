/*
 * StudentDetails.cpp
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

#include "server/widgets/StudentDetails.h"
#include "ui_StudentDetails.h"

/*! Constructs StudentDetails. */
StudentDetails::StudentDetails(int openClassID, int id, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::StudentDetails)
{
	ui->setupUi(this);
	classID = openClassID;
	studentID = id;
	ui->titleLabel->setText(dbMgr.userName(studentID));
	ui->statsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->statsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	// Set up charts
	speedChart = new QChart;
	speedSeries = new QLineSeries;
	mistakesChart = new QChart;
	mistakesSeries = new QLineSeries;
	timeChart = new QChart;
	timeSeries = new QLineSeries;
	// Create chart views
	QChartView *speedChartView = new QChartView(speedChart, ui->statsChartTab);
	QChartView *mistakesChartView = new QChartView(mistakesChart, ui->statsChartTab);
	QChartView *timeChartView = new QChartView(timeChart, ui->statsChartTab);
	ui->statsChartTabLayout->addWidget(speedChartView);
	ui->statsChartTabLayout->addWidget(mistakesChartView);
	ui->statsChartTabLayout->addWidget(timeChartView);
	// Init charts
	// Speed
	speedChart->addSeries(speedSeries);
	speedChart->legend()->hide();
	speedChart->setTitle(tr("Speed"));
	// Mistakes
	mistakesChart->addSeries(mistakesSeries);
	mistakesChart->legend()->hide();
	mistakesChart->setTitle(tr("Mistakes"));
	// Time
	timeChart->addSeries(timeSeries);
	timeChart->legend()->hide();
	timeChart->setTitle(tr("Time"));
	// Connections
	connect(ui->backButton, &QPushButton::clicked, this, &StudentDetails::backClicked);
	connect(ui->packBox, SIGNAL(activated(int)), this, SLOT(refresh()));
	connect(ui->lessonBox, SIGNAL(activated(int)), this, SLOT(refresh()));
	connect(ui->sublessonBox, SIGNAL(activated(int)), this, SLOT(refresh()));
	connect(ui->exerciseBox, SIGNAL(activated(int)), this, SLOT(refresh()));
	connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));
	connect(ui->statsTable, SIGNAL(itemSelectionChanged()), SLOT(refreshTable()));
	refresh();
}

/*! Destroys the StudentDetails object. */
StudentDetails::~StudentDetails()
{
	delete ui;
}

/*!
 * Connected from exercise selection comboboxes (%activated() signal) and refreshButton->clicked().\n
 * Refreshes comboboxes and other widgets.
 *
 * \see refreshTable()
 */
void StudentDetails::refresh(void)
{
	// Save old indexes
	int oldP, oldL, oldS, oldE;
	oldP = ui->packBox->currentIndex();
	oldL = ui->lessonBox->currentIndex();
	oldS = ui->sublessonBox->currentIndex();
	oldE = ui->exerciseBox->currentIndex();
	// Packs
	ui->packBox->clear();
	QStringList packs = dbMgr.studentPacks(classID, studentID);
	for(int i = 0; i < packs.count(); i++)
		ui->packBox->addItem(BuiltInPacks::packName(packs[i]));
	if(ui->packBox->count() == 0)
	{
		ui->exerciseFrame->hide();
		ui->statsFrame->hide();
		ui->noInfoFrame->show();
		return;
	}
	else
	{
		ui->exerciseFrame->show();
		ui->statsFrame->show();
		ui->noInfoFrame->hide();
	}
	if(oldP == -1)
		oldP = 0;
	ui->packBox->setCurrentIndex(oldP);
	// Lessons
	ui->lessonBox->clear();
	QList<int> lessons = dbMgr.studentLessons(classID, studentID, packs[oldP]);
	for(int i = 0; i < lessons.count(); i++)
		ui->lessonBox->addItem(ConfigParser::lessonTr(lessons[i]));
	if(oldL == -1)
		oldL = 0;
	ui->lessonBox->setCurrentIndex(oldL);
	// Sublessons
	ui->sublessonBox->clear();
	QList<int> sublessons = dbMgr.studentSublessons(classID, studentID, packs[oldP],
		lessons[ui->lessonBox->currentIndex()]);
	for(int i = 0; i < sublessons.count(); i++)
		ui->sublessonBox->addItem(ConfigParser::sublessonName(sublessons[i]));
	if(oldS == -1)
		oldS = 0;
	ui->sublessonBox->setCurrentIndex(oldS);
	// Exercise
	ui->exerciseBox->clear();
	QList<int> exercises = dbMgr.studentExercises(classID, studentID, packs[oldP],
		lessons[ui->lessonBox->currentIndex()],
		sublessons[ui->sublessonBox->currentIndex()]);
	for(int i = 0; i < exercises.count(); i++)
		ui->exerciseBox->addItem(ConfigParser::exerciseTr(exercises[i]));
	if(oldE == -1)
		oldE = 0;
	ui->exerciseBox->setCurrentIndex(oldE);
	// Table
	refreshTable();
}

/*!
 * Connected from statsTable->itemSelectionChanged().\n
 * Refreshes statsTable.
 *
 * \see refresh()
 */
void StudentDetails::refreshTable(void)
{
	if(ui->packBox->count() == 0)
		return;
	// Init lists
	QStringList packs = dbMgr.studentPacks(classID, studentID);
	QList<int> lessons = dbMgr.studentLessons(classID, studentID, packs[ui->packBox->currentIndex()]);
	QList<int> sublessons = dbMgr.studentSublessons(classID, studentID, packs[ui->packBox->currentIndex()],
		lessons[ui->lessonBox->currentIndex()]);
	QList<int> exercises = dbMgr.studentExercises(classID, studentID, packs[ui->packBox->currentIndex()],
		lessons[ui->lessonBox->currentIndex()],
		sublessons[ui->sublessonBox->currentIndex()]);
	// Init table
	ui->statsTable->clear();
	ui->statsTable->setColumnCount(3);
	ui->statsTable->setHorizontalHeaderLabels({ tr("Speed"), tr("Mistakes"), tr("Time") });
	QString pack = packs[ui->packBox->currentIndex()];
	int lesson = lessons[ui->lessonBox->currentIndex()];
	int sublesson = sublessons[ui->sublessonBox->currentIndex()];
	int exercise = exercises[ui->exerciseBox->currentIndex()];
	QList<QVariantMap> entries = dbMgr.historyEntries(classID, studentID, pack, lesson, sublesson, exercise);
	int i, count = entries.count();
	ui->statsTable->setRowCount(count);
	// Load entries
	QTableWidgetItem *item;
	for(i = 0; i < count; i++)
	{
		QVariantMap entry = entries[i];
		// Speed
		item = new QTableWidgetItem(QString::number(entry["speed"].toInt()));
		ui->statsTable->setItem(i, 0, item);
		// Mistakes
		item = new QTableWidgetItem(QString::number(entry["mistakes"].toInt()));
		ui->statsTable->setItem(i, 1, item);
		// Time
		item = new QTableWidgetItem(QString::number(entry["duration"].toInt()));
		ui->statsTable->setItem(i, 2, item);
	}
	// Refresh charts
	refreshCharts();
}

/*! Refreshes the charts. */
void StudentDetails::refreshCharts(void)
{
	speedSeries->clear();
	mistakesSeries->clear();
	timeSeries->clear();
	for(int i = 0; i < ui->statsTable->rowCount(); i++)
	{
		// Speed
		speedSeries->append(i, ui->statsTable->item(i, 0)->text().toInt());
		// Mistakes
		mistakesSeries->append(i, ui->statsTable->item(i, 1)->text().toInt());
		// Time
		timeSeries->append(i, ui->statsTable->item(i, 2)->text().toInt());
	}
	// Speed
	speedChart->removeSeries(speedSeries);
	speedChart->addSeries(speedSeries);
	speedChart->createDefaultAxes();
	speedChart->axes(Qt::Vertical).value(0)->setMin(0);
	// Mistakes
	mistakesChart->removeSeries(mistakesSeries);
	mistakesChart->addSeries(mistakesSeries);
	mistakesChart->createDefaultAxes();
	mistakesChart->axes(Qt::Vertical).value(0)->setMin(0);
	// Time
	timeChart->removeSeries(timeSeries);
	timeChart->addSeries(timeSeries);
	timeChart->createDefaultAxes();
	timeChart->axes(Qt::Vertical).value(0)->setMin(0);
	// Set theme
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QChart::ChartTheme theme;
	if(settings.value("theme/theme", "0").toInt() == 1)
		theme = QChart::ChartThemeDark;
	else
		theme = QChart::ChartThemeLight;
	speedChart->setTheme(theme);
	mistakesChart->setTheme(theme);
	timeChart->setTheme(theme);
}

/*! Overrides QWidget#changeEvent(). */
void StudentDetails::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::StyleChange)
		refreshCharts();
	else if(event->type() == QEvent::LanguageChange)
	{
		ui->retranslateUi(this);
		refreshTable();
	}
	QWidget::changeEvent(event);
}
