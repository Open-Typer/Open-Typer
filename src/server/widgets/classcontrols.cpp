/*
 * classcontrols.cpp
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

#include "server/widgets/classcontrols.h"
#include "ui_classcontrols.h"

/*! Constructs classControls. */
classControls::classControls(int openClassID, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::classControls)
{
	ui->setupUi(this);
	classID = openClassID;
	setupTable();
	verify();
	// Set up charts
	speedChart = new QChart;
	mistakesChart = new QChart;
	timeChart = new QChart;
	// Create chart views
	QChartView *speedChartView = new QChartView(speedChart, ui->chartFrame);
	QChartView *mistakesChartView = new QChartView(mistakesChart, ui->chartFrame);
	QChartView *timeChartView = new QChartView(timeChart, ui->chartFrame);
	ui->chartLayout->addWidget(speedChartView);
	ui->chartLayout->addWidget(mistakesChartView);
	ui->chartLayout->addWidget(timeChartView);
	// Init charts
	// Speed
	speedChart->setTitle(tr("Speed"));
	// Mistakes
	mistakesChart->setTitle(tr("Mistakes"));
	// Time
	timeChart->setTitle(tr("Time"));
	refreshCharts();
	// Connections
	connect(ui->loadExerciseButton,SIGNAL(clicked()),this,SLOT(loadExercise()));
	connect(ui->studentsTable,SIGNAL(itemSelectionChanged()),this,SLOT(verify()));
	connect(ui->studentsTable,&QTableWidget::itemDoubleClicked,this,&classControls::openDetails);
	connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addStudent()));
	connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(removeStudent()));
	connect(ui->editButton,SIGNAL(clicked()),this,SLOT(editStudent()));
	connect(ui->detailsButton,SIGNAL(clicked()),this,SLOT(openDetails()));
	connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(refreshCharts()));
	connect(ui->packBox,SIGNAL(activated(int)),this,SLOT(refreshCharts()));
	connect(ui->lessonBox,SIGNAL(activated(int)),this,SLOT(refreshCharts()));
	connect(ui->sublessonBox,SIGNAL(activated(int)),this,SLOT(refreshCharts()));
	connect(ui->exerciseBox,SIGNAL(activated(int)),this,SLOT(refreshCharts()));
	connect(ui->refreshButton,SIGNAL(clicked()),this,SLOT(refreshCharts()));
#ifndef Q_OS_WASM
	connect(serverPtr, &monitorServer::loggedInStudentsChanged, this, &classControls::setupTable);
#endif // Q_OS_WASM
}

/*! Destroys the classControls object. */
classControls::~classControls()
{
	delete ui;
#ifndef Q_OS_WASM
	if(serverPtr)
		disconnect(serverPtr, nullptr, nullptr, nullptr);
#endif
}


/*! Loads the students. */
void classControls::setupTable(void)
{
	ui->studentsTable->clear();
	ui->studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->studentsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	// Columns
	ui->studentsTable->setColumnCount(3);
	ui->studentsTable->setHorizontalHeaderLabels({ tr("Full name"), tr("Username"), tr("Status", "Online/offline status") });
	// Rows
	QList<int> students = dbMgr.studentIDs(classID);
	ui->studentsTable->setRowCount(students.count());
	for(int i=0; i < students.count(); i++)
	{
		// Full name
		QTableWidgetItem *item = new QTableWidgetItem(dbMgr.userName(students[i]));
		ui->studentsTable->setItem(i, 0, item);
		// Username
		item = new QTableWidgetItem(dbMgr.userNickname(students[i]));
		ui->studentsTable->setItem(i, 1, item);
		// Status
		QString status = tr("Offline");
#ifndef Q_OS_WASM
		if(serverPtr->isLoggedIn(dbMgr.userNickname(students[i])))
			status = tr("Online");
#endif // Q_OS_WASM
		item = new QTableWidgetItem(status);
		ui->studentsTable->setItem(i, 2, item);
	}
}

/*! Overrides QWidget#changeEvent(). */
void classControls::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::StyleChange)
		refreshCharts();
	else if(event->type() == QEvent::LanguageChange)
	{
		ui->retranslateUi(this);
		setupTable();
	}
	QWidget::changeEvent(event);
}

/*!
 * Connected from studentsTable->itemSelectionChanged().\n
 * Checks if everything is correct and enables buttons.
 */
void classControls::verify(void)
{
	// Check students table
	bool enable = ui->studentsTable->selectionModel()->hasSelection();
	ui->editButton->setEnabled(enable);
	ui->removeButton->setEnabled(enable);
	ui->detailsButton->setEnabled(enable);
}

