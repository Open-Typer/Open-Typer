/*
 * packeditor.h
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

#ifndef PACKEDITOR_H
#define PACKEDITOR_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include "packEditor/packselector.h"
#include "core/configfile.h"

namespace Ui {
	class packEditor;
}

/*!
 * \brief The packEditor class is the pack editor window.
 *
 * \image html packEditor.png
 *
 * packEditor uses this widget for tab content.\n
 *
 * Creating a new file:
 * \code
 * packEditor editor;
 * \endcode
 *
 * Opening existing file:
 * \code
 * editor.openFile(fileName, false, false)
 * \endcode
 *
 * Opening existing file read-only:
 * \code
 * editor.openFile(fileName, false, true)
 * \endcode
 */
class packEditor : public QDialog
{
	Q_OBJECT
	public:
		explicit packEditor(QWidget *parent = nullptr);
		~packEditor();
		void openFile(QString path, bool newf, bool rdonly);
		QString getFileName(void);

	private:
		Ui::packEditor *ui;
		QSettings settings;
		configParser parser;
		QString saveFileName;
		void openPrebuilt(void);
		void openFile(void);
		void refreshUi(bool newLesson, bool newSublesson, bool newExercise);
		void updateTitle(void);
		bool newFile, readOnly, saved;
		void deleteExerciseLine(int lesson, int sublesson, int level);
		void changeExercisePos(QString lessonDesc, int lesson, int sublesson, int level, int nlesson, int nsublesson, int nlevel);
		bool skipBoxUpdates, skipTextUpdates, skipTextRefresh;

	protected:
		void closeEvent(QCloseEvent *event) override;
		void keyPressEvent(QKeyEvent *event) override;

	public slots:
		void createNewFile(void);
		void closeFile(bool createNew = false, bool open = false, bool openPrebuiltPack = false);
		void save(void);
		void saveAs(void);

	private slots:
		void addLesson(void);
		void addSublesson(void);
		void addExercise(void);
		void removeExercise(void);
		void changeLessonDesc(const QString rawLessonDesc);
		void setRevisionLesson(void);
		void updateText(void);
		void restoreText(void);
		void switchLesson(void);
		void switchSublesson(void);
		void switchExercise(void);
		void changeRepeating(int index);
		void changeRepeatLength(int limitExt);
		void changeLineLength(int lengthExt);
};

#endif // PACKEDITOR_H
