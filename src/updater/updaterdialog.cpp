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

/*!
 * Constructs UpdaterDialog.
 * \param[in] downloading Whether to show a progress dialog.
 */
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
		ui->yesButton->hide();
		ui->noButton->hide();
		ui->notice->setText(tr("Downloading update, please wait..."));
		setMinimumHeight(100);
		setMaximumHeight(100);
		connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(abortDownload()));
	}
	else
	{
		ui->progressBar->hide();
		ui->cancelButton->hide();
		connect(ui->yesButton,SIGNAL(clicked()),this,SLOT(accept()));
		connect(ui->noButton,SIGNAL(clicked()),this,SLOT(reject()));
	}
}

/*! Destroys the UpdaterDialog object. */
UpdaterDialog::~UpdaterDialog()
{
	delete ui;
}

/*! Sets current build version. */
void UpdaterDialog::setCurrentVer(QString ver)
{
	ui->currentVerLabel->setText(
		ui->currentVerLabel->text() + " " + ver);
}

/*! Sets new build version. */
void UpdaterDialog::setNewVer(QString ver)
{
	ui->newVerLabel->setText(
		ui->newVerLabel->text() + " " + ver);
	ui->changeLogLabel->setText(tr("Change log is available at") + " <a href=\"" + _GITHUB_REPO + "/releases/tag/" + ver + "\">GitHub</a>");
}

/*! Updates progress bar of the progress dialog. */
void UpdaterDialog::updateProgress(int percentage)
{
	ui->progressBar->setRange(0,100);
	ui->progressBar->setValue(percentage);
}

/*!
 * Connected from cancelButton->clicked().\n
 * Aborts the download and emits cancelDownload().
 *
 * \see cancelDownload()
 */
void UpdaterDialog::abortDownload(void)
{
	emit cancelDownload();
	close();
}

/*!
 * Overrides QDialog#closeEvent().\n
 * Aborts the download before closing the window.
 *
 * \see cancelDownload()
 */
void UpdaterDialog::closeEvent(QCloseEvent *event)
{
	emit cancelDownload();
	event->accept();
}
