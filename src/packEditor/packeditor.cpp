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

/*! Constructs packEditor. */
packEditor::packEditor(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::packEditor)
{
	ui->setupUi(this);
	setWindowState(Qt::WindowMaximized);
	// New file action
	connect(ui->newFileAction,SIGNAL(triggered()),this,SLOT(createNewFile()));
	// Open file action
	connect(ui->openFileAction,SIGNAL(triggered()),this,SLOT(openFile()));
	// Open built-in pack action
	connect(ui->openPrebuiltAction,SIGNAL(triggered()),this,SLOT(openPrebuilt()));
	// Tab close button
	connect(ui->fileTabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
	// Default values
	newFile=false;
	fileID=0;
	defaultFileName = tr("Unnamed") + ".typer";
	// Create new pack
	createNewFile();
}

/*! Destroys the packEditor object. */
packEditor::~packEditor()
{
	delete ui;
}

/*! Applies settings. Must be run before exec() or show(). */
void packEditor::init(void)
{
	if(newFile)
	{
		createNewFile();
	}
}

/*! Enables creation of new file when the editor starts. \see createNewFile() */
void packEditor::setNewFile(bool value)
{
	newFile = value;
}

/*!
 * Connected from newFileButton->clicked().\n
 * Opens a new file tab.
 * \see setNewFile()
 */
void packEditor::createNewFile(void)
{
	fileID++;
	QString newFileName = defaultFileName;
	packView *newTab = new packView(this,fileID);
	ui->fileTabWidget->addTab(newTab,newFileName);
	newTab->openFile(newFileName,true,false);
}

/*!
 * Connected from openFileButton->clicked().\n
 * Opens a file in a new tab.
 */
void packEditor::openFile(void)
{
	QFileDialog openDialog;
	openDialog.setFileMode(QFileDialog::AnyFile);
	openDialog.setNameFilter(tr("Open-Typer pack files") + " (*.typer)" + ";;" + tr("All files") + " (*)");
	if(openDialog.exec())
	{
		// Get selected file
		QString newFileName = openDialog.selectedFiles()[0];
		fileID++;
		packView *newTab = new packView(this,fileID);
		ui->fileTabWidget->addTab(newTab,newFileName);
		newTab->openFile(newFileName,false,false);
		ui->fileTabWidget->setCurrentIndex(ui->fileTabWidget->count()-1);
	}
	fixDuplicates();
}

/*! Opens a built-in pack in a new tab. \see packSelector */
void packEditor::openPrebuilt(void)
{
	packSelector packSel;
	packSel.setStyleSheet(styleSheet());
	if(packSel.exec() == QDialog::Accepted)
	{
		QString newFileName = ":res/configs/" + packSel.selectedConfig();
		fileID++;
		packView *newTab = new packView(this,fileID);
		ui->fileTabWidget->addTab(newTab,newFileName);
		newTab->openFile(newFileName,false,true);
		ui->fileTabWidget->setCurrentIndex(ui->fileTabWidget->count()-1);
	}
}

/*! Closes duplicate tabs. */
void packEditor::fixDuplicates(void)
{
	int i, i2, count = ui->fileTabWidget->count();
	for(i=0; i < count; i++)
	{
		for(i2=0; i2 < i; i2++)
		{
			QString text1 = ((packView*)ui->fileTabWidget->widget(i2))->getFileName();
			QString text2 = ((packView*)ui->fileTabWidget->widget(i))->getFileName();
			if((text1 == text2) && (text2 != defaultFileName))
			{
				if(i2 == ui->fileTabWidget->currentIndex())
					closeTab(i);
				else
					closeTab(i2);
				ui->fileTabWidget->setCurrentIndex(0);
				fixDuplicates();
				return;
			}
		}
	}
}

/*! Used by packView to set the tab text. \see packView */
void packEditor::setFileName(QString newFileName, QWidget *sourceWidget)
{
	ui->fileTabWidget->setTabText(
		ui->fileTabWidget->indexOf(sourceWidget),
		newFileName);
}

/*!
 * Connected from fileTabWidget->tabCloseRequested().\n
 * Closes a tab.
 * \see packView#closeFile()
 */
void packEditor::closeTab(int id)
{
	if(((packView*)ui->fileTabWidget->widget(id))->closeFile())
		ui->fileTabWidget->removeTab(id);
}

/*! Closes all tabs. */
bool packEditor::closeAll(void)
{
	int count = ui->fileTabWidget->count();
	int i, closedCount=0, closedTabs[count];
	for(i=0; i < count; i++)
	{
		if(((packView*)ui->fileTabWidget->widget(i))->closeFile())
		{
			closedTabs[closedCount] = i;
			closedCount++;
		}
		else
		{
			for(i=0; i < closedCount; i++)
				ui->fileTabWidget->removeTab(closedTabs[i]);
			return false;
		}
	}
	for(i=0; i < closedCount; i++)
		ui->fileTabWidget->removeTab(closedTabs[i]);
	return true;
}

/*!
 * Overrides QDialog#closeEvent().\n
 * Closes all tabs before closing the window.
 * \see closeAll()
 */
void packEditor::closeEvent(QCloseEvent *event)
{
	if(closeAll())
		event->accept();
	else
		event->ignore();
}

/*!
 * Overrides QDialog#keyPressEvent().\n
 * Prevents escape key from closing the dialog.
 */
void packEditor::keyPressEvent(QKeyEvent *event)
{
	// Block Esc key
	if(event->key() != Qt::Key_Escape)
		QDialog::keyPressEvent(event);
}
