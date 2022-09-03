/*
 * TimeDialog.cpp
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

#include "TimeDialog.h"
#include "ui_TimeDialog.h"

/*! Constructs TimeDialog. */
TimeDialog::TimeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TimeDialog)
{
	ui->setupUi(this);
	setTime(ui->timeEdit->time());
	connect(ui->timeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(setTime(QTime)));
	connect(ui->startButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

/*! Destroys the TimeDialog class. */
TimeDialog::~TimeDialog()
{
	delete ui;
}

/*!
 * Connected from timeEdit->timeChanged().\n
 * Sets time based on the value of timeEdit.
 */
void TimeDialog::setTime(QTime time)
{
	hours = time.hour();
	minutes = time.minute();
	seconds = time.second();
}
