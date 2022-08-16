/*
 * opentyper.cpp
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

#include "opentyper.h"
#include "ui_opentyper.h"

/*! Constructs the main window. */
OpenTyper::OpenTyper(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::OpenTyper),
	settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat)
{
	ui->setupUi(this);
	QGridLayout *inputLabelLayout = new QGridLayout(ui->inputLabel);
	ui->mistakeLabel->setHorizontalAdjust(false);
	ui->mistakeLabel->setParent(ui->inputLabel);
	inputLabelLayout->addWidget(ui->mistakeLabel);
	inputLabelLayout->setContentsMargins(0,0,0,0);
	inputLabelLayout->setAlignment(ui->mistakeLabel, Qt::AlignLeft | Qt::AlignTop);
	QVBoxLayout *remainingTextAreaLayout = new QVBoxLayout(ui->remainingTextArea);
	ui->keyboardFrame->setParent(ui->remainingTextArea);
	remainingTextAreaLayout->addWidget(ui->keyboardFrame);
	remainingTextAreaLayout->setAlignment(ui->keyboardFrame, Qt::AlignHCenter | Qt::AlignBottom);
	localThemeEngine.setParent(this);
	client.setErrorDialogs(false);
	studentUsername = "";
	studentPassword = "";
	oldConfigName = "";
	errorWords.clear();
#ifdef Q_OS_WASM
	settings.setValue("main/clientdisabled", true);
#endif // Q_OS_WASM
	// Set language
	if(settings.value("main/language","").toString() == "")
		langMgr.setLanguage(-1);
	else
		langMgr.setLanguage(langMgr.boxItems.indexOf(settings.value("main/language","").toString()) - 1);
	ui->retranslateUi(this);
	// Opacity effect
	QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
	ui->levelLabel->setGraphicsEffect(opacityEffect);
	refreshAll();
	// Set mode
	changeMode(0);
	// Load text
	updateText();
	// Connections
	connect(&client, SIGNAL(disconnected()), this, SLOT(updateStudent()));
	connect(&client, &monitorClient::exerciseReceived, this, &OpenTyper::loadReceivedExercise);
	connect(&client, &monitorClient::initExReceived, this, &OpenTyper::waitForReceivedExercise);
	// File menu
	connect(ui->actionOpenText, &QAction::triggered, this, &OpenTyper::openExerciseFromFile);
	connect(ui->actionOpenPack, &QAction::triggered, this, &OpenTyper::openPack);
	connect(ui->actionNewPack, &QAction::triggered, this, &OpenTyper::openEditor);
	connect(ui->actionPrint, &QAction::triggered, ui->printButton, &QPushButton::clicked);
	// View menu
	connect(ui->actionViewNavigation, &QAction::toggled, ui->navigationFrame, &QWidget::setVisible);
	connect(ui->actionViewExOptions, &QAction::toggled, ui->exerciseOptionsFrame, &QWidget::setVisible);
	connect(ui->actionViewState, &QAction::toggled, ui->stateFrame, &QWidget::setVisible);
	// Student menu
	connect(ui->actionLogIn, &QAction::triggered, this, &OpenTyper::openStudentOptions);
	// Tools menu
	connect(ui->actionTypingTest, &QAction::triggered, ui->testButton, &QPushButton::clicked);
	// Exercise menu
	connect(ui->actionStats, &QAction::triggered, ui->statsButton, &QPushButton::clicked);
	connect(ui->actionTimedEx, &QAction::triggered, ui->timedExerciseButton, &QPushButton::clicked);
	connect(ui->actionErrorWords, &QAction::triggered, ui->errorWordsButton, &QPushButton::clicked);
	connect(ui->actionReverseText, &QAction::triggered, ui->reversedTextButton, &QPushButton::clicked);
	connect(ui->actionCorrectMistakes, &QAction::toggled, ui->correctMistakesCheckBox, &QCheckBox::setChecked);
	connect(ui->correctMistakesCheckBox, &QCheckBox::toggled, ui->actionCorrectMistakes, &QAction::setChecked);
	connect(ui->actionHideText, &QAction::toggled, ui->hideTextCheckBox, &QCheckBox::setChecked);
	connect(ui->hideTextCheckBox, &QCheckBox::toggled, ui->actionHideText, &QAction::setChecked);
	// Settings menu
	connect(ui->actionPreferences, &QAction::triggered, ui->optionsButton, &QPushButton::clicked);
	// Help menu
	connect(ui->actionDocs, &QAction::triggered, this, []() { QDesktopServices::openUrl(QUrl("https://open-typer.github.io/Open-Typer")); });
	connect(ui->actionAboutProgram, &QAction::triggered, this, &OpenTyper::showAboutDialog);
	connect(ui->actionAboutQt, &QAction::triggered, this, [this]() { QMessageBox::aboutQt(this); });
	// Widgets
	connect(ui->inputLabel, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(keyPress(QKeyEvent*)));
	connect(ui->inputLabel, SIGNAL(keyReleased(QKeyEvent*)), this, SLOT(keyRelease(QKeyEvent*)));
	connect(ui->optionsButton, SIGNAL(clicked()), this, SLOT(openOptions()));
	connect(ui->openButton, &QPushButton::clicked, this, &OpenTyper::openExerciseFromFile);
	connect(ui->repeatButton, SIGNAL(clicked()), this, SLOT(repeatLevel()));
	connect(ui->closeCustomExButton, &QPushButton::clicked, this, [this](){ customLevelLoaded = false; repeatLevel(); });
	connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(nextLevel()));
	connect(ui->previousButton, SIGNAL(clicked()), this, SLOT(previousLevel()));
	connect(ui->lessonSelectionList, SIGNAL(activated(int)), this, SLOT(lessonSelectionListIndexChanged(int)));
	connect(ui->sublessonSelectionList, SIGNAL(activated(int)), this, SLOT(sublessonSelectionListIndexChanged(int)));
	connect(ui->levelSelectionList, SIGNAL(activated(int)), this, SLOT(levelSelectionListIndexChanged(int)));
	connect(ui->errorWordsButton, &QPushButton::clicked, this, &OpenTyper::loadErrorWords);
	connect(ui->reversedTextButton, &QPushButton::clicked, this, &OpenTyper::loadReversedText);
	connect(ui->zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
	connect(ui->zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
	connect(ui->timedExerciseButton, SIGNAL(clicked()), this, SLOT(initTimedExercise()));
	connect(ui->stopTimedExButton, &QPushButton::clicked, ui->timedExerciseButton, &QPushButton::clicked);
	connect(ui->statsButton, SIGNAL(clicked()), this, SLOT(showExerciseStats()));
	connect(ui->printButton, &QPushButton::clicked, this, &OpenTyper::printText);
	connect(ui->exportButton, &QPushButton::clicked, this, &OpenTyper::exportText);
	connect(ui->testButton, &QPushButton::clicked, this, &OpenTyper::startTest);
	connect(ui->hideTextCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
		ui->currentLineArea->setVisible(!checked);
		ui->textSeparationLine->setVisible(!checked);
		updateText();
	});
	connect(ui->keyboardFrame, &keyboardWidget::visibilityChanged, this, &OpenTyper::updateFont);
	// Theme engine
	connect(&globalThemeEngine, &themeEngine::fontChanged, this, &OpenTyper::updateFont);
	connect(&globalThemeEngine, &themeEngine::colorChanged, this, &OpenTyper::setColors);
	connect(&globalThemeEngine, &themeEngine::styleChanged, this, &OpenTyper::setColors);
	connect(&globalThemeEngine, &themeEngine::themeChanged, this, &OpenTyper::loadTheme);
	// Theme
	if(settings.contains("main/windowState") && settings.contains("main/windowGeometry"))
	{
		restoreState(settings.value("main/windowState").toByteArray());
		restoreGeometry(settings.value("main/windowGeometry").toByteArray());
	}
	else if(!isVisible() && !firstRun)
	{
#ifdef Q_OS_WASM
		show();
#else
		QMetaObject::invokeMethod(this, "showMaximized", Qt::QueuedConnection);
#endif // Q_OS_WASM
	}
	if(!isVisible() && !firstRun)
		show();
	loadTheme();
	// Start timer (used to update currentTimeNumber every second)
	secLoop = new QTimer(this);
	connect(secLoop, SIGNAL(timeout()), this, SLOT(updateCurrentTime()));
	connect(&timedExTimer, &QTimer::timeout, this, &OpenTyper::updateCurrentTime);
	secLoop->start(500);
#ifdef Q_OS_WASM
	ui->printButton->hide();
	ui->actionPrint->setEnabled(false);
#else
	// Check for updates
	if(settings.value("main/updatechecks", true).toBool())
		new Updater();
#endif // Q_OS_WASM
}

