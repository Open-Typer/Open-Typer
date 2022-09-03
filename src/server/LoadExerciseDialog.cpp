/*
 * LoadExerciseDialog.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#include "server/LoadExerciseDialog.h"
#include "ui_LoadExerciseDialog.h"

/*! Constructs LoadExerciseDialog. */
LoadExerciseDialog::LoadExerciseDialog(QList<int> availableTargets, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LoadExerciseDialog),
	m_targets(availableTargets),
	settings(FileUtils::mainSettingsLocation(), QSettings::IniFormat)
{
	init();
}

/*! Constructs LoadExerciseDialog for use on the local computer. */
LoadExerciseDialog::LoadExerciseDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LoadExerciseDialog),
	settings(FileUtils::mainSettingsLocation(), QSettings::IniFormat)
{
	local = true;
	init();
}

/*! Initializes the dialog. */
void LoadExerciseDialog::init(void)
{
	ui->setupUi(this);
	changeSource();
	changeMode();
	// Get selected exercise
	QString oldText = ui->selectedExButton->text();
	QString lessonTr = ConfigParser::lessonTr(publicPos::currentLesson);
	QString sublessonTr = ConfigParser::sublessonName(publicPos::currentSublesson);
	QString exerciseTr = ConfigParser::exerciseTr(publicPos::currentExercise);
	ui->selectedExButton->setText(oldText.arg(lessonTr + ", " + sublessonTr + ", " + exerciseTr));
	// Load selected exercise
	QString packPath = "", packName = settings.value("main/configfile", "").toString();
	if(settings.value("main/customconfig", false).toBool())
		packPath = packName;
	else
		packPath = ":/res/configs/" + packName;
	ConfigParser parser;
	if(parser.open(packPath))
	{
		m_exerciseText = parser.exerciseText(publicPos::currentLesson, publicPos::currentSublesson, publicPos::currentExercise);
		m_lineLength = parser.exerciseLineLength(publicPos::currentLesson, publicPos::currentSublesson, publicPos::currentExercise);
		m_includeNewLines = true;
	}
	if(local)
	{
		ui->textPreviewCheckBox->hide();
		ui->targetBox->hide();
	}
	else
	{
		// Set up targets
		for(int i = 0; i < m_targets.count(); i++)
		{
			QString name;
			if(settings.value("server/fullmode", false).toBool())
				name = dbMgr.userName(m_targets[i]);
			else
				name = dbMgr.deviceName(m_targets[i]);
			QCheckBox *checkBox = new QCheckBox(name, ui->targetBox);
			ui->targetLayout->addWidget(checkBox);
			checkBox->setChecked(true);
			connect(checkBox, &QCheckBox::toggled, this, &LoadExerciseDialog::verify);
			targets[m_targets[i]] = checkBox;
		}
	}
	verify();
	// Connections
	connect(ui->selectedExButton, &QCheckBox::toggled, this, &LoadExerciseDialog::changeSource);
	connect(ui->fromFileButton, &QCheckBox::toggled, this, &LoadExerciseDialog::changeSource);
	connect(ui->browseButton, &QPushButton::clicked, this, &LoadExerciseDialog::openFile);
	connect(ui->modeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoadExerciseDialog::changeMode);
	connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &LoadExerciseDialog::accept);
	connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &QDialog::reject);
}

/*! Destroys the LoadExerciseDialog object. */
LoadExerciseDialog::~LoadExerciseDialog()
{
	delete ui;
}

/*! Returns the text of the selected exercise. */
QString LoadExerciseDialog::exerciseText(void)
{
	if(ui->fromFileButton->isChecked())
	{
		if(fileName == "")
			return "";
		QFile file(fileName);
		if(file.open(QIODevice::ReadOnly | QIODevice::Text))
			return file.readAll();
	}
	else if(ui->selectedExButton->isChecked())
		return m_exerciseText;
	return "";
}

/*! Returns the maximum line length of the selected exercise. */
int LoadExerciseDialog::lineLength(void)
{
	if(ui->fromFileButton->isChecked())
		return ConfigParser::defaultLineLength;
	else if(ui->selectedExButton->isChecked())
		return m_lineLength;
	return 0;
}

/*! Returns true if the user has enabled new line inclusion. */
bool LoadExerciseDialog::includeNewLines(void)
{
	if(ui->fromFileButton->isChecked())
		return true;
	else if(ui->selectedExButton->isChecked())
		return m_includeNewLines;
	return false;
}

/*! Returns selected mode. */
int LoadExerciseDialog::mode(void)
{
	return ui->modeBox->currentIndex();
}

/*! Returns selected time limit. */
QTime LoadExerciseDialog::timeLimit(void)
{
	return ui->timeEdit->time();
}

/*! Returns true if the user has enabled mistake correction. */
bool LoadExerciseDialog::correctMistakes(void)
{
	return ui->correctMistakesCheckBox->isChecked();
}

/*! Returns true if the user has enabled user interface lock. */
bool LoadExerciseDialog::lockUi(void)
{
	return ui->lockUiCheckBox->isChecked();
}

/*! Returns true if the user has hidden the exercise text. */
bool LoadExerciseDialog::hideText(void)
{
	return ui->hideTextCheckBox->isChecked();
}

/*! Returns true if the user has allowed exercise text preview. */
bool LoadExerciseDialog::allowTextPreview(void)
{
	return ui->textPreviewCheckBox->isChecked();
}

/*! Returns list of selected targets. */
QList<int> LoadExerciseDialog::selectedTargets(void)
{
	QList<int> out;
	auto keys = targets.keys();
	for(int i = 0; i < keys.count(); i++)
	{
		if(targets[keys[i]]->isChecked())
			out += keys[i];
	}
	return out;
}

/*! Checks input values and enables the apply button. */
void LoadExerciseDialog::verify(void)
{
	bool enable = true;
	if(ui->fromFileButton->isChecked() && (fileName == ""))
		enable = false;
	if(!local)
	{
		int count = 0;
		auto keys = targets.keys();
		for(int i = 0; i < keys.count(); i++)
		{
			if(targets[keys[i]]->isChecked())
				count++;
		}
		if(count == 0)
			enable = false;
	}
	ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(enable);
}

/*! Changes the text source. */
void LoadExerciseDialog::changeSource(void)
{
	ui->selectedFileWidget->setEnabled(ui->fromFileButton->isChecked());
	verify();
}

/*! Opens exercise file. */
void LoadExerciseDialog::openFile(void)
{
	QString newFileName = QFileDialog::getOpenFileName(this);
	if(!newFileName.isNull())
	{
		QFile file(newFileName);
		if(file.size() > 8192) // Maximum size
		{
			QMessageBox::critical(this, QString(), tr("This file is too large!"));
			return;
		}
		if(file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			fileName = newFileName;
			file.close();
		}
		if(fileName != "")
		{
			QFileInfo fileInfo(fileName);
			ui->selectedFileLabel->setText(fileInfo.fileName());
		}
	}
	verify();
}

/*! Changes the exercise mode. */
void LoadExerciseDialog::changeMode(void)
{
	bool enableTimeEdit = (ui->modeBox->currentIndex() == 1);
	ui->timeLabel->setEnabled(enableTimeEdit);
	ui->timeEdit->setEnabled(enableTimeEdit);
	verify();
}
