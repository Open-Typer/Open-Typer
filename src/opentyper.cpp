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
	// Load packs (configs)
	loadConfigs();
	// Read config
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	// Config file (lesson pack) name
	QString configName = settings.value("main/configfile","").toString(); // default: "" (empty QString)
	if(configName == "")
	{
		// No config file selected, use sk_SK-QWERTZ-B1
		// TODO: Change this to en_US after a config for it is added.
		configName = "sk_SK-QWERTZ-B1";
	}
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
	// Theme
	ui->themeBox->setCurrentIndex(settings.value("theme/theme","0").toInt());
	changeTheme(ui->themeBox->currentIndex());
	// Connect signals to slots
	connectAll();
	// Check for updates
	#ifdef Q_OS_WINDOWS
	#ifdef _WIN32
	new Updater();
	#endif
	#endif
	// Select "Training" tab
	ui->tabWidget->setCurrentIndex(1);
	// Load config and start
	char *configPath = loadConfig(configName);
	if(configPath == NULL)
		exit(1);
	currentLesson = 1;
	currentSublesson = 1;
	currentLevel = 1;
	repeatLevel();
}

OpenTyper::~OpenTyper()
{
	delete ui;
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
	// **Lesson packs tab**
	// List of lesson packs
	connect(ui->packList,
		SIGNAL(activated(int)),
		this,
		SLOT(packListIndexChanged(int)));
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
	// Open exercise from file button
	connect(ui->openExerciseButton,
		SIGNAL(clicked()),
		this,
		SLOT(openExerciseFromFile()));
	// **Customize tab**
	// Font selector
	connect(ui->fontComboBox,
		SIGNAL(currentFontChanged(QFont)),
		this,
		SLOT(changeFont(QFont)));
	// Font size box
	connect(ui->fontSizeBox,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(changeFontSize(int)));
	// Bold text button
	connect(ui->boldTextButton,
		SIGNAL(clicked()),
		this,
		SLOT(setBoldText()));
	// Italic text button
	connect(ui->italicTextButton,
		SIGNAL(clicked()),
		this,
		SLOT(setItalicText()));
	// Underline text button
	connect(ui->underlineTextButton,
		SIGNAL(clicked()),
		this,
		SLOT(setUnderlineText()));
	// Reset font button
	connect(ui->fontResetButton,
		SIGNAL(clicked()),
		this,
		SLOT(resetFont()));
	// Change level text color button
	connect(ui->levelTextColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(changeLevelTextColor()));
	// Change input text color button
	connect(ui->inputTextColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(changeInputTextColor()));
	// Reset text color button
	connect(ui->resetTextColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(resetTextColors()));
	// Change background color button
	connect(ui->bgColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(changeBgColor()));
	// Change paper color button
	connect(ui->paperColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(changePaperColor()));
	// Reset background and paper color button
	connect(ui->resetBgPaperColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(resetBgPaperColors()));
	// Theme selector
	connect(ui->themeBox,
		SIGNAL(activated(int)),
		this,
		SLOT(changeTheme(int)));
	// Start timer
	secLoop->start(1000);
}

void OpenTyper::loadConfigs(void)
{
	QDirIterator it(":/res/configs/",QDirIterator::NoIteratorFlags);
	int i;
	QString item, current;
	QStringList items;
	while(it.hasNext())
	{
		item = it.next();
		current = "";
		for(i=14; i < QStringLen(item); i++)
			current += item[i];
		items += current;
	}
	ui->packList->addItems(items);
}

