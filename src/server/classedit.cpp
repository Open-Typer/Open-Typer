/*
 * classedit.cpp
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

#include "server/classedit.h"
#include "ui_classedit.h"

/*! Constructs classEdit. */
classEdit::classEdit(bool newClass, int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::classEdit)
{
	ui->setupUi(this);
	ui->okButton->setEnabled(false);
	// Set up list of icons
	QDirIterator icons(":/class-icons",QDirIterator::NoIteratorFlags);
	QStringList iconNames;
	while(icons.hasNext())
		iconNames += icons.next();
	iconNames.sort();
	for(int i=0; i < iconNames.count(); i++)
	{
		QListWidgetItem *item = new QListWidgetItem(QIcon(iconNames[i]),"");
		ui->iconList->addItem(item);
	}
	if(newClass)
		ui->iconList->setCurrentRow(0);
	else
		ui->iconList->setCurrentRow(dbMgr.classIcon(id));
	if(newClass)
		setWindowTitle(tr("New class"));
	else
	{
		setWindowTitle(tr("Edit class"));
		ui->nameEdit->setText(dbMgr.className(id));
	}
	creatingNewClass = newClass;
	classID = id;
	// Set up list of users
	ui->ownerBox->clear();
	QList<int> users = dbMgr.teacherIDs();
	for(int i=0; i < users.count(); i++)
		ui->ownerBox->addItem(dbMgr.userName(users[i]));
	updateOwner(ui->ownerBox->currentText());
	// Connections
	connect(ui->nameEdit,&QLineEdit::textChanged,this,&classEdit::verify);
	connect(ui->ownerBox,SIGNAL(currentTextChanged(const QString)),this,SLOT(updateOwner(const QString)));
	connect(ui->passwordEdit,&QLineEdit::textChanged,this,&classEdit::verify);
	connect(ui->okButton,SIGNAL(clicked()),this,SLOT(finish()));
}

/*! Destroys the classEdit object. */
classEdit::~classEdit()
{
	delete ui;
}

/*!
 * Connected from all line edits' textChanged() signal.\n
 * Checks if all info is correct and enables the OK button.
 */
void classEdit::verify(void)
{
	ui->okButton->setEnabled(false);
	// Check class name
	if(ui->nameEdit->text().count() == 0)
		return;
	QStringList classes = dbMgr.classNames();
	for(int i=0; i < classes.count(); i++)
	{
		if(classes[i] == ui->nameEdit->text())
		{
			if((dbMgr.classIDs().value(i) != classID) || creatingNewClass)
				return;
		}
	}
	// Check password
	if(ui->passwordEdit->text() == "")
		return;
	// Everything is correct
	ui->okButton->setEnabled(true);
}

/*!
 * Connected from ownerBox->currentTextChanged().\n
 * Updates passwordLabel.
 */
void classEdit::updateOwner(const QString name)
{
	ui->passwordLabel->setText("Password for " + name + ":");
	verify();
}

/*!
 * Connected from okButton->clicked().\n
 * Checks owner password, creates or edits the class and closes the dialog.
 */
void classEdit::finish(void)
{
	int owner = dbMgr.teacherIDs().value(ui->ownerBox->currentIndex());
	if(dbMgr.auth(owner, ui->passwordEdit->text()))
	{
		if(creatingNewClass)
			dbMgr.addClass(ui->nameEdit->text(), owner, ui->iconList->currentRow());
		else
			dbMgr.editClass(classID, ui->nameEdit->text(), owner, ui->iconList->currentRow());
		accept();
	}
}
