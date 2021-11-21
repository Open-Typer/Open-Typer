/*
 * packview.cpp
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

#include "packEditor/packview.h"
#include "ui_packview.h"

packView::packView(QWidget *parent, int fileID_arg) :
	QWidget(parent),
	ui(new Ui::packView)
{
	ui->setupUi(this);
	// Save button
	connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(save()));
	// Save button
	connect(ui->saveAsButton,SIGNAL(clicked()),this,SLOT(saveAs()));
	// Add buttons
	connect(ui->newLessonButton,SIGNAL(clicked()),this,SLOT(addLesson()));
	connect(ui->newSublessonButton,SIGNAL(clicked()),this,SLOT(addSublesson()));
	connect(ui->newExerciseButton,SIGNAL(clicked()),this,SLOT(addExercise()));
	// Remove button
	connect(ui->removeExerciseButton,SIGNAL(clicked()),this,SLOT(removeExercise()));
	// Lesson options
	connect(ui->lessonDescEdit,SIGNAL(textEdited(const QString)),this,SLOT(changeLessonDesc(const QString)));
	connect(ui->lessonDescRevisionButton,SIGNAL(clicked()),this,SLOT(setRevisionLesson()));
	// Exercise text
	connect(ui->levelTextEdit,SIGNAL(textChanged()),this,SLOT(updateText()));
	// Comboboxes
	connect(ui->lessonSelectionBox,SIGNAL(activated(int)),this,SLOT(switchLesson()));
	connect(ui->sublessonSelectionBox,SIGNAL(activated(int)),this,SLOT(switchSublesson()));
	connect(ui->exerciseSelectionBox,SIGNAL(activated(int)),this,SLOT(switchExercise()));
	connect(ui->repeatingBox,SIGNAL(activated(int)),this,SLOT(changeRepeating(int)));
	// Spin boxes
	connect(ui->repeatLengthBox,SIGNAL(valueChanged(int)),this,SLOT(changeRepeatLength(int)));
	connect(ui->lineLengthBox,SIGNAL(valueChanged(int)),this,SLOT(changeLineLength(int)));
	// Default values
	fileID = fileID_arg;
	newFile=true;
	readOnly=true;
	saved=false;
	skipBoxUpdates=false;
	skipTextUpdates=false;
	skipTextRefresh=false;
	editorWindow = parent;
}

packView::~packView()
{
	delete ui;
}

void packView::openFile(QString path, bool newf, bool rdonly)
{
	saveFileName = path;
	newFile = newf;
	readOnly = rdonly;
	targetFileName = getConfigLoc() + "/editor" + QString::number(fileID) + ".tmp";
	targetFile = fopen(targetFileName.toStdString().c_str(),"w");
	chmod(targetFileName.toStdString().c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if(targetFile == NULL)
	{
		QMessageBox errBox;
		errBox.setText(tr("Write error"));
		errBox.setStyleSheet(styleSheet());
		errBox.exec();
		exit(errno);
	}
	fclose(targetFile);
	if(newFile)
	{
		addLesson();
	}
	else
	{
		QFile::remove(targetFileName);
		QFile::copy(saveFileName,targetFileName);
		chmod(targetFileName.toStdString().c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		refreshUi(false,false,false);
		ui->lessonSelectionBox->setCurrentIndex(0);
		refreshUi(false,false,false);
		ui->sublessonSelectionBox->setCurrentIndex(0);
		refreshUi(false,false,false);
		ui->exerciseSelectionBox->setCurrentIndex(0);
		saved=true;
	}
	refreshUi(false,false,false);
}

bool packView::closeFile(void)
{
	if(!saved)
	{
		QFile saveQFile(saveFileName);
		QFileInfo saveQFileInfo(saveQFile.fileName());
		QString _saveFileName = saveQFileInfo.fileName();
		QMessageBox notSavedBox;
		notSavedBox.setText(tr("Save changes to") + " \"" + _saveFileName + "\" " + tr("before closing?"));
		QPushButton *yesButton = new QPushButton(tr("Save"));
		QPushButton *cancelButton = new QPushButton(tr("Cancel"));
		QPushButton *noButton = new QPushButton(tr("Close without saving"));
		notSavedBox.addButton(yesButton,QMessageBox::YesRole);
		notSavedBox.addButton(cancelButton,QMessageBox::RejectRole);
		notSavedBox.addButton(noButton,QMessageBox::NoRole);
		notSavedBox.setIcon(QMessageBox::Warning);
		notSavedBox.setStyleSheet(styleSheet());
		int ret = notSavedBox.exec();
		switch(ret) {
			case 0:
				save();
				if(!saved)
					return false;
				break;
			case 1:
				return false;
				break;
		}
	}
	QFile::remove(targetFileName);
	return true;
}

void packView::refreshUi(bool newLesson, bool newSublesson, bool newExercise)
{
	updateTabTitle();
	targetFile = fopen(targetFileName.toStdString().c_str(),"rb");
	// Lesson selector
	int oldLesson = ui->lessonSelectionBox->currentIndex();
	int i, count = _lesson_count(targetFile);
	if(newLesson)
		count++;
	ui->lessonSelectionBox->clear();
	QStringList lessons;
	QString _lessonDesc;
	for(i=1; i <= count; i++)
	{
		if(newLesson && (i == count))
			_lessonDesc = "";
		else
			_lessonDesc = parseDesc(_lesson_desc(targetFile,i));
		if(_lessonDesc == "")
			lessons += tr("Lesson") + " " + QString::number(i);
		else
			lessons += tr("Lesson") + " " + QString::number(i) +
				" " + _lessonDesc;
	}
	ui->lessonSelectionBox->addItems(lessons);
	if(newLesson)
		oldLesson = ui->lessonSelectionBox->count()-1;
	ui->lessonSelectionBox->setCurrentIndex(oldLesson);
	// Sublesson selector
	int oldSublesson = ui->sublessonSelectionBox->currentIndex();
	int sublessonCount;
	if(newLesson)
	{
		sublessonCount = 0;
		newSublesson=true;
	}
	else
		sublessonCount = _lesson_sublesson_count(targetFile,oldLesson+1);
	if(newSublesson)
		sublessonCount++;
	ui->sublessonSelectionBox->clear();
	QStringList sublessons;
	int i2=0;
	for(i=1; i <= sublessonCount+i2; i++)
	{
		if(_lesson_sublesson_level_count(targetFile,oldLesson+1,i) > 0)
			sublessons += _sublesson_text(i);
		else
		{
			sublessons += " (" + tr("empty") + ")";
			if(!((newLesson || newSublesson) && (i >= sublessonCount+i2)))
				i2++;
		}
	}
	ui->sublessonSelectionBox->addItems(sublessons);
	if(newSublesson)
		oldSublesson = ui->sublessonSelectionBox->count()-1;
	ui->sublessonSelectionBox->setCurrentIndex(oldSublesson);
	// Exercise selector
	int oldExercise = ui->exerciseSelectionBox->currentIndex();
	int exerciseCount = _lesson_sublesson_level_count(targetFile,oldLesson+1,oldSublesson+1);
	ui->exerciseSelectionBox->clear();
	QStringList exercises;
	for(i=1; i <= exerciseCount; i++)
		exercises += tr("Exercise") + " " + QString::number(i);
	ui->exerciseSelectionBox->addItems(exercises);
	if(newExercise)
		oldExercise = ui->exerciseSelectionBox->count()-1;
	ui->exerciseSelectionBox->setCurrentIndex(oldExercise);
	fclose(targetFile);
	if(newLesson || newSublesson)
		addExercise();
	else
	{
		restoreText();
		targetFile = fopen(targetFileName.toStdString().c_str(),"rb");
		// Lesson description
		QString lessonDesc = "";
		QString rawLessonDesc = _lesson_desc(targetFile,oldLesson+1);
		for(i=0; i < rawLessonDesc.count(); i++)
		{
			if((rawLessonDesc[i] == '%') && (rawLessonDesc[i+1] == 'b'))
			{
				lessonDesc += ' ';
				i++;
			}
			else
				lessonDesc += rawLessonDesc[i];
		}
		ui->lessonDescEdit->setText(lessonDesc);
		// Repeat type
		char *repeatType = _lesson_sublesson_level_repeat_type(targetFile,oldLesson+1,oldSublesson+1,oldExercise+1);
		if((strcmp(repeatType,"w") == 0) || (strcmp(repeatType,"rw") == 0))
			ui->repeatingBox->setCurrentIndex(1);
		else if(strcmp(repeatType,"s") == 0)
			ui->repeatingBox->setCurrentIndex(2);
		else
			ui->repeatingBox->setCurrentIndex(0);
		// Text length
		if(ui->repeatingBox->currentIndex() != 0)
		{
			ui->repeatLengthBox->setEnabled(true);
			skipBoxUpdates=true;
			ui->repeatLengthBox->setValue(_lesson_sublesson_level_limit_extension(targetFile,oldLesson+1,oldSublesson+1,oldExercise+1));
			skipBoxUpdates=false;
		}
		else
			ui->repeatLengthBox->setEnabled(false);
		// Line length
		skipBoxUpdates=true;
		ui->lineLengthBox->setValue(_lesson_sublesson_level_length_extension(targetFile,oldLesson+1,oldSublesson+1,oldExercise+1));
		skipBoxUpdates=false;
		fclose(targetFile);
	}
}

void packView::updateTabTitle(void)
{
	QFile saveQFile(saveFileName);
	QFileInfo saveQFileInfo(saveQFile.fileName());
	QString _saveFileName = saveQFileInfo.fileName();
	if(saved)
		((packEditor*)(editorWindow))->setFileName(_saveFileName,(QWidget*)this);
	else
		((packEditor*)(editorWindow))->setFileName(_saveFileName + "*",(QWidget*)this);
}

void packView::addLesson(void)
{
	refreshUi(true,false,false);
}

void packView::addSublesson(void)
{
	if(ui->lessonSelectionBox->count() == 0)
		return;
	refreshUi(false,true,false);
}

void packView::addExercise(void)
{
	if(ui->sublessonSelectionBox->count() == 0)
		return;
	targetFile = fopen(targetFileName.toStdString().c_str(),"a");
	_add_level(targetFile,
		ui->lessonSelectionBox->currentIndex()+1,
		ui->sublessonSelectionBox->currentIndex()+1,
		ui->exerciseSelectionBox->count()+1,
		false,(char*)"0",120,60,(char*)"",
		tr("New exercise").toStdString().c_str());
	fclose(targetFile);
	saved=false;
	refreshUi(false,false,true);
}

void packView::removeExercise(void)
{
	targetFile = fopen(targetFileName.toStdString().c_str(),"rb");
	int i, targetLesson, targetSublesson, targetLevel;
	targetLesson = ui->lessonSelectionBox->currentIndex()+1;
	targetSublesson = ui->sublessonSelectionBox->currentIndex()+1;
	targetLevel = ui->exerciseSelectionBox->currentIndex()+1;
	int oldCount = _lesson_sublesson_level_count(targetFile,targetLesson,targetSublesson);
	// Delete line
	char *lessonDesc = _lesson_desc(targetFile,targetLesson);
	deleteExerciseLine(targetLesson,targetSublesson,targetLevel);
	// Fix empty space
	for(i = targetLevel; i <= oldCount-1; i++)
	{
		changeExercisePos(lessonDesc,targetLesson,targetSublesson,i+1,targetLesson,targetSublesson,i);
	}
	if(targetLevel == oldCount)
		ui->exerciseSelectionBox->setCurrentIndex(oldCount-2);
	refreshUi(false,false,false);
}

void packView::changeExercisePos(char *lessonDesc,int lesson, int sublesson, int level, int nlesson, int nsublesson, int nlevel)
{
	int limitExt, lengthExt;
	targetFile = fopen(targetFileName.toStdString().c_str(),"r");
	limitExt = _lesson_sublesson_level_limit_extension(targetFile,lesson,sublesson,level);
	lengthExt = _lesson_sublesson_level_length_extension(targetFile,lesson,sublesson,level);
	char *repeatType = _lesson_sublesson_level_repeat_type(targetFile,lesson,sublesson,level);
	if(nlevel != 1)
		lessonDesc = (char*) "";
	char *targetText = _lesson_sublesson_level_raw_text(targetFile,lesson,sublesson,level);
	fclose(targetFile);
	deleteExerciseLine(lesson,sublesson,level);
	targetFile = fopen(targetFileName.toStdString().c_str(),"a");
	bool repeat;
	repeat = !(strcmp(repeatType,"0") == 0);
	_add_level(targetFile,nlesson,nsublesson,nlevel,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText);
	fclose(targetFile);
	saved=false;
}

void packView::deleteExerciseLine(int lesson, int sublesson, int level)
{
	int targetLine, curLine;
	targetFile = fopen(targetFileName.toStdString().c_str(),"r");
	targetLine = _lesson_sublesson_level_line(targetFile,lesson,sublesson,level);
	fclose(targetFile);
	QFile targetQFile(targetFileName);
	if(targetQFile.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		QString out;
		QTextStream stream(&targetQFile);
		curLine=0;
		while(!stream.atEnd())
		{
			curLine++;
			QString line = stream.readLine();
			if(curLine != targetLine)
			{
				out.append(line + "\n");
			}
		}
		targetQFile.resize(0);
		stream << out;
		targetQFile.close();
	}
}

void packView::changeLessonDesc(const QString rawLessonDesc)
{
	char *rawLessonDesc2 = (char*) malloc(rawLessonDesc.count()*2+1);
	strcpy(rawLessonDesc2,rawLessonDesc.toStdString().c_str());
	char *lessonDesc = (char*) malloc(strlen(rawLessonDesc2)*2+1);
	strcpy(lessonDesc,"");
	for(unsigned int i=0; i < strlen(rawLessonDesc2); i++)
	{
		if((rawLessonDesc2[i] == ',') || (rawLessonDesc2[i] == ';') || (rawLessonDesc2[i] == '\\'))
			strcat(lessonDesc,"\\");
		if(rawLessonDesc2[i] == ' ')
			strcat(lessonDesc,"%b");
		else
			strncat(lessonDesc,&rawLessonDesc2[i],1);
	}
	targetFile = fopen(targetFileName.toStdString().c_str(),"r");
	int sublessonCount = _lesson_sublesson_count(targetFile,ui->lessonSelectionBox->currentIndex()+1);
	fclose(targetFile);
	int i, i2=0;
	for(i=1; i <= sublessonCount+i2; i++)
	{
		targetFile = fopen(targetFileName.toStdString().c_str(),"r");
		int levelCount = _lesson_sublesson_level_count(
			targetFile,
			ui->lessonSelectionBox->currentIndex()+1,i);
		fclose(targetFile);
		if(levelCount > 0)
		{
			changeExercisePos(
				lessonDesc,
				ui->lessonSelectionBox->currentIndex()+1,i,1,
				ui->lessonSelectionBox->currentIndex()+1,i,1);
		}
		else
			i2++;
	}
	refreshUi(false,false,false);
}

void packView::setRevisionLesson(void)
{
	changeLessonDesc("%r");
}

void packView::updateText(void)
{
	if(skipTextUpdates)
		return;
	int lesson, sublesson, level, limitExt, lengthExt;
	lesson = ui->lessonSelectionBox->currentIndex()+1;
	sublesson = ui->sublessonSelectionBox->currentIndex()+1;
	level = ui->exerciseSelectionBox->currentIndex()+1;
	targetFile = fopen(targetFileName.toStdString().c_str(),"r");
	limitExt = _lesson_sublesson_level_limit_extension(targetFile,lesson,sublesson,level);
	lengthExt = _lesson_sublesson_level_length_extension(targetFile,lesson,sublesson,level);
	char *repeatType = _lesson_sublesson_level_repeat_type(targetFile,lesson,sublesson,level);
	char *lessonDesc;
	if((sublesson == 1) && (level == 1))
		lessonDesc = _lesson_desc(targetFile,lesson);
	else
		lessonDesc = (char*) "";
	fclose(targetFile);
	deleteExerciseLine(lesson,sublesson,level);
	QString sourceText = ui->levelTextEdit->toPlainText();
	QString targetText = "";
	for(int i=0; i < sourceText.count(); i++)
	{
		if(sourceText[i] == '\n')
			targetText += " ";
		else
			targetText += sourceText[i];
	}
	targetFile = fopen(targetFileName.toStdString().c_str(),"a");
	bool repeat;
	repeat = !(strcmp(repeatType,"0") == 0);
	_add_level(targetFile,lesson,sublesson,level,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText.toStdString().c_str());
	fclose(targetFile);
	saved=false;
	skipTextRefresh=true;
	restoreText();
	skipTextRefresh=false;
	updateTabTitle();
}

void packView::restoreText(void)
{
	targetFile = fopen(targetFileName.toStdString().c_str(),"rb");
	ui->levelLabel->setText(
		_init_level(
			_lesson_sublesson_level_text(
				targetFile,
				ui->lessonSelectionBox->currentIndex()+1,
				ui->sublessonSelectionBox->currentIndex()+1,
				ui->exerciseSelectionBox->currentIndex()+1),
			_lesson_sublesson_level_length_extension(
				targetFile,
				ui->lessonSelectionBox->currentIndex()+1,
				ui->sublessonSelectionBox->currentIndex()+1,
				ui->exerciseSelectionBox->currentIndex()+1)));
	QString textLengthStr = tr("Text length:") + " ";
	if(skipTextRefresh)
	{
		ui->textLengthLabel->setText(textLengthStr + QString::number(ui->levelTextEdit->toPlainText().count()));
		fclose(targetFile);
		return;
	}
	skipTextUpdates=true;
	ui->levelTextEdit->setPlainText(_lesson_sublesson_level_raw_text(
		targetFile,
		ui->lessonSelectionBox->currentIndex()+1,
		ui->sublessonSelectionBox->currentIndex()+1,
		ui->exerciseSelectionBox->currentIndex()+1));
	skipTextUpdates=false;
	ui->textLengthLabel->setText(textLengthStr + QString::number(ui->levelTextEdit->toPlainText().count()));
	fclose(targetFile);
}

void packView::switchLesson(void)
{
	for(int i=0; i < 2; i++)
	{
		ui->sublessonSelectionBox->setCurrentIndex(0);
		ui->exerciseSelectionBox->setCurrentIndex(0);
		refreshUi(false,false,false);
	}
}

void packView::switchSublesson(void)
{
	for(int i=0; i < 2; i++)
	{
		ui->exerciseSelectionBox->setCurrentIndex(0);
		refreshUi(false,false,false);
	}
}

void packView::switchExercise(void)
{
	refreshUi(false,false,false);
}

void packView::changeRepeating(int index)
{
	int lesson, sublesson, level, limitExt, lengthExt;
	lesson = ui->lessonSelectionBox->currentIndex()+1;
	sublesson = ui->sublessonSelectionBox->currentIndex()+1;
	level = ui->exerciseSelectionBox->currentIndex()+1;
	targetFile = fopen(targetFileName.toStdString().c_str(),"r");
	limitExt = _lesson_sublesson_level_limit_extension(targetFile,lesson,sublesson,level);
	lengthExt = _lesson_sublesson_level_length_extension(targetFile,lesson,sublesson,level);
	char *repeatType;
	switch(index) {
		case 1:
			// Words
			if(sublesson == 2)
				repeatType = (char*) "rw";
			else
				repeatType = (char*) "w";
			break;
		// Obsolete
		/*case 2:
			// Strings
			repeatType = (char*) "s";
			break;*/
		default:
			// None
			repeatType = (char*) "0";
			break;
	}
	char *lessonDesc;
	if((sublesson == 1) && (level == 1))
		lessonDesc = _lesson_desc(targetFile,lesson);
	else
		lessonDesc = (char*) "";
	char *targetText = _lesson_sublesson_level_raw_text(targetFile,lesson,sublesson,level);
	fclose(targetFile);
	deleteExerciseLine(lesson,sublesson,level);
	targetFile = fopen(targetFileName.toStdString().c_str(),"a");
	bool repeat;
	repeat = !(strcmp(repeatType,"0") == 0);
	_add_level(targetFile,lesson,sublesson,level,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText);
	fclose(targetFile);
	saved=false;
	refreshUi(false,false,false);
}

