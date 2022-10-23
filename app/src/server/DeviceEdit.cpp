/*
 * DeviceEdit.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#include "server/DeviceEdit.h"
#include "ui_DeviceEdit.h"

/*! Constructs DeviceEdit. */
DeviceEdit::DeviceEdit(int deviceID, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DeviceEdit),
	m_deviceID(deviceID)
{
	ui->setupUi(this);
	newDevice = (deviceID == 0);
	if(newDevice)
		setWindowTitle(tr("Add device"));
	else
	{
		setWindowTitle(tr("Edit device"));
		oldAddress = dbMgr.deviceAddress(deviceID);
		ui->nameEdit->setText(dbMgr.deviceName(deviceID));
		ui->ipEdit->setText(QHostAddress(oldAddress.toIPv4Address()).toString());
	}
	verify();
	// Connections
	connect(ui->nameEdit, &QLineEdit::textChanged, this, &DeviceEdit::verify);
	connect(ui->ipEdit, &QLineEdit::textChanged, this, &DeviceEdit::verify);
	connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &DeviceEdit::finish);
	connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &DeviceEdit::reject);
}

/*! Destroys the DeviceEdit object. */
DeviceEdit::~DeviceEdit()
{
	delete ui;
}

/*! Checks if everything is correct and enables the Save button. */
void DeviceEdit::verify(void)
{
	ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
	// Check name
	if(ui->nameEdit->text() == "")
		return;
	// Check IP address
	if(!ui->ipEdit->hasAcceptableInput())
		return;
	QHostAddress address = QHostAddress(QHostAddress(ui->ipEdit->text()).toIPv4Address());
	if(((dbMgr.findDevice(address) != 0) && (address != oldAddress)) || (address.toString() == "0.0.0.0"))
		return;
	// Everything is correct
	ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
}

/*! Adds the device and closes the window. */
void DeviceEdit::finish(void)
{
	if(newDevice)
		dbMgr.addDevice(ui->nameEdit->text(), QHostAddress(ui->ipEdit->text()));
	else
		dbMgr.editDevice(m_deviceID, ui->nameEdit->text(), QHostAddress(ui->ipEdit->text()));
	accept();
}