/*! Destroys the Open-Typer object. */
OpenTyper::~OpenTyper()
{
	delete ui;
#ifndef Q_OS_WASM
	if(serverPtr)
		serverPtr->deleteLater();
#endif // Q_OS_WASM
	if(testLoaded && uploadResult && client.available())
		client.sendRequest("put", {"abortExercise"});
	settings.setValue("main/windowState", saveState());
	settings.setValue("main/windowGeometry", saveGeometry());
}

/*! Initializes the program and loads all settings.
 * \see changeMode
 * \see setColors
 * \see repeatLevel
 */
void OpenTyper::refreshAll(void)
{
#ifndef Q_OS_WASM
	// Start or stop server
	if(settings.value("main/networkEnabled", false).toBool() && (settings.value("server/mode", 2).toInt() == 1))
	{
		if(!serverPtr)
			serverPtr = new monitorServer(true, this);
		ui->serverFrame->show();
		if(serverPtr->isListening())
		{
			if(ui->serverFrameLayout->count() == 0)
			{
				serverManager *serverManagerWidget = new serverManager(ui->serverFrame);
				serverManagerWidget->setAttribute(Qt::WA_DeleteOnClose);
				ui->serverFrameLayout->addWidget(serverManagerWidget);
				connect(serverManagerWidget, &serverManager::widgetExpanded, this, [this]() { ui->paper->hide(); ui->controlFrame->setEnabled(false); ui->bottomPanel->setEnabled(false); });
				connect(serverManagerWidget, &serverManager::widgetCollapsed, this, [this]() { ui->paper->show(); ui->controlFrame->setEnabled(true); ui->bottomPanel->setEnabled(true); });
			}
			else
			{
				serverManager *serverManagerWidget = (serverManager*) ui->serverFrameLayout->itemAt(0)->widget();
				serverManagerWidget->init();
				serverManagerWidget->expand();
				serverManagerWidget->collapse();
			}
		}
		else
		{
			ui->serverFrame->hide();
			if(ui->serverFrameLayout->count() > 0)
				ui->serverFrameLayout->itemAt(0)->widget()->close();
		}
	}
	else
	{
		if(serverPtr)
			serverPtr->deleteLater();
		ui->serverFrame->hide();
		if(ui->serverFrameLayout->count() > 0)
			ui->serverFrameLayout->itemAt(0)->widget()->close();
	}
#endif // Q_OS_WASM
	// Config file (lesson pack) name
	QString configName = settings.value("main/configfile","").toString();
	if(configName == "")
	{
		firstRun = true;
		loadTheme();
		initialSetup *dialog = new initialSetup;
		dialog->show();
		QMetaObject::invokeMethod(this,"hide",Qt::QueuedConnection);
#ifdef Q_OS_WASM
		connect(dialog, &QDialog::accepted, this, [this]() { show(); refreshAll(); } );
#else
		connect(dialog, &QDialog::accepted, this, [this]() { showMaximized(); refreshAll(); } );
#endif // Q_OS_WASM
		return;
	}
	bool packChanged = (configName != oldConfigName);
	oldConfigName = configName;
	// Custom pack
	customConfig = settings.value("main/customconfig","false").toBool();
	// Space new line
	spaceNewline = settings.value("main/spacenewline","true").toBool();
	// Error penalty
	errorPenalty = settings.value("main/errorpenalty","10").toInt();
	// Load config and start
	if(packChanged)
	{
		QString configPath = loadConfig(configName);
		if(configPath == "")
			return;
		// Set up keyboard widget
		if(customConfig)
			ui->keyboardFrame->hide();
		else
		{
			// language_COUNTRY-VARIANT-SUFFIX
			QString localeStr = "", variant = "";
			bool localeRead = false;
			for(int i=0; i < publicConfigName.count(); i++)
			{
				if(publicConfigName[i] == '-')
				{
					if(localeRead)
						break;
					else
						localeRead = true;
				}
				else
				{
					if(localeRead)
						variant += publicConfigName[i];
					else
						localeStr += publicConfigName[i];
				}
			}
			QLocale locale(localeStr);
			if(ui->keyboardFrame->loadLayout(locale.language(),locale.country(),variant))
				ui->keyboardFrame->show();
			else
				ui->keyboardFrame->hide();
		}
		// Reset position
		currentLesson = 1;
		currentSublesson = 1;
		currentLevel = 1;
		repeatLevel();
	}
	else
	{
		updateLessonList();
		ui->lessonSelectionList->setCurrentIndex(currentLesson-1);
	}
	// Client
	updateStudent();
}

/*!
 * Opens a pack file using configParser and returns the file name of the pack file.
 * \param[in] configName If there's a custom pack opened, configName represents the absolute path to the pack. Otherwise, it represents the built-in pack name.
 * \param[in] packContent If it's not empty, a QBuffer with pack content is created and configParser treats it as a regular file.
 * \see configParser
 */
QString OpenTyper::loadConfig(QString configName, QByteArray packContent)
{
	customLevelLoaded=false;
	QString configPath = "";
	if(customConfig)
		configPath = configName;
	else
		configPath = ":/res/configs/" + configName;
	// Open selected config
	if(!parser.bufferOpened())
		parser.close();
	if(packContent == "")
	{
		bool bufferOpened = parser.bufferOpened();
		bool openSuccess;
		if(bufferOpened && customConfig)
			openSuccess = true;
		else
			openSuccess = parser.open(configPath);
		if(!openSuccess && !bufferOpened)
		{
			settings.setValue("main/configfile","");
			refreshAll();
			return QString();
		}
	}
	else
		parser.loadToBuffer(packContent);
	// Update lessonSelectionList widget
	updateLessonList();
	if(customConfig)
		configName = configPath;
	// Save selected config to settings
	settings.setValue("main/configfile",configName);
	if(customConfig)
	{
		QFile configQFile(configName);
		QFileInfo configQFileInfo(configQFile.fileName());
		ui->trainingPackNameLabel->setText(configQFileInfo.fileName());
	}
	else
		ui->trainingPackNameLabel->setText(builtinPacks::packName(configName));
	publicConfigName = configName;
	return configPath;
}

/*!
 * Initializes the specified exercise.
 * \param[in] lessonID, sublessonID, levelID Exercise location (lesson, sublesson and exercise ID).
 * \see loadConfig
 * \see configParser
 * \see levelFinalInit
 */
void OpenTyper::startLevel(int lessonID, int sublessonID, int levelID)
{
	// Update selected lesson
	ui->lessonSelectionList->setCurrentIndex(lessonID-1);
	// Get sublesson count
	sublessonCount = parser.sublessonCount(lessonID);
	// Check if -1 (last sublesson in current lesson) was passed
	if(sublessonID == -1)
		sublessonID = sublessonCount;
	// Update sublesson and exercise lists
	// This must happen before level loading!
	loadLesson(lessonID,sublessonID);
	// Check if -1 (last level in current sublesson) was passed
	if(levelID == -1)
		levelID = parser.exerciseCount(lessonID,sublessonID+sublessonListStart);
	// Load length extension
	if(!customLevelLoaded)
		levelLengthExtension = parser.exerciseLineLength(lessonID,sublessonID+sublessonListStart,levelID);
	// Load level text
	if(customLevelLoaded)
		level = customLevel;
	else
		level = parser.exerciseText(lessonID,
			sublessonID+sublessonListStart,
			levelID);
	if(uploadResult)
	{
		if((studentUsername != "") || (!client.fullMode() && client.isPaired()))
			client.sendRequest("put", {"abortExercise"});
		uploadResult = false;
	}
	// Get lesson count
	lessonCount = parser.lessonCount();
	// Get level count (in current lesson)
	levelCount = parser.exerciseCount(lessonID,sublessonID+sublessonListStart);
	// Update level list
	loadSublesson(levelID);
	// Make lesson, sublesson and level info public
	currentLesson=lessonID;
	currentSublesson=sublessonID;
	currentAbsoluteSublesson=sublessonID+sublessonListStart;
	currentLevel=levelID;
	// Init level
	levelFinalInit();
}