/*!
 * Connected from addButton->clicked().\n
 * Opens studentEdit and adds a new student.
 *
 * \see studentEdit
 */
void classControls::addStudent(void)
{
	studentEdit dialog(true,classID);
	dialog.exec();
	setupTable();
}

/*!
 * Connected from removeButton->clicked().\n
 * Removes selected student.
 */
void classControls::removeStudent(void)
{
	int studentID = dbMgr.studentIDs(classID).value(ui->studentsTable->selectionModel()->selectedRows()[0].row());
	QMessageBox confirmDialog;
	confirmDialog.setWindowTitle(tr("Confirm"));
	confirmDialog.setText(tr("Are you sure you want to remove student %1?").arg(dbMgr.userName(studentID)));
	confirmDialog.setInformativeText(tr("This will remove whole training history of the student!"));
	QPushButton *yesButton = confirmDialog.addButton(tr("Yes"),QMessageBox::YesRole);
	QPushButton *noButton = confirmDialog.addButton(tr("No"),QMessageBox::NoRole);
	confirmDialog.setIcon(QMessageBox::Question);
	confirmDialog.exec();
	if(confirmDialog.clickedButton() == yesButton)
		dbMgr.removeStudentFromClass(studentID, classID);
	else if(confirmDialog.clickedButton() == noButton)
		return;
	setupTable();
}

/*!
 * Connected from editButton->clicked().\n
 * Opens studentEdit and edits the student.
 *
 * \see studentEdit
 */
void classControls::editStudent(void)
{
	studentEdit dialog(false, classID, dbMgr.studentIDs(classID).value(ui->studentsTable->selectionModel()->selectedRows()[0].row()));
	dialog.exec();
	setupTable();
}

/*!
 * Connected from detailsButton->clicked() and studentsTable->itemDoubleClicked().\n
 * Emits detailsClicked() signal.
 *
 * \see detailsClicked()
 */
void classControls::openDetails(void)
{
	emit detailsClicked(dbMgr.studentIDs(classID).value(ui->studentsTable->selectionModel()->selectedRows()[0].row()));
}

/*! Refreshes the charts. */
void classControls::refreshCharts(void)
{
	QList<int> studentIDs = dbMgr.studentIDs(classID);
	// Save old indexes
	int oldP, oldL, oldS, oldE;
	oldP = ui->packBox->currentIndex();
	oldL = ui->lessonBox->currentIndex();
	oldS = ui->sublessonBox->currentIndex();
	oldE = ui->exerciseBox->currentIndex();
	// Packs
	ui->packBox->clear();
	QStringList packs = dbMgr.recordedPacks(classID);
	for(int i=0; i < packs.count(); i++)
		ui->packBox->addItem(builtinPacks::packName(packs[i]));
	if(ui->packBox->count() == 0)
	{
		ui->exerciseFrame->hide();
		ui->chartFrame->hide();
		ui->refreshButton->hide();
		return;
	}
	else
	{
		ui->exerciseFrame->show();
		ui->chartFrame->show();
		ui->refreshButton->show();
	}
	if(oldP == -1)
		oldP = 0;
	ui->packBox->setCurrentIndex(oldP);
	// Lessons
	ui->lessonBox->clear();
	QList<int> lessons = dbMgr.recordedLessons(classID, packs[oldP]);
	for(int i=0; i < lessons.count(); i++)
		ui->lessonBox->addItem(configParser::lessonTr(lessons[i]));
	if(oldL == -1)
		oldL = 0;
	ui->lessonBox->setCurrentIndex(oldL);
	// Sublessons
	ui->sublessonBox->clear();
	QList<int> sublessons = dbMgr.recordedSublessons(classID, packs[oldP],
			lessons[ui->lessonBox->currentIndex()]);
	for(int i=0; i < sublessons.count(); i++)
		ui->sublessonBox->addItem(configParser::sublessonName(sublessons[i]));
	if(oldS == -1)
		oldS = 0;
	ui->sublessonBox->setCurrentIndex(oldS);
	// Exercises
	ui->exerciseBox->clear();
	QList<int> exercises = dbMgr.recordedExercises(classID, packs[oldP],
			lessons[ui->lessonBox->currentIndex()],
			sublessons[ui->sublessonBox->currentIndex()]);
	for(int i=0; i < exercises.count(); i++)
		ui->exerciseBox->addItem(configParser::exerciseTr(exercises[i]));
	if(oldE == -1)
		oldE = 0;
	ui->exerciseBox->setCurrentIndex(oldE);
	// Refresh charts
	speedChart->removeAllSeries();
	mistakesChart->removeAllSeries();
	timeChart->removeAllSeries();
	QString pack = packs[ui->packBox->currentIndex()];
	int lesson = lessons[ui->lessonBox->currentIndex()];
	int sublesson = sublessons[ui->sublessonBox->currentIndex()];
	int exercise = exercises[ui->exerciseBox->currentIndex()];
	for(int i=0; i < studentIDs.count(); i++)
	{
		int i2;
		QList<QVariantMap> entries = dbMgr.historyEntries(classID, studentIDs[i], pack, lesson, sublesson, exercise);
		QString studentName = dbMgr.userName(studentIDs[i]);
		QLineSeries *speedSeries = new QLineSeries;
		QLineSeries *mistakesSeries = new QLineSeries;
		QLineSeries *timeSeries = new QLineSeries;
		speedSeries->setName(studentName);
		mistakesSeries->setName(studentName);
		timeSeries->setName(studentName);
		for(i2=0; i2 < entries.count(); i2++)
		{
			QVariantMap entry = entries[i2];
			speedSeries->append(i2, entry["speed"].toInt());
			mistakesSeries->append(i2, entry["mistakes"].toInt());
			timeSeries->append(i2, entry["duration"].toInt());
		}
		speedChart->addSeries(speedSeries);
		speedChart->createDefaultAxes();
		mistakesChart->addSeries(mistakesSeries);
		mistakesChart->createDefaultAxes();
		timeChart->addSeries(timeSeries);
		timeChart->createDefaultAxes();
	}
	// Set theme
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QChart::ChartTheme theme;
	if(settings.value("theme/theme","0").toInt() == 1)
		theme = QChart::ChartThemeDark;
	else
		theme = QChart::ChartThemeLight;
	speedChart->setTheme(theme);
	mistakesChart->setTheme(theme);
	timeChart->setTheme(theme);
}

