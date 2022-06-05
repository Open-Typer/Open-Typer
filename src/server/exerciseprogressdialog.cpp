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

namespace exerciseProgressDialogConfig {
	int dialogCount = 0;
}

/*! Constructs exerciseProgressDialog. */
exerciseProgressDialog::exerciseProgressDialog(int classID, QList<int> targets, QString exerciseText, int lineLength, bool includeNewLines, int mode, int timeLimitSecs, bool correctMistakes, bool lockUi, bool hideText, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::exerciseProgressDialog),
	exerciseTargets(targets),
	m_exerciseText(exerciseText),
	m_lineLength(lineLength),
	m_includeNewLines(includeNewLines),
	m_mode(mode),
	m_timeLimit(timeLimitSecs),
	m_correctMistakes(correctMistakes),
	m_lockUi(lockUi),
	m_hideText(hideText)
{
	setAttribute(Qt::WA_DeleteOnClose);
	exerciseProgressDialogConfig::dialogCount++;
	ui->setupUi(this);
	if(targets.count() > 0)
		ui->classEdit->setText(dbMgr.className(classID));
	// Load targets
	setupTable();
	// Connections
#ifndef Q_OS_WASM
	connect(serverPtr, &monitorServer::resultUploaded, this, &exerciseProgressDialog::loadResult);
	connect(serverPtr, &monitorServer::exerciseAborted, this, &exerciseProgressDialog::abortExercise);
	connect(serverPtr, &monitorServer::deviceConfigurationChanged, this, &exerciseProgressDialog::setupTable);
	connect(ui->startButton, &QToolButton::clicked, this, [this]() {
		QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
		bool fullMode = settings.value("server/fullmode", false).toBool();
		QList<QByteArray> usernames;
		QList<QHostAddress> addresses;
		for(int i=0; i < exerciseTargets.count(); i++)
		{
			if(fullMode)
				usernames += dbMgr.userNickname(exerciseTargets[i]).toUtf8();
			else
				addresses += dbMgr.deviceAddress(exerciseTargets[i]);
		}
		QByteArray includeNewLines = "false", correctMistakes = "false", lockUi = "false", hideText = "false";
		if(m_includeNewLines)
			includeNewLines = "true";
		if(m_correctMistakes)
			correctMistakes = "true";
		if(m_lockUi)
			lockUi = "true";
		if(m_hideText)
			hideText = "true";
		QStringList signalArgs = {
			m_exerciseText,
			QString::number(m_lineLength),
			includeNewLines,
			QString::number(m_mode),
			QString::number(m_timeLimit),
			correctMistakes,
			lockUi,
			hideText
		};
		if(fullMode)
			serverPtr->sendSignal("loadExercise", signalArgs, usernames);
		else
			serverPtr->sendSignal("loadExercise", signalArgs, addresses);
		ui->startButton->hide();
		started = true;
		setupTable();
	});
#endif // Q_OS_WASM
	connect(ui->printButton, &QPushButton::clicked, this, &exerciseProgressDialog::printAll);
	connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &QDialog::close);
}

/*! Destroys the exerciseProgressDialog object. */
exerciseProgressDialog::~exerciseProgressDialog()
{
	delete ui;
	exerciseProgressDialogConfig::dialogCount--;
}