/*! Updates list of lessons. */
void OpenTyper::updateLessonList(void)
{
	ui->lessonSelectionList->clear();
	QStringList lessons;
	QString _lessonDesc;
	int i, count = parser.lessonCount();
	for(i=1; i <= count; i++)
	{
		_lessonDesc = configParser::parseDesc(parser.lessonDesc(i));
		if(_lessonDesc == "")
			lessons += configParser::lessonTr(i);
		else
			lessons += configParser::lessonTr(i) + " " + _lessonDesc;
	}
	ui->lessonSelectionList->addItems(lessons);
}

/*! Updates list of sublessons. */
void OpenTyper::loadLesson(int lessonID, int sublessonID)
{
	// Sublessons
	ui->sublessonSelectionList->clear();
	QStringList sublessons;
	sublessonListStart = 0;
	int i, i2=0;
	for(i=1; i <= sublessonCount+i2; i++)
	{
		if(parser.exerciseCount(lessonID,i) > 0)
			sublessons += configParser::sublessonName(i);
		else
		{
			i2++;
			if(sublessonID+i2 > i)
				sublessonListStart++;
		}
	}
	ui->sublessonSelectionList->addItems(sublessons);
	ui->sublessonSelectionList->setCurrentIndex(sublessonID-1);
}

/*! Updates list of exercises. */
void OpenTyper::loadSublesson(int levelID)
{
	// Exercises
	ui->levelSelectionList->clear();
	QStringList levels;
	for(int i=1; i <= levelCount; i++)
		levels += configParser::exerciseTr(i);
	ui->levelSelectionList->addItems(levels);
	ui->levelSelectionList->setCurrentIndex(levelID-1);
}

/*!
 * Sets initial values and shows the exercise text.
 * \see startLevel
 * \see updateText
 */
void OpenTyper::levelFinalInit(bool updateClient)
{
	// Init level
	publicPos::currentLesson = currentLesson;
	publicPos::currentSublesson = currentAbsoluteSublesson;
	publicPos::currentExercise = currentLevel;
	if(currentMode == 1)
		level += '\n';
	ui->exerciseChecksFrame->setEnabled(true);
	preview = false;
	currentLine=0;
	levelPos=0;
	displayPos=0;
	absolutePos = 0;
	linePos = 0;
	levelMistakes=0;
	totalHits=0;
	recordedCharacters.clear();
	recordedMistakes.clear();
	deadKeys=0;
	levelInProgress=false;
	lastTime=0;
	mistake=false;
	ignoreMistakeLabelAppend=false;
	mistakeTextHtml = "";
	mistakeLabelHtml = "";
	ui->mistakeLabel->setHtml(mistakeLabelHtml);
	ui->currentTimeNumber->setText("0");
	ui->currentMistakesNumber->setText("0");
	ui->closeCustomExButton->setVisible(customLevelLoaded);
	// Init level input
	input = "";
	inputTextHtml = "";
	displayInput = "";
	ui->inputLabel->setAcceptRichText(true);
	ui->inputLabel->setHtml(displayInput);
	updateText();
	// Update student session
	if(updateClient)
		updateStudent();
}

/*!
 * Converts exercise text (line wrapping, limited number of lines).
 * \see levelFinalInit
 * \see configParser
 */
void OpenTyper::updateText(void)
{
	ui->currentLineArea->show();
	ui->inputLabel->setFocusPolicy(Qt::StrongFocus);
	ui->typingSpace->setFocusPolicy(Qt::NoFocus);
	ui->typingSpace->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->textSeparationLine->show();
	ui->remainingTextArea->show();
	ui->exportButton->hide();
	displayLevel = configParser::initExercise(level,levelLengthExtension);
	lineCount = displayLevel.count('\n');
	// Process exercise text
	finalDisplayLevel = configParser::initExercise(level,levelLengthExtension,false,currentLine);
	QString currentLineText = "";
	QString remainingText = "";
	int i, line = 0;
	for(i=0; i < finalDisplayLevel.count(); i++)
	{
		remainingText += finalDisplayLevel[i];
		if((finalDisplayLevel[i] == '\n') || (i+1 >= finalDisplayLevel.count()))
		{
			if(line == 0)
			{
				currentLineText = remainingText;
				remainingText = "";
				if(currentLineText[currentLineText.count()-1] == '\n')
					currentLineText.remove(currentLineText.count()-1,1);
			}
			line++;
		}
	}
	ui->levelCurrentLineLabel->setText(currentLineText);
	ui->centralwidget->layout()->activate();
	ui->levelLabel->setText(remainingText);
	((QGraphicsOpacityEffect*)ui->levelLabel->graphicsEffect())->setOpacity(0.5);
	updateFont();
	if(ui->hideTextCheckBox->isChecked())
	{
		ui->currentLineArea->hide();
		ui->textSeparationLine->hide();
		ui->levelLabel->setPlainText("");
	}
	blockInput = false;
}

/*! Connected from repeatButton.\n
 * Resets currently selected exercise.
 * \see startLevel
 */
void OpenTyper::repeatLevel(void)
{
	startLevel(currentLesson,currentSublesson,currentLevel);
}

/*! Connected from nextButton.\n
 * Selects the next exercise.
 * \see repeatLevel
 */
void OpenTyper::nextLevel(void)
{
	customLevelLoaded=false;
	if(currentLevel == levelCount)
	{
		if(currentSublesson == sublessonCount)
		{
			if(currentLesson == lessonCount)
				currentLesson=1;
			else
				currentLesson++;
			currentSublesson=1;
		}
		else
			currentSublesson++;
		currentLevel=1;
	}
	else
		currentLevel++;
	repeatLevel();
}

/*! Connected from previousButton.\n
	 * Selects the previous exercise.
	 * \see repeatLevel
	 */
void OpenTyper::previousLevel(void)
{
	customLevelLoaded=false;
	if(currentLevel == 1)
	{
		if(currentSublesson == 1)
		{
			if(currentLesson == 1)
				currentLesson = lessonCount;
			else
				currentLesson--;
			currentSublesson=-1;
		}
		else
			currentSublesson--;
		currentLevel=-1;
	}
	else
		currentLevel--;
	repeatLevel();
}

/*! Connected from optionsButton.\n
 * Opens options window.
 */
void OpenTyper::openOptions(void)
{
	settings.sync();
	optionsWindow *optionsWin = new optionsWindow(this);
	optionsWin->init();
	optionsWin->open();
	optionsWin->setWindowModality(Qt::WindowModal);
	connect(optionsWin, &QDialog::finished, this, [this]() { show(); refreshAll(); });
}

/*!
 * Connected from studentButton->clicked().\n
 * Opens student authentication dialog (studentOptions).
 *
 * \see studentOptions
 */
void OpenTyper::openStudentOptions(void)
{
	studentOptions *dialog = new studentOptions(this);
	dialog->setWindowModality(Qt::WindowModal);
	connect(dialog, &QDialog::accepted, this, [dialog,this]() {
		studentUsername = dialog->username;
		studentPassword = dialog->password;
		updateStudent();
	});
	dialog->open();
}

/*! Updates student session. */
void OpenTyper::updateStudent(void)
{
	if(client.available() && client.sendRequest("get", { "serverMode" }).at(1) == "full")
		ui->actionLogIn->setEnabled(true);
	else
	{
		ui->actionLogIn->setEnabled(false);
		ui->studentLabel->setText("");
		bool enableStats = !customLevelLoaded && !customConfig && (currentMode == 0);
		ui->statsButton->setEnabled(enableStats);
		ui->actionStats->setEnabled(enableStats);
		return;
	}
	QStringList response = client.sendRequest("get",{"username"});
	if(response[0] == "ok")
	{
		if(studentUsername == "")
			client.sendRequest("logout",{studentUsername});
		else
		{
			QString username = response[1];
			ui->studentLabel->setText(tr("Logged in as %1").arg(username));
			bool enableStats = !customLevelLoaded && !customConfig && (currentMode == 0);
			ui->statsButton->setEnabled(enableStats);
			ui->actionStats->setEnabled(enableStats);
			return;
		}
	}
	else
	{
		if(studentUsername != "")
		{
			response = client.sendRequest("auth",{studentUsername,studentPassword});
			if(response[0] == "ok")
			{
				updateStudent();
				return;
			}
			else
			{
				studentUsername = "";
				studentPassword = "";
			}
		}
	}
	ui->studentLabel->setText(tr("Not logged in."));
	bool enableStats = !customLevelLoaded && !customConfig && (currentMode == 0);
	ui->statsButton->setEnabled(enableStats);
	ui->actionStats->setEnabled(enableStats);
}