char *OpenTyper::loadConfig(QString configName)
{
	// Returns config file name, which can be opened later.
	QString configLoc = getConfigLoc();
	char *configPath = (char*) malloc(strlen(qPrintable(configLoc))+1+strlen(qPrintable(configName))+1);
	sprintf(configPath,"%s/%s",qPrintable(configLoc),qPrintable(configName));
	// Create config directory if it doesn't exist
	QDir configDir(configLoc);
	if(!configDir.exists())
		configDir.mkpath(configLoc);
	// Extract selected config
	chmod(configPath, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	QFile::remove(configPath);
	QFile::copy(":/res/configs/"+configName,configPath);
	// Open extracted config (just to check that everything is OK and to update lesson list)
	errno=0;
	FILE *configCheckFile = fopen(configPath,"rb");
	if(errno != 0)
	{
		QMessageBox errBox;
		errBox.setText("Failed to read read configuration "+configName+": "+strerror(errno));
		errBox.setStyleSheet(styleSheet());
		errBox.exec();
		// Select default configuration
		configName = "sk_SK-QWERTZ-B1";
		return loadConfig(configName);
	}
	// Update lessonSelectionList widget
	ui->lessonSelectionList->clear();
	QStringList lessons;
	QString _lessonDesc;
	int i, count = _lesson_count(configCheckFile);
	for(i=1; i <= count; i++)
	{
		_lessonDesc = parseDesc(_lesson_desc(configCheckFile,i),tr("Revision"));
		if(_lessonDesc == "")
			lessons += tr("Lesson") + " " + QString::number(i);
		else
			lessons += tr("Lesson") + " " + QString::number(i) +
				" " + _lessonDesc;
	}
	ui->lessonSelectionList->addItems(lessons);
	fclose(configCheckFile);
	// Update packList widget
	int index = ui->packList->findText(configName);
	ui->packList->setCurrentIndex(index);
	// Save selected config to settings
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	settings.setValue("main/configfile",configName);
	ui->trainingPackNameLabel->setText(configName);
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
					sublessons += tr("Fingering");
					break;
				case 2:
					sublessons += tr("Words");
					break;
				case 3:
					sublessons += tr("Sentences");
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
	ui->mistakeLabel->setText("");
	ui->currentTimeNumber->setText("0");
	ui->currentMistakesNumber->setText("0");
	// Init level input
	input = "";
	displayInput = "";
	ui->inputLabel->setTextFormat(Qt::RichText);
	ui->inputLabel->setText(displayInput+"<span style='color: blue; font-family: FreeSans'>|</span>");
}

void OpenTyper::repeatLevel(void)
{
	FILE *cr = fopen(loadConfig(publicConfigName),"rb");
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
void OpenTyper::packListIndexChanged(int index)
{
	char *configPath = loadConfig(ui->packList->itemText(index));
	if(configPath == NULL)
	{
		level = "";
		input = "";
	}
	currentLesson = 1;
	currentSublesson = 1;
	currentLevel = 1;
	repeatLevel();
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
		case Qt::Key_Enter:
			return true;
			break;
		case Qt::Key_Return:
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

void OpenTyper::keyPressEvent(QKeyEvent *event)
{
	if(isSpecialKey(event) && (event->key() != Qt::Key_Backspace))
		return;
	if(levelPos == 0)
	{
		levelTimer.start();
		levelInProgress=true;
	}
	if((event->text() == level[levelPos]) && !mistake)
	{
		input += event->text();
		displayInput += event->text();
		if(displayLevel[displayPos] == '\n')
		{
			displayInput += "<br>";
			ui->mistakeLabel->setText(
				ui->mistakeLabel->text() + "<br>");
		}
		else
		{
			if(ignoreMistakeLabelAppend)
				ignoreMistakeLabelAppend=false;
			else
				ui->mistakeLabel->setText(
					ui->mistakeLabel->text() + "<span style='color: rgba(0,0,0,0)'>" + event->text() + "</span>");
		}
		ui->inputLabel->setText(displayInput+"<span style='color: blue; font-family: FreeSans'>|</span>");
		levelPos++;
		displayPos++;
	}
	else
	{
		if(mistake)
		{
			if(event->key() == Qt::Key_Backspace)
			{
				ui->inputLabel->setText(displayInput+"<span style='color: blue; font-family: FreeSans'>|</span>");
				if(!ignoreMistakeLabelAppend)
					ui->mistakeLabel->setText(
						ui->mistakeLabel->text() + "_");
				mistake=false;
				ignoreMistakeLabelAppend=true;
			}
		}
		else
		{
			if(!isSpecialKey(event))
			{
				ui->inputLabel->setText(displayInput);
				QString errorAppend;
				if(event->text() == " ")
					errorAppend = "_";
				else
					errorAppend = event->text();
				ui->inputLabel->setText(ui->inputLabel->text() + "<span style='color: red';'>" + errorAppend + "</span>" + "<span style='color: blue; font-family: FreeSans'>|</span>");
				levelMistakes++;
				ui->currentMistakesNumber->setText(QString::number(levelMistakes));
				mistake=true;
			}
		}
	}
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

void OpenTyper::changeFont(QFont font)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setFamily(font.family());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	ui->mistakeLabel->setFont(oldFont);
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	settings.setValue("theme/font",font.family());
	adjustSize();
}

void OpenTyper::changeFontSize(int size)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setPointSize(size);
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	ui->mistakeLabel->setFont(oldFont);
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	settings.setValue("theme/fontsize",size);
	adjustSize();
}

void OpenTyper::setBoldText(void)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setBold(ui->boldTextButton->isChecked());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	ui->mistakeLabel->setFont(oldFont);
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	settings.setValue("theme/fontbold",ui->boldTextButton->isChecked());
	adjustSize();
}

void OpenTyper::setItalicText(void)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setItalic(ui->italicTextButton->isChecked());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	ui->mistakeLabel->setFont(oldFont);
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	settings.setValue("theme/fontitalic",ui->italicTextButton->isChecked());
	adjustSize();
}

