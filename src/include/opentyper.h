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

/*! The main window class, which shows the UI, text and handles input and output. */
class OpenTyper : public QMainWindow
{
	Q_OBJECT

public:
	OpenTyper(QWidget *parent = nullptr); /*!< Constructs the main window. */
	~OpenTyper(); /*!< Destroys the object. */

private:
	Ui::OpenTyper *ui;
	languageManager *langMgr;
	/*! Sets up all connections. */
	void connectAll(void);
	QSettings *settings;
	configParser *parser;
	/*!
	 * Opens a pack file using configParser.
	 * \param[in] configName If there's a custom pack opened, configName represents the absolute path to the pack. Otherwise, it represents the built-in pack name.
	 * \see configParser
	 */
	QString loadConfig(QString configName);
	/*!
	 * Initializes the specified exercise.
	 * \param[in] lesson, sublesson, level Exercise location (lesson, sublesson and exercise ID).
	 * \see loadConfig
	 * \see configParser
	 * \see levelFinalInit
	 */
	void startLevel(int lesson, int sublesson, int level);
	/*!
	 * Sets initial values and shows the exercise text.
	 * \see startLevel
	 * \see updateText
	 */
	void levelFinalInit(void);
	/*!
	 * Converts exercise text (line wrapping, limited number of lines).
	 * \see levelFinalInit
	 * \see configParser
	 */
	void updateText(void);
	QString level, displayLevel, finalDisplayLevel, input, displayInput, publicConfigName;
	int lessonCount, sublessonCount, levelCount, currentLesson, currentSublesson, currentLevel, currentLine, levelPos, displayPos, levelMistakes, totalHits, levelHits, levelLengthExtension;
	int deadKeys;
	int sublessonListStart;
	QElapsedTimer levelTimer;
	QTimer *secLoop;
	bool levelInProgress, mistake, ignoreMistakeLabelAppend;
	QString inputLabelHtml, mistakeLabelHtml;
	int lastTime;
	/*!
	 * Gets number of lines of a string.
	 * Will be moved to stringUtils later.
	 * \see stringUtils
	 */
	int _line_count(QString str);
	Updater *versionHdw;
	/*! Adjusts the width of the paper widget. \see labelWidth*/
	void adjustSize(void);
	/*!
	 * Gets the width of a QLabel with a text.\n
	 * This function is obsolete and will be removed after the paper widget starts using layouts.
	 * \see adjustSize
	 */
	int labelWidth(QLabel *targetLabel, QString labelText);
	/*! Sets exercise text font and saves it in the settings. \see customizationOptions#setFont */
	void setFont(QString fontFamily, int fontSize, bool fontBold, bool fontItalic, bool fontUnderline);
	int levelTextRedColor, levelTextGreenColor, levelTextBlueColor;
	int inputTextRedColor, inputTextGreenColor, inputTextBlueColor;
	int bgRedColor, bgGreenColor, bgBlueColor;
	int paperRedColor, paperGreenColor, paperBlueColor;
	int panelRedColor, panelGreenColor, panelBlueColor;
	bool customLevelTextColor, customInputTextColor, customBgColor, customPaperColor, customPanelColor;
	/*! Sets custom colors (if they are set) or default colors. \see customizationOptions#setColors */
	void setColors(void);
	/*! Loads the style sheet of the selected theme. \see customizationOptions#updateTheme */
	void updateTheme(void);
	bool customLevelLoaded;
	bool customConfig;
	bool spaceNewline;
	int errorPenalty;
	bool fullScreenPaper, lineCountLimit;
	int textViewMode;
	QTranslator *translator;
	/*! Initializes the program and loads all settings.
	 * \param[in] setLang Whether to set the display language.
	 * \see changeLanguage
	 * \see changeMode
	 * \see updateTheme
	 * \see setColors
	 * \see repeatLevel
	 */
	void refreshAll(bool setLang);
	/*! Changes the operation mode.\n
	 * Supported modes are default (0) and timed exercise mode (1).
	 */
	void changeMode(int mode);
	int currentMode;
	int timedExHours, timedExMinutes, timedExSeconds;
	bool timedExStarted;

private slots:
	/*! Connected from inputLabelWidget#keyPressed signal.\n
	 * Handles all key presses, counts hits, displays typed characters and counts mistakes.
	 */
	void keyPress(QKeyEvent *event);
	/*! Connected from optionsButton.\n
	 * Opens options window.
	 */
	void openOptions(void);
	/*! Connected from openPackButton.\n
	 * Shows a file dialog and opens a custom pack.
	 */
	void openPack(void);
	/*! Connected from repeatButton.\n
	 * Resets currently selected exercise.
	 * \see startLevel
	 */
	void repeatLevel(void);
	/*! Connected from nextButton.\n
	 * Selects the next exercise.
	 * \see repeatLevel
	 */
	void nextLevel(void);
	/*! Connected from previousButton.\n
	 * Selects the previous exercise.
	 * \see repeatLevel
	 */
	void previousLevel(void);
	/*! Connected from secLoop.\n
	 * Runs periodically and updates time widgets.
	 */
	void updateCurrentTime(void);
	/*! Connected from lessonSelectionList.\n
	 * Selects the lesson selected in lessonSelectionList.
	 */
	void lessonSelectionListIndexChanged(int index);
	/*! Connected from sublessonSelectionList.\n
	 * Selects the sublesson selected in sublessonSelectionList.
	 */
	void sublessonSelectionListIndexChanged(int index);
	/*! Connected from levelSelectionList.\n
	 * Selects the exercise selected in levelSelectionList.
	 */
	void levelSelectionListIndexChanged(int index);
	/*! Connected from openExerciseButton.\n
	 * Shows a file dialog and opens a custom exercise.
	 */
	void openExerciseFromFile(void);
	/*! Connected from optionsWindow#languageChanged.\n
	 * Retranslates the UI in the selected language.
	 * \param[in] index Index of the selected language in the list of languages
	 * \see languageManager
	 */
	void changeLanguage(int index);
	/*! Connected from zoomInButton.\n
	 * Increases the text size.
	 * \see zoomOut
	 */
	void zoomIn(void);
	/*! Connected from zoomOutButton.\n
	 * Decreases the text size.
	 * \see zoomIn
	 */
	void zoomOut(void);
	/*! Connected from timedExerciseButton.\n
	 * Switches to timed exercise mode.
	 */
	void initTimedExercise(void);

public slots:
	/*! Connected from openEditorButton.\n
	 * Opens the editor.
	 */
	void openEditor(void);
};

#endif // OPENTYPER_H
