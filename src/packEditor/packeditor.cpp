/*
 * packeditor.cpp
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

#include "packEditor/packeditor.h"
#include "ui_packeditor.h"

packEditor::packEditor(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::packEditor)
{
	ui->setupUi(this);
	setWindowState(Qt::WindowMaximized);
	// New file button
	connect(ui->newFileButton,SIGNAL(clicked()),this,SLOT(createNewFile()));
	// Open file button
	connect(ui->openFileButton,SIGNAL(clicked()),this,SLOT(openFile()));
	// Open built-in pack button
	connect(ui->openPrebuiltButton,SIGNAL(clicked()),this,SLOT(openPrebuilt()));
	// Close button
	connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(close()));
	// Tab close button
	connect(ui->fileTabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
	// Default values
	newFile=false;
	fileID=0;
	defaultFileName = tr("Unnamed") + ".typer";
}

packEditor::~packEditor()
{
	delete ui;
}

void packEditor::init(void)
{
	if(newFile)
	{
		createNewFile();
	}
}

void packEditor::setNewFile(bool value)
{
	newFile = value;
}

void packEditor::createNewFile(void)
{
	fileID++;
	QString newFileName = defaultFileName;
	packView *newTab = new packView(this,fileID);
	ui->fileTabWidget->addTab(newTab,newFileName);
	newTab->openFile(newFileName,true,false);
}

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

void packEditor::openPrebuilt(void)
{
	packSelector packSel;
	packSel.setStyleSheet(styleSheet());
	if(packSel.exec() == QDialog::Accepted)
	{
		QString newFileName = getConfigLoc() + "/" + packSel.selectedConfig();
		fileID++;
		packView *newTab = new packView(this,fileID);
		ui->fileTabWidget->addTab(newTab,newFileName);
		newTab->openFile(newFileName,false,true);
		ui->fileTabWidget->setCurrentIndex(ui->fileTabWidget->count()-1);
	}
}

void packEditor::close(void)
{
	if(closeAll())
		done(0);
}

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

void packEditor::setFileName(QString newFileName, QWidget *sourceWidget)
{
	ui->fileTabWidget->setTabText(
		ui->fileTabWidget->indexOf(sourceWidget),
		newFileName);
}

void packEditor::closeTab(int id)
{
	if(((packView*)ui->fileTabWidget->widget(id))->closeFile())
		ui->fileTabWidget->removeTab(id);
}

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

void packEditor::closeEvent(QCloseEvent *event)
{
	if(closeAll())
		event->accept();
	else
		event->ignore();
}
