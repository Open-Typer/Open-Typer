/*
 * ExerciseProgressDialog.cpp
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

#include "server/ExerciseProgressDialog.h"
#include "ui_ExerciseProgressDialog.h"

namespace ExerciseProgressDialogConfig {
	int dialogCount = 0;
}

/*! Constructs ExerciseProgressDialog. */
ExerciseProgressDialog::ExerciseProgressDialog(int classID, QList<int> targets, QString exerciseText, int lineLength, bool includeNewLines, int mode, int timeLimitSecs, bool correctMistakes, bool lockUi, bool hideText, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ExerciseProgressDialog),
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
	ExerciseProgressDialogConfig::dialogCount++;
	ui->setupUi(this);
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	if((targets.count() > 0) && settings.value("server/fullmode", false).toBool())
		ui->classEdit->setText(dbMgr.className(classID));
#ifndef Q_OS_WASM
	if(!settings.value("server/fullmode", false).toBool())
	{
		// Clear old student names
		for(int i = 0; i < exerciseTargets.count(); i++)
			serverPtr->setDeviceStudentName(exerciseTargets[i], "");
	}
#endif // Q_OS_WASM
	// Load targets
	setupTable();
	// Connections
#ifndef Q_OS_WASM
	connect(serverPtr, &monitorServer::resultUploaded, this, &ExerciseProgressDialog::loadResult);
	connect(serverPtr, &monitorServer::exerciseAborted, this, &ExerciseProgressDialog::abortExercise);
	connect(serverPtr, &monitorServer::deviceConfigurationChanged, this, &ExerciseProgressDialog::setupTable);
	connect(ui->startButton, &QToolButton::clicked, this, [this]() {
		QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
		bool fullMode = settings.value("server/fullmode", false).toBool();
		QList<QByteArray> usernames;
		QList<QHostAddress> addresses;
		for(int i = 0; i < exerciseTargets.count(); i++)
		{
			if(!(abortList.contains(exerciseTargets[i]) && abortList[exerciseTargets[i]]))
			{
				if(fullMode)
					usernames += dbMgr.userNickname(exerciseTargets[i]).toUtf8();
				else
					addresses += dbMgr.deviceAddress(exerciseTargets[i]);
			}
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
	connect(ui->printButton, &QPushButton::clicked, this, &ExerciseProgressDialog::printAll);
	connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &QDialog::close);
	connect(ui->studentsTable, &QTableWidget::cellChanged, this, &ExerciseProgressDialog::uploadChangedName);
}

/*! Destroys the ExerciseProgressDialog object. */
ExerciseProgressDialog::~ExerciseProgressDialog()
{
	delete ui;
	ExerciseProgressDialogConfig::dialogCount--;
}

/*! Loads the targets. */
void ExerciseProgressDialog::setupTable(void)
{
	loadingTable = true;
	targetMap.clear();
	int abortCount = abortList.values().count(true);
	for(int i = 0; i < abortList.keys().count(); i++)
	{
		if(results.contains(abortList[abortList.keys().at(i)]))
			abortCount--;
	}
	ui->printButton->setEnabled(results.keys().count() + abortCount == exerciseTargets.count());
	ui->studentsTable->clearContents();
	ui->studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->studentsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	bool fullMode = settings.value("server/fullmode", false).toBool();
	// Rows
	QList<int> targets = exerciseTargets;
	ui->studentsTable->setRowCount(targets.count());
	for(int i = 0; i < targets.count(); i++)
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
		targetMap[item] = targets[i];
		// Username
		QString username;
		if(fullMode)
			username = dbMgr.userNickname(targets[i]);
		else
			username = dbMgr.deviceName(targets[i]);
		item = new QTableWidgetItem(username);
		ui->studentsTable->setItem(i, 1, item);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		// Result
		if(results.contains(targets[i]))
		{
			QPushButton *button = new QPushButton(tr("View"), this);
			ui->studentsTable->setCellWidget(i, 2, button);
			connect(button, &QPushButton::clicked, this, [this, targets, i, name]() {
				ExportDialog *dialog = new ExportDialog(inputTexts[targets[i]], results[targets[i]], recordedMistakeLists[targets[i]], this);
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
		else
		{
			if(abortList.contains(targets[i]) && abortList[targets[i]])
				item = new QTableWidgetItem(tr("Aborted"));
			else if(!started)
				item = new QTableWidgetItem(tr("Waiting..."));
			else
				item = new QTableWidgetItem(tr("In progress..."));
			ui->studentsTable->setItem(i, 2, item);
			item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		}
		// Mark
		QString mark = "-";
		if(results.contains(targets[i]) && results[targets[i]].contains("mark"))
			mark = results[targets[i]]["mark"].toString();
		item = new QTableWidgetItem(mark);
		ui->studentsTable->setItem(i, 3, item);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	}
	loadingTable = false;
}

/*! Loads the uploaded result. */
void ExerciseProgressDialog::loadResult(int targetID, QString inputText, QVector<QPair<QString, int>> recordedCharacters, qreal time)
{
	if(!exerciseTargets.contains(targetID))
		return;
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QString exerciseText = configParser::initExercise(m_exerciseText, m_lineLength);
	int grossHits, mistakes;
	if(m_correctMistakes)
	{
		// A simple string comparison can be used here,
		// because there are only "change" diffs.
		QList<QVariantMap> recordedMistakes;
		int srcPos = 0;
		for(int targetPos = 0; targetPos < inputText.count(); targetPos++)
		{
			if(srcPos >= exerciseText.count() || targetPos >= inputText.count())
				break;
			if(exerciseText[srcPos] != inputText[targetPos])
			{
				QVariantMap mistakeMap;
				mistakeMap["pos"] = targetPos;
				mistakeMap["previous"] = QString(exerciseText[srcPos]);
				recordedMistakes += mistakeMap;
				if(exerciseText[srcPos] == '\n')
					targetPos++;
			}
			srcPos++;
		}
		mistakes = recordedMistakes.count();
		recordedMistakeLists[targetID] = recordedMistakes;
		grossHits = 0;
		for(int i = 0; i < recordedCharacters.count(); i++)
			grossHits += recordedCharacters[i].second;
	}
	else
		recordedMistakeLists[targetID] = stringUtils::validateExercise(exerciseText, inputText, recordedCharacters, &grossHits, &mistakes, nullptr, (m_mode == 1), m_timeLimit);
	qreal exerciseTime = m_mode == 1 ? m_timeLimit : time;
	int netHits = std::max(0, grossHits - mistakes * settings.value("main/errorpenalty", "10").toInt());
	qreal netHitsPerMinute = netHits * (60 / exerciseTime);
	QVariantMap result;
	result["grossHits"] = grossHits;
	result["netHits"] = netHits;
	result["netHitsPerMinute"] = netHitsPerMinute;
	result["mistakes"] = mistakes;
	result["penalty"] = settings.value("main/errorpenalty", "10").toInt();
	result["time"] = exerciseTime / 60.0;
	results[targetID] = result;
	inputTexts[targetID] = inputText;
	setupTable();
}

/*! Aborts the given student's exercise. */
void ExerciseProgressDialog::abortExercise(int userID)
{
	if(!exerciseTargets.contains(userID))
		return;
	abortList[userID] = true;
	setupTable();
}

/*! Overrides QDialog#closeEvent(). */
void ExerciseProgressDialog::closeEvent(QCloseEvent *event)
{
	int abortCount = abortList.values().count(true);
	for(int i = 0; i < abortList.keys().count(); i++)
	{
		if(results.contains(abortList[abortList.keys().at(i)]))
			abortCount--;
	}
	if(results.keys().count() + abortCount != exerciseTargets.count())
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
void ExerciseProgressDialog::printAll(void)
{
	for(int i = 0; i < exerciseTargets.count(); i++)
	{
		if(abortList.contains(exerciseTargets[i]) && abortList[exerciseTargets[i]] && !results.contains(exerciseTargets[i]))
			continue;
		ExportDialog dialog(inputTexts[exerciseTargets[i]], results[exerciseTargets[i]], recordedMistakeLists[exerciseTargets[i]], this);
		QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
		QString name;
		if(settings.value("server/fullmode", false).toBool())
			name = dbMgr.userName(exerciseTargets[i]);
#ifndef Q_OS_WASM
		else
			name = serverPtr->deviceStudentName(exerciseTargets[i]);
#endif // Q_OS_WASM
		dialog.setStudentName(name);
		dialog.setClassName(ui->classEdit->text());
		dialog.setNumber(ui->numberEdit->text());
		if(results[exerciseTargets[i]].contains("mark"))
			dialog.setMark(results[exerciseTargets[i]]["mark"].toString());
		QMessageBox msgBox;
		msgBox.setText(tr("Printing result of student %1...").arg(name));
		QPushButton *printButton = msgBox.addButton(tr("Print"), QMessageBox::YesRole);
		QPushButton *nextButton;
		if(i < exerciseTargets.count() - 1)
			nextButton = msgBox.addButton(tr("Next student"), QMessageBox::NoRole);
		else
			nextButton = msgBox.addButton(QMessageBox::Close);
		msgBox.exec();
		if(msgBox.clickedButton() == printButton)
		{
			dialog.printResult();
			dialog.close();
		}
		else if(msgBox.clickedButton() != nextButton)
			break;
	}
}

/*! Uploads a changed name to the student. */
void ExerciseProgressDialog::uploadChangedName(int row, int column)
{
	if(column != 0 || loadingTable)
		return;
	auto item = ui->studentsTable->item(row, column);
#ifndef Q_OS_WASM
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	if(settings.value("server/fullmode", false).toBool())
		serverPtr->sendSignal("changeName", { item->text() }, { dbMgr.userNickname(targetMap[item]).toUtf8() });
	else
		serverPtr->sendSignal("changeName", { item->text() }, { dbMgr.deviceAddress(targetMap[item]) });
#endif // Q_OS_WASM
}
