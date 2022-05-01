/*
 * exerciseprogressdialog.cpp
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

#include "server/exerciseprogressdialog.h"
#include "ui_exerciseprogressdialog.h"

/*! Constructs exerciseProgressDialog. */
exerciseProgressDialog::exerciseProgressDialog(QList<int> students, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::exerciseProgressDialog),
	exerciseStudents(students)
{
	ui->setupUi(this);
	// Load students
	setupTable();
	// Connections
	connect(serverPtr, &monitorServer::resultUploaded, this, &exerciseProgressDialog::loadResult);
	connect(serverPtr, &monitorServer::exerciseAborted, this, &exerciseProgressDialog::abortExercise);
	connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &QDialog::accept);
}

/*! Destroys the exerciseProgressDialog object. */
exerciseProgressDialog::~exerciseProgressDialog()
{
	delete ui;
}

/*! Loads the students. */
void exerciseProgressDialog::setupTable(void)
{
	ui->studentsTable->clearContents();
	ui->studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->studentsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	// Rows
	QList<int> students = serverPtr->runningExerciseStudents();
	auto keys = abortList.keys();
	for(int i=0; i < keys.count(); i++)
	{
		if(abortList[keys[i]] && !students.contains(keys[i]))
			students += keys[i];
	}
	ui->studentsTable->setRowCount(students.count());
	for(int i=0; i < students.count(); i++)
	{
		// Full name
		QTableWidgetItem *item = new QTableWidgetItem(dbMgr.userName(students[i]));
		ui->studentsTable->setItem(i, 0, item);
		// Username
		item = new QTableWidgetItem(dbMgr.userNickname(students[i]));
		ui->studentsTable->setItem(i, 1, item);
		// Result
		if(abortList.contains(students[i]) && abortList[students[i]])
			ui->studentsTable->setItem(i, 2, new QTableWidgetItem(tr("Aborted")));
		else if(results.contains(students[i]))
		{
			QPushButton *button = new QPushButton(tr("View"), this);
			ui->studentsTable->setCellWidget(i, 2, button);
			connect(button, &QPushButton::clicked, this, [this, students, i]() {
				exportDialog *dialog = new exportDialog(inputTexts[students[i]], results[students[i]], recordedMistakeLists[students[i]], this);
				dialog->setWindowModality(Qt::WindowModal);
				dialog->open();
				dialog->showMaximized();
				connect(dialog, &QDialog::finished, this, &exerciseProgressDialog::setupTable);
			});
		}
		else
			ui->studentsTable->setItem(i, 2, new QTableWidgetItem(tr("In progress...")));
		// Mark
		QString mark = "-";
		if(results[students[i]].contains("mark"))
			mark = QString::number(results[students[i]]["mark"].toInt());
		item = new QTableWidgetItem(mark);
		ui->studentsTable->setItem(i, 3, item);
	}
}

/*! Loads the uploaded result. */
void exerciseProgressDialog::loadResult(int userID, QList<QVariantMap> recordedMistakes, QString inputText, int grossHits, int netHits, double netHitsPerMinute, int mistakes)
{
	if(!exerciseStudents.contains(userID))
		return;
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QVariantMap result;
	result["grossHits"] = grossHits;
	result["netHits"] = netHits;
	result["netHitsPerMinute"] = netHitsPerMinute;
	result["mistakes"] = mistakes;
	result["penalty"] = settings.value("main/errorpenalty","10").toInt();
	results[userID] = result;
	inputTexts[userID] = inputText;
	recordedMistakeLists[userID] = recordedMistakes;
	setupTable();
}

/*! Aborts the given student's exercise. */
void exerciseProgressDialog::abortExercise(int userID)
{
	if(!exerciseStudents.contains(userID))
		return;
	abortList[userID] = true;
	setupTable();
}
