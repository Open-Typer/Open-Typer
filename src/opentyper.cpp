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

#include <QLabel>
#include <QKeyEvent>
#include <QString>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QDialog>
#include <QDirIterator>
#include <QTimer>
#ifdef _WIN32
#include <windows.h>
#endif
#include "opentyper.h"
#include "ui_opentyper.h"
#include "levelsummary.h"
#include "configfile.h"
#include "utils.h"

#ifdef _WIN32
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

void OpenTyper::updateColors(void)
{
	// Set paper color
	QPalette paperPalette = ui->paper->palette();
	paperPalette.setColor(QPalette::Window,paperPalette.color(paperPalette.Base));
	ui->paper->setPalette(paperPalette);
}

OpenTyper::OpenTyper(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::OpenTyper)
{
	ui->setupUi(this);
	updateColors();
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
	// Create timer (used to update currentTimeNumber every second)
	QTimer *secLoop = new QTimer(this);
	// Connect signals to slots
	connect(secLoop,SIGNAL(timeout()),this,SLOT(updateCurrentTime()));
	connect(ui->packList,SIGNAL(activated(int)),this,SLOT(packListIndexChanged(int)));
	connect(ui->repeatButton,SIGNAL(clicked()),this,SLOT(repeatLevel()));
	connect(ui->nextButton,SIGNAL(clicked()),this,SLOT(nextLevel()));
	connect(ui->previousButton,SIGNAL(clicked()),this,SLOT(previousLevel()));
	connect(ui->lessonSelectionList,SIGNAL(activated(int)),this,SLOT(lessonSelectionListIndexChanged(int)));
	connect(ui->sublessonSelectionList,SIGNAL(activated(int)),this,SLOT(sublessonSelectionListIndexChanged(int)));
	connect(ui->levelSelectionList,SIGNAL(activated(int)),this,SLOT(levelSelectionListIndexChanged(int)));
	connect(ui->randomOrderCheckBox,SIGNAL(clicked(bool)),this,SLOT(randomOrderCheckBoxChanged(bool)));
	// Start timer
	secLoop->start(1000);
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

QString OpenTyper::getConfigLoc(void)
{
	return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Open-Typer";
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
	#ifdef _WIN32
	qt_ntfs_permission_lookup++;
	DeleteFileA(qPrintable(configPath));
	#endif
	QFile::remove(configPath);
	QFile::copy(":/res/configs/"+configName,configPath);
	// Open extracted config (just to check that everything is OK and to update lesson list)
	errno=0;
	FILE *configCheckFile = fopen(configPath,"rb");
	if(errno != 0)
	{
		QMessageBox errBox;
		errBox.setText("Failed to read read configuration "+configName+": "+strerror(errno));
		errBox.exec();
		// Select default configuration
		configName = "sk_SK-QWERTZ-B1";
		return loadConfig(configName);
	}
	// Update lessonSelectionList widget
	ui->lessonSelectionList->clear();
	QStringList lessons;
	int i, count = _lesson_count(configCheckFile);
	for(i=1; i <= count; i++)
		lessons += tr("Lesson") + " " + QString::number(i);
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

QString OpenTyper::_init_level(QString level)
{
	int len, len2, i, line_pos=0;
	QString out = "";
	len = QStringLen(level);
	QString word = "";
	bool first_word=true;
	for(i=0; i < len; i++)
	{
		if((level[i] == ' ') || (i+1 >= len))
		{
			if(i+1 >= len)
			{
				word += level[i];
			}
			len2 = QStringLen(word);
			if(line_pos+len2 > _REPEAT_LIMIT*levelLengthExtension)
			{
				out += '\n';
				line_pos=0;
			}
			if(!first_word && (line_pos > 0))
			{
				out += ' ';
			}
			first_word=false;
			out += word;
			word = "";
			if(!first_word)
				line_pos++;
			line_pos=line_pos+len2;
		}
		else
		{
			word += level[i];
		}
	}
	return out;
}

void OpenTyper::startLevel(FILE *cr, int lessonID, int sublessonID, int levelID)
{
	// Update selected lesson
	ui->lessonSelectionList->setCurrentIndex(lessonID-1);
	// Check if -1 (last sublesson in current lesson) was passed
	if(sublessonID == -1)
		sublessonID = _lesson_sublesson_count(cr,lessonID);
	// Check if -1 (last level in current sublesson) was passed
	if(levelID == -1)
		levelID = _lesson_sublesson_level_count(cr,lessonID,sublessonID);
	// Get sublesson count
	sublessonCount = _lesson_sublesson_count(cr,lessonID);
	// Update sublesson list
	// This must happen before level loading!
	ui->sublessonSelectionList->clear();
	QStringList sublessons;
	for(int i=1; i <= sublessonCount; i++)
	{
		switch(i) {
			case 1:
				sublessons += tr("Finger-key-combinations");
				break;
			case 2:
				sublessons += tr("Words");
				break;
			case 3:
				sublessons += tr("Sentences");
				break;
			default:
				sublessons += tr("Sublesson") + " " + QString::number(i);
				
		}
	}
	ui->sublessonSelectionList->addItems(sublessons);
	ui->sublessonSelectionList->setCurrentIndex(sublessonID-1);
	// Update random order check box
	if(sublessonID == 2) // words
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
	levelLengthExtension = _lesson_sublesson_level_length_extension(cr,lessonID,sublessonID,levelID);
	// Load level text
	level = _lesson_sublesson_level_text(cr,
		lessonID,
		sublessonID,
		levelID,
		ui->randomOrderCheckBox->isChecked());
	displayLevel = _init_level(level);
	ui->levelLabel->setText(displayLevel);
	// Adjust paper width
	QString longString = "";
	for(int i=0; i < _REPEAT_LIMIT*levelLengthExtension; i++)
		longString += ' ';
	int newWidth = ui->levelLabel->fontMetrics().boundingRect(longString).width() +
		(ui->levelLabel->font().pointSize()) * 5;
	ui->levelLabel->resize(newWidth,
		ui->levelLabel->height());
	ui->inputLabel->resize(newWidth,
		ui->inputLabel->height());
	ui->paper->setMinimumWidth(newWidth+20);
	// Init level input
	input = "";
	displayInput = "";
	ui->inputLabel->setTextFormat(Qt::RichText);
	ui->inputLabel->setText(displayInput+"<span style='color: blue';'>|</span>");
	// Get lesson count
	lessonCount = _lesson_count(cr);
	// Get level count (in current lesson)
	levelCount = _lesson_sublesson_level_count(cr,lessonID,sublessonID);
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
	levelPos=0;
	displayPos=0;
	levelMistakes=0;
	levelInProgress=false;
	lastTime=0;
	mistake=false;
	ui->currentTimeNumber->setText("0");
	ui->currentMistakesNumber->setText("0");
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
	if(event->key() == Qt::Key_Shift)
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
		}
		ui->inputLabel->setText(displayInput+"<span style='color: blue';'>|</span>");
		levelPos++;
		displayPos++;
	}
	else
	{
		if(mistake)
		{
			if(event->key() == Qt::Key_Backspace)
			{
				ui->inputLabel->setText(displayInput+"<span style='color: blue';'>|</span>");
				mistake=false;
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
				ui->inputLabel->setText(ui->inputLabel->text() + "<span style='color: red';'>" + errorAppend + "</span>" + "<span style='color: blue';'>|</span>");
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
		}
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
	updateColors();
}
