/*
 * ExerciseProgressDialog.h
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

#ifndef EXERCISEPROGRESSDIALOG_H
#define EXERCISEPROGRESSDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QMap>
#include <QPushButton>
#include <QMessageBox>
#include "ExportDialog.h"
#include "DatabaseManager.h"
#include "ConfigParser.h"
#ifndef Q_OS_WASM
#include "MonitorServer.h"
#endif // Q_OS_WASM

namespace ExerciseProgressDialogConfig {
	extern int dialogCount;
}

namespace Ui {
	class ExerciseProgressDialog;
}

/*!
 * \brief The ExerciseProgressDialog class is a dialog for exercise monitoring.
 * \image html ExerciseProgressDialog.png
 */
class ExerciseProgressDialog : public QDialog
{
		Q_OBJECT
	public:
		explicit ExerciseProgressDialog(int classID, QList<int> targets, QString exerciseText, int lineLength, bool includeNewLines, int mode, int timeLimitSecs, bool correctMistakes, bool lockUi, bool hideText, QWidget *parent = nullptr);
		~ExerciseProgressDialog();

	private:
		Ui::ExerciseProgressDialog *ui;
		QList<int> exerciseTargets;
		QMap<int, QString> names;
		QMap<int, QVariantMap> results;
		QMap<int, QString> inputTexts;
		QMap<int, bool> abortList;
		QMap<int, QList<QVariantMap>> recordedMistakeLists;
		QMap<QTableWidgetItem *, int> targetMap;
		QString m_exerciseText;
		int m_lineLength;
		bool m_includeNewLines;
		int m_mode, m_timeLimit;
		bool m_correctMistakes, m_lockUi, m_hideText;
		bool started = false, loadingTable = false;

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		void setupTable(void);
		void loadResult(int targetID, QString inputText, QVector<QPair<QString, int>> recordedCharacters, qreal time);
		void abortExercise(int targetID);
		void printAll(void);
		void uploadChangedName(int row, int column);
};

#endif // EXERCISEPROGRESSDIALOG_H
