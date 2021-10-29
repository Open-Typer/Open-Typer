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
	// Text buttons
	connect(ui->saveTextButton,SIGNAL(clicked()),this,SLOT(updateText()));
	connect(ui->restoreTextButton,SIGNAL(clicked()),this,SLOT(restoreText()));
	// Comboboxes
	connect(ui->lessonSelectionBox,SIGNAL(activated(int)),this,SLOT(switchLesson()));
	connect(ui->sublessonSelectionBox,SIGNAL(activated(int)),this,SLOT(switchSublesson()));
	connect(ui->exerciseSelectionBox,SIGNAL(activated(int)),this,SLOT(switchExercise()));
	// Default values
	fileID = fileID_arg;
	newFile=true;
	readOnly=true;
	saved=false;
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
	targetFile = fopen(qPrintable(targetFileName),"w");
	chmod(qPrintable(targetFileName), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
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
		chmod(qPrintable(targetFileName), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
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
	QFile saveQFile(saveFileName);
	QFileInfo saveQFileInfo(saveQFile.fileName());
	QString _saveFileName = saveQFileInfo.fileName();
	if(saved)
		((packEditor*)(editorWindow))->setFileName(_saveFileName,(QWidget*)this);
	else
		((packEditor*)(editorWindow))->setFileName(_saveFileName + "*",(QWidget*)this);
	targetFile = fopen(qPrintable(targetFileName),"rb");
	((packEditor*)(editorWindow))->fixDuplicates();
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
			_lessonDesc = parseDesc(_lesson_desc(targetFile,i),tr("Revision"));
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
			sublessons += " (" + tr("empty") + ")";
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
	}
}

void packView::addLesson(void)
{
	refreshUi(true,false,false);
}

void packView::addSublesson(void)
{
	refreshUi(false,true,false);
}

void packView::addExercise(void)
{
	targetFile = fopen(qPrintable(targetFileName),"a");
	_add_level(targetFile,
		ui->lessonSelectionBox->currentIndex()+1,
		ui->sublessonSelectionBox->currentIndex()+1,
		ui->exerciseSelectionBox->count()+1,
		false,(char*)"0",120,60,(char*)"",
		qPrintable(tr("New exercise")));
	fclose(targetFile);
	saved=false;
	refreshUi(false,false,true);
}

void packView::updateText(void)
{
	int lesson, sublesson, level, targetLine, curLine, limitExt, lengthExt;
	lesson = ui->lessonSelectionBox->currentIndex()+1;
	sublesson = ui->sublessonSelectionBox->currentIndex()+1;
	level = ui->exerciseSelectionBox->currentIndex()+1;
	targetFile = fopen(qPrintable(targetFileName),"r");
	targetLine = _lesson_sublesson_level_line(targetFile,lesson,sublesson,level);
	limitExt = _lesson_sublesson_level_limit_extension(targetFile,lesson,sublesson,level);
	lengthExt = _lesson_sublesson_level_length_extension(targetFile,lesson,sublesson,level);
	char *repeatType = _lesson_sublesson_level_repeat_type(targetFile,lesson,sublesson,level);
	char *lessonDesc;
	if((sublesson == 1) && (level == 1))
		lessonDesc = _lesson_desc(targetFile,lesson);
	else
		lessonDesc = (char*) "";
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
		QString sourceText = ui->levelTextEdit->toPlainText();
		QString targetText = "";
		for(int i=0; i < QStringLen(sourceText); i++)
		{
			if(sourceText[i] == '\n')
				targetText += " ";
			else
				targetText += sourceText[i];
		}
		targetFile = fopen(qPrintable(targetFileName),"a");
		bool repeat;
		repeat = !(strcmp(repeatType,"0") == 0);
		_add_level(targetFile,lesson,sublesson,level,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText.toStdString().c_str());
		fclose(targetFile);
		saved=false;
		refreshUi(false,false,false);
	}
}

void packView::restoreText(void)
{
	targetFile = fopen(qPrintable(targetFileName),"rb");
	ui->levelLabel->setText(
		_init_level(
			_lesson_sublesson_level_text(
				targetFile,
				ui->lessonSelectionBox->currentIndex()+1,
				ui->sublessonSelectionBox->currentIndex()+1,
				ui->exerciseSelectionBox->currentIndex()+1,
				false),
			60));
	ui->levelTextEdit->setPlainText(ui->levelLabel->text());
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

void packView::save(void)
{
	if(newFile || readOnly)
		saveAs();
	else
	{
		QFile saveFile(saveFileName);
		if(saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QFile targetQFile(targetFileName);
			if(targetQFile.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QTextStream stream(&targetQFile);
				while(!stream.atEnd())
				{
					QString line = stream.readLine();
					saveFile.write(qPrintable(line + "\n"));
				}
				saved=true;
				refreshUi(false,false,false);
			}
			else
				readOnly=true;
			targetQFile.close();
		}
		else
			readOnly=true;
		saveFile.close();
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