void packView::changeRepeatLength(int limitExt)
{
	if(skipBoxUpdates)
		return;
	int lesson, sublesson, level, lengthExt;
	lesson = ui->lessonSelectionBox->currentIndex()+1;
	sublesson = ui->sublessonSelectionBox->currentIndex()+1;
	level = ui->exerciseSelectionBox->currentIndex()+1;
	targetFile = fopen(targetFileName.toStdString().c_str(),"rb");
	lengthExt = _lesson_sublesson_level_length_extension(targetFile,lesson,sublesson,level);
	char *repeatType = _lesson_sublesson_level_repeat_type(targetFile,lesson,sublesson,level);
	char *lessonDesc;
	if((sublesson == 1) && (level == 1))
		lessonDesc = _lesson_desc(targetFile,lesson);
	else
		lessonDesc = (char*) "";
	char *targetText = _lesson_sublesson_level_raw_text(targetFile,lesson,sublesson,level);
	fclose(targetFile);
	deleteExerciseLine(lesson,sublesson,level);
	targetFile = fopen(targetFileName.toStdString().c_str(),"a");
	bool repeat;
	repeat = !(strcmp(repeatType,"0") == 0);
	_add_level(targetFile,lesson,sublesson,level,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText);
	fclose(targetFile);
	saved=false;
	refreshUi(false,false,false);
}