/*! Connected from lessonSelectionList.\n
 * Selects the lesson selected in lessonSelectionList.
 */
void OpenTyper::lessonSelectionListIndexChanged(int index)
{
	currentLesson = index+1;
	currentSublesson = 1;
	currentLevel = 1;
	customLevelLoaded = false;
	repeatLevel();
}

/*! Connected from sublessonSelectionList.\n
 * Selects the sublesson selected in sublessonSelectionList.
 */
void OpenTyper::sublessonSelectionListIndexChanged(int index)
{
	currentSublesson = index+1;
	currentLevel = 1;
	customLevelLoaded = false;
	repeatLevel();
}

/*! Connected from levelSelectionList.\n
 * Selects the exercise selected in levelSelectionList.
 */
void OpenTyper::levelSelectionListIndexChanged(int index)
{
	currentLevel = index+1;
	customLevelLoaded = false;
	repeatLevel();
}

/*! Connected from openExerciseButton.\n
 * Shows a file dialog and opens a custom exercise.
 */
void OpenTyper::openExerciseFromFile(void)
{
	auto fileContentReady = [this](const QString &fileName, const QByteArray &fileContent) {
		if(!fileName.isEmpty())
		{
			if(fileContent.size() > 8192) // Maximum size
			{
				QMessageBox *errBox = new QMessageBox(this);
				errBox->setText(tr("This file is too large!"));
				errBox->setWindowModality(Qt::WindowModal);
				errBox->open();
			}
			else
			{
				levelLengthExtension = configParser::defaultLineLength;
				loadText(fileContent, true);
			}
		}
	};
#ifdef Q_OS_WASM
	QFileDialog::getOpenFileContent(QString(),  fileContentReady);
#else
	QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), tr("Text files") + " (*.txt)" + ";;" + tr("All files") + " (*)");
	if(fileName != "")
	{
		QFile file(fileName);
		if(file.open(QIODevice::ReadOnly | QIODevice::Text))
			fileContentReady(fileName, file.readAll());
	}
#endif
}

/*! Loads custom text. */
void OpenTyper::loadText(QByteArray text, bool includeNewLines, bool updateClient)
{
	level = "";
	QBuffer in(&text);
	in.open(QBuffer::ReadOnly | QBuffer::Text);
	while (!in.atEnd())
	{
		QString line = QString(in.readLine()).remove('\n');
		if(level == "")
			level = line;
		else
		{
			if(includeNewLines)
				level += "\n" + line;
			else
				level += " " + line;
		}
	}
	customLevel = level;
	customLevelLoaded=true;
	levelFinalInit(updateClient);
}

/*! Generates text from error words. */
void OpenTyper::loadErrorWords(void)
{
	if(errorWords.count() == 0)
	{
		QMessageBox *msgBox = new QMessageBox(this);
		msgBox->setWindowModality(Qt::WindowModal);
		msgBox->setText(tr("You don't have any error words."));
		msgBox->setIcon(QMessageBox::Information);
		msgBox->open();
		return;
	}
	int wordCount = 25;
	if(errorWords.count() > wordCount)
		wordCount += errorWords.count() - wordCount;
	QStringList usedWords, outputWords;
	usedWords.clear();
	outputWords.clear();
	int index = 0;
	for(int i=0; i < wordCount; i++)
	{
		QString word;
		if(i % 5 == 0)
		{
			do {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
				index = QRandomGenerator::global()->bounded(0, errorWords.count());
#else
				index = qrand() % errorWords.count();
#endif
				word = errorWords[index];
			} while(usedWords.contains(word));
		}
		word = errorWords[index];
		usedWords += word;
		outputWords += word;
		if(usedWords.count() == errorWords.count())
			usedWords.clear();
	}
	loadText(outputWords.join(" ").toUtf8());
}

/*! Generates reversed text. */
void OpenTyper::loadReversedText(void)
{
	QString oldText = configParser::initText(level);
	QString newText = "";
	for(int i = oldText.count()-1; i >= 0; i--)
		newText += level[i];
	loadText(newText.toUtf8(), true);
}

/*! Connected from inputLabelWidget#keyPressed signal.\n
 * Handles all key presses, counts hits, displays typed characters and counts mistakes.
 */