void OpenTyper::setUnderlineText(void)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setUnderline(ui->underlineTextButton->isChecked());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	settings.setValue("theme/fontunderline",ui->underlineTextButton->isChecked());
	adjustSize();
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
	// Update input widgets
	ui->fontComboBox->setCurrentFont(newFont);
	ui->fontSizeBox->setValue(fontSize);
	ui->boldTextButton->setChecked(fontBold);
	ui->italicTextButton->setChecked(fontItalic);
	ui->underlineTextButton->setChecked(fontUnderline);
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

void OpenTyper::resetFont(void)
{
	// Default font
	setFont("Courier", 	// Family
		14,		// Point size
		false,		// Bold
		false,		// Italic
		false);	// Underline
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

void OpenTyper::saveColorSettings(void)
{
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	// Level text
	settings.setValue("theme/customleveltextcolor",customLevelTextColor);
	settings.setValue("theme/leveltextred",levelTextRedColor);
	settings.setValue("theme/leveltextgreen",levelTextGreenColor);
	settings.setValue("theme/leveltextblue",levelTextBlueColor);
	// Input text
	settings.setValue("theme/custominputtextcolor",customInputTextColor);
	settings.setValue("theme/inputtextred",inputTextRedColor);
	settings.setValue("theme/inputtextgreen",inputTextGreenColor);
	settings.setValue("theme/inputtextblue",inputTextBlueColor);
	// Background
	settings.setValue("theme/custombgcolor",customBgColor);
	settings.setValue("theme/bgred",bgRedColor);
	settings.setValue("theme/bggreen",bgGreenColor);
	settings.setValue("theme/bgblue",bgBlueColor);
	// Paper
	settings.setValue("theme/custompapercolor",customPaperColor);
	settings.setValue("theme/paperred",paperRedColor);
	settings.setValue("theme/papergreen",paperGreenColor);
	settings.setValue("theme/paperblue",paperBlueColor);
}

void OpenTyper::setColors(void)
{
	// Reset style sheets
	ui->inputLabel->setStyleSheet("");
	ui->mainFrame->setStyleSheet("");
	ui->paper->setStyleSheet("");
	// Update theme
	updateTheme();
	char *styleSheet;
	// Set level text color
	if(customLevelTextColor)
	{
		styleSheet = (char*) malloc(128);
		sprintf(styleSheet,"color: rgb(%d, %d, %d)",levelTextRedColor,levelTextGreenColor,levelTextBlueColor);
		ui->levelLabel->setStyleSheet(styleSheet);
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
		styleSheet = (char*) malloc(128);
		sprintf(styleSheet,"color: rgb(%d, %d, %d)",inputTextRedColor,inputTextGreenColor,inputTextBlueColor);
		ui->inputLabel->setStyleSheet(styleSheet);
	}
	else
	{
		// Default input text color
		inputTextRedColor = ui->inputLabel->palette().color(QPalette::Text).red();
		inputTextGreenColor = ui->inputLabel->palette().color(QPalette::Text).green();
		inputTextBlueColor = ui->inputLabel->palette().color(QPalette::Text).blue();
		styleSheet = (char*) malloc(128);
		sprintf(styleSheet,"color: rgb(%d, %d, %d)",inputTextRedColor,inputTextGreenColor,inputTextBlueColor);
		ui->inputLabel->setStyleSheet(styleSheet);
	}
	// Set paper color
	if(customPaperColor)
	{
		styleSheet = (char*) malloc(128);
		sprintf(styleSheet,"background-color: rgb(%d, %d, %d)",paperRedColor,paperGreenColor,paperBlueColor);
		ui->paper->setStyleSheet(styleSheet);
	}
	else
	{
		// Reset background color before setting paper color
		ui->mainFrame->setStyleSheet("");
		// Default paper color
		paperRedColor = ui->paper->palette().color(QPalette::Base).red();
		paperGreenColor = ui->paper->palette().color(QPalette::Base).green();
		paperBlueColor = ui->paper->palette().color(QPalette::Base).blue();
		styleSheet = (char*) malloc(128);
		sprintf(styleSheet,"background-color: rgb(%d, %d, %d)",paperRedColor,paperGreenColor,paperBlueColor);
		ui->paper->setStyleSheet(styleSheet);
		// Fix inputLabel automatically set background color
		ui->inputLabel->setStyleSheet(
			ui->inputLabel->styleSheet() + ";\nbackground-color: rgba(0,0,0,0)");
	}
	// Set background color
	if(customBgColor)
	{
		styleSheet = (char*) malloc(128);
		sprintf(styleSheet,"background-color: rgb(%d, %d, %d)",bgRedColor,bgGreenColor,bgBlueColor);
		ui->mainFrame->setStyleSheet(styleSheet);
	}
	else
	{
		// Default background color
		bgRedColor = ui->mainFrame->palette().color(QPalette::Window).red();
		bgGreenColor = ui->mainFrame->palette().color(QPalette::Window).green();
		bgBlueColor = ui->mainFrame->palette().color(QPalette::Window).blue();
	}
}
void OpenTyper::changeLevelTextColor(void)
{
	SimpleColorDialog colorDialog;
	colorDialog.setColor(levelTextRedColor,
		levelTextGreenColor,
		levelTextBlueColor);
	colorDialog.setStyleSheet(styleSheet());
	if(colorDialog.exec() == QDialog::Accepted)
	{
		levelTextRedColor = colorDialog.redColor;
		levelTextGreenColor = colorDialog.greenColor;
		levelTextBlueColor = colorDialog.blueColor;
		customLevelTextColor = true;
		saveColorSettings();
		setColors();
	}
}

void OpenTyper::changeInputTextColor(void)
{
	SimpleColorDialog colorDialog;
	colorDialog.setColor(inputTextRedColor,
		inputTextGreenColor,
		inputTextBlueColor);
	colorDialog.setStyleSheet(styleSheet());
	if(colorDialog.exec() == QDialog::Accepted)
	{
		inputTextRedColor = colorDialog.redColor;
		inputTextGreenColor = colorDialog.greenColor;
		inputTextBlueColor = colorDialog.blueColor;
		customInputTextColor = true;
		saveColorSettings();
		setColors();
	}
}

void OpenTyper::resetTextColors(void)
{
	// There's no need to set RGB values because they're defined in setColors()
	customLevelTextColor = false;
	customInputTextColor = false;
	saveColorSettings();
	setColors();
}

void OpenTyper::changeBgColor(void)
{
	SimpleColorDialog colorDialog;
	colorDialog.setColor(bgRedColor,
		bgGreenColor,
		bgBlueColor);
	colorDialog.setStyleSheet(styleSheet());
	if(colorDialog.exec() == QDialog::Accepted)
	{
		bgRedColor = colorDialog.redColor;
		bgGreenColor = colorDialog.greenColor;
		bgBlueColor = colorDialog.blueColor;
		customBgColor = true;
		saveColorSettings();
		setColors();
	}
}

void OpenTyper::changePaperColor(void)
{
	SimpleColorDialog colorDialog;
	colorDialog.setColor(paperRedColor,
		paperGreenColor,
		paperBlueColor);
	colorDialog.setStyleSheet(styleSheet());
	if(colorDialog.exec() == QDialog::Accepted)
	{
		paperRedColor = colorDialog.redColor;
		paperGreenColor = colorDialog.greenColor;
		paperBlueColor = colorDialog.blueColor;
		customPaperColor = true;
		saveColorSettings();
		setColors();
	}
}

void OpenTyper::resetBgPaperColors(void)
{
	// There's no need to set RGB values because they're defined in setColors()
	customBgColor = false;
	customPaperColor = false;
	saveColorSettings();
	setColors();
}

void OpenTyper::changeTheme(int index)
{
	updateTheme();
	setColors();
	QSettings settings(getConfigLoc()+"/config.ini",QSettings::IniFormat);
	settings.setValue("theme/theme",index);
}
void OpenTyper::updateTheme(void)
{
	QFile darkSheet(":/dark-theme/style.qss");
	QFile lightSheet(":/light-theme/style.qss");
	char *paperStyleSheet;
	switch(ui->themeBox->currentIndex()) {
		case 0:
			// System (default)
			setStyleSheet("");
			paperStyleSheet = (char*) malloc(128);
			sprintf(paperStyleSheet,"background-color: rgb(%d,%d,%d)",
				palette().color(QPalette::Base).red(),
				palette().color(QPalette::Base).green(),
				palette().color(QPalette::Base).blue());
			ui->paper->setStyleSheet(paperStyleSheet);
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
			break;
	}
}
