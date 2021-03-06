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
	ui->timeLabel->hide();
	ui->hitsLabel->hide();
	ui->netHitsLabel->hide();
	ui->grossHitsLabel->hide();
	ui->mistakesLabel->hide();
	// Connections
	connect(ui->previewButton,SIGNAL(clicked()),this,SLOT(accept()));
}

/*! Destroys the levelSummary object. */
levelSummary::~levelSummary()
{
	delete ui;
}

/*! Sets and shows the exercise time. */
void levelSummary::setTotalTime(double time)
{
	ui->timeLabel->setText(
		ui->timeLabel->text() + " " + QString::number(time) + " " + tr("seconds"));
	ui->timeLabel->show();
}

/*! Sets and shows the total number of hits (excluding error penalty). */
void levelSummary::setTotalHits(int hits)
{
	ui->hitsLabel->setText(
		ui->hitsLabel->text() + " " + QString::number(hits));
	ui->hitsLabel->show();
}

/*! Sets and shows the number of net hits per minute. */
void levelSummary::setNetHits(int hits)
{
	ui->netHitsLabel->setText(
		ui->netHitsLabel->text() + " " + QString::number(hits));
	ui->netHitsLabel->show();
}

/*! Sets and shows the number of gross hits per minute. */
void levelSummary::setGrossHits(int hits)
{
	ui->grossHitsLabel->setText(
		ui->grossHitsLabel->text() + " " + QString::number(hits));
	ui->grossHitsLabel->show();
}

/*! Sets and shows the number of mistakes */
void levelSummary::setMistakes(int mistakes)
{
	ui->mistakesLabel->setText(
		ui->mistakesLabel->text() + " " + QString::number(mistakes));
	ui->mistakesLabel->show();
}