void OpenTyper::keyPress(QKeyEvent *event)
{
	if(blockInput || event->isAutoRepeat() || ((currentMode == 1) && !timedExStarted))
		return;
	int highlightID = event->key();
	if((input.count() < level.count()) && (event->key() == Qt::Key_Shift))
	{
		QPoint keyPos = ui->keyboardFrame->findKey(QString(displayLevel[displayPos]));
		keyboardWidget::Finger finger = ui->keyboardFrame->keyFinger(keyPos.x(),keyPos.y());
		if(keyboardWidget::fingerHand(finger) == 0)
			highlightID = -2;
	}
	ui->keyboardFrame->highlightKey(highlightID);
	if(keyboardUtils::isDeadKey(event->key()))
	{
		deadKeys++;
		// Count modifier key used with the dead key
		if(event->modifiers() != Qt::NoModifier)
			deadKeys++;
		return;
	}
	if(keyboardUtils::isSpecialKey(event) && (event->key() != Qt::Key_Backspace))
		return;
	if((levelPos == 0) && !levelInProgress)
	{
		errorWords.clear();
		ui->exerciseChecksFrame->setEnabled(false);
		levelTimer.start();
		secLoop->start(500);
		levelInProgress=true;
	}
	QString keyText = event->text();
	if((keyText == "'") && (displayLevel[displayPos] == QString("‘")))
		keyText = "‘";
	if((keyText == "‘") && (displayLevel[displayPos] == '\''))
		keyText = "'";
	if((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter))
		keyText = "\n";
	QString convertedKeyText = keyText.toHtmlEscaped().replace(" ", "&nbsp;");
	convertedKeyText.replace(" ", "&nbsp;");
	bool correctChar = (( ((displayLevel[displayPos] == '\n') && ((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter))) ||
		(((displayLevel[displayPos] != '\n') || (spaceNewline && !ui->correctMistakesCheckBox->isChecked())) && (keyText == level[levelPos]))) && !mistake);
	if(correctChar || !ui->correctMistakesCheckBox->isChecked())
	{
		if(!mistake && ignoreMistakeLabelAppend)
		{
			mistakeLabelHtml += "_";
			mistakeTextHtml += "_";
		}
		if(event->key() == Qt::Key_Backspace)
		{
			input = input.remove(input.count()-1, 1);
			QString plainText = displayInput;
			if(plainText.count() == 0)
			{
				if(currentLine == 0)
					displayInput = plainText.remove(plainText.count()-1, 1);
				else if(input.count() > 0)
				{
					displayInput = input.split('\n').last();
					currentLine--;
					linePos = 0;
					updateText();
				}
			}
			else
				displayInput = plainText.remove(plainText.count()-1, 1);
			if(input.count() > 0)
			{
				levelPos--;
				displayPos--;
				absolutePos--;
				linePos--;
				recordedCharacters.remove(recordedCharacters.count()-1);
			}
			plainText = QTextDocumentFragment::fromHtml(inputTextHtml).toPlainText();
			inputTextHtml = plainText.remove(plainText.count()-1, 1).toHtmlEscaped().replace(" ", "&nbsp;");
			inputTextHtml.replace("\n","<br>");
		}
		else
		{
			if((((keyText == "\n") || ((keyText == " ") && spaceNewline)) && (displayLevel[displayPos] == "\n")) || (keyText == "\n"))
			{
				inputTextHtml += "<br>";
				mistakeTextHtml += "<br>";
				displayInput = "";
				linePos = 0;
				mistakeLabelHtml = "";
				keyText = "\n";
				convertedKeyText = "";
				currentLine++;
				ignoreMistakeLabelAppend = false;
				updateText();
				if((currentMode == 1) && (currentLine >= lineCount-1))
				{
					currentLine=0;
					updateText();
					levelPos=-1;
					displayPos=-1;
					linePos = -1;
					deadKeys=0;
					mistake=false;
					mistakeLabelHtml = "";
					if(ui->hideTextCheckBox->isChecked())
						ui->mistakeLabel->setHtml(mistakeTextHtml);
					else
						ui->mistakeLabel->setHtml(mistakeLabelHtml);
					displayInput = "";
					if(ui->hideTextCheckBox->isChecked())
						ui->inputLabel->setHtml(input);
					else
						ui->inputLabel->setHtml(displayInput);
				}
			}
			else
			{
				if(ignoreMistakeLabelAppend)
					ignoreMistakeLabelAppend=false;
				else
				{
					QString mistakeLabelAppend = keyText == "\n" ? "<br>" : "&nbsp;";
					mistakeTextHtml += mistakeLabelAppend;
					mistakeLabelHtml += mistakeLabelAppend;
				}
				displayInput += keyText;
				linePos++;
			}
			input += keyText;
			inputTextHtml += convertedKeyText;
			levelPos++;
			displayPos++;
			absolutePos++;
			int charHits = 1;
			// Count modifier keys
			if(event->modifiers() != Qt::NoModifier)
				charHits++;
			// Count dead keys
			charHits += deadKeys;
			totalHits += charHits;
			recordedCharacters += QPair<QString,int>(keyText, charHits);
			deadKeys = 0;
		}
	}
	else
	{
		if(mistake)
		{
			deadKeys = 0;
			if(event->key() == Qt::Key_Backspace)
			{
				mistake=false;
				ignoreMistakeLabelAppend=true;
			}
		}
		else
		{
			if(!keyboardUtils::isSpecialKey(event))
			{
				QList<QVariantMap> mistakesToRemove;
				for(int i=0; i < recordedMistakes.count(); i++)
				{
					if(recordedMistakes[i]["pos"] == absolutePos)
						mistakesToRemove += recordedMistakes[i];
				}
				levelMistakes -= mistakesToRemove.count();
				for(int i=0; i < mistakesToRemove.count(); i++)
					recordedMistakes.removeAll(mistakesToRemove[i]);
				QVariantMap currentMistake;
				currentMistake["pos"] = absolutePos;
				currentMistake["previous"] = keyText;
				currentMistake["type"] = "change";
				recordedMistakes += currentMistake;
				QString errorAppend;
				if(ignoreMistakeLabelAppend)
				{
					QTextCursor cursor = ui->mistakeLabel->textCursor();
					cursor.deletePreviousChar();
					ui->mistakeLabel->setTextCursor(cursor);
				}
				if(keyText == " ")
					errorAppend = "_";
				else if(keyText == "\n")
					errorAppend = "↵<br>";
				else
					errorAppend = convertedKeyText;
				if(ui->hideTextCheckBox->isChecked())
					ui->inputLabel->setHtml(input.toHtmlEscaped().replace(" ", "&nbsp;").replace("\n", "<br>") + "<span style='color: red;'>" + errorAppend + "</span>");
				else
					ui->inputLabel->setHtml(displayInput.toHtmlEscaped().replace(" ", "&nbsp;").replace("\n", "<br>") + "<span style='color: red;'>" + errorAppend + "</span>");
				levelMistakes++;
				ui->currentMistakesNumber->setText(QString::number(levelMistakes));
				mistake=true;
				QString errorWord = stringUtils::wordAt(level, levelPos);
				if((errorWord != "") && !errorWords.contains(errorWord))
					errorWords += errorWord;
				deadKeys = 0;
			}
		}
	}
	if(!mistake)
	{
		if(ui->hideTextCheckBox->isChecked())
			ui->inputLabel->setHtml(input.toHtmlEscaped().replace(" ", "&nbsp;").replace("\n", "<br>"));
		else
			ui->inputLabel->setHtml(displayInput.toHtmlEscaped().replace(" ", "&nbsp;").replace("\n", "<br>"));
	}
	QString mistakeLabelFinal;
	if(ui->hideTextCheckBox->isChecked())
		mistakeLabelFinal = mistake ? mistakeTextHtml + "&nbsp;" : mistakeTextHtml;
	else
		mistakeLabelFinal = mistake ? mistakeLabelHtml + "&nbsp;" : mistakeLabelHtml;
	if(!mistake && ignoreMistakeLabelAppend)
		mistakeLabelFinal += "_";
	ui->mistakeLabel->setHtml(mistakeLabelFinal);
	ui->mistakeLabel->setFixedWidth(ui->inputLabel->width());
	if(mistake)
		ui->mistakeLabel->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
	else
		ui->mistakeLabel->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	ui->inputLabel->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
	ui->typingSpace->ensureWidgetVisible(ui->inputLabel);
	if(((displayPos >= displayLevel.count()) && ui->correctMistakesCheckBox->isChecked()) || (currentLine >= lineCount+1))
	{
		if(currentLine >= lineCount+1)
			input.remove(input.count()-1, 1);
		keyRelease(event);
		lastTime = levelTimer.elapsed()/1000;
		lastTimeF = levelTimer.elapsed()/1000.0;
		endExercise(true, true, false, true, true);
	}
}

