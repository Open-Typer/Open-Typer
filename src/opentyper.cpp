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
	ui->mistakeLabel->setParent(ui->inputLabel);
	inputLabelLayout->addWidget(ui->mistakeLabel);
	inputLabelLayout->setContentsMargins(0,0,0,0);
	QMenu *openMenu = new QMenu(ui->openButton);
	QAction *openExerciseAction = openMenu->addAction(tr("Open custom exercise"));
	QAction *openPackAction = openMenu->addAction(tr("Open custom pack"));
	connect(openExerciseAction, &QAction::triggered, this, &OpenTyper::openExerciseFromFile);
	connect(openPackAction, &QAction::triggered, this, &OpenTyper::openPack);
	ui->openButton->setMenu(openMenu);
	connect(ui->openButton, &QToolButton::clicked, openExerciseAction, &QAction::triggered);
	localThemeEngine.setParent(this);
	client.setErrorDialogs(false);
	studentUsername = "";
	studentPassword = "";
	oldConfigName = "";
	refreshAll(true);
	// Connect signals to slots
	connectAll();
#ifndef Q_OS_WASM
	// Check for updates
	new Updater();
#endif
}

/*! Destroys the Open-Typer object. */
OpenTyper::~OpenTyper()
{
	delete ui;
}

/*! Initializes the program and loads all settings.
 * \param[in] setLang Whether to set the display language.
 * \see changeLanguage
 * \see changeMode
 * \see setColors
 * \see repeatLevel
 */
