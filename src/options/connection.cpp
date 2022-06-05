/*
 * connection.cpp
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

#include "options/connection.h"
#include "ui_connection.h"

/*! Constructs connectionOptions. */
connectionOptions::connectionOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::connectionOptions),
	settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat)
{
	ui->setupUi(this);
	refresh();
	changeAddress();
	// Connections
	connect(ui->IPEdit,&QLineEdit::textChanged,this,&connectionOptions::changeAddress);
	connect(ui->portEdit,SIGNAL(valueChanged(int)),this,SLOT(changeAddress()));
	connect(ui->clientButton, &QRadioButton::toggled, this, &connectionOptions::changeMode);
	connect(ui->serverButton, &QRadioButton::toggled, this, &connectionOptions::changeMode);
	connect(ui->fullModeCheckBox, &QCheckBox::toggled, this, &connectionOptions::setFullMode);
	connect(ui->testButton,SIGNAL(clicked()),this,SLOT(testConnection()));
}

/*! Destroys the connectionOptions object. */
connectionOptions::~connectionOptions()
{
	delete ui;
}

/*! Reloads settings. */
void connectionOptions::refresh(void)
{
#ifndef Q_OS_WASM
	int mode = settings.value("server/mode", 2).toInt();
	bool serverMode = (mode == 1);
	if(serverMode)
	{
		if(!serverPtr)
			serverPtr = new monitorServer;
		ui->serverButton->setChecked(true);
		ui->fullModeCheckBox->show();
		ui->fullModeCheckBox->setChecked(settings.value("server/fullmode", false).toBool());
	}
	else
	{
		ui->clientButton->setChecked(true);
		ui->fullModeCheckBox->hide();
		if(serverPtr)
			serverPtr->deleteLater();
	}
	ui->IPLabel->setVisible(!serverMode);
	ui->IPEdit->setVisible(!serverMode);
	ui->testButton->setVisible(!serverMode);
	ui->statusLabel->setVisible(!serverMode);
	ui->statusValueLabel->setVisible(!serverMode);
#endif // Q_OS_WASM
	ui->IPEdit->setText(QHostAddress(client.serverAddress().toIPv4Address()).toString());
	ui->portEdit->setValue(client.serverPort());
	ui->testButton->setEnabled(client.enabled());
}

/*! Changes the mode (client or server). */
void connectionOptions::changeMode(void)
{
	if(ui->serverButton->isChecked())
		settings.setValue("server/mode", 1);
	else
		settings.setValue("server/mode", 2);
	refresh();
}

/*! Toggles full mode. */
void connectionOptions::setFullMode(bool enable)
{
	settings.setValue("server/fullmode", enable);
}

/*!
 * Connected from %textChanged() signal of %IPEdit and %portEdit.\n
 * Changes server address settings.
 */
void connectionOptions::changeAddress(void)
{
	settings.setValue("server/address",ui->IPEdit->text());
	settings.setValue("server/port",ui->portEdit->text());
	settings.sync();
#ifndef Q_OS_WASM
	if((settings.value("server/mode", 2).toInt() == 1) && serverPtr)
	{
		serverPtr->close();
		serverPtr->deleteLater();
		serverPtr = new monitorServer;
	}
#endif // Q_OS_WASM
	ui->statusValueLabel->setText(tr("Unknown"));
}

/*! Tests server connection. */
void connectionOptions::testConnection(void)
{
	ui->statusValueLabel->setText("...");
	ui->testButton->setEnabled(false);
	client.enableClient();
	if(client.available() && client.isPaired())
		ui->statusValueLabel->setText("OK");
	else
		ui->statusValueLabel->setText(tr("Failed to connect"));
	client.close();
	ui->testButton->setEnabled(true);
}
