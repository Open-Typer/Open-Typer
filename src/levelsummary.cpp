/*
 * levelsummary.cpp
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

#include "levelsummary.h"
#include "ui_levelsummary.h"

levelSummary::levelSummary(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::levelSummary)
{
	ui->setupUi(this);
	connect(ui->yesButton,SIGNAL(clicked()),this,SLOT(accept()));
	connect(ui->noButton,SIGNAL(clicked()),this,SLOT(reject()));
}

levelSummary::~levelSummary()
{
	delete ui;
}

void levelSummary::setTotalTime(double time)
{
	ui->timeLabel->setText(
		ui->timeLabel->text() + " " + QString::number(time) + " " + tr("seconds"));
}

void levelSummary::setHits(int hits)
{
	ui->hpmLabel->setText(
		ui->hpmLabel->text() + " " + QString::number(hits));
}

void levelSummary::setMistakes(int mistakes)
{
	ui->mistakesLabel->setText(
		ui->mistakesLabel->text() + " " + QString::number(mistakes));
}