/*! Ends the exercise by (optionally) saving the results and showing the levelSummary dialog. */
void OpenTyper::endExercise(bool showNetHits, bool showGrossHits, bool showTotalHits, bool showTime, bool showMistakes)
{
	levelInProgress=false;
	input.replace("‘", "'");
	displayLevel.replace("‘", "'");
	if(ui->correctMistakesCheckBox->isChecked())
		input = stringUtils::addMistakes(input, &recordedMistakes);
	else
	{
		recordedMistakes = stringUtils::validateExercise(displayLevel, input, recordedCharacters, &totalHits, &levelMistakes, &errorWords, (currentMode == 1), lastTimeF);
		netHits = std::max(0, totalHits - (levelMistakes * errorPenalty));
		ui->currentMistakesNumber->setText(QString::number(levelMistakes));
	}
	QMap<int, QVariantMap*> mistakesMap;
	for(int i=0; i < recordedMistakes.count(); i++)
		mistakesMap[recordedMistakes[i]["pos"].toInt()] = &recordedMistakes[i];
	inputTextHtml = "";
	QStringList lines = input.split("\n");
	int pos = 0, delta = 0;
	mistakeTextHtml = "";
	for(int i=0; i < lines.count(); i++)
	{
		QString addLine = lines[i].toHtmlEscaped().replace(" ", "&nbsp;");
		inputTextHtml += "<span style=\"color: rgba(0, 0, 0, 0);\">" + addLine + "</span><br>";
		// Add line with correct characters
		int oldPos = pos;
		int count = lines[i].count();
		for(int j=0; j <= lines[i].count(); j++)
		{
			QString inputChar;
			if(j < count)
				inputChar = QString(input[pos]).toHtmlEscaped().replace(" ", "&nbsp;");
			else
				inputChar = "&nbsp;";
			if(mistakesMap.contains(pos))
			{
				QString correct;
				if(ui->correctMistakesCheckBox->isChecked())
					correct = displayLevel[pos - delta];
				else
					correct = mistakesMap[pos]->value("previous").toString();
				QString type = mistakesMap[pos]->value("type").toString();
				if(type == "deletion")
				{
					mistakeTextHtml += correct.split("\n").at(0).toHtmlEscaped().replace(" ", "&nbsp;");
					inputTextHtml += QString("&nbsp;").repeated(correct.split("\n").at(0).count());
				}
				else
					mistakeTextHtml += correct.toHtmlEscaped().replace(" ", "&nbsp;").replace("\n", "&nbsp;");
				if(type == "change")
				{
					if(correct == "\n")
						delta++;
				}
				else
					mistakeTextHtml += "&nbsp;";
				inputTextHtml += inputChar;
			}
			else if(j < count)
			{
				mistakeTextHtml += "<span style=\"color: rgba(0, 0, 0, 0);\">" + QString(input[pos]).toHtmlEscaped().replace(" ", "&nbsp;") + "</span>";
				inputTextHtml += inputChar;
			}
			if(j < count)
				pos++;
		}
		inputTextHtml += "<br>";
		// Add line with underlined mistakes
		mistakeTextHtml += "<br>";
		pos = oldPos;
		for(int j=0; j <= count; j++)
		{
			QString append = "&nbsp;";
			if(mistakesMap.contains(pos))
			{
				QString correct;
				if(ui->correctMistakesCheckBox->isChecked())
					correct = displayLevel[pos];
				else
					correct = mistakesMap[pos]->value("previous").toString();
				if((mistakesMap[pos]->value("type").toString() == "deletion") && correct.contains("\n"))
					mistakeTextHtml += "<u>&nbsp;";
				else
					mistakeTextHtml += "<u>" + QString("&nbsp;").repeated(std::max(1, correct.count()));
			}
			else if(j < count)
				mistakeTextHtml += "<span style=\"color: rgba(0, 0, 0, 0);\">" + QString(input[pos]).toHtmlEscaped().replace(" ", "&nbsp;") + "</span>";
			if(mistakesMap.contains(pos))
			{
				mistakeTextHtml += "</u>";
				if(mistakesMap[pos]->value("type").toString() == "deletion")
					mistakeTextHtml += "&nbsp;";
			}
			if(j < count)
				pos++;
		}
		mistakeTextHtml += "<br>";
		pos++;
	}
	netHits = std::max(0, totalHits - levelMistakes * settings.value("main/errorpenalty","10").toInt());
	int netHitsPerMinute = netHits*(60/(levelTimer.elapsed()/1000.0));
	int grossHitsPerMinute = totalHits*(60/(levelTimer.elapsed()/1000.0));
	int time = levelTimer.elapsed()/1000;
	if(!customLevelLoaded && !customConfig && ui->correctMistakesCheckBox->isChecked())
	{
		if(studentUsername != "")
		{
			updateStudent();
			client.sendRequest("put",
				{"result",publicConfigName.toUtf8(),QByteArray::number(currentLesson),QByteArray::number(currentAbsoluteSublesson),QByteArray::number(currentLevel),
				QByteArray::number(grossHitsPerMinute),QByteArray::number(levelMistakes),QByteArray::number(time)});
		}
		else
			historyParser::addHistoryEntry(publicConfigName,currentLesson,currentAbsoluteSublesson,currentLevel,
				{QString::number(grossHitsPerMinute),QString::number(levelMistakes),QString::number(time)});
	}
	if(testLoaded)
	{
		ui->correctMistakesCheckBox->setChecked(correctMistakesOld);
		ui->hideTextCheckBox->setChecked(hideTextOld);
		if(isFullScreen())
		{
			showNormal();
			restoreGeometry(oldGeometry);
		}
		if(uploadResult && ((studentUsername != "") || (!client.fullMode() && client.isPaired())))
		{
			updateStudent();
			QStringList list = {"recordedCharacters"};
			for(int i=0; i < recordedCharacters.count(); i++)
			{
				QPair<QString, int> character = recordedCharacters[i];
				list += character.first;
				list += QString::number(character.second);
			}
			client.sendRequest("put", list);
			client.sendRequest("put", {"testResult", input, QString::number(lastTimeF)});
		}
		ui->controlFrame->setEnabled(true);
		ui->menuBar->setEnabled(true);
		testLoaded = false;
	}
	levelSummary *msgBox = new levelSummary(this);
	if(showNetHits)
		msgBox->setNetHits(netHitsPerMinute);
	if(showGrossHits)
		msgBox->setGrossHits(grossHitsPerMinute);
	if(showTime)
		msgBox->setTotalTime(time);
	if(showTotalHits)
		msgBox->setTotalHits(totalHits);
	if(showMistakes)
	msgBox->setMistakes(levelMistakes);
	msgBox->setWindowModality(Qt::WindowModal);
	connect(msgBox, &QDialog::accepted, this, [this]() {
		changeMode(0);
		ui->exerciseChecksFrame->setEnabled(false);
		ui->exportButton->show();
		preview = true;
		// Load saved text
		ui->inputLabel->setHtml(inputTextHtml);
		ui->mistakeLabel->setHtml(mistakeTextHtml);
		ui->mistakeLabel->setFixedWidth(ui->mistakeLabel->document()->size().width());
		// Move cursor to the end
		ui->mistakeLabel->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
		ui->inputLabel->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
		// Hide other widgets
		ui->currentLineArea->hide();
		ui->textSeparationLine->hide();
		ui->remainingTextArea->hide();
		ui->inputLabel->setFocusPolicy(Qt::NoFocus);
		ui->typingSpace->setFocusPolicy(Qt::StrongFocus);
		ui->typingSpace->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		updateFont();
		blockInput = true;
	});
	connect(msgBox, &QDialog::rejected, this, [this]() {
		changeMode(0);
		if(customLevelLoaded)
			levelFinalInit();
		else
			repeatLevel();
	});
	msgBox->open();
}

/*!
 * Connected from inputLabelWidget#keyReleased signal.\n
 * Dehighlights keys on the virtual keyboard.
 * \see keyboardWidget
 */
void OpenTyper::keyRelease(QKeyEvent *event)
{
	ui->keyboardFrame->dehighlightKey(event->key());
	if(event->key() == Qt::Key_Shift)
		ui->keyboardFrame->dehighlightKey(-2);
}

/*! Connected from secLoop.\n
 * Runs periodically and updates time widgets.
 */
void OpenTyper::updateCurrentTime(void)
{
	int time;
	if(levelInProgress)
		time = levelTimer.elapsed()/1000;
	else
		time = lastTime;
	if(currentMode == 1)
	{
		QTime limitTime;
		if(timedExStarted)
			limitTime.setHMS(timedExHours,timedExMinutes,timedExSeconds);
		else
			limitTime.setHMS(0,0,3);
		QTime currentTime = limitTime.addSecs(time*(-1));
		if((currentTime > limitTime) || (currentTime.hour()+currentTime.minute()+currentTime.second() == 0))
		{
			if(timedExStarted)
			{
				if(levelInProgress)
				{
					ui->timedExCountdownLabel->hide();
					lastTime = QTime(0, 0, 0).secsTo(limitTime);
					lastTimeF = QTime(0, 0, 0).msecsTo(limitTime)/1000.0;
					endExercise(true, true, true, true, true);
				}
			}
			else
			{
				timedExStarted = true;
				levelTimer.start();
				timedExTimer.setSingleShot(true);
				limitTime.setHMS(timedExHours,timedExMinutes,timedExSeconds);
				timedExTimer.start(QTime(0, 0, 0).msecsTo(limitTime));
				secLoop->start(500);
				ui->timedExTime->show();
				ui->timedExCountdownLabel->hide();
			}
		}
		if(timedExStarted)
		{
			time = levelTimer.elapsed()/1000;
			limitTime.setHMS(timedExHours,timedExMinutes,timedExSeconds);
			if(levelInProgress)
				ui->timedExTime->setTime(limitTime.addSecs(time*(-1)));
			else
				ui->timedExTime->setTime(QTime(0,0,0));
		}
		else
			ui->timedExCountdownLabel->setText(QString::number(currentTime.second()));
	}
	else
		ui->currentTimeNumber->setText(QString::number(time));
}

/*! Loads theme from settings. */
void OpenTyper::loadTheme(void)
{
	// Load style
	localThemeEngine.updateStyle();
	// Load colors
	setColors();
	// Load font
	updateFont();
}

/*! Updates text font. */
void OpenTyper::updateFont(void)
{
	QFont newFont = themeEngine::font();
	QFont mistakeLabelFont = themeEngine::errorFont();
	// Update labels
	ui->levelCurrentLineLabel->setFont(newFont);
	ui->levelLabel->setFont(newFont);
	ui->inputLabel->setFont(newFont);
	ui->mistakeLabel->setFont(mistakeLabelFont);
	ui->mistakeLabel->setFixedWidth(ui->mistakeLabel->document()->size().width());
	int scrollBarWidth = ui->typingSpace->verticalScrollBar()->size().width();
	QTextDocument *tmpDoc = ui->levelCurrentLineLabel->document()->clone(this);
	ui->currentLineArea->setFixedSize(tmpDoc->size().toSize() + QSize(scrollBarWidth, 0));
	tmpDoc->setPlainText(displayLevel);
	ui->typingSpace->setFixedWidth(std::max(tmpDoc->size().toSize().width()+12,
		std::max(tmpDoc->size().toSize().width()+12,
		ui->keyboardFrame->width()+12)));
	ui->typingSpace->setMinimumHeight(0);
	ui->typingSpace->setMaximumHeight(ui->inputLabel->document()->size().height() * 2);
	if(!preview)
		ui->typingSpace->setMaximumWidth(QWIDGETSIZE_MAX);
	else
	{
		ui->typingSpace->setFixedWidth(ui->inputLabel->document()->size().width() + scrollBarWidth*2);
		ui->typingSpace->setMaximumHeight(QWIDGETSIZE_MAX);
	}
}

