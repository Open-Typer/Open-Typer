/*
 * testwaitdialog.cpp
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

#include "testwaitdialog.h"
#include "ui_testwaitdialog.h"

/*! Constructs testWaitDialog. */
testWaitDialog::testWaitDialog(monitorClient *client, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::testWaitDialog),
	m_client(client)
{
	ui->setupUi(this);
	ui->nameEdit->setEnabled(!client->fullMode());
	ui->textArea->hide();
	font = themeEngine::font();
	font.setPointSize(12);
	ui->textLabel->setFont(themeEngine::font());
	// Disable focus of cancel button so that it doesn't react to return/enter key
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setFocusPolicy(Qt::NoFocus);
	// Connections
	connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &testWaitDialog::close);
	connect(ui->nameEdit, &QLineEdit::textChanged, this, [this](QString text) { m_client->sendRequest("put", { "name", text }); });
	connect(client, &monitorClient::studentNameChanged, this, [this](QString name) {
		// When the name changes on server side, disable editing on client side
		ui->nameEdit->setEnabled(false);
		ui->nameEdit->setText(name);
	});
}

/*! Destroys the testWaitDialog object. */
testWaitDialog::~testWaitDialog()
{
	delete ui;
}

/*! Returns the user name. */
QString testWaitDialog::name(void)
{
	return ui->nameEdit->text();
}

/*! Sets the user name. */
void testWaitDialog::setName(QString name)
{
	ui->nameEdit->setText(name);
}

/*! Returns true if the user name is read-only. */
bool testWaitDialog::nameReadOnly(void)
{
	return ui->nameEdit->isReadOnly();
}

/*! Toggles user name edit read-only option. */
void testWaitDialog::setNameReadOnly(bool readOnly)
{
	ui->nameEdit->setReadOnly(readOnly);
}

/*! Sets the exercise text. */
void testWaitDialog::setText(QString text)
{
	ui->textArea->show();
	ui->textLabel->setText(text);
	QFontMetrics fontMetrics = ui->textLabel->fontMetrics();
	int width = 0;
	QStringList lines = text.split("\n");
	for(int i=0; i < lines.count(); i++)
		width = std::max(width, fontMetrics.boundingRect(lines[i]).width());
	ui->textArea->setFixedWidth(width + ui->textArea->verticalScrollBar()->width());
}

/*! Overrides QDialog::closeEvent(). */
void testWaitDialog::closeEvent(QCloseEvent *event)
{
	QMessageBox *question = new QMessageBox(this);
	question->setWindowModality(Qt::WindowModal);
	question->setText(tr("Are you sure?"));
	question->setIcon(QMessageBox::Question);
	question->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	connect(question, &QDialog::finished, this, [this, question]() {
		if(question->result() == QMessageBox::Yes)
		{
			m_client->sendRequest("put", {"abortExercise"});
			reject();
		}
	});
	question->open();
	event->ignore();
}

/*!
 * Overrides QDialog#keyPressEvent().\n
 * Prevents escape key from closing the dialog.
 */
void testWaitDialog::keyPressEvent(QKeyEvent *event)
{
	// Block Esc key
	if(event->key() != Qt::Key_Escape)
		QDialog::keyPressEvent(event);
}
