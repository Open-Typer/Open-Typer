/*
 * levelsummary.cpp
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

#include "levelsummary.h"
#include "ui_levelsummary.h"

/*! Constructs the levelSummary dialog. */
levelSummary::levelSummary(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::levelSummary)
{
	ui->setupUi(this);
	ui->okButton->hide();
	ui->timeLabel->hide();
	ui->hitsLabel->hide();
	ui->hpmLabel->hide();
	ui->mistakesLabel->hide();
	ui->showSummaryCheckBox->setChecked(false);
	showSummary = ui->showSummaryCheckBox->isChecked();
	// Connections
	connect(ui->yesButton,SIGNAL(clicked()),this,SLOT(accept()));
	connect(ui->noButton,SIGNAL(clicked()),this,SLOT(reject()));
	connect(ui->okButton,SIGNAL(clicked()),this,SLOT(accept()));
	connect(ui->showSummaryCheckBox,SIGNAL(toggled(bool)),this,SLOT(setShowSummary(bool)));
}

/*! Destroys the levelSummary object. */
levelSummary::~levelSummary()
{
	delete ui;
}

/*!
 * Hides the "continue to the next exercise" question and shows an OK button.
 * \image html levelSummaryOK.png
 */
void levelSummary::showOK(void)
{
	ui->yesButton->hide();
	ui->noButton->hide();
	ui->questionLabel->hide();
	ui->okButton->show();
}

/*! Sets and shows the exercise time. */
void levelSummary::setTotalTime(double time)
{
	ui->timeLabel->setText(
		ui->timeLabel->text() + " " + QString::number(time) + " " + tr("seconds"));
	ui->timeLabel->show();
}

/*! Sets and shows the total number of hits (excluding error penalty). */
void levelSummary::setHitCount(int hits)
{
	ui->hitsLabel->setText(
		ui->hitsLabel->text() + " " + QString::number(hits));
	ui->hitsLabel->show();
}

/*! Sets and shows the number of hits per minute. */
void levelSummary::setHits(int hits)
{
	ui->hpmLabel->setText(
		ui->hpmLabel->text() + " " + QString::number(hits));
	ui->hpmLabel->show();
}

/*! Sets and shows the number of mistakes */
void levelSummary::setMistakes(int mistakes)
{
	ui->mistakesLabel->setText(
		ui->mistakesLabel->text() + " " + QString::number(mistakes));
	ui->mistakesLabel->show();
}

/*! Connected from showSummaryCheckBox->toggled(). */
void levelSummary::setShowSummary(bool value)
{
	showSummary = value;
}