/*! Sets custom colors (if they are set) or default colors. */
void OpenTyper::setColors(void)
{
	// Set exercise text color
	if(!themeEngine::customExerciseTextColor())
		localThemeEngine.resetExerciseTextColor();
	QString levelLabelStyleSheet = themeEngine::exerciseTextStyleSheet();
	ui->levelLabel->setStyleSheet(levelLabelStyleSheet);
	ui->levelCurrentLineLabel->setStyleSheet(levelLabelStyleSheet);
	// Set input text color
	if(!themeEngine::customInputTextColor())
		localThemeEngine.resetInputTextColor();
	ui->inputLabel->setStyleSheet(themeEngine::inputTextStyleSheet());
	// Set background color
	if(!themeEngine::customBgColor())
		localThemeEngine.resetBgColor();
	ui->centralwidget->setStyleSheet(themeEngine::bgStyleSheet());
	// Set paper color
	if(!themeEngine::customPaperColor())
		localThemeEngine.resetPaperColor();
	ui->paper->setStyleSheet(themeEngine::paperStyleSheet());
	QColor lineColor = palette().color(QPalette::Window);
	ui->textSeparationLine->setStyleSheet("QFrame { background-color: rgb(" +
		QString::number(lineColor.red()) + "," + 
		QString::number(lineColor.green()) + "," +
		QString::number(lineColor.blue()) +
		"); }");
	// Set panel color
	if(!themeEngine::customPanelColor())
		localThemeEngine.resetPanelColor();
	QString panelStyleSheet = themeEngine::panelStyleSheet();
	ui->controlFrame->setStyleSheet(panelStyleSheet);
	ui->bottomPanel->setStyleSheet(panelStyleSheet);
	ui->serverFrame->setStyleSheet(qApp->styleSheet());
	ui->menuBar->setStyleSheet(panelStyleSheet);
	// Set keyboard color
	QColor keyBorderColor = palette().color(QPalette::Text);
	keyBorderColor = QColor::fromRgb(keyBorderColor.red() + (128-keyBorderColor.red()),
		keyBorderColor.green() + (128-keyBorderColor.green()),
		keyBorderColor.blue() + (128-keyBorderColor.blue()));
	QColor keyBgColor = palette().color(QPalette::Window);
	keyBgColor = QColor::fromRgb(keyBgColor.red() + (128-keyBgColor.red())/10,
		keyBgColor.green() + (128-keyBgColor.green())/10,
		keyBgColor.blue() + (128-keyBgColor.blue())/10);
	ui->keyboardFrame->setKeyColor(keyBgColor,keyBorderColor);
}

/*! Connected from openPackButton.\n
 * Shows a file dialog and opens a custom pack.
 */
void OpenTyper::openPack(void)
{
	auto fileContentReady = [this](const QString &fileName, const QByteArray &fileContent) {
		if(!fileName.isEmpty())
		{
			customConfig=true;
			settings.setValue("main/customconfig",customConfig);
			loadConfig(fileName, fileContent);
			refreshAll();
		}
	};
#ifdef Q_OS_WASM
	QFileDialog::getOpenFileContent(QString(),  fileContentReady);
#else
	QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), tr("Open-Typer pack files") + " (*.typer)" + ";;" + tr("All files") + " (*)");
	if(fileName != "")
	{
		QFile file(fileName);
		if(file.open(QIODevice::ReadOnly | QIODevice::Text))
			fileContentReady(fileName, file.readAll());
	}
#endif
}

/*! Connected from openEditorButton.\n
 * Opens the editor.
 */
void OpenTyper::openEditor(void)
{
	// Close pack file
	QString oldFileName = parser.fileName();
	parser.close();
	// Open editor
	packEditor *editorWindow;
	editorWindow = new packEditor(this);
	editorWindow->setWindowFlag(Qt::WindowMinimizeButtonHint,true);
	editorWindow->setWindowFlag(Qt::WindowMaximizeButtonHint,true);
	editorWindow->setWindowModality(Qt::WindowModal);
	connect(editorWindow, &QDialog::finished, this, [oldFileName,this]() {
		// Open pack file
		parser.open(oldFileName);
	});
	editorWindow->open();
}

/*! Overrides QWidget#changeEvent().
 * Retranslates UI when the display language changes.
 */
void OpenTyper::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::LanguageChange)
	{
		ui->retranslateUi(this);
		if(settings.value("main/configfile", "").toString() == "")
			return;
		globalThemeEngine.updateThemeList();
		localThemeEngine.updateStyle();
		updateLessonList();
		ui->lessonSelectionList->setCurrentIndex(currentLesson-1);
		loadLesson(currentLesson,currentSublesson);
		loadSublesson(currentLevel);
	}
	else
		QWidget::changeEvent(event);
}

/*! Connected from zoomInButton.\n
 * Increases the text size.
 * \see zoomOut
 */
void OpenTyper::zoomIn(void)
{
	localThemeEngine.increaseFontSize(2);
	updateFont();
}

/*! Connected from zoomOutButton.\n
 * Decreases the text size.
 * \see zoomIn
 */
void OpenTyper::zoomOut(void)
{
	localThemeEngine.increaseFontSize(-2);
	updateFont();
}

/*! Changes the operation mode.\n
 * Supported modes are default (0) and timed exercise mode (1).
 */
void OpenTyper::changeMode(int mode, bool enableStudentUpdate)
{
	switch(mode) {
		case 0:
			// Default mode
			ui->masterControlFrame->show();
			ui->timedExControlFrame->hide();
			break;
		case 1:
			// Timed exercise mode
			ui->masterControlFrame->hide();
			ui->timedExControlFrame->show();
			break;
	}
	currentMode = mode;
	if(enableStudentUpdate)
		updateStudent();
}

/*! Connected from timedExerciseButton.\n
 * Switches to timed exercise mode.
 */
void OpenTyper::initTimedExercise(void)
{
	if(currentMode == 1)
	{
		// Switch back to default mode
		changeMode(0);
		repeatLevel();
	}
	else
	{
		timeDialog *timeSelect = new timeDialog(this);
		timeSelect->setWindowModality(Qt::WindowModal);
		connect(timeSelect, &QDialog::accepted, this, [timeSelect,this]() {
			timedExHours = timeSelect->hours;
			timedExMinutes = timeSelect->minutes;
			timedExSeconds = timeSelect->seconds;
			timedExStarted = false;
			changeMode(1);
			ui->timedExCountdownLabel->setText("3");
			ui->timedExTime->setTime(QTime(timedExHours,timedExMinutes,timedExSeconds));
			ui->timedExTime->hide();
			ui->timedExCountdownLabel->show();
			levelFinalInit();
			levelInProgress = true;
			levelTimer.start();
			secLoop->start(500);
		});
		timeSelect->open();
	}
}

/*!
 * Connected from statsButton->clicked().\n
 * Opens statsDialog.
 *
 * \see statsDialog
 */
void OpenTyper::showExerciseStats(void)
{
	statsDialog *dialog;
	if((studentUsername != ""))
		dialog = new statsDialog(&client,publicConfigName,currentLesson,currentAbsoluteSublesson,currentLevel,this);
	else if(!customLevelLoaded && !customConfig)
		dialog = new statsDialog(nullptr,publicConfigName,currentLesson,currentAbsoluteSublesson,currentLevel,this);
	else
		return;
	dialog->setWindowModality(Qt::WindowModal);
	dialog->open();
}

/*! Connected from client.exerciseReceived(). */
void OpenTyper::loadReceivedExercise(QByteArray text, int lineLength, bool includeNewLines, int mode, int time, bool correctMistakes, bool lockUi, bool hideText)
{
	if(waitDialog)
		waitDialog->accept();
	waitDialog = nullptr;
	startReceivedExercise(text, lineLength, includeNewLines, mode, time, correctMistakes, lockUi, hideText, true);
}

