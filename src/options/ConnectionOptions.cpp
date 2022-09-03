/*
 * ConnectionOptions.cpp
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

#include "options/ConnectionOptions.h"
#include "ui_ConnectionOptions.h"

/*! Constructs ConnectionOptions. */
ConnectionOptions::ConnectionOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ConnectionOptions),
	settings(FileUtils::mainSettingsLocation(), QSettings::IniFormat)
{
	ui->setupUi(this);
	refresh();
	changeAddress();
	// Connections
	connect(ui->networkOptionsCheckBox, &QCheckBox::toggled, this, &ConnectionOptions::toggleNetworkOptions);
	connect(ui->IPEdit, &QLineEdit::textChanged, this, &ConnectionOptions::changeAddress);
	connect(ui->portEdit, SIGNAL(valueChanged(int)), this, SLOT(changeAddress()));
	connect(ui->clientButton, &QRadioButton::toggled, this, &ConnectionOptions::changeMode);
	connect(ui->serverButton, &QRadioButton::toggled, this, &ConnectionOptions::changeMode);
	connect(ui->fullModeCheckBox, &QCheckBox::toggled, this, &ConnectionOptions::setFullMode);
	connect(ui->testButton, SIGNAL(clicked()), this, SLOT(testConnection()));
}

/*! Destroys the ConnectionOptions object. */
ConnectionOptions::~ConnectionOptions()
{
	if(settings.value("main/networkEnabled", false).toBool() && (settings.value("server/mode", 2).toInt() == 2))
		testConnection(true);
	delete ui;
}

/*! Reloads settings. */
void ConnectionOptions::refresh(void)
{
#ifndef Q_OS_WASM
	int mode = settings.value("server/mode", 2).toInt();
	bool serverMode = (mode == 1);
	if(serverMode)
	{
		if(!serverPtr)
			serverPtr = new MonitorServer;
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
	bool networkEnabled = settings.value("main/networkEnabled", false).toBool();
	ui->networkOptionsCheckBox->setChecked(networkEnabled);
	ui->networkOptions->setEnabled(networkEnabled);
	ui->IPEdit->setText(QHostAddress(client.serverAddress().toIPv4Address()).toString());
	ui->portEdit->setValue(client.serverPort());
	ui->testButton->setEnabled(client.enabled());
}

/*! Toggles network options. */
void ConnectionOptions::toggleNetworkOptions(bool checked)
{
	settings.setValue("main/networkEnabled", checked);
	refresh();
}

/*! Changes the mode (client or server). */
void ConnectionOptions::changeMode(void)
{
	if(ui->serverButton->isChecked())
		settings.setValue("server/mode", 1);
	else
		settings.setValue("server/mode", 2);
	refresh();
}

/*! Toggles full mode. */
void ConnectionOptions::setFullMode(bool enable)
{
	settings.setValue("server/fullmode", enable);
}

/*!
 * Connected from %textChanged() signal of %IPEdit and %portEdit.\n
 * Changes server address settings.
 */
void ConnectionOptions::changeAddress(void)
{
	settings.setValue("server/address", ui->IPEdit->text());
	settings.setValue("server/port", ui->portEdit->text());
	settings.sync();
#ifndef Q_OS_WASM
	if((settings.value("server/mode", 2).toInt() == 1) && serverPtr)
	{
		serverPtr->close();
		serverPtr->deleteLater();
		serverPtr = new MonitorServer;
	}
#endif // Q_OS_WASM
	ui->statusValueLabel->setText(tr("Unknown"));
}

/*! Tests server connection. */
void ConnectionOptions::testConnection(bool silent)
{
	ui->statusValueLabel->setText("...");
	ui->testButton->setEnabled(false);
	client.setErrorDialogs(!silent);
	client.enableClient();
	bool available = client.available(), paired = false;
	if(available)
		paired = client.isPaired();
	if(client.available() && paired)
		ui->statusValueLabel->setText("OK");
	else
	{
		ui->statusValueLabel->setText(tr("Failed to connect"));
		if(available && !paired)
		{
			QMessageBox *pairMsg = new QMessageBox(QMessageBox::Critical, tr("Error"), tr("Please allow the following address on the server:"), QMessageBox::Ok, this);
			QString address = MonitorClient::localAddress().toString();
			if(MonitorClient::serverAddress() == QHostAddress(QHostAddress::LocalHost))
				address = QHostAddress(QHostAddress::LocalHost).toString();
			pairMsg->setInformativeText(address);
			pairMsg->setWindowModality(Qt::WindowModal);
			pairMsg->open();
		}
	}
	client.close();
	ui->testButton->setEnabled(true);
}
