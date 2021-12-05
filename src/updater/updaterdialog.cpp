/*
 * updaterdialog.cpp
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

#include "updater/updaterdialog.h"
#include "ui_updaterdialog.h"

UpdaterDialog::UpdaterDialog(QWidget *parent, bool downloading) :
	QDialog(parent),
	ui(new Ui::UpdaterDialog)
{
	ui->setupUi(this);
	_downloading = downloading;
	if(downloading)
	{
		ui->title->hide();
		ui->versionComparisonFrame->hide();
		ui->buttonFrame->hide();
		ui->notice->setText(tr("Downloading update, please wait..."));
	}
	else
	{
		ui->progressBar->hide();
		connect(ui->yesButton,SIGNAL(clicked()),this,SLOT(accept()));
		connect(ui->noButton,SIGNAL(clicked()),this,SLOT(reject()));
	}
}

UpdaterDialog::~UpdaterDialog()
{
	delete ui;
}

void UpdaterDialog::setCurrentVer(QString ver)
{
	ui->currentVerLabel->setText(
		ui->currentVerLabel->text() + " " + ver);
}

void UpdaterDialog::setNewVer(QString ver)
{
	ui->newVerLabel->setText(
		ui->newVerLabel->text() + " " + ver);
	ui->changeLogLabel->setText(tr("Change log is available at") + " <a href=\"" + _GITHUB_REPO + "/releases/tag/" + ver + "\">GitHub</a>");
}

void UpdaterDialog::updateProgress(int percentage)
{
	ui->progressBar->setRange(0,100);
	ui->progressBar->setValue(percentage);
}

void UpdaterDialog::closeEvent(QCloseEvent *event)
{
	if(_downloading)
		event->ignore();
	else
		event->accept();
}
