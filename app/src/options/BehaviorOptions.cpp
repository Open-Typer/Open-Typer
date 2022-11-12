/*
 * BehaviorOptions.cpp
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

#include "options/BehaviorOptions.h"
#include "ui_BehaviorOptions.h"

/*! Constructs BehaviorOptions. */
BehaviorOptions::BehaviorOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::BehaviorOptions)
{
	ui->setupUi(this);
	// Load settings
	// Error penalty
	ui->errorPenaltyBox->setValue(Settings::errorPenalty());
	// Mistake limit
	bool mistakeLimit = Settings::mistakeLimit();
	ui->mistakeLimitCheckBox->setChecked(mistakeLimit);
	ui->mistakeCharsBox->setEnabled(mistakeLimit);
	ui->mistakeCharsBox->setValue(Settings::mistakeChars());
	// Settings lock
	ui->lockSettingsCheckBox->setChecked(Settings::settingsLockEnabled());
	if(!ui->lockSettingsCheckBox->isChecked())
	{
		ui->oldSettingsPasswdLabel->hide();
		ui->oldSettingsPasswdEdit->hide();
		ui->newSettingsPasswdLabel->hide();
		ui->newSettingsPasswdEdit->hide();
		ui->repeatSettingsPasswdLabel->hide();
		ui->repeatSettingsPasswdEdit->hide();
		ui->applySettingsLockButton->hide();
	}
	// Updates
#ifdef Q_OS_WIN
	ui->updatesCheckBox->setChecked(Settings::updateChecks());
#else
	ui->updatesBox->hide();
#endif // Q_OS_WIN
	// Connect
	// Error penalty box
	connect(ui->errorPenaltyBox, SIGNAL(valueChanged(int)), this, SLOT(setErrorPenalty(int)));
	// Mistake limit check box
	connect(ui->mistakeLimitCheckBox, &QCheckBox::toggled, this, &BehaviorOptions::toggleMistakeLimit);
	// Mistake characters box
	connect(ui->mistakeCharsBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BehaviorOptions::setMistakeChars);
	// Settings lock
	connect(ui->lockSettingsCheckBox, &QCheckBox::clicked, this, &BehaviorOptions::toggleSettingsLock);
	connect(ui->applySettingsLockButton, &QPushButton::clicked, this, &BehaviorOptions::setSettingsPassword);
	// Updates check box
	connect(ui->updatesCheckBox, &QCheckBox::toggled, this, [](bool checked) {
		Settings::setUpdateChecks(checked);
	});
}

/*! Destroys the BehaviorOptions object. */
BehaviorOptions::~BehaviorOptions()
{
	delete ui;
}

/*!
 * Connected from errorPenaltyBox#valueChanged().
 * Sets error penalty value.
 */
void BehaviorOptions::setErrorPenalty(int value)
{
	Settings::setErrorPenalty(value);
}

/*!
 * Toggles mistake limit.
 */
void BehaviorOptions::toggleMistakeLimit(bool checked)
{
	Settings::setMistakeLimit(checked);
	ui->mistakeCharsBox->setEnabled(checked);
}

/*!
 * Sets number of characters with max. 1 mistake.
 */
void BehaviorOptions::setMistakeChars(int value)
{
	Settings::setMistakeChars(value);
}

/*! Toggles settings lock. */
void BehaviorOptions::toggleSettingsLock(bool checked)
{
	if(checked)
	{
		// User has enabled settings lock
		ui->oldSettingsPasswdLabel->hide();
		ui->oldSettingsPasswdEdit->hide();
		ui->newSettingsPasswdLabel->show();
		ui->newSettingsPasswdEdit->show();
		ui->repeatSettingsPasswdLabel->show();
		ui->repeatSettingsPasswdEdit->show();
		ui->applySettingsLockButton->show();
	}
	else if(Settings::containsSettingsLockPasswd() && (Settings::settingsLockPasswd() != "-")) // "-" means that there's no password set
	{
		// User has disabled settings lock and there's an existing password
		QInputDialog *dialog = new QInputDialog(this);
		dialog->setWindowModality(Qt::WindowModal);
		dialog->setLabelText(tr("Settings lock password:"));
		dialog->setInputMode(QInputDialog::TextInput);
		dialog->setTextEchoMode(QLineEdit::Password);
		connect(dialog, &QDialog::accepted, this, [this, dialog]() {
			QCryptographicHash hash(QCryptographicHash::Sha256);
			hash.addData(dialog->textValue().toUtf8());
			if(hash.result().toHex() == Settings::settingsLockPasswd())
			{
				// Password verification succeeded
				ui->oldSettingsPasswdLabel->hide();
				ui->oldSettingsPasswdEdit->hide();
				ui->newSettingsPasswdLabel->hide();
				ui->newSettingsPasswdEdit->hide();
				ui->repeatSettingsPasswdLabel->hide();
				ui->repeatSettingsPasswdEdit->hide();
				ui->applySettingsLockButton->hide();
				Settings::setSettingsLockPasswd("-"); // "-" means that there's no password set
				Settings::setSettingsLockEnabled(false);
			}
			else
			{
				// Password verification failed
				ui->lockSettingsCheckBox->setChecked(true);
				QMessageBox::warning(this, QString(), tr("Incorrect password!"));
			}
		});
		connect(dialog, &QDialog::rejected, this, [this]() {
			ui->lockSettingsCheckBox->setChecked(true); // Password verification aborted
		});
		dialog->open();
	}
	else
	{
		// User has disabled settings lock and there isn't any existing password
		ui->oldSettingsPasswdLabel->hide();
		ui->oldSettingsPasswdEdit->hide();
		ui->newSettingsPasswdLabel->hide();
		ui->newSettingsPasswdEdit->hide();
		ui->repeatSettingsPasswdLabel->hide();
		ui->repeatSettingsPasswdEdit->hide();
		ui->applySettingsLockButton->hide();
	}
}

/*! Sets the settings lock password. */
void BehaviorOptions::setSettingsPassword(void)
{
	bool enabling = true;
	if(Settings::containsSettingsLockPasswd() && (Settings::settingsLockPasswd() != "-")) // "-" means that there's no password set
	{
		// User is changing existing password, verify it first
		QCryptographicHash hash(QCryptographicHash::Sha256);
		hash.addData(ui->oldSettingsPasswdEdit->text().toUtf8());
		if(hash.result().toHex() != Settings::settingsLockPasswd())
		{
			QMessageBox::warning(this, QString(), tr("Incorrect password!"));
			return;
		}
		enabling = false;
	}
	if(ui->newSettingsPasswdEdit->text() != ui->repeatSettingsPasswdEdit->text())
	{
		QMessageBox::warning(this, QString(), tr("The passwords do not match."));
		return;
	}
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(ui->newSettingsPasswdEdit->text().toUtf8());
	Settings::setSettingsLockPasswd(hash.result().toHex());
	Settings::setSettingsLockEnabled(true);
	ui->oldSettingsPasswdEdit->clear();
	ui->newSettingsPasswdEdit->clear();
	ui->repeatSettingsPasswdEdit->clear();
	ui->oldSettingsPasswdLabel->show();
	ui->oldSettingsPasswdEdit->show();
	if(enabling)
		QMessageBox::information(this, QString(), tr("Settings lock enabled"));
	else
		QMessageBox::information(this, QString(), tr("Password changed"));
}
