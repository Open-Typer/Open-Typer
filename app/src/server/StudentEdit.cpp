/*
 * StudentEdit.cpp
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

#include "server/StudentEdit.h"
#include "ui_StudentEdit.h"

/*! Constructs StudentEdit. */
StudentEdit::StudentEdit(bool newStudent, int class_id, int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::StudentEdit),
	classID(class_id)
{
	ui->setupUi(this);
	if(newStudent)
	{
		setWindowTitle(tr("New student"));
		ui->resetPasswordButton->hide();
		ui->usernameLabel->hide();
		ui->usernameEdit->hide();
	}
	else
	{
		setWindowTitle(tr("Edit student"));
		ui->passwordEdit->hide();
		ui->repeatPasswordLabel->hide();
		ui->repeatPasswordEdit->hide();
		ui->nameEdit->setText(dbMgr.userName(id));
	}
	creatingNewStudent = newStudent;
	passwordReset = false;
	if(creatingNewStudent)
		userID = 0;
	else
		userID = id;
	classes = dbMgr.classIDs();
	QList<int> classesToRemove;
	classesToRemove += classID;
	for(int i = 0; i < classes.count(); i++)
	{
		if(dbMgr.studentIDs(classes[i]).count() == 0)
			classesToRemove += classes[i];
	}
	for(int i = 0; i < classesToRemove.count(); i++)
		classes.removeAll(classesToRemove[i]);
	ui->classBox->clear();
	for(int i = 0; i < classes.count(); i++)
		ui->classBox->addItem(dbMgr.className(classes[i]));
	verify();
	// Connections
	connect(ui->nameEdit, &QLineEdit::textChanged, this, &StudentEdit::verify);
	connect(ui->passwordEdit, &QLineEdit::textChanged, this, &StudentEdit::verify);
	connect(ui->repeatPasswordEdit, &QLineEdit::textChanged, this, &StudentEdit::verify);
	connect(ui->resetPasswordButton, SIGNAL(clicked()), this, SLOT(resetPassword()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(finish()));
	connect(ui->classBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StudentEdit::openClass);
	connect(ui->newStudentButton, &QRadioButton::toggled, this, &StudentEdit::switchMode);
	connect(ui->existingStudentButton, &QRadioButton::toggled, this, &StudentEdit::switchMode);
	ui->newStudentButton->setChecked(true);
	if(!creatingNewStudent || (classes.count() == 0))
	{
		ui->newStudentButton->hide();
		ui->existingStudentButton->hide();
	}
	else
		openClass();
}

/*! Destroys the StudentEdit object. */
StudentEdit::~StudentEdit()
{
	delete ui;
}

/*!
 * Connected from all line edits.\n
 * Checks if everything is correct and enables the OK button.
 */
void StudentEdit::verify(void)
{
	ui->okButton->setEnabled(false);
	// Check name
	if(ui->newStudentButton->isChecked())
	{
		if(ui->nameEdit->text() == "")
			return;
		// Set username
		QString name = ui->nameEdit->text();
		QString username;
		if(creatingNewStudent)
			username = "_";
		else
			username = QString::number(userID) + "_";
		for(int i = 0; i < name.count(); i++)
		{
			if(name[i] == ' ')
				username += ".";
			else if(name[i].toLatin1() > 0)
				username += name[i].toLower();
			else
				username += "-";
		}
		ui->usernameEdit->setText(username);
		if(creatingNewStudent || passwordReset)
		{
			// Check password
			if(ui->passwordEdit->text() == "")
				return;
			if(ui->passwordEdit->text() != ui->repeatPasswordEdit->text())
				return;
		}
	}
	// Everything is correct
	ui->okButton->setEnabled(true);
}

/*
 * Connected from resetPasswordButton->clicked().\n
 * Shows password boxes.
 */
void StudentEdit::resetPassword(void)
{
	ui->resetPasswordButton->hide();
	ui->passwordEdit->show();
	ui->repeatPasswordLabel->show();
	ui->repeatPasswordEdit->show();
	passwordReset = true;
	verify();
}

/*!
 * Connected from okButton->clicked().\n
 * Edits or creates the user and closes the window.
 */
void StudentEdit::finish(void)
{
	if(creatingNewStudent)
	{
		if(ui->newStudentButton->isChecked())
			dbMgr.addUser(ui->nameEdit->text(), DatabaseManager::Role_Student, ui->passwordEdit->text(), ui->usernameEdit->text(), classID);
		else
			dbMgr.addStudentToClass(students[ui->studentBox->currentIndex()], classID);
	}
	else
	{
		if(passwordReset)
			dbMgr.editUser(userID, ui->nameEdit->text(), DatabaseManager::Role_Student, ui->passwordEdit->text(), ui->usernameEdit->text());
		else
			dbMgr.editUser(userID, ui->nameEdit->text(), DatabaseManager::Role_Student, "", ui->usernameEdit->text());
	}
	accept();
}

/*! Changes the mode (new student, existing student). */
void StudentEdit::switchMode(void)
{
	if(ui->newStudentButton->isChecked())
	{
		ui->newStudentForm->show();
		ui->existingStudentForm->hide();
	}
	else
	{
		ui->existingStudentForm->show();
		ui->newStudentForm->hide();
	}
}

/*! Opens the selected class. */
void StudentEdit::openClass(void)
{
	int selected = classes[ui->classBox->currentIndex()];
	students = dbMgr.studentIDs(selected);
	ui->studentBox->clear();
	for(int i = 0; i < students.count(); i++)
		ui->studentBox->addItem(dbMgr.userName(students[i]) + " (" + dbMgr.userNickname(students[i]) + ")");
}
