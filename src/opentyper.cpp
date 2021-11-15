/*
 * opentyper.cpp
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

#include "opentyper.h"
#include "ui_opentyper.h"

OpenTyper::OpenTyper(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::OpenTyper)
{
	ui->setupUi(this);
	langMgr = new languageManager;
	refreshAll();
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	// Connect signals to slots
	connectAll();
	// Check for updates
	#ifdef Q_OS_WINDOWS
	#ifdef _WIN32
	new Updater();
	#endif
	#endif
}

OpenTyper::~OpenTyper()
{
	delete ui;
}

void OpenTyper::refreshAll(void)
{
	// Read config
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	// Set language
	if(settings.value("main/language","").toString() == "")
		changeLanguage(0);
	else
		changeLanguage(langMgr->boxItems.indexOf(settings.value("main/language","").toString()));
	// Config file (lesson pack) name
	QString configName = settings.value("main/configfile","").toString(); // default: "" (empty QString)
	if(configName == "")
	{
		// No config file selected, use sk_SK-QWERTZ-B1
		// TODO: Change this to en_US after a config for it is added.
		configName = "sk_SK-QWERTZ-B1";
	}
	// Custom pack
	customConfig = settings.value("main/customconfig","false").toBool();
	// Font
	setFont(settings.value("theme/font","Courier").toString(),
		settings.value("theme/fontsize","14").toInt(),
		settings.value("theme/fontbold","false").toBool(),
		settings.value("theme/fontitalic","false").toBool(),
		settings.value("theme/fontunderline","false").toBool());
	// Colors
	// Level text
	customLevelTextColor = settings.value("theme/customleveltextcolor","false").toBool();
	levelTextRedColor = settings.value("theme/leveltextred","0").toInt();
	levelTextGreenColor = settings.value("theme/leveltextgreen","0").toInt();
	levelTextBlueColor = settings.value("theme/leveltextblue","0").toInt();
	// Input text
	customInputTextColor = settings.value("theme/custominputtextcolor","false").toBool();
	inputTextRedColor = settings.value("theme/inputtextred","0").toInt();
	inputTextGreenColor = settings.value("theme/inputtextgreen","0").toInt();
	inputTextBlueColor = settings.value("theme/inputtextblue","0").toInt();
	// Background
	customBgColor = settings.value("theme/custombgcolor","false").toBool();
	bgRedColor = settings.value("theme/bgred","0").toInt();
	bgGreenColor = settings.value("theme/bggreen","0").toInt();
	bgBlueColor = settings.value("theme/bgblue","0").toInt();
	// Paper
	customPaperColor = settings.value("theme/custompapercolor","false").toBool();
	paperRedColor = settings.value("theme/paperred","0").toInt();
	paperGreenColor = settings.value("theme/papergreen","0").toInt();
	paperBlueColor = settings.value("theme/paperblue","0").toInt();
	// Panel
	customPanelColor = settings.value("theme/custompanelcolor","false").toBool();
	panelRedColor = settings.value("theme/panelred","0").toInt();
	panelGreenColor = settings.value("theme/panelgreen","0").toInt();
	panelBlueColor = settings.value("theme/panelblue","0").toInt();
	// Theme
	updateTheme();
	// Space bar new lines
	if(settings.value("main/spacenewline","true").toBool())
		ui->spaceNewlineCheckBox->setCheckState(Qt::Checked);
	else
		ui->spaceNewlineCheckBox->setCheckState(Qt::Unchecked);
	// Load config and start
	char *configPath = loadConfig(configName);
	if(configPath == NULL)
		exit(1);
	currentLesson = 1;
	currentSublesson = 1;
	currentLevel = 1;
	repeatLevel();
}

void OpenTyper::connectAll(void)
{
	// Create timer (used to update currentTimeNumber every second)
	QTimer *secLoop = new QTimer(this);
	// **Timers**
	// Updates current time in seconds
	connect(secLoop,
		SIGNAL(timeout()),
		this,
		SLOT(updateCurrentTime()));
	// inputLabel
	connect(ui->inputLabel,
		SIGNAL(keyPressed(QKeyEvent*)),
		this,
		SLOT(keyPress(QKeyEvent*)));
	// Options button
	connect(ui->optionsButton,
		SIGNAL(clicked()),
		this,
		SLOT(openOptions()));
	// Open pack button
	connect(ui->openPackButton,
		SIGNAL(clicked()),
		this,
		SLOT(openPack()));
	// Open editor button
	connect(ui->openEditorButton,
		SIGNAL(clicked()),
		this,
		SLOT(openEditor()));
	// **Training tab**
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
	// Random order checkbox
	connect(ui->randomOrderCheckBox,
		SIGNAL(clicked(bool)),
		this,
		SLOT(randomOrderCheckBoxChanged(bool)));
	// Space new line checkbox
	connect(ui->spaceNewlineCheckBox,
		SIGNAL(clicked(bool)),
		this,
		SLOT(spaceNewlineCheckBoxChanged(bool)));
	// Open exercise from file button
	connect(ui->openExerciseButton,
		SIGNAL(clicked()),
		this,
		SLOT(openExerciseFromFile()));
	// Start timer
	secLoop->start(1000);
}

char *OpenTyper::loadConfig(QString configName)
{
	// Returns config file name, which can be opened later.
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	char *configPath = (char*) "";
	if(customConfig)
	{
		configPath = (char*) malloc(strlen(configName.toStdString().c_str())+1);
		strcpy(configPath,configName.toStdString().c_str());
	}
	else
	{
		QString configLoc = getConfigLoc();
		configPath = (char*) malloc(strlen(configLoc.toStdString().c_str())+1+strlen(configName.toStdString().c_str())+1);
		sprintf(configPath,"%s/%s",configLoc.toStdString().c_str(),configName.toStdString().c_str());
		// Create config directory if it doesn't exist
		QDir configDir(configLoc);
		if(!configDir.exists())
			configDir.mkpath(configLoc);
		// Extract selected config
		chmod(configPath, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		QFile::remove(configPath);
		QFile::copy(":/res/configs/"+configName,configPath);
	}
	// Open extracted config (just to check that everything is OK and to update lesson list)
	errno=0;
	FILE *configCheckFile;
	#ifdef Q_OS_WINDOWS
	configCheckFile = _wfopen(str_to_wcs(configPath),L"rb");
	#else
	configCheckFile = fopen(configPath,"rb");
	#endif
	if(errno != 0)
	{
		QMessageBox errBox;
		errBox.setText("Failed to read read configuration "+configName+": "+strerror(errno));
		errBox.setStyleSheet(styleSheet());
		errBox.exec();
		// Select default configuration
		configName = "sk_SK-QWERTZ-B1";
		customConfig=false;
		settings.setValue("main/customconfig",customConfig);
		printf("%s\n",strerror(errno));
		return loadConfig(configName);
	}
	// Update lessonSelectionList widget
	ui->lessonSelectionList->clear();
	QStringList lessons;
	QString _lessonDesc;
	int i, count = _lesson_count(configCheckFile);
	for(i=1; i <= count; i++)
	{
		_lessonDesc = parseDesc(_lesson_desc(configCheckFile,i));
		if(_lessonDesc == "")
			lessons += tr("Lesson") + " " + QString::number(i);
		else
			lessons += tr("Lesson") + " " + QString::number(i) +
				" " + _lessonDesc;
	}
	ui->lessonSelectionList->addItems(lessons);
	fclose(configCheckFile);
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
		ui->trainingPackNameLabel->setText(_pack_name(configName));
	publicConfigName = configName;
	return configPath;
}

int OpenTyper::_line_count(QString str)
{
	int out=0, i, len = QStringLen(str);
	for(i=0; i < len; i++)
	{
		if((str[i] == '\n') || (i == len))
			out++;
	}
	return out;
}

void OpenTyper::startLevel(FILE *cr, int lessonID, int sublessonID, int levelID)
{
	customLevelLoaded=false;
	// Update selected lesson
	ui->lessonSelectionList->setCurrentIndex(lessonID-1);
	// Get sublesson count
	sublessonCount = _lesson_sublesson_count(cr,lessonID);
	// Update sublesson list
	// This must happen before level loading!
	ui->sublessonSelectionList->clear();
	QStringList sublessons;
	int i, i2=0;
	for(i=1; i <= sublessonCount+i2; i++)
	{
		if(_lesson_sublesson_level_count(cr,lessonID,i) > 0)
		{
			switch(i) {
				case 1:
					sublessons += tr("Touch");
					break;
				case 2:
					sublessons += tr("Words");
					break;
				case 3:
					sublessons += tr("Sentences");
					break;
				case 4:
					sublessons += tr("Text");
					break;
				default:
					sublessons += tr("Sublesson") + " " + QString::number(i);
					break;
			}
		}
		else
		{
			i2++;
		}
	}
	sublessonListStart = i2;
	// Check if -1 (last sublesson in current lesson) was passed
	if(sublessonID == -1)
		sublessonID = _lesson_sublesson_count(cr,lessonID);
	// Check if -1 (last level in current sublesson) was passed
	if(levelID == -1)
		levelID = _lesson_sublesson_level_count(cr,lessonID,sublessonID+sublessonListStart);
	ui->sublessonSelectionList->addItems(sublessons);
	ui->sublessonSelectionList->setCurrentIndex(sublessonID-1);
	// Update random order check box
	if(sublessonID+sublessonListStart == 2) // words
	{
		ui->randomOrderCheckBox->setEnabled(true);
		ui->randomOrderCheckBox->setCheckable(true);
		QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
		QString randomize = settings.value("main/randomwords","false").toString(); // default: "false"
		if(randomize == "true")
			ui->randomOrderCheckBox->setCheckState(Qt::Checked);
		else
			ui->randomOrderCheckBox->setCheckState(Qt::Unchecked);
	}
	else
	{
		ui->randomOrderCheckBox->setCheckable(false);
		ui->randomOrderCheckBox->setEnabled(false);
		ui->randomOrderCheckBox->setCheckState(Qt::Unchecked);
	}
	// Load length extension
	levelLengthExtension = _lesson_sublesson_level_length_extension(cr,lessonID,sublessonID+sublessonListStart,levelID);
	// Load level text
	level = _lesson_sublesson_level_text(cr,
		lessonID,
		sublessonID+sublessonListStart,
		levelID,
		ui->randomOrderCheckBox->isChecked());
	// Get lesson count
	lessonCount = _lesson_count(cr);
	// Get level count (in current lesson)
	levelCount = _lesson_sublesson_level_count(cr,lessonID,sublessonID+sublessonListStart);
	// Update level list
	ui->levelSelectionList->clear();
	QStringList levels;
	for(int i=1; i <= levelCount; i++)
		levels += tr("Exercise") + " " + QString::number(i);
	ui->levelSelectionList->addItems(levels);
	ui->levelSelectionList->setCurrentIndex(levelID-1);
	// Make lesson, sublesson and level info public
	currentLesson=lessonID;
	currentSublesson=sublessonID;
	currentLevel=levelID;
	// Init level
	levelFinalInit();
}

void OpenTyper::levelFinalInit(void)
{
	// Init level
	displayLevel = _init_level(level,levelLengthExtension);
	ui->levelLabel->setText(displayLevel);
	adjustSize();
	setColors();
	levelPos=0;
	displayPos=0;
	levelMistakes=0;
	levelInProgress=false;
	lastTime=0;
	mistake=false;
	ignoreMistakeLabelAppend=false;
	mistakeLabelHtml = "";
	ui->mistakeLabel->setHtml(mistakeLabelHtml);
	ui->currentTimeNumber->setText("0");
	ui->currentMistakesNumber->setText("0");
	// Init level input
	input = "";
	displayInput = "";
	ui->inputLabel->setAcceptRichText(true);
	ui->inputLabel->setHtml(displayInput);
}

void OpenTyper::repeatLevel(void)
{
	FILE *cr;
	#ifdef Q_OS_WINDOWS
	cr = _wfopen(str_to_wcs(loadConfig(publicConfigName)),L"rb");
	#else
	cr = fopen(loadConfig(publicConfigName),"rb");
	#endif
	startLevel(cr,currentLesson,currentSublesson,currentLevel);
	fclose(cr);
}

void OpenTyper::nextLevel(void)
{
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
void OpenTyper::previousLevel(void)
{
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

void OpenTyper::openOptions(void)
{
	optionsWindow *optionsWin = new optionsWindow;
	optionsWin->setStyleSheet(styleSheet());
	optionsWin->init();
	connect(optionsWin,SIGNAL(languageChanged(int)),this,SLOT(changeLanguage(int)));
	optionsWin->exec();
	refreshAll();
}

void OpenTyper::lessonSelectionListIndexChanged(int index)
{
	currentLesson = index+1;
	currentSublesson = 1;
	currentLevel = 1;
	repeatLevel();
}

void OpenTyper::sublessonSelectionListIndexChanged(int index)
{
	currentSublesson = index+1;
	currentLevel = 1;
	repeatLevel();
}

void OpenTyper::levelSelectionListIndexChanged(int index)
{
	currentLevel = index+1;
	repeatLevel();
}

void OpenTyper::randomOrderCheckBoxChanged(bool checked)
{
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	if(checked)
		settings.setValue("main/randomwords","true");
	else
		settings.setValue("main/randomwords","false");
	repeatLevel();
}

void OpenTyper::spaceNewlineCheckBoxChanged(bool checked)
{
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	if(checked)
		settings.setValue("main/spacenewline","true");
	else
		settings.setValue("main/spacenewline","false");
}

void OpenTyper::openExerciseFromFile(void)
{
	// Show file dialog
	QFileDialog openDialog;
	openDialog.setFileMode(QFileDialog::AnyFile);
	openDialog.setNameFilter(tr("Text files") + " (*.txt)" + ";;" + tr("All files") + " (*)");
	if(openDialog.exec())
	{
		// Get selected file
		QString fileName = openDialog.selectedFiles()[0];
		QFile exerciseFile(fileName);
		if(exerciseFile.size() > 2048) // Maximum size
		{
			QMessageBox errBox;
			errBox.setText(tr("This file is too large!"));
			errBox.setStyleSheet(styleSheet());
			errBox.exec();
			return;
		}
		// Show paper config dialog
		paperConfigDialog pconfig;
		pconfig.setStyleSheet(styleSheet());
		pconfig.exec();
		levelLengthExtension = pconfig.lineLength;
		// Read selected file
		if(!exerciseFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox errBox;
			errBox.setText(tr("Could not open the file."));
			errBox.setStyleSheet(styleSheet());
			errBox.exec();
			return;
		}
		level = "";
		QTextStream in(&exerciseFile);
		while (!in.atEnd())
		{
			QString line = in.readLine();
			if(level == "")
				level = line;
			else
				level += " " + line;
		}
		customLevelLoaded=true;
		levelFinalInit();
	}
}

bool OpenTyper::isSpecialKey(QKeyEvent *event)
{
	if(event->text() == "")
		return true;
	switch(event->key()) {
		case Qt::Key_Delete:
			return true;
			break;
		case Qt::Key_Backspace:
			return true;
			break;
		case Qt::Key_Backtab:
			return true;
			break;
		case Qt::Key_Escape:
			return true;
			break;
		default:
			return false;
			break;
	}
}

void OpenTyper::keyPress(QKeyEvent *event)
{
	if(isSpecialKey(event) && (event->key() != Qt::Key_Backspace))
		return;
	if(levelPos == 0)
	{
		levelTimer.start();
		levelInProgress=true;
	}
	if((((displayLevel[displayPos] == '\n') && ((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter))) || (((displayLevel[displayPos] != '\n') || ui->spaceNewlineCheckBox->isChecked()) && (event->text() == level[levelPos]))) && !mistake)
	{
		input += event->text();
		displayInput += event->text();
		if(displayLevel[displayPos] == '\n')
		{
			displayInput += "<br>";
			mistakeLabelHtml += "<br>";
			ui->mistakeLabel->setHtml(mistakeLabelHtml);
		}
		else
		{
			if(ignoreMistakeLabelAppend)
				ignoreMistakeLabelAppend=false;
			else
			{
				mistakeLabelHtml += "<span style='color: rgba(0,0,0,0)'>" + event->text() + "</span>";
				ui->mistakeLabel->setHtml(mistakeLabelHtml);
			}
		}
		ui->inputLabel->setHtml(displayInput);
		levelPos++;
		displayPos++;
	}
	else
	{
		if(mistake)
		{
			if(event->key() == Qt::Key_Backspace)
			{
				ui->inputLabel->setHtml(displayInput);
				if(!ignoreMistakeLabelAppend)
				{
					mistakeLabelHtml += "_";
					ui->mistakeLabel->setHtml(mistakeLabelHtml);
				}
				mistake=false;
				ignoreMistakeLabelAppend=true;
			}
		}
		else
		{
			if(!isSpecialKey(event))
			{
				ui->inputLabel->setHtml(displayInput);
				QString errorAppend;
				if(event->text() == " ")
					errorAppend = "_";
				else if((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter))
					errorAppend = "↵<br>";
				else
					errorAppend = event->text();
				ui->inputLabel->setHtml(displayInput + "<span style='color: red';'>" + errorAppend + "</span>");
				levelMistakes++;
				ui->currentMistakesNumber->setText(QString::number(levelMistakes));
				mistake=true;
			}
		}
	}
	ui->inputLabel->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
	if(QStringLen(input) >= QStringLen(level))
	{
		levelInProgress=false;
		lastTime = levelTimer.elapsed()/1000;
		levelSummary msgBox;
		msgBox.setTotalTime(levelTimer.elapsed()/1000);
		msgBox.setHits(levelPos*(60/(levelTimer.elapsed()/1000.0)));
		msgBox.setMistakes(levelMistakes);
		msgBox.setStyleSheet(styleSheet());
		int ret = msgBox.exec();
		if(ret == QDialog::Accepted)
		{
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
			customLevelLoaded=false;
		}
		if(customLevelLoaded)
			levelFinalInit();
		else
			repeatLevel();
	}
}

void OpenTyper::updateCurrentTime(void)
{
	int time;
	if(levelInProgress)
		time = levelTimer.elapsed()/1000;
	else
		time = lastTime;
	ui->currentTimeNumber->setText(QString::number(time));
	setColors();
}

void OpenTyper::setFont(QString fontFamily, int fontSize, bool fontBold, bool fontItalic, bool fontUnderline)
{
	QFont newFont, mistakeLabelFont;
	// Set font
	newFont.setFamily(fontFamily);
	newFont.setPointSize(fontSize);
	newFont.setBold(fontBold);
	newFont.setItalic(fontItalic);
	newFont.setUnderline(fontUnderline);
	// mistakeLabel cannot have underlined font enabled
	mistakeLabelFont = newFont;
	mistakeLabelFont.setUnderline(false);
	// Update labels
	ui->levelLabel->setFont(newFont);
	ui->inputLabel->setFont(newFont);
	ui->mistakeLabel->setFont(mistakeLabelFont);
	// Save settings
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	settings.setValue("theme/font",fontFamily);
	settings.setValue("theme/fontsize",fontSize);
	settings.setValue("theme/fontbold",fontBold);
	settings.setValue("theme/fontitalic",fontItalic);
	settings.setValue("theme/fontunderline",fontUnderline);
}

int OpenTyper::labelWidth(QLabel *targetLabel)
{
	// Checks every line and gets the best label width
	int i, current, max=50, len;
	QString labelText = targetLabel->text();
	QString line = "";
	len = QStringLen(labelText);
	for(i=0; i < len; i++)
	{
		if((labelText[i] == '\n') || (i+1 == len))
		{
			current = targetLabel->fontMetrics().boundingRect(line).width() +
				targetLabel->font().pointSize() * 5;
			if(current > max)
				max = current;
			line = "";
		}
		line += labelText[i];
	}
	return max;
}

void OpenTyper::adjustSize(void)
{
	// Adjust paper width
	int newWidth = labelWidth(ui->levelLabel);
	ui->levelLabel->resize(newWidth,
		ui->levelLabel->height());
	ui->inputLabel->resize(newWidth,
		ui->inputLabel->height());
	ui->paper->setMinimumWidth(newWidth+20);
	// Adjust levelSpace, levelLabel and inputLabel height
	int newHeight = _line_count(displayLevel) *
		(ui->levelLabel->fontMetrics().capHeight()) * 2 + 60;
	ui->levelSpace->setMinimumHeight(newHeight);
	ui->levelLabel->resize(ui->levelLabel->width(),
		newHeight);
	ui->inputLabel->resize(ui->inputLabel->width(),
		newHeight);
}

void OpenTyper::setColors(void)
{
	// Reset style sheets
	ui->inputLabel->setStyleSheet("");
	ui->mainFrame->setStyleSheet("");
	ui->paper->setStyleSheet("");
	// Update theme
	updateTheme();
	char *_styleSheet;
	// Set level text color
	if(customLevelTextColor)
	{
		_styleSheet = (char*) malloc(128);
		sprintf(_styleSheet,"color: rgb(%d, %d, %d)",levelTextRedColor,levelTextGreenColor,levelTextBlueColor);
		ui->levelLabel->setStyleSheet(_styleSheet);
	}
	else
	{
		// Default level text color
		levelTextRedColor = 0;
		levelTextGreenColor = 125;
		levelTextBlueColor = 175;
		ui->levelLabel->setStyleSheet("color: rgb(0, 125, 175)");
	}
	// Set input text color
	if(customInputTextColor)
	{
		_styleSheet = (char*) malloc(128);
		sprintf(_styleSheet,"color: rgb(%d, %d, %d)",inputTextRedColor,inputTextGreenColor,inputTextBlueColor);
		ui->inputLabel->setStyleSheet(_styleSheet);
	}
	else
	{
		// Default input text color
		inputTextRedColor = ui->inputLabel->palette().color(QPalette::Text).red();
		inputTextGreenColor = ui->inputLabel->palette().color(QPalette::Text).green();
		inputTextBlueColor = ui->inputLabel->palette().color(QPalette::Text).blue();
		_styleSheet = (char*) malloc(128);
		sprintf(_styleSheet,"color: rgb(%d, %d, %d)",inputTextRedColor,inputTextGreenColor,inputTextBlueColor);
		ui->inputLabel->setStyleSheet(_styleSheet);
	}
	// Set paper color
	if(customPaperColor)
	{
		_styleSheet = (char*) malloc(128);
		sprintf(_styleSheet,"background-color: rgb(%d, %d, %d)",paperRedColor,paperGreenColor,paperBlueColor);
		ui->paper->setStyleSheet(_styleSheet);
	}
	else
	{
		// Reset background color before setting paper color
		ui->mainFrame->setStyleSheet("");
		// Default paper color
		paperRedColor = ui->paper->palette().color(QPalette::Base).red();
		paperGreenColor = ui->paper->palette().color(QPalette::Base).green();
		paperBlueColor = ui->paper->palette().color(QPalette::Base).blue();
		_styleSheet = (char*) malloc(128);
		sprintf(_styleSheet,"background-color: rgb(%d, %d, %d)",paperRedColor,paperGreenColor,paperBlueColor);
		ui->paper->setStyleSheet(_styleSheet);
		// Fix inputLabel automatically set background color
		ui->inputLabel->setStyleSheet(
			ui->inputLabel->styleSheet() + ";\nbackground-color: rgba(0,0,0,0)");
	}
	// Set panel color
	if(customPanelColor)
	{
		_styleSheet = (char*) malloc(128);
		sprintf(_styleSheet,"QFrame { background-color: rgb(%d, %d, %d); } QCheckBox { background-color: rgb(%d, %d, %d); }",
			panelRedColor,panelGreenColor,panelBlueColor,
			panelRedColor,panelGreenColor,panelBlueColor);
		ui->controlFrame->setStyleSheet(_styleSheet);
	}
	// Set background color
	if(customBgColor)
	{
		_styleSheet = (char*) malloc(128);
		sprintf(_styleSheet,"background-color: rgb(%d, %d, %d)",bgRedColor,bgGreenColor,bgBlueColor);
		ui->mainFrame->setStyleSheet(_styleSheet);
	}
	else
	{
		// Default background color
		bgRedColor = ui->mainFrame->palette().color(QPalette::Window).red();
		bgGreenColor = ui->mainFrame->palette().color(QPalette::Window).green();
		bgBlueColor = ui->mainFrame->palette().color(QPalette::Window).blue();
	}
}

void OpenTyper::updateTheme(void)
{
	QFile darkSheet(":/dark-theme/style.qss");
	QFile lightSheet(":/light-theme/style.qss");
	char *paperStyleSheet, *panelStyleSheet;
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	switch(settings.value("theme/theme","0").toInt()) {
		case 0:
			// System (default)
			setStyleSheet("");
			paperStyleSheet = (char*) malloc(128);
			sprintf(paperStyleSheet,"background-color: rgb(%d,%d,%d)",
				palette().color(QPalette::Base).red(),
				palette().color(QPalette::Base).green(),
				palette().color(QPalette::Base).blue());
			panelStyleSheet = (char*) malloc(128);
			sprintf(panelStyleSheet,"background-color: rgb(%d,%d,%d)",
				palette().color(QPalette::Midlight).red(),
				palette().color(QPalette::Midlight).green(),
				palette().color(QPalette::Midlight).blue());
			ui->paper->setStyleSheet(paperStyleSheet);
			ui->controlFrame->setStyleSheet("QFrame { " + QString(panelStyleSheet) + "; }");
			break;
		case 1:
			// Dark
			if(darkSheet.exists())
			{
				darkSheet.open(QFile::ReadOnly | QFile::Text);
				QTextStream ts(&darkSheet);
				setStyleSheet(ts.readAll());
			}
			else
			{
				printf("D: Failed to open dark style\n");
				darkSheet.open(QFile::ReadOnly | QFile::Text);
				qDebug() << darkSheet.errorString();
			}
			ui->paper->setStyleSheet("background-color: rgb(15, 25, 35)");
			ui->controlFrame->setStyleSheet("QFrame { background-color: rgb(20, 33, 47); }");
			break;
		case 2:
			// Light
			if(lightSheet.exists())
			{
				lightSheet.open(QFile::ReadOnly | QFile::Text);
				QTextStream ts(&lightSheet);
				setStyleSheet(ts.readAll());
			}
			else
			{
				printf("D: Failed to open light style\n");
			}
			ui->paper->setStyleSheet("background-color: rgb(255, 255, 255)");
			ui->controlFrame->setStyleSheet("QFrame { background-color: rgb(255,255,255); }");
			break;
	}
}

void OpenTyper::openPack(void)
{
	QFileDialog openDialog;
	openDialog.setFileMode(QFileDialog::AnyFile);
	openDialog.setNameFilter(tr("Open-Typer pack files") + " (*.typer)" + ";;" + tr("All files") + " (*)");
	if(openDialog.exec())
	{
		// Get selected file
		QString openFileName = openDialog.selectedFiles()[0];
		customConfig=true;
		QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
		settings.setValue("main/customconfig",customConfig);
		loadConfig(openFileName);
		repeatLevel();
	}
}

void OpenTyper::openEditor(void)
{
	// Hide main window
	hide();
	// Open editor
	packEditor editorWindow;
	editorWindow.setWindowFlag(Qt::WindowMinimizeButtonHint,true);
	editorWindow.setWindowFlag(Qt::WindowMaximizeButtonHint,true);
	editorWindow.setStyleSheet(styleSheet());
	editorWindow.init();
	editorWindow.exec();
	// Show main window
	show();
	activateWindow();
}

void OpenTyper::changeLanguage(int index)
{
	QLocale targetLocale;
	if(index == 0)
		targetLocale = QLocale::system();
	else
		targetLocale = QLocale(langMgr->supportedLanguages[index-1],langMgr->supportedCountries[index-1]);
	QCoreApplication::removeTranslator(translator);
	translator = new QTranslator();
	if(translator->load(targetLocale,QLatin1String("Open-Typer"),QLatin1String("_"),QLatin1String(":/res/lang")))
		QCoreApplication::installTranslator(translator);
	ui->retranslateUi(this);
}
