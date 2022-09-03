/*
 * StudentOptions.cpp
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

#include "options/StudentOptions.h"
#include "ui_StudentOptions.h"

/*! Constructs StudentOptions. */
StudentOptions::StudentOptions(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::StudentOptions)
{
	ui->setupUi(this);
	client = new monitorClient;
	ui->errorLabel->hide();
	// Connections
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(auth()));
}

/*! Destroys the StudentOptions object. */
StudentOptions::~StudentOptions()
{
	delete ui;
}

/*!
 * Connected from okButton->clicked().\n
 * Checks student password and closes the dialog.
 */
void StudentOptions::auth(void)
{
	QStringList requestData;
	requestData.clear();
	requestData += ui->usernameEdit->text();
	requestData += ui->passwordEdit->text();
	auto response = client->sendRequest("auth", requestData);
	if(response[0] == "ok")
	{
		username = ui->usernameEdit->text();
		password = ui->passwordEdit->text();
		client->close();
		accept();
	}
	else
	{
		if(response.count() <= 1 || response[1] == "password")
			ui->errorLabel->setText(tr("Incorrect password"));
		else
			ui->errorLabel->setText(tr("Your class is closed or the user does not exist"));
		ui->errorLabel->show();
		int labelWidth = ui->errorLabel->geometry().width();
		ui->errorLabel->adjustSize();
		QPropertyAnimation *anim = new QPropertyAnimation(ui->errorLabel, "geometry");
		anim->setDuration(250);
		QRect animGeometry = ui->errorLabel->geometry();
		animGeometry.setWidth(labelWidth);
		anim->setStartValue(QRect(animGeometry.x(), animGeometry.y(), animGeometry.width(), 0));
		anim->setEndValue(animGeometry);
		anim->start();
	}
	client->close();
}
