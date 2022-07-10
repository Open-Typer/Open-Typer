/*
 * packeditor.cpp
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

#include "packEditor/packeditor.h"
#include "ui_packeditor.h"

/*! Constructs packEditor and creates a new file. */
packEditor::packEditor(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::packEditor)
{
	ui->setupUi(this);
	// File menu
	connect(ui->newFileAction, &QAction::triggered, this, [this]() { closeFile(true); }); // closeFile() will create a new file because createNew = true
	connect(ui->openFileAction, &QAction::triggered, this, [this]() { closeFile(false, true); }); // closeFile() will open the file because open = true
	connect(ui->openPrebuiltAction, &QAction::triggered, this, [this]() { closeFile(false, false, true); }); // closeFile() will open the file because openPrebuiltPack = true
	connect(ui->saveAction, &QAction::triggered, this, &packEditor::save);
	connect(ui->saveAsAction, &QAction::triggered, this, &packEditor::saveAs);
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
	createNewFile();
	skipBoxUpdates=false;
	skipTextUpdates=false;
	skipTextRefresh=false;
	editorWindow = parent;
}

/*! Destroys the packEditor object. */
packEditor::~packEditor()
{
	delete ui;
}

/*! Creates a new file. */
void packEditor::createNewFile(void)
{
	openFile(tr("Unnamed") + ".typer", true, true);
	saved = false;
}

/*! Opens a built-in pack. \see packSelector */
void packEditor::openPrebuilt(void)
{
	packSelector *packSel = new packSelector(this);
	connect(packSel, &QDialog::accepted, this, [packSel,this]() {
		openFile(":res/configs/" + packSel->selectedConfig(), false, true);
	});
	packSel->open();
}

/*! Opens a file selected by the user. */
void packEditor::openFile(void)
{
#ifdef Q_OS_WASM
	auto readOpenedFile = [this](const QString &fileName, const QByteArray &fileContent) {
		if(!fileName.isEmpty())
		{
			saveFileName = fileName;
			newFile = false;
			readOnly = false;
			parser.loadToBuffer(fileContent);
			refreshUi(false,false,false);
			ui->lessonSelectionBox->setCurrentIndex(0);
			refreshUi(false,false,false);
			ui->sublessonSelectionBox->setCurrentIndex(0);
			refreshUi(false,false,false);
			ui->exerciseSelectionBox->setCurrentIndex(0);
			saved = true;
		}
	};
	QFileDialog::getOpenFileContent("Open-Typer pack (*.typer)", readOpenedFile);
#else
	QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), "Open-Typer pack (*.typer)");
	if(!fileName.isEmpty())
		openFile(fileName, false, false);
#endif // Q_OS_WASM
}

/*!
 * Opens a file.
 * \param[in] path File name.
 * \param[in] newf Whether to create a new file.
 * \param[in] rdonly Whether to open the file read-only.
 */
