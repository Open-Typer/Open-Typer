/*
 * MainWindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QDialog>
#include <QTimer>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QTextCursor>
#include <QTranslator>
#include "InitialSetup.h"
#include "widgets/inputlabel.h"
#include "widgets/languagelist.h"
#include "server/widgets/servermanager.h"
#include "updater/updater.h"
#include "packEditor/packeditor.h"
#include "options/optionswindow.h"
#include "options/studentoptions.h"
#include "ExerciseSummary.h"
#include "timedialog.h"
#include "statsdialog.h"
#include "ExportDialog.h"
#include "testwaitdialog.h"
#include "core/configfile.h"
#include "core/utils.h"
#include "core/packs.h"
#include "core/language.h"
#include "core/theme.h"
#ifndef Q_OS_WASM
#include "core/server.h"
#endif // Q_OS_WASM

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class is the main window class. It shows the UI, text and handles input and output.
 *
 * \image html MainWindow.png
 */
class MainWindow : public QMainWindow
{
		Q_OBJECT
	public:
		MainWindow(QWidget *parent = nullptr);
		~MainWindow();

	private:
		Ui::MainWindow *ui;
		QSettings settings;
		languageManager langMgr;
		configParser parser;
		monitorClient client;
		QString studentUsername, studentPassword;
		QString loadConfig(QString configName, QByteArray packContent = "");
		void startLevel(int lesson, int sublesson, int level);
		void updateLessonList(void);
		void loadLesson(int lessonID, int sublessonID);
		void loadSublesson(int levelID);
		void levelFinalInit(bool updateClient = true);
		void updateText(void);
		QString level, displayLevel, finalDisplayLevel, input, displayInput, publicConfigName, oldConfigName;
		int lessonCount, sublessonCount, levelCount, currentLesson, currentSublesson, currentAbsoluteSublesson, currentLevel, currentLine, levelPos, displayPos, levelMistakes, totalHits, netHits, levelLengthExtension;
		int lineCount, linePos, absolutePos;
		int deadKeys;
		QVector<QPair<QString, int>> recordedCharacters;
		QList<QVariantMap> recordedMistakes;
		int sublessonListStart;
		QElapsedTimer levelTimer;
		QTimer *secLoop, timedExTimer;
		bool levelInProgress, mistake, ignoreMistakeLabelAppend;
		QString inputLabelHtml, mistakeLabelHtml;
		QString inputTextHtml, mistakeTextHtml;
		int lastTime;
		double lastTimeF;
		themeEngine localThemeEngine;
		void setColors(void);
		bool customLevelLoaded = false;
		QString customLevel;
		bool customConfig = false;
		bool spaceNewline;
		int errorPenalty;
		QTranslator translator;
		void refreshAll(void);
		void changeMode(int mode, bool enableStudentUpdate = true);
		int currentMode = 0;
		int timedExHours, timedExMinutes, timedExSeconds;
		bool timedExStarted;
		bool blockInput;
		void loadText(QByteArray text, bool includeNewLines = false, bool updateClient = true);
		void endExercise(bool showNetHits, bool showGrossHits, bool showTotalHits, bool showTime, bool showMistakes);
		QStringList errorWords;
		void loadErrorWords(void);
		void loadReversedText(void);
		void exportText(void);
		bool preview = false, uploadResult = false, testLoaded = false, correctMistakesOld = true, hideTextOld = false;
		testWaitDialog *waitDialog = nullptr;
		QByteArray oldGeometry;
		bool firstRun = false;

	protected:
		void changeEvent(QEvent *event);

	private slots:
		void loadTheme(void);
		void updateFont(void);
		void keyPress(QKeyEvent *event);
		void keyRelease(QKeyEvent *event);
		void openOptions(void);
		void openStudentOptions(void);
		void openPack(void);
		void repeatLevel(void);
		void nextLevel(void);
		void previousLevel(void);
		void updateCurrentTime(void);
		void lessonSelectionListIndexChanged(int index);
		void sublessonSelectionListIndexChanged(int index);
		void levelSelectionListIndexChanged(int index);
		void openExerciseFromFile(void);
		void zoomIn(void);
		void zoomOut(void);
		void initTimedExercise(void);
		void updateStudent(void);
		void showExerciseStats(void);
		void loadReceivedExercise(QByteArray text, int lineLength, bool includeNewLines, int mode, int time, bool correctMistakes, bool lockUi, bool hideText);
		void startReceivedExercise(QByteArray text, int lineLength, bool includeNewLines, int mode, int time, bool correctMistakes, bool lockUi, bool hideText, bool upload);
		void waitForReceivedExercise(QString text, int lineLength);
		void printText(void);
		void openEditor(void);
		void startTest(void);
		void showAboutDialog(void);
};

#endif // MAINWINDOW_H