/*! Loads the targets. */
void exerciseProgressDialog::setupTable(void)
{
	ui->printButton->setEnabled(results.keys().count() + abortList.values().count(true) == exerciseTargets.count());
	ui->studentsTable->clearContents();
	ui->studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->studentsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	bool fullMode = settings.value("server/fullmode", false).toBool();
	// Rows
	QList<int> targets = exerciseTargets;
	ui->studentsTable->setRowCount(targets.count());
	for(int i=0; i < targets.count(); i++)
	{
		QString name;
		// Full name
		if(fullMode)
			name = dbMgr.userName(targets[i]);
#ifndef Q_OS_WASM
		else
			name = serverPtr->deviceStudentName(targets[i]);
#endif // Q_OS_WASM
		QTableWidgetItem *item = new QTableWidgetItem(name);
		ui->studentsTable->setItem(i, 0, item);
		// Username
		QString username;
		if(fullMode)
			username = dbMgr.userNickname(targets[i]);
		else
			username = dbMgr.deviceName(targets[i]);
		item = new QTableWidgetItem(username);
		ui->studentsTable->setItem(i, 1, item);
		// Result
		if(abortList.contains(targets[i]) && abortList[targets[i]])
			ui->studentsTable->setItem(i, 2, new QTableWidgetItem(tr("Aborted")));
		else if(results.contains(targets[i]))
		{
			QPushButton *button = new QPushButton(tr("View"), this);
			ui->studentsTable->setCellWidget(i, 2, button);
			connect(button, &QPushButton::clicked, this, [this, targets, i, name]() {
				exportDialog *dialog = new exportDialog(inputTexts[targets[i]], results[targets[i]], recordedMistakeLists[targets[i]], this);
				dialog->setStudentName(name);
				dialog->setClassName(ui->classEdit->text());
				dialog->setNumber(ui->numberEdit->text());
				if(results[targets[i]].contains("mark"))
					dialog->setMark(results[targets[i]]["mark"].toString());
				dialog->setWindowModality(Qt::WindowModal);
				dialog->open();
				dialog->showMaximized();
				connect(dialog, &QDialog::finished, this, [this, dialog, targets, i]() {
					ui->classEdit->setText(dialog->className());
					ui->numberEdit->setText(dialog->number());
					results[targets[i]]["mark"] = dialog->mark();
					setupTable();
				});
			});
		}
		else if(!started)
			ui->studentsTable->setItem(i, 2, new QTableWidgetItem(tr("Waiting...")));
		else
			ui->studentsTable->setItem(i, 2, new QTableWidgetItem(tr("In progress...")));
		// Mark
		QString mark = "-";
		if(results.contains(targets[i]) && results[targets[i]].contains("mark"))
			mark = results[targets[i]]["mark"].toString();
		item = new QTableWidgetItem(mark);
		ui->studentsTable->setItem(i, 3, item);
	}
}

/*! Loads the uploaded result. */
void exerciseProgressDialog::loadResult(int targetID, QList<QVariantMap> recordedMistakes, QString inputText, int grossHits, int netHits, double netHitsPerMinute, int mistakes, double time)
{
	if(!exerciseTargets.contains(targetID))
		return;
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QVariantMap result;
	result["grossHits"] = grossHits;
	result["netHits"] = netHits;
	result["netHitsPerMinute"] = netHitsPerMinute;
	result["mistakes"] = mistakes;
	result["penalty"] = settings.value("main/errorpenalty","10").toInt();
	result["time"] = time;
	results[targetID] = result;
	inputTexts[targetID] = inputText;
	recordedMistakeLists[targetID] = recordedMistakes;
	setupTable();
}

/*! Aborts the given student's exercise. */
void exerciseProgressDialog::abortExercise(int userID)
{
	if(!exerciseTargets.contains(userID))
		return;
	abortList[userID] = true;
	setupTable();
}

/*! Overrides QDialog#closeEvent(). */
void exerciseProgressDialog::closeEvent(QCloseEvent *event)
{
	if(results.keys().count() + abortList.values().count(true) != exerciseTargets.count())
	{
		QMessageBox::StandardButton button = QMessageBox::question(this, QString(), tr("Some targets have not finished yet."), QMessageBox::Discard | QMessageBox::Cancel);
		if(button == QMessageBox::Discard)
			event->accept();
		else
			event->ignore();
	}
	else
		event->accept();
}

/*! Prints all results. */
void exerciseProgressDialog::printAll(void)
{
	for(int i=0; i < exerciseTargets.count(); i++)
	{
		if(abortList.contains(exerciseTargets[i]) && abortList[exerciseTargets[i]])
			continue;
		exportDialog dialog(inputTexts[exerciseTargets[i]], results[exerciseTargets[i]], recordedMistakeLists[exerciseTargets[i]], this);
		dialog.setStudentName(dbMgr.userName(exerciseTargets[i]));
		dialog.setClassName(ui->classEdit->text());
		dialog.setNumber(ui->numberEdit->text());
		if(results[exerciseTargets[i]].contains("mark"))
			dialog.setMark(results[exerciseTargets[i]]["mark"].toString());
		QMessageBox msgBox;
		msgBox.setText(tr("Printing result of student %1...").arg(dbMgr.userName(exerciseTargets[i])));
		QPushButton *printButton = msgBox.addButton(tr("Print"), QMessageBox::YesRole);
		QPushButton *nextButton;
		if(i < exerciseTargets.count()-1)
			nextButton = msgBox.addButton(tr("Next student"), QMessageBox::NoRole);
		else
			nextButton = msgBox.addButton(QMessageBox::Close);
		msgBox.exec();
		if(msgBox.clickedButton() == printButton)
		{
			dialog.printResult();
			dialog.close();
		}
		else if(msgBox.clickedButton() == nextButton)
			break;
		else
			break;
	}
}
