/*
 * LoadExerciseDialog.h
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

#ifndef LOADEXERCISEDIALOG_H
#define LOADEXERCISEDIALOG_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include "ConfigParser.h"

namespace Ui {
	class LoadExerciseDialog;
}

/*!
 * \brief The LoadExerciseDialog class is a dialog for exercise loading.
 *
 * \image html LoadExerciseDialog.png
 */
class CORE_LIB_EXPORT LoadExerciseDialog : public QDialog
{
		Q_OBJECT
	public:
		explicit LoadExerciseDialog(QMap<int, QString> availableTargets, QWidget *parent = nullptr);
		LoadExerciseDialog(QWidget *parent = nullptr);
		~LoadExerciseDialog();
		Q_INVOKABLE void init(void);
		Q_INVOKABLE QString exerciseText(void);
		Q_INVOKABLE int lineLength(void);
		Q_INVOKABLE bool includeNewLines(void);
		Q_INVOKABLE int mode(void);
		Q_INVOKABLE QTime timeLimit(void);
		Q_INVOKABLE int timeLimitSecs(void);
		Q_INVOKABLE bool correctMistakes(void);
		Q_INVOKABLE bool lockUi(void);
		Q_INVOKABLE bool hideText(void);
		Q_INVOKABLE bool allowTextPreview(void);
		Q_INVOKABLE QList<int> selectedTargets(void);

	private:
		Ui::LoadExerciseDialog *ui;
		QString fileName = "";
		QMap<int, QString> m_targets;
		QMap<int, QCheckBox *> targets;
		QString m_exerciseText = "";
		int m_lineLength = 0;
		bool m_includeNewLines = false;
		bool local = false;
		QString oldSelectedExText;

	private slots:
		void verify(void);
		void changeSource(void);
		void openFile(void);
		void changeMode(void);
};

#endif // LOADEXERCISEDIALOG_H