void OpenTyper::refreshAll(bool setLang)
{
	// Set language
	if(setLang)
	{
		if(settings.value("main/language","").toString() == "")
			changeLanguage(0,false,false);
		else
			changeLanguage(langMgr.boxItems.indexOf(settings.value("main/language","").toString()),false,false);
	}
	// Config file (lesson pack) name
	QString configName = settings.value("main/configfile","").toString();
	if(configName == "")
	{
		loadTheme();
		initialSetup *dialog = new initialSetup;
		dialog->show();
		QMetaObject::invokeMethod(this,"hide",Qt::QueuedConnection);
		connect(dialog, &QDialog::accepted, this, [this]() { show(); refreshAll(true); } );
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
	// Class monitor client
	updateStudent();
	// Theme
	if(!isVisible())
		show();
	loadTheme();
	// Set mode
	changeMode(0);
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
}

/*! Sets up all connections. */
void OpenTyper::connectAll(void)
{
	// Create timer (used to update currentTimeNumber every second)
	secLoop = new QTimer(this);
	// Client disconnected signal
	connect(&client,
		SIGNAL(disconnected()),
		this,
		SLOT(updateStudent()));
	// Client exercise received signal
	connect(&client,
		&monitorClient::exerciseReceived,
		this,
		&OpenTyper::loadReceivedExercise);
	// **Timers**
	// Updates current time in seconds
	connect(secLoop,
		SIGNAL(timeout()),
		this,
		SLOT(updateCurrentTime()));
	// **Widgets**
	// inputLabel
	connect(ui->inputLabel,
		SIGNAL(keyPressed(QKeyEvent*)),
		this,
		SLOT(keyPress(QKeyEvent*)));
	connect(ui->inputLabel,
		SIGNAL(keyReleased(QKeyEvent*)),
		this,
		SLOT(keyRelease(QKeyEvent*)));
	// Options button
	connect(ui->optionsButton,
		SIGNAL(clicked()),
		this,
		SLOT(openOptions()));
	// Open editor button
	connect(ui->openEditorButton,
		SIGNAL(clicked()),
		this,
		SLOT(openEditor()));
	// Student options button
	connect(ui->studentButton,
		SIGNAL(clicked()),
		this,
		SLOT(openStudentOptions()));
	// Repeat exercise button
	connect(ui->repeatButton,
		SIGNAL(clicked()),
		this,
		SLOT(repeatLevel()));
	// Next exercise button
	connect(ui->nextButton,
		SIGNAL(clicked()),
		this,
		SLOT(nextLevel()));
	// Previous exercise button
	connect(ui->previousButton,
		SIGNAL(clicked()),
		this,
		SLOT(previousLevel()));
	// List of lessons
	connect(ui->lessonSelectionList,
		SIGNAL(activated(int)),
		this,
		SLOT(lessonSelectionListIndexChanged(int)));
	// List of sub lessons
	connect(ui->sublessonSelectionList,
		SIGNAL(activated(int)),
		this,
		SLOT(sublessonSelectionListIndexChanged(int)));
	// List of levels
	connect(ui->levelSelectionList,
		SIGNAL(activated(int)),
		this,
		SLOT(levelSelectionListIndexChanged(int)));
	// Zoom in button
	connect(ui->zoomInButton,
		SIGNAL(clicked()),
		this,
		SLOT(zoomIn()));
	// Zoom out button
	connect(ui->zoomOutButton,
		SIGNAL(clicked()),
		this,
		SLOT(zoomOut()));
	// Timed exercise button
	connect(ui->timedExerciseButton,
		SIGNAL(clicked()),
		this,
		SLOT(initTimedExercise()));
	// Stats button
	connect(ui->statsButton,
		SIGNAL(clicked()),
		this,
		SLOT(showExerciseStats()));
	// Theme engine
	connect(&globalThemeEngine, &themeEngine::fontChanged, this, &OpenTyper::updateFont);
	connect(&globalThemeEngine, &themeEngine::colorChanged, this, &OpenTyper::setColors);
	connect(&globalThemeEngine, &themeEngine::styleChanged, this, &OpenTyper::setColors);
	connect(&globalThemeEngine, &themeEngine::themeChanged, this, &OpenTyper::loadTheme);
	// Start timer
	secLoop->start(500);
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
			refreshAll(false);
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
	levelLengthExtension = parser.exerciseLineLength(lessonID,sublessonID+sublessonListStart,levelID);
	// Load level text
	if(customLevelLoaded)
		level = customLevel;
	else
		level = parser.exerciseText(lessonID,
			sublessonID+sublessonListStart,
			levelID);
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
			lessons += tr("Lesson") + " " + QString::number(i);
		else
			lessons += tr("Lesson") + " " + QString::number(i) +
				" " + _lessonDesc;
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
		levels += tr("Exercise") + " " + QString::number(i);
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
	if(currentMode == 1)
		level += '\n';
	currentLine=0;
	updateText();
	levelPos=0;
	displayPos=0;
	levelMistakes=0;
	totalHits=0;
	levelHits=0;
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
	// Init level input
	input = "";
	inputTextHtml = "";
	displayInput = "";
	ui->inputLabel->setAcceptRichText(true);
	ui->inputLabel->setHtml(displayInput);
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
	ui->levelCurrentLineLabel->show();
	ui->textSeparationLine->show();
	ui->levelLabel->show();
	displayLevel = configParser::initExercise(level,levelLengthExtension);
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
	ui->inputLabel->setMinimumHeight(0);
	ui->mistakeLabel->setMinimumHeight(0);
	ui->inputLabel->setPlainText(displayLevel);
	ui->levelCurrentLineLabel->setMinimumWidth(ui->inputLabel->document()->size().width());
	ui->levelCurrentLineLabel->setText(currentLineText);
	ui->inputLabel->setHtml(displayInput);
	ui->levelLabel->setText(remainingText);
	((QGraphicsOpacityEffect*)ui->levelLabel->graphicsEffect())->setOpacity(0.5);
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
	connect(optionsWin,SIGNAL(languageChanged(int)),this,SLOT(changeLanguage(int)));
	optionsWin->open();
	optionsWin->setWindowModality(Qt::WindowModal);
	connect(optionsWin, &QDialog::finished, this, [this]() { show(); refreshAll(false); });
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
	if(client.available())
	{
		ui->studentButton->show();
		ui->studentLabel->show();
	}
	else
	{
		ui->studentButton->hide();
		ui->studentLabel->hide();
		if(customLevelLoaded || customConfig || (currentMode != 0))
			ui->statsButton->hide();
		else
			ui->statsButton->show();
		return;
	}
	QList<QByteArray> response = client.sendRequest("get",{"username"});
	if(response[0] == "ok")
	{
		if(studentUsername == "")
			client.sendRequest("logout",{studentUsername.toUtf8()});
		else
		{
			QString username = response[1];
			ui->studentLabel->setText(tr("Logged in as %1").arg(username));
			if(customLevelLoaded || customConfig || (currentMode != 0))
				ui->statsButton->hide();
			else
				ui->statsButton->show();
			return;
		}
	}
	else
	{
		if(studentUsername != "")
		{
			response = client.sendRequest("auth",{studentUsername.toUtf8(),studentPassword.toUtf8()});
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
	if(customLevelLoaded || customConfig || (currentMode != 0))
		ui->statsButton->hide();
	else
		ui->statsButton->show();
}

/*! Connected from lessonSelectionList.\n
 * Selects the lesson selected in lessonSelectionList.
 */
void OpenTyper::lessonSelectionListIndexChanged(int index)
{
	currentLesson = index+1;
	currentSublesson = 1;
	currentLevel = 1;
	repeatLevel();
}

/*! Connected from sublessonSelectionList.\n
 * Selects the sublesson selected in sublessonSelectionList.
 */
void OpenTyper::sublessonSelectionListIndexChanged(int index)
{
	currentSublesson = index+1;
	currentLevel = 1;
	repeatLevel();
}

/*! Connected from levelSelectionList.\n
 * Selects the exercise selected in levelSelectionList.
 */
void OpenTyper::levelSelectionListIndexChanged(int index)
{
	currentLevel = index+1;
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
			if(fileContent.size() > 2048) // Maximum size
			{
				QMessageBox *errBox = new QMessageBox(this);
				errBox->setText(tr("This file is too large!"));
				errBox->setWindowModality(Qt::WindowModal);
				errBox->open();
			}
			else
			{
				// Show paper config dialog
				paperConfigDialog *pconfig = new paperConfigDialog(this);
				pconfig->setWindowModality(Qt::WindowModal);
				connect(pconfig, &QDialog::accepted, this, [pconfig,fileContent,this]() {
					levelLengthExtension = pconfig->lineLength;
					loadText(fileContent,pconfig->includeNewLines);
				});
				pconfig->open();
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
		QString line = in.readLine();
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

/*! Connected from inputLabelWidget#keyPressed signal.\n
 * Handles all key presses, counts hits, displays typed characters and counts mistakes.
 */
void OpenTyper::keyPress(QKeyEvent *event)
{
	if(blockInput)
		return;
	if(event->isAutoRepeat())
		return;
	if((currentMode == 1) && !timedExStarted)
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
	QString convertedKeyText = keyText.toHtmlEscaped();
	if((((displayLevel[displayPos] == '\n') && ((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter))) || (((displayLevel[displayPos] != '\n') || spaceNewline) && (keyText == level[levelPos]))) && !mistake)
	{
		input += keyText;
		displayInput += convertedKeyText;
		inputTextHtml += convertedKeyText;
		if(displayLevel[displayPos] == '\n')
		{
			inputTextHtml += "<br>";
			mistakeTextHtml += "<br>";
			displayInput = "";
			mistakeLabelHtml = "";
			ui->mistakeLabel->setHtml(mistakeLabelHtml);
			currentLine++;
			updateText();
		}
		else
		{
			if(ignoreMistakeLabelAppend)
				ignoreMistakeLabelAppend=false;
			else
			{
				QString mistakeLabelAppend = "<span style='color: rgba(0,0,0,0)'>" + keyText + "</span>";
				mistakeTextHtml += mistakeLabelAppend;
				mistakeLabelHtml += mistakeLabelAppend;
				ui->mistakeLabel->setHtml(mistakeLabelHtml);
			}
		}
		ui->inputLabel->setHtml(displayInput);
		levelPos++;
		displayPos++;
		totalHits++;
		levelHits++;
		// Count modifier keys
		if(event->modifiers() != Qt::NoModifier)
		{
			totalHits++;
			levelHits++;
		}
		// Count dead keys
		totalHits += deadKeys;
		levelHits += deadKeys;
		deadKeys = 0;
	}
	else
	{
		if(mistake)
		{
			deadKeys = 0;
			if(event->key() == Qt::Key_Backspace)
			{
				ui->inputLabel->setHtml(displayInput);
				if(!ignoreMistakeLabelAppend)
				{
					mistakeLabelHtml += "_";
					mistakeTextHtml += "_";
					ui->mistakeLabel->setHtml(mistakeLabelHtml);
				}
				mistake=false;
				ignoreMistakeLabelAppend=true;
			}
		}
		else
		{
			if(!keyboardUtils::isSpecialKey(event))
			{
				ui->inputLabel->setHtml(displayInput);
				QString errorAppend;
				if(keyText == " ")
					errorAppend = "_";
				else if((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter))
					errorAppend = "↵<br>";
				else
					errorAppend = convertedKeyText;
				ui->inputLabel->setHtml(displayInput + "<span style='color: red';'>" + errorAppend + "</span>");
				levelMistakes++;
				ui->currentMistakesNumber->setText(QString::number(levelMistakes));
				mistake=true;
				deadKeys = 0;
				levelHits -= errorPenalty;
				if(levelHits < 0)
					levelHits = 0;
			}
		}
	}
	ui->mistakeLabel->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
	ui->inputLabel->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
	ui->inputLabel->setMinimumWidth(ui->inputLabel->document()->size().width());
	ui->mistakeLabel->setMinimumWidth(ui->mistakeLabel->document()->size().width());
	if(input.count() >= level.count())
	{
		if(currentMode == 1)
		{
			currentLine=0;
			updateText();
			levelPos=0;
			displayPos=0;
			deadKeys=0;
			mistake=false;
			ignoreMistakeLabelAppend=false;
			mistakeLabelHtml = "";
			ui->mistakeLabel->setHtml(mistakeLabelHtml);
			input = "";
			displayInput = "";
			ui->inputLabel->setHtml(displayInput);
		}
		else
		{
			keyRelease(event);
			levelInProgress=false;
			lastTime = levelTimer.elapsed()/1000;
			int speed = levelHits*(60/(levelTimer.elapsed()/1000.0));
			int realSpeed = totalHits*(60/(levelTimer.elapsed()/1000.0));
			int time = levelTimer.elapsed()/1000;
			if((studentUsername != "") && !customLevelLoaded && !customConfig)
			{
				updateStudent();
				client.sendRequest("put",
					{"result",publicConfigName.toUtf8(),QByteArray::number(currentLesson),QByteArray::number(currentAbsoluteSublesson),QByteArray::number(currentLevel),
					QByteArray::number(realSpeed),QByteArray::number(levelMistakes),QByteArray::number(time)});
			}
			else if(!customLevelLoaded && !customConfig)
				historyParser::addHistoryEntry(publicConfigName,currentLesson,currentAbsoluteSublesson,currentLevel,
					{QString::number(realSpeed),QString::number(levelMistakes),QString::number(time)});
			levelSummary *msgBox = new levelSummary(this);
			msgBox->setTotalTime(time);
			msgBox->setHits(speed);
			msgBox->setMistakes(levelMistakes);
			msgBox->setWindowModality(Qt::WindowModal);
			connect(msgBox, &QDialog::accepted, this, [this]() {
				// Load saved text
				ui->inputLabel->setHtml(inputTextHtml);
				ui->mistakeLabel->setHtml(mistakeTextHtml);
				// Set width
				ui->inputLabel->setMinimumWidth(ui->inputLabel->document()->size().width());
				ui->mistakeLabel->setMinimumWidth(ui->mistakeLabel->document()->size().width());
				// Set height
				ui->inputLabel->setMinimumHeight(ui->inputLabel->document()->size().height());
				ui->mistakeLabel->setMinimumHeight(ui->mistakeLabel->document()->size().height());
				// Move cursor to the end
				ui->mistakeLabel->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
				ui->inputLabel->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
				// Hide other widgets
				ui->levelCurrentLineLabel->hide();
				ui->textSeparationLine->hide();
				ui->levelLabel->setText(""); // Using hide() breaks the layout, it's better to set empty text
				blockInput = true;
			});
			connect(msgBox, &QDialog::rejected, this, [this]() {
				if(customLevelLoaded)
					levelFinalInit();
				else
					repeatLevel();
			});
			msgBox->open();
		}
	}
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
					// Show summary
					levelInProgress=false;
					lastTime = levelTimer.elapsed()/1000;
					levelSummary *msgBox = new levelSummary(this);
					ui->timedExCountdownLabel->hide();
					msgBox->setTotalTime(levelTimer.elapsed()/1000);
					msgBox->setHitCount(totalHits);
					msgBox->setHits(levelHits*(60/(levelTimer.elapsed()/1000.0)));
					msgBox->setMistakes(levelMistakes);
					msgBox->setWindowModality(Qt::WindowModal);
					connect(msgBox, &QDialog::accepted, this, [this]() {
						// Load saved text
						ui->inputLabel->setHtml(inputTextHtml);
						ui->mistakeLabel->setHtml(mistakeTextHtml);
						// Set width
						ui->inputLabel->setMinimumWidth(ui->inputLabel->document()->size().width());
						ui->mistakeLabel->setMinimumWidth(ui->mistakeLabel->document()->size().width());
						// Set height
						ui->inputLabel->setMinimumHeight(ui->inputLabel->document()->size().height());
						ui->mistakeLabel->setMinimumHeight(ui->mistakeLabel->document()->size().height());
						// Move cursor to the end
						ui->mistakeLabel->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
						ui->inputLabel->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
						// Hide other widgets
						ui->levelCurrentLineLabel->hide();
						ui->textSeparationLine->hide();
						ui->levelLabel->setText(""); // Using hide() breaks the layout, it's better to set empty text
						blockInput = true;
					});
					connect(msgBox, &QDialog::rejected, this, [this]() { repeatLevel(); } );
					connect(msgBox, &QDialog::finished, this, [this]() { changeMode(0);; } );
					msgBox->open();
				}
			}
			else
			{
				timedExStarted = true;
				levelTimer.start();
				secLoop->start(500);
				ui->timedExRemainingLabel->show();
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
	QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
	opacityEffect->setOpacity(0.5);
	ui->levelLabel->setGraphicsEffect(opacityEffect);
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
			refreshAll(false);
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
	packEditor *editorWindow = new packEditor(this);
	editorWindow->setWindowFlag(Qt::WindowMinimizeButtonHint,true);
	editorWindow->setWindowFlag(Qt::WindowMaximizeButtonHint,true);
	editorWindow->init();
	editorWindow->setWindowModality(Qt::WindowModal);
	connect(editorWindow, &QDialog::finished, this, [oldFileName,this]() {
		// Open pack file
		parser.open(oldFileName);
	});
	editorWindow->open();
}

/*! Connected from optionsWindow#languageChanged.\n
 * Retranslates the UI in the selected language.
 * \param[in] index Index of the selected language in the list of languages
 * \param[in] enableRefresh Toggles call to refreshAll()
 * \param[in] enableListReload Toggles lesson and sublesson list reload
 * \see languageManager
 */
void OpenTyper::changeLanguage(int index, bool enableRefresh, bool enableListReload)
{
	QLocale targetLocale;
	if(index == 0)
		targetLocale = QLocale::system();
	else
		targetLocale = QLocale(langMgr.supportedLanguages[index-1],langMgr.supportedCountries[index-1]);
	QCoreApplication::removeTranslator(&translator);
	if(translator.load(targetLocale,QLatin1String("Open-Typer"),QLatin1String("_"),QLatin1String(":/res/lang")))
		QCoreApplication::installTranslator(&translator);
	ui->retranslateUi(this);
	if(enableRefresh)
		refreshAll(false);
	if(enableListReload)
	{
		loadLesson(currentLesson,currentSublesson);
		loadSublesson(currentLevel);
	}
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
void OpenTyper::changeMode(int mode)
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
			ui->timedExRemainingLabel->hide();
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
void OpenTyper::loadReceivedExercise(QByteArray text, int lineLength, bool includeNewLines)
{
	levelLengthExtension = lineLength;
	loadText(text,includeNewLines,false);
}
