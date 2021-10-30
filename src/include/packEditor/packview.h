/*
 * packview.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

#ifndef PACKVIEW_H
#define PACKVIEW_H

#include <QWidget>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <sys/stat.h>
#include "packEditor/packeditor.h"
#include "core/utils.h"
#include "core/configfile.h"

namespace Ui {
	class packView;
}

class packView : public QWidget
{
	Q_OBJECT
	public:
		explicit packView(QWidget *parent = nullptr, int fileID_arg = 1);
		~packView();
		void openFile(QString path, bool newf, bool rdonly);
		bool closeFile(void);
		QString getFileName(void);

	private:
		Ui::packView *ui;
		QWidget *editorWindow;
		FILE *targetFile;
		QString targetFileName, saveFileName;
		int fileID;
		void refreshUi(bool newLesson, bool newSublesson, bool newExercise);
		bool newFile, readOnly, saved;
		void deleteExerciseLine(int lesson, int sublesson, int level);
		void changeExercisePos(char *lessonDesc, int lesson, int sublesson, int level, int nlesson, int nsublesson, int nlevel);
		bool skipBoxUpdates;

	private slots:
		void save(void);
		void saveAs(void);
		void addLesson(void);
		void addSublesson(void);
		void addExercise(void);
		void removeExercise(void);
		void updateText(void);
		void restoreText(void);
		void switchLesson(void);
		void switchSublesson(void);
		void switchExercise(void);
		void changeRepeating(int index);
		void changeRepeatLength(int limitExt);
		void changeLineLength(int lengthExt);
};

#endif // PACKVIEW_H
