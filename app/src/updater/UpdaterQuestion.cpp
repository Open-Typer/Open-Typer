/*
 * UpdaterQuestion.cpp
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

#include "updater/UpdaterQuestion.h"
#include "ui_UpdaterQuestion.h"

/*! Constructs UpdaterQuestion. */
UpdaterQuestion::UpdaterQuestion(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UpdaterQuestion)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	connect(ui->buttonBox->button(QDialogButtonBox::Yes), &QPushButton::clicked, this, &UpdaterQuestion::accepted);
	connect(ui->buttonBox->button(QDialogButtonBox::No), &QPushButton::clicked, this, &UpdaterQuestion::close);
}

/*! Destroys the UpdaterQuestion object. */
UpdaterQuestion::~UpdaterQuestion()
{
	delete ui;
}