/*! Starts received exercise or a local typing test. */
void OpenTyper::startReceivedExercise(QByteArray text, int lineLength, bool includeNewLines, int mode, int time, bool correctMistakes, bool lockUi, bool hideText, bool upload)
{
	changeMode(mode, false);
	levelLengthExtension = lineLength;
	loadText(text,includeNewLines,false);
	if(mode == 1)
	{
		ui->timedExCountdownLabel->setText("3");
		QTime exTime = QTime(0, 0, 0).addSecs(time);
		timedExHours = exTime.hour();
		timedExMinutes = exTime.minute();
		timedExSeconds = exTime.second();
		timedExStarted = false;
		ui->timedExTime->setTime(exTime);
		ui->timedExTime->hide();
		ui->timedExCountdownLabel->show();
		levelInProgress = true;
		levelTimer.start();
		secLoop->start(500);
	}
	correctMistakesOld = ui->correctMistakesCheckBox->isChecked();
	hideTextOld = ui->hideTextCheckBox->isChecked();
	ui->correctMistakesCheckBox->setChecked(correctMistakes);
	ui->hideTextCheckBox->setChecked(hideText);
	if(lockUi)
	{
		ui->controlFrame->setEnabled(false);
		ui->menuBar->setEnabled(false);
		oldGeometry = saveGeometry();
		showFullScreen();
	}
	testLoaded = true;
	uploadResult = upload;
}

/*! Opens testWaitDialog and waits until the received exercise starts. */
void OpenTyper::waitForReceivedExercise(QString text, int lineLength)
{
	waitDialog = new testWaitDialog(&client, this);
	if(text != "")
		waitDialog->setText(configParser::initExercise(text, lineLength));
	QString name = "";
	if(client.fullMode())
	{
		QStringList response = client.sendRequest("get", { "name" });
		name = response.count() > 1 ? response[1] : "";
		waitDialog->setNameReadOnly(true);
	}
	waitDialog->setName(name);
	waitDialog->setWindowModality(Qt::WindowModal);
	waitDialog->setAttribute(Qt::WA_DeleteOnClose);
	testWaitDialog *dialogPtr = waitDialog;
	connect(dialogPtr, &QDialog::finished, this, [this]() { waitDialog = nullptr; });
	waitDialog->open();
}

/*!
 * Opens exportDialog.
 *
 * \see exportDialog
 */
void OpenTyper::exportText(void)
{
	QVariantMap result;
	result["grossHits"] = totalHits;
	result["netHits"] = netHits;
	result["netHitsPerMinute"] = (double) netHits*(60.0/lastTimeF);
	result["mistakes"] = levelMistakes;
	result["penalty"] = errorPenalty;
	result["time"] = lastTimeF/60.0;
	exportDialog *dialog = new exportDialog(input, result, recordedMistakes, this);
	dialog->setWindowModality(Qt::WindowModal);
	dialog->open();
	dialog->showMaximized();
}

/*! Prints the exercise text. */
void OpenTyper::printText(void)
{
	#ifndef Q_OS_WASM
	// Set up printer
	QPrinter printer(QPrinter::HighResolution);
	QPrinter *printerPtr = &printer;
	QPrintPreviewDialog dialog(&printer);
	connect(&dialog, &QPrintPreviewDialog::paintRequested, this, [this, printerPtr]() {
		// Print
		printerPtr->setPageMargins(QMarginsF(25, 25, 15, 25), QPageLayout::Millimeter);
		QPainter painter;
		painter.begin(printerPtr);
		QFont font = ui->levelLabel->font();
		painter.setFont(font);
		QTextDocument *document = ui->levelLabel->document()->clone(this);
		QString title = "";
		if(!customLevelLoaded)
			title = QString("<u>%1 / %2 / %3</u><br><br>").arg(configParser::lessonTr(publicPos::currentLesson),
				configParser::sublessonName(publicPos::currentSublesson),
				configParser::exerciseTr(publicPos::currentExercise));
		document->setHtml(QString("<body>%1%2</body>").arg(title,
			displayLevel.toHtmlEscaped().replace("\n", "<br>")));
		font.setPointSize(50);
		document->setDefaultFont(font);
		document->documentLayout()->setPaintDevice(printerPtr);
		document->setDefaultStyleSheet("body { color: black; }");
		document->setPageSize(printerPtr->pageRect(QPrinter::DevicePixel).size());
		double scale = printerPtr->pageRect(QPrinter::DevicePixel).width() / double(document->size().width());
		int fontHeight = QFontMetrics(painter.font(), printerPtr).height();
		QStringList lines = document->toHtml().split("<br>");
		int relativeLine = 0, page = 0, fromPage = printerPtr->fromPage()-1, toPage = printerPtr->toPage()-1;
		for(int i=0; i < lines.count(); i++)
		{
			int rangeEnd = toPage;
			if(rangeEnd == -1)
				rangeEnd = page+1;
			if(fontHeight*relativeLine > printerPtr->pageRect(QPrinter::DevicePixel).height())
			{
				if(((page+1 >= fromPage) && (page+1 <= rangeEnd)) && ((page >= fromPage) && (page <= rangeEnd)))
					printerPtr->newPage();
				relativeLine = 0;
				page++;
			}
			document->setHtml(lines[i]);
			if((page >= fromPage) && (page <= rangeEnd))
			{
				painter.resetTransform();
				painter.scale(scale, scale);
				painter.translate(0, fontHeight*relativeLine);
				document->drawContents(&painter);
			}
			relativeLine++;
		}
		painter.end();
	});
	dialog.exec();
#endif // Q_OS_WASM
}

/*! Starts typing test. */
void OpenTyper::startTest(void)
{
	loadExerciseDialog *dialog;
	bool fullMode = settings.value("server/fullmode", false).toBool();
#ifdef Q_OS_WASM
	dialog = new loadExerciseDialog(this);
#else
	if(serverPtr && serverPtr->isListening() && (!fullMode || (dbMgr.activeClass != 0)))
	{
		QList<int> targets;
		if(fullMode)
			targets = dbMgr.studentIDs(dbMgr.activeClass);
		else
			targets = dbMgr.deviceIDs();
		QList<int> onlineTargets, occupiedTargets = serverPtr->runningExerciseStudents();
		for(int i=0; i < targets.count(); i++)
		{
			if(fullMode)
			{
				if(serverPtr->isLoggedIn(dbMgr.userNickname(targets[i])) && !occupiedTargets.contains(targets[i]))
					onlineTargets += targets[i];
			}
			else if(serverPtr->isConnected(dbMgr.deviceAddress(targets[i])))
				onlineTargets += targets[i];
		}
		dialog = new loadExerciseDialog(onlineTargets, this);
	}
	else
		dialog = new loadExerciseDialog(this);
#endif // Q_OS_WASM
	dialog->setWindowModality(Qt::WindowModal);
	dialog->open();
	connect(dialog, &QDialog::accepted, this, [this, dialog, fullMode]() {
#ifndef Q_OS_WASM
		if(serverPtr && serverPtr->isListening() && (!fullMode || (dbMgr.activeClass != 0)))
			classControls::startExercise(dialog);
		else
#endif // Q_OS_WASM
			startReceivedExercise(dialog->exerciseText().toUtf8(), dialog->lineLength(), dialog->includeNewLines(),
				dialog->mode(), QTime(0, 0, 0).secsTo(dialog->timeLimit()), dialog->correctMistakes(), dialog->lockUi(), dialog->hideText(), false);
	});
}

/*! Shows about program dialog. */
void OpenTyper::showAboutDialog(void)
{
	QString buildDate = __DATE__;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	QString buildYear = buildDate.sliced(7, 4);
#else
	QString buildYear = buildDate.mid(7, 4);
#endif
	QMessageBox::about(this, QString(),
		"<b>Open-Typer</b><br><br>" +
#ifdef BUILD_VERSION
		tr("Version: %1").arg(QCoreApplication::applicationVersion()) + "<br>" +
#endif // BUILD_VERSION
#ifdef BUILD_REVISION
		tr("Revision: %1").arg(BUILD_REVISION) + "<br>" +
#endif // BUILD_REVISION
		tr("Source code: %1").arg("<a href=\"https://github.com/Open-Typer/Open-Typer\">https://github.com/Open-Typer/Open-Typer</a>") + "<br><br>" +
		QString("Copyright © %1-%2 %3").arg("2021", buildYear, "adazem009") + "<br>" +
		tr("Published with the GNU General Public License.")
	);
}