void packEditor::openFile(QString path, bool newf, bool rdonly)
{
	saveFileName = path;
	newFile = newf;
	readOnly = rdonly;
	if(newFile)
	{
		// Clear buffer
		parser.loadToBuffer("");
		addLesson();
	}
	else
	{
		// Load existing data into buffer
		QFile openedFile(path);
		openedFile.open(QFile::ReadOnly | QFile::Text);
		parser.loadToBuffer(openedFile.readAll());
		openedFile.close();
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

/*! Closes opened file. */
void packEditor::closeFile(bool createNew, bool open, bool openPrebuiltPack)
{
	if(saved)
	{
		if(createNew)
			createNewFile();
		else if(open)
			openFile();
		else if(openPrebuiltPack)
			openPrebuilt();
		else
			accept();
	}
	else
	{
		QFile saveQFile(saveFileName);
		QFileInfo saveQFileInfo(saveQFile.fileName());
		QString _saveFileName = saveQFileInfo.fileName();
		QMessageBox *notSavedBox = new QMessageBox(this);
		notSavedBox->setText(tr("Save changes to") + " \"" + _saveFileName + "\" " + tr("before closing?"));
		QPushButton *yesButton = new QPushButton(tr("Save"));
		QPushButton *cancelButton = new QPushButton(tr("Cancel"));
		QPushButton *noButton = new QPushButton(tr("Close without saving"));
		notSavedBox->addButton(yesButton,QMessageBox::YesRole);
		notSavedBox->addButton(cancelButton,QMessageBox::RejectRole);
		notSavedBox->addButton(noButton,QMessageBox::NoRole);
		notSavedBox->setIcon(QMessageBox::Warning);
		notSavedBox->setWindowModality(Qt::WindowModal);
		connect(notSavedBox, &QDialog::finished, this, [notSavedBox, yesButton, cancelButton, createNew, open, openPrebuiltPack, this]() {
			if(notSavedBox->clickedButton() == yesButton)
				save();
			if(notSavedBox->clickedButton() != cancelButton)
			{
				parser.close();
				if(createNew)
					createNewFile();
				else if(open)
					openFile();
				else if(openPrebuiltPack)
					openPrebuilt();
				else
					accept();
			}
		});
		notSavedBox->open();
	}
}

/*!
 * Updates the title, loads selected exercise and refreshes widgets.
 *
 * \param[in] newLesson Whether to add a new lesson.
 * \param[in] newSublesson Whether to add a new sublesson.
 * \param[in] newExercise Whether to add a new exercise.
 *
 * \see updateTitle()
 * \see addLesson()
 * \see addSublesson()
 * \see addExercise()
 * \see configParser
 */
void packEditor::refreshUi(bool newLesson, bool newSublesson, bool newExercise)
{
	updateTitle();
	// Lesson selector
	int oldLesson = ui->lessonSelectionBox->currentIndex();
	int i, count = parser.lessonCount();
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
			_lessonDesc = configParser::parseDesc(parser.lessonDesc(i));
		if(_lessonDesc == "")
			lessons += configParser::lessonTr(i);
		else
			lessons += configParser::lessonTr(i) + " " + _lessonDesc;
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
		sublessonCount = parser.sublessonCount(oldLesson+1);
	if(newSublesson)
		sublessonCount++;
	ui->sublessonSelectionBox->clear();
	QStringList sublessons;
	int i2=0;
	for(i=1; i <= sublessonCount+i2; i++)
	{
		if(parser.exerciseCount(oldLesson+1,i) > 0)
			sublessons += configParser::sublessonName(i);
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
	int exerciseCount = parser.exerciseCount(oldLesson+1,oldSublesson+1);
	ui->exerciseSelectionBox->clear();
	QStringList exercises;
	for(i=1; i <= exerciseCount; i++)
		exercises += configParser::exerciseTr(i);
	ui->exerciseSelectionBox->addItems(exercises);
	if(newExercise)
		oldExercise = ui->exerciseSelectionBox->count()-1;
	ui->exerciseSelectionBox->setCurrentIndex(oldExercise);
	if(newLesson || newSublesson)
		addExercise();
	else
	{
		restoreText();
		// Lesson description
		QString lessonDesc = "";
		QString rawLessonDesc = parser.lessonDesc(oldLesson+1);
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
		QString repeatType = parser.exerciseRepeatType(oldLesson+1,oldSublesson+1,oldExercise+1);
		if((repeatType == "w") || (repeatType == "rw")) // Don't remove "rw", it's for old packs
			ui->repeatingBox->setCurrentIndex(1);
		else if(repeatType == "s")
			ui->repeatingBox->setCurrentIndex(2);
		else
			ui->repeatingBox->setCurrentIndex(0);
		// Text length
		if(ui->repeatingBox->currentIndex() != 0)
		{
			ui->repeatLengthBox->setEnabled(true);
			skipBoxUpdates=true;
			ui->repeatLengthBox->setValue(parser.exerciseRepeatLimit(oldLesson+1,oldSublesson+1,oldExercise+1));
			skipBoxUpdates=false;
		}
		else
			ui->repeatLengthBox->setEnabled(false);
		// Line length
		skipBoxUpdates=true;
		ui->lineLengthBox->setValue(parser.exerciseLineLength(oldLesson+1,oldSublesson+1,oldExercise+1));
		skipBoxUpdates=false;
	}
}

/*! Updates the title. */
void packEditor::updateTitle(void)
{
	QFile saveQFile(saveFileName);
	QFileInfo saveQFileInfo(saveQFile.fileName());
	QString _saveFileName = saveQFileInfo.fileName();
	ui->title->setText(saved ? _saveFileName : _saveFileName + "*");
}

/*!
 * Connected from newLessonButton->clicked().\n
 * Adds a new lesson.
 *
 * \see addSublesson()
 * \see addExercise()
 */
void packEditor::addLesson(void)
{
	refreshUi(true,false,false);
}

/*!
 * Connected from newSublessonButton->clicked().\n
 * Adds a new sublesson.
 *
 * \see addLesson()
 * \see addExercise()
 */
void packEditor::addSublesson(void)
{
	if(ui->lessonSelectionBox->count() == 0)
		return;
	refreshUi(false,true,false);
}

/*!
 * Connected from newExerciseButton->clicked().\n
 * Adds a new exercise.
 *
 * \see addLesson()
 * \see addSublesson()
 */
void packEditor::addExercise(void)
{
	if(ui->sublessonSelectionBox->count() == 0)
		return;
	parser.addExercise(ui->lessonSelectionBox->currentIndex()+1,
		ui->sublessonSelectionBox->currentIndex()+1,
		ui->exerciseSelectionBox->count()+1,
		false,"0",120,60,"",
		tr("New exercise"));
	saved=false;
	refreshUi(false,false,true);
}

/*!
 * Connected from removeExerciseButton->clicked().\n
 * Removes selected exercise.
 */
void packEditor::removeExercise(void)
{
	int i, targetLesson, targetSublesson, targetLevel;
	targetLesson = ui->lessonSelectionBox->currentIndex()+1;
	targetSublesson = ui->sublessonSelectionBox->currentIndex()+1;
	targetLevel = ui->exerciseSelectionBox->currentIndex()+1;
	int oldCount = parser.exerciseCount(targetLesson,targetSublesson);
	// Delete line
	QString lessonDesc = parser.lessonDesc(targetLesson);
	deleteExerciseLine(targetLesson,targetSublesson,targetLevel);
	// Fix empty space
	for(i = targetLevel; i <= oldCount-1; i++)
		changeExercisePos(lessonDesc,targetLesson,targetSublesson,i+1,targetLesson,targetSublesson,i);
	if(targetLevel == oldCount)
		ui->exerciseSelectionBox->setCurrentIndex(oldCount-2);
	refreshUi(false,false,false);
}

/*! Moves exercise to a new position (lesson, sublesson, exercise). */
void packEditor::changeExercisePos(QString lessonDesc, int lesson, int sublesson, int level, int nlesson, int nsublesson, int nlevel)
{
	int limitExt, lengthExt;
	limitExt = parser.exerciseRepeatLimit(lesson,sublesson,level);
	lengthExt = parser.exerciseLineLength(lesson,sublesson,level);
	QString repeatType = parser.exerciseRepeatType(lesson,sublesson,level);
	if(nlevel != 1)
		lessonDesc = "";
	QString targetText = parser.exerciseRawText(lesson,sublesson,level);
	deleteExerciseLine(lesson,sublesson,level);
	bool repeat;
	repeat = (repeatType != "0");
	parser.addExercise(nlesson,nsublesson,nlevel,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText);
	saved=false;
}

/*! Removes an exercise from the temporary file. */
void packEditor::deleteExerciseLine(int lesson, int sublesson, int level)
{
	int targetLine, curLine;
	targetLine = parser.exerciseLine(lesson,sublesson,level);
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write(parser.data());
	QByteArray out;
	buffer.seek(0);
	curLine=0;
	while(!buffer.atEnd())
	{
		curLine++;
		QByteArray line = buffer.readLine();
		if(curLine != targetLine)
			out += line;
	}
	buffer.close();
	parser.loadToBuffer(out);
}

/*!
 * Connected from lessonDescEdit->textEdited().\n
 * Changes lesson description based on the value in lessonDescEdit.
 */
void packEditor::changeLessonDesc(const QString rawLessonDesc)
{
	QString lessonDesc = "";
	for(int i=0; i < rawLessonDesc.count(); i++)
	{
		if((rawLessonDesc[i] == ',') || (rawLessonDesc[i] == ';') || (rawLessonDesc[i] == '\\'))
			lessonDesc += "\\";
		if(rawLessonDesc[i] == ' ')
			lessonDesc += "%b";
		else
			lessonDesc += rawLessonDesc[i];
	}
	int sublessonCount = parser.sublessonCount(ui->lessonSelectionBox->currentIndex()+1);
	int i, i2=0;
	for(i=1; i <= sublessonCount+i2; i++)
	{
		int levelCount = parser.exerciseCount(ui->lessonSelectionBox->currentIndex()+1,i);
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

/*!
 * Connected from lessonDescRevisionButton->clicked()
 * Marks current lesson as a revision lesson.
 * \see changeLessonDesc()
 */
void packEditor::setRevisionLesson(void)
{
	changeLessonDesc("%r");
}

/*!
 * Connected from levelTextEdit->textChanged().\n
 * Updates exercise text.
 */
void packEditor::updateText(void)
{
	if(skipTextUpdates)
		return;
	int lesson, sublesson, level, limitExt, lengthExt;
	lesson = ui->lessonSelectionBox->currentIndex()+1;
	sublesson = ui->sublessonSelectionBox->currentIndex()+1;
	level = ui->exerciseSelectionBox->currentIndex()+1;
	limitExt = parser.exerciseRepeatLimit(lesson,sublesson,level);
	lengthExt = parser.exerciseLineLength(lesson,sublesson,level);
	QString repeatType = parser.exerciseRepeatType(lesson,sublesson,level);
	QString lessonDesc;
	if((sublesson == 1) && (level == 1))
		lessonDesc = parser.lessonDesc(lesson);
	else
		lessonDesc = "";
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
	bool repeat;
	repeat = (repeatType != "0");
	parser.addExercise(lesson,sublesson,level,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText);
	saved=false;
	skipTextRefresh=true;
	restoreText();
	skipTextRefresh=false;
	updateTitle();
}

/*! Loads current exercise text. */
void packEditor::restoreText(void)
{
	ui->levelLabel->setText(
		configParser::initExercise(
			parser.exerciseText(
				ui->lessonSelectionBox->currentIndex()+1,
				ui->sublessonSelectionBox->currentIndex()+1,
				ui->exerciseSelectionBox->currentIndex()+1),
			parser.exerciseLineLength(
				ui->lessonSelectionBox->currentIndex()+1,
				ui->sublessonSelectionBox->currentIndex()+1,
				ui->exerciseSelectionBox->currentIndex()+1)));
	QString textLengthStr = tr("Text length:") + " ";
	if(skipTextRefresh)
	{
		ui->textLengthLabel->setText(textLengthStr + QString::number(ui->levelTextEdit->toPlainText().count()));
		return;
	}
	skipTextUpdates=true;
	ui->levelTextEdit->setPlainText(parser.exerciseRawText(
		ui->lessonSelectionBox->currentIndex()+1,
		ui->sublessonSelectionBox->currentIndex()+1,
		ui->exerciseSelectionBox->currentIndex()+1));
	skipTextUpdates=false;
	ui->textLengthLabel->setText(textLengthStr + QString::number(ui->levelTextEdit->toPlainText().count()));
}

/*!
 * Connected from lessonSelectionBox->activated().\n
 * Selects lesson selected in lessonSelectionBox.
 *
 * \see switchSublesson
 * \see switchExercise
 */
void packEditor::switchLesson(void)
{
	for(int i=0; i < 2; i++)
	{
		ui->sublessonSelectionBox->setCurrentIndex(0);
		ui->exerciseSelectionBox->setCurrentIndex(0);
		refreshUi(false,false,false);
	}
}

/*!
 * Connected from sublessonSelectionBox->activated().\n
 * Selects sublesson selected in sublessonSelectionBox.
 *
 * \see switchLesson
 * \see switchExercise
 */
void packEditor::switchSublesson(void)
{
	for(int i=0; i < 2; i++)
	{
		ui->exerciseSelectionBox->setCurrentIndex(0);
		refreshUi(false,false,false);
	}
}

/*!
 * Connected from exerciseSelectionBox->activated().\n
 * Selects exercise selected in exerciseSelectionBox.
 *
 * \see switchLesson
 * \see switchSublesson
 */
void packEditor::switchExercise(void)
{
	refreshUi(false,false,false);
}

/*!
 * Connected from repeatingBox->activated().\n
 * Changes current exercise repeat method.
 */
void packEditor::changeRepeating(int index)
{
	int lesson, sublesson, level, limitExt, lengthExt;
	lesson = ui->lessonSelectionBox->currentIndex()+1;
	sublesson = ui->sublessonSelectionBox->currentIndex()+1;
	level = ui->exerciseSelectionBox->currentIndex()+1;
	limitExt = parser.exerciseRepeatLimit(lesson,sublesson,level);
	lengthExt = parser.exerciseLineLength(lesson,sublesson,level);
	QString repeatType;
	switch(index) {
		case 1:
			// Words
			repeatType = "w";
			break;
		default:
			// None
			repeatType = "0";
			break;
	}
	QString lessonDesc;
	if((sublesson == 1) && (level == 1))
		lessonDesc = parser.lessonDesc(lesson);
	else
		lessonDesc = "";
	QString targetText = parser.exerciseRawText(lesson,sublesson,level);
	deleteExerciseLine(lesson,sublesson,level);
	bool repeat;
	repeat = (repeatType != "0");
	parser.addExercise(lesson,sublesson,level,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText);
	saved=false;
	refreshUi(false,false,false);
}

/*!
 * Connected from repeatLengthBox->valueChanged().\n
 * Changes current exercise maximum number of characters (if repeating is enabled).
 */
void packEditor::changeRepeatLength(int limitExt)
{
	if(skipBoxUpdates)
		return;
	int lesson, sublesson, level, lengthExt;
	lesson = ui->lessonSelectionBox->currentIndex()+1;
	sublesson = ui->sublessonSelectionBox->currentIndex()+1;
	level = ui->exerciseSelectionBox->currentIndex()+1;
	lengthExt = parser.exerciseLineLength(lesson,sublesson,level);
	QString repeatType = parser.exerciseRepeatType(lesson,sublesson,level);
	QString lessonDesc;
	if((sublesson == 1) && (level == 1))
		lessonDesc = parser.lessonDesc(lesson);
	else
		lessonDesc = "";
	QString targetText = parser.exerciseRawText(lesson,sublesson,level);
	deleteExerciseLine(lesson,sublesson,level);
	bool repeat;
	repeat = (repeatType != "0");
	parser.addExercise(lesson,sublesson,level,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText);
	saved=false;
	refreshUi(false,false,false);
}

/*!
 * Connected from lineLengthBox->valueChanged().\n
 * Changes current exercise maximum number of characters in one line.
 */
void packEditor::changeLineLength(int lengthExt)
{
	if(skipBoxUpdates)
		return;
	int lesson, sublesson, level, limitExt;
	lesson = ui->lessonSelectionBox->currentIndex()+1;
	sublesson = ui->sublessonSelectionBox->currentIndex()+1;
	level = ui->exerciseSelectionBox->currentIndex()+1;
	limitExt = parser.exerciseRepeatLimit(lesson,sublesson,level);
	QString repeatType = parser.exerciseRepeatType(lesson,sublesson,level);
	QString lessonDesc;
	if((sublesson == 1) && (level == 1))
		lessonDesc = parser.lessonDesc(lesson);
	else
		lessonDesc = "";
	QString targetText = parser.exerciseRawText(lesson,sublesson,level);
	deleteExerciseLine(lesson,sublesson,level);
	bool repeat;
	repeat = (repeatType == "0");
	parser.addExercise(lesson,sublesson,level,repeat,repeatType,limitExt,lengthExt,lessonDesc,targetText);
	saved=false;
	refreshUi(false,false,false);
}

/*!
 * Connected from saveButton->clicked().\n
 * Saves opened pack file.
 */
void packEditor::save(void)
{
	if(newFile || readOnly)
		saveAs();
	else
	{
		QFile saveQFile(saveFileName);
		if(saveQFile.open(QFile::WriteOnly | QFile::Text))
		{
			saveQFile.write(parser.data());
			saved=true;
			refreshUi(false,false,false);
		}
		else
			readOnly=true;
	}
}

/*!
 * Connected from saveAsButton->clicked().\n
 * Saves opened pack file as another file.
 */
void packEditor::saveAs(void)
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

/*! Returns pack file name. */
QString packEditor::getFileName(void)
{
	return saveFileName;
}

/*!
 * Overrides QDialog#closeEvent().\n
 * Closes the opened file before closing the window.
 */
void packEditor::closeEvent(QCloseEvent *event)
{
	closeFile();
	event->ignore();
}
