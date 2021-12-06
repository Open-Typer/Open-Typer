/*
 * opentyper.h
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

#ifndef OPENTYPER_H
#define OPENTYPER_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QSettings>
#include <QDialog>
#include <QTimer>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QTextCursor>
#include <QTranslator>
#include "widgets/inputlabel.h"
#include "widgets/languagelist.h"
#include "updater/updater.h"
#include "simplecolordialog.h"
#include "paperconfigdialog.h"
#include "packEditor/packeditor.h"
#include "options/optionswindow.h"
#include "levelsummary.h"
#include "timedialog.h"
#include "core/configfile.h"
#include "core/utils.h"
#include "core/packs.h"
#include "core/language.h"

QT_BEGIN_NAMESPACE
namespace Ui { class OpenTyper; }
QT_END_NAMESPACE

class OpenTyper : public QMainWindow
{
	Q_OBJECT

public:
	OpenTyper(QWidget *parent = nullptr);
	~OpenTyper();
	QSettings *settings;
	configParser *parser;
	QString loadConfig(QString configName);
	void startLevel(int lesson, int sublesson, int level);
	void levelFinalInit(void);
	void updateText(void);
	QString level, displayLevel, finalDisplayLevel, input, displayInput, publicConfigName;
	int lessonCount, sublessonCount, levelCount, currentLesson, currentSublesson, currentLevel, currentLine, levelPos, displayPos, levelMistakes, levelHits, levelLengthExtension;
	int deadKeys;
	int sublessonListStart;
	QElapsedTimer levelTimer;
	bool levelInProgress, mistake, ignoreMistakeLabelAppend;
	QString inputLabelHtml, mistakeLabelHtml;
	int lastTime;
	int _line_count(QString str);
	Updater *versionHdw;
	void autoUpdate(void);
	void adjustSize(void);
	int labelWidth(QLabel *targetLabel, QString labelText);
	void setFont(QString fontFamily, int fontSize, bool fontBold, bool fontItalic, bool fontUnderline);
	int levelTextRedColor, levelTextGreenColor, levelTextBlueColor;
	int inputTextRedColor, inputTextGreenColor, inputTextBlueColor;
	int bgRedColor, bgGreenColor, bgBlueColor;
	int paperRedColor, paperGreenColor, paperBlueColor;
	int panelRedColor, panelGreenColor, panelBlueColor;
	bool customLevelTextColor, customInputTextColor, customBgColor, customPaperColor, customPanelColor;
	void saveColorSettings(void);
	void setColors(void);
	void updateTheme(void);
	bool customLevelLoaded;
	bool customConfig;
	bool spaceNewline;
	int errorPenalty;
	bool fullScreenPaper, lineCountLimit;
	int textViewMode;
	QTranslator *translator;
	void refreshAll(bool setLang);
	void changeMode(int mode);

private:
	Ui::OpenTyper *ui;
	languageManager *langMgr;
	void connectAll(void);

private slots:
	void keyPress(QKeyEvent *event);
	void openOptions(void);
	void openPack(void);
	void repeatLevel(void);
	void nextLevel(void);
	void previousLevel(void);
	void updateCurrentTime(void);
	void lessonSelectionListIndexChanged(int index);
	void sublessonSelectionListIndexChanged(int index);
	void levelSelectionListIndexChanged(int index);
	void spaceNewlineCheckBoxChanged(bool checked);
	void openExerciseFromFile(void);
	void changeLanguage(int index);
	void zoomIn(void);
	void zoomOut(void);
	void initTimedExercise(void);

public slots:
	void openEditor(void);
};

#endif // OPENTYPER_H