/*!
 * Connected from loadExerciseButton.\n
 * Loads exercise from a text file and sends it to all students in this class.
 */
void classControls::loadExercise(void)
{
#ifndef Q_OS_WASM
	QList<int> students, allStudents = dbMgr.studentIDs(classID), occupiedStudents = serverPtr->runningExerciseStudents();
	for(int i=0; i < allStudents.count(); i++)
	{
		if(serverPtr->isLoggedIn(dbMgr.userNickname(allStudents[i])) && !occupiedStudents.contains(allStudents[i]))
			students += allStudents[i];
	}
	loadExerciseDialog dialog(students);
	if(dialog.exec() == QDialog::Accepted)
		startExercise(&dialog);
#endif // Q_OS_WASM
}

/*! Starts the exercise loaded by the dialog. */
void classControls::startExercise(loadExerciseDialog *dialog)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	bool fullMode = settings.value("server/fullmode", false).toBool();
	QList<QByteArray> usernames;
	QList<QHostAddress> addresses;
	auto selectedTargets = dialog->selectedTargets();
	for(int i=0; i < selectedTargets.count(); i++)
	{
		if(fullMode)
			usernames += dbMgr.userNickname(selectedTargets[i]).toUtf8();
		else
			addresses += dbMgr.deviceAddress(selectedTargets[i]);
	}
	QByteArray includeNewLines = "false", correctMistakes = "false", lockUi = "false", hideText = "false";
	if(dialog->includeNewLines())
		includeNewLines = "true";
	if(dialog->correctMistakes())
		correctMistakes = "true";
	if(dialog->lockUi())
		lockUi = "true";
	if(dialog->hideText())
		hideText = "true";
#ifndef Q_OS_WASM
	QStringList signalArgs = {
		dialog->allowTextPreview() ? dialog->exerciseText() : "",
		QString::number(dialog->lineLength())
	};
	if(fullMode)
		serverPtr->sendSignal("initExercise", signalArgs, usernames);
	else
		serverPtr->sendSignal("initExercise", signalArgs, addresses);
	exerciseProgressDialog *progressDialog = new exerciseProgressDialog(dbMgr.activeClass, selectedTargets, dialog->exerciseText(), dialog->lineLength(), dialog->includeNewLines(),
		dialog->mode(), QTime(0, 0, 0).secsTo(dialog->timeLimit()), dialog->correctMistakes(), dialog->lockUi(), dialog->hideText());
	progressDialog->show();
#endif // Q_OS_WASM
}
