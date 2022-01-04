/*
 * statsdialog.cpp
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

#include "statsdialog.h"
#include "ui_statsdialog.h"

/*! Constructs statsDialog. */
statsDialog::statsDialog(monitorClient *client, QString configName, int lesson, int sublesson, int exercise, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::statsDialog)
{
	ui->setupUi(this);
	// Load data
	QList<QByteArray> response = client->sendRequest("get",{"resultcount",configName.toUtf8(),QByteArray::number(lesson),QByteArray::number(sublesson),QByteArray::number(exercise)});
	if(response[0] != "ok")
	{
		QMetaObject::invokeMethod(this,"reject",Qt::QueuedConnection);
		return;
	}
	ui->statsTable->clear();
	ui->statsTable->setColumnCount(3);
	ui->statsTable->setHorizontalHeaderLabels({ tr("Speed"), tr("Mistakes"), tr("Time") });
	int i, count = response[1].toInt();
	ui->statsTable->setRowCount(count);
	QTableWidgetItem *item;
	for(i=0; i < count; i++)
	{
		response = client->sendRequest("get",{"result",configName.toUtf8(),QByteArray::number(lesson),QByteArray::number(sublesson),QByteArray::number(exercise),QByteArray::number(i)});
		if(response[0] != "ok")
		{
			QMetaObject::invokeMethod(this,"reject",Qt::QueuedConnection);
			return;
		}
		// Speed
		item = new QTableWidgetItem(QString(response[1]));
		ui->statsTable->setItem(i,0,item);
		// Mistakes
		item = new QTableWidgetItem(QString(response[2]));
		ui->statsTable->setItem(i,1,item);
		// Time
		item = new QTableWidgetItem(QString(response[3]));
		ui->statsTable->setItem(i,2,item);
	}
	// Connections
	connect(ui->okButton,SIGNAL(clicked()),this,SLOT(accept()));
}

/*! Destroys the statsDialog object. */
statsDialog::~statsDialog()
{
	delete ui;
}
