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
	// Tab change
	connect(ui->fileTabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
	// Tab close button
	connect(ui->fileTabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
	// Default values
	newFile=false;
	allowClosing = false;
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
 * Connected from fileTabWidget->currentChanged().
 */
void packEditor::tabChanged(int index)
{
	disconnect(ui->saveAction,nullptr,nullptr,nullptr);
	disconnect(ui->saveAsAction,nullptr,nullptr,nullptr);
	ui->saveAction->setEnabled(ui->fileTabWidget->count() != 0);
	ui->saveAsAction->setEnabled(ui->fileTabWidget->count() != 0);
	if(index != -1)
	{
		connect(ui->saveAction,&QAction::triggered,(packView*)ui->fileTabWidget->widget(index),&packView::save);
		connect(ui->saveAsAction,&QAction::triggered,(packView*)ui->fileTabWidget->widget(index),&packView::saveAs);
	}
}

/*!
 * Connected from fileTabWidget->tabCloseRequested().\n
 * Closes a tab.
 * \see packView#closeFile()
 */
void packEditor::closeTab(int id)
{
	connect((packView*) ui->fileTabWidget->widget(id), &packView::closed, this, [id,this]() {
		ui->fileTabWidget->removeTab(id);
		disconnect(ui->saveAction,nullptr,nullptr,nullptr);
		disconnect(ui->saveAsAction,nullptr,nullptr,nullptr);
		sender()->deleteLater();
	});
	connect((packView*) ui->fileTabWidget->widget(id), &packView::notClosed, this, [id,this]() {
		disconnect(sender(),nullptr,nullptr,nullptr);
	});
	((packView*)ui->fileTabWidget->widget(id))->closeFile();
}

/*! Closes all tabs. */
void packEditor::closeAll(void)
{
	int count = ui->fileTabWidget->count();
	int i;
	for(i=0; i < count; i++)
	{
		if(i+1 < count)
			connect((packView*) ui->fileTabWidget->widget(i), &packView::closed, (packView*) ui->fileTabWidget->widget(i+1), &packView::closeFile);
		if(i == 0)
			((packView*) ui->fileTabWidget->widget(i))->closeFile();
		connect((packView*) ui->fileTabWidget->widget(i), &packView::closed, this, [i,this]() {
			ui->fileTabWidget->removeTab(i);
		});
		connect((packView*) ui->fileTabWidget->widget(i), &packView::notClosed, this, [this]() {
			disconnect(sender(),nullptr,nullptr,nullptr);
		});
		if(i+1 >= count)
			connect((packView*) ui->fileTabWidget->widget(i), &packView::closed, this, &packEditor::allTabsClosed);
	}
	if(count == 0)
		emit allTabsClosed();
}

/*!
 * Overrides QDialog#closeEvent().\n
 * Closes all tabs before closing the window.
 * \see closeAll()
 */
void packEditor::closeEvent(QCloseEvent *event)
{
	if(allowClosing)
	{
		QDialog::closeEvent(event);
		return;
	}
	disconnect(this,&packEditor::allTabsClosed,nullptr,nullptr);
	connect(this, &packEditor::allTabsClosed, this, [this]() {
		allowClosing = true;
		close();
	});
	closeAll();
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
