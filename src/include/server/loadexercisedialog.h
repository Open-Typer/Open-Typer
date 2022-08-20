/*
 * loadexercisedialog.h
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

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "core/configfile.h"
#include "core/database.h"

namespace Ui {
	class loadExerciseDialog;
}

/*!
 * \brief The loadExerciseDialog class is a dialog for exercise configuration on the server.
 *
 * \image html loadExerciseDialog.png
 */
class loadExerciseDialog : public QDialog
{
		Q_OBJECT
	public:
		explicit loadExerciseDialog(QList<int> availableTargets, QWidget *parent = nullptr);
		loadExerciseDialog(QWidget *parent = nullptr);
		~loadExerciseDialog();
		QString exerciseText(void);
		int lineLength(void);
		bool includeNewLines(void);
		int mode(void);
		QTime timeLimit(void);
		bool correctMistakes(void);
		bool lockUi(void);
		bool hideText(void);
		bool allowTextPreview(void);
		QList<int> selectedTargets(void);

	private:
		Ui::loadExerciseDialog *ui;
		void init(void);
		QString fileName = "";
		QList<int> m_targets;
		QMap<int, QCheckBox *> targets;
		QSettings settings;
		QString m_exerciseText = "";
		int m_lineLength = 0;
		bool m_includeNewLines = false;
		bool local = false;

	private slots:
		void verify(void);
		void changeSource(void);
		void openFile(void);
		void changeMode(void);
};

#endif // LOADEXERCISEDIALOG_H