void packView::changeLineLength(int lengthExt)
{
	if(skipBoxUpdates)
		return;
	int lesson, sublesson, level, limitExt;
	lesson = ui->lessonSelectionBox->currentIndex()+1;
	sublesson = ui->sublessonSelectionBox->currentIndex()+1;
	level = ui->exerciseSelectionBox->currentIndex()+1;
	targetFile = fopen(targetFileName.toStdString().c_str(),"rb");
	limitExt = _lesson_sublesson_level_limit_extension(targetFile,lesson,sublesson,level);
	char *repeatType = _lesson_sublesson_level_repeat_type(targetFile,lesson,sublesson,level);
	char *lessonDesc;
	if((sublesson == 1) && (level == 1))
		lessonDesc = _lesson_desc(targetFile,lesson);
	else
		lessonDesc = (char*) "";
	char *targetText = _lesson_sublesson_level_raw_text(targetFile,lesson,sublesson,level);
	fclose(targetFile);
	deleteExerciseLine(lesson,sublesson,level);
	targetFile = fopen(targetFileName.toStdString().c_str(),"a");
	bool repeat;
	repeat = !(strcmp(repeatType,"0") == 0);
	_add_level(targetFile,lesson,sublesson,level,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText);
	fclose(targetFile);
	saved=false;
	refreshUi(false,false,false);
}

void packView::save(void)
{
	if(newFile || readOnly)
		saveAs();
	else
	{
		QFile saveQFile(saveFileName);
		if(saveQFile.exists())
			QFile::remove(saveFileName);
		if(QFile::copy(targetFileName,saveFileName))
		{
			saved=true;
			refreshUi(false,false,false);
		}
		else
			readOnly=true;
	}
}

void packView::saveAs(void)
{
	QFileDialog saveDialog;
	saveDialog.setFileMode(QFileDialog::AnyFile);
	saveDialog.setNameFilter(tr("Open-Typer pack files") + " (*.typer)" + ";;" + tr("All files") + " (*)");
	saveDialog.setAcceptMode(QFileDialog::AcceptSave);
	if(saveDialog.exec())
	{
		// Get selected file
		saveFileName = saveDialog.selectedFiles()[0];
		// Save
		newFile=false;
		readOnly=false;
		save();
	}
}

QString packView::getFileName(void)
{
	return saveFileName;
}
