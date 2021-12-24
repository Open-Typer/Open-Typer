/*
 * connection.cpp
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

#include "options/connection.h"
#include "ui_connection.h"

/*! Constructs connectionOptions. */
connectionOptions::connectionOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::connectionOptions)
{
	ui->setupUi(this);
	settings = new QSettings(fileUtils::mainSettingsLocation(),QSettings::IniFormat);
	client = new monitorClient;
	ui->IPEdit->setText(QHostAddress(client->serverAddress().toIPv4Address()).toString());
	ui->portEdit->setValue(client->serverPort());
	ui->serverCheckBox->setChecked(client->enabled());
	ui->testButton->setEnabled(client->enabled());
	changeAddress();
	// Connections
	connect(ui->IPEdit,&QLineEdit::textChanged,this,&connectionOptions::changeAddress);
	connect(ui->portEdit,SIGNAL(valueChanged(int)),this,SLOT(changeAddress()));
	connect(ui->serverCheckBox,SIGNAL(clicked(bool)),this,SLOT(changeServerState(bool)));
	connect(ui->testButton,SIGNAL(clicked()),this,SLOT(testConnection()));
}

/*! Destroys the connectionOptions object. */
connectionOptions::~connectionOptions()
{
	delete ui;
}

/*!
 * Connected from %textChanged() signal of %IPEdit and %portEdit.\n
 * Changes server address settings.
 */
void connectionOptions::changeAddress(void)
{
	settings->setValue("server/address",ui->IPEdit->text());
	settings->setValue("server/port",ui->portEdit->text());
	ui->statusValueLabel->setText(tr("Unknown"));
}

/*!
 * Connected from serverCheckBox->clicked().\n
 * Enables or disables class monitor server connection.
 */
void connectionOptions::changeServerState(bool enable)
{
	settings->setValue("server/enabled",enable);
	ui->testButton->setEnabled(enable);
	if(enable)
		emit testConnection();
}

/*!
 * Connected from testButton->clicked().\n
 * Tests server connection.
 */
void connectionOptions::testConnection(void)
{
	ui->statusValueLabel->setText("...");
	ui->testButton->setEnabled(false);
	if(client->available())
		ui->statusValueLabel->setText("OK");
	else
		ui->statusValueLabel->setText(tr("Failed to connect"));
	ui->testButton->setEnabled(true);
}
