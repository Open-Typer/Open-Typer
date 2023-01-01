/*
 * OptionsWindow.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2023 - adazem009
 * Copyright (C) 2022 - yeti0904
 * Copyright (C) 2022 - Roker2
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

#include "options/OptionsWindow.h"
#include "ui_OptionsWindow.h"

#include <QInputDialog>
#include <QCryptographicHash>
#include <QPushButton>
#include "AddonApi.h"
#include "Settings.h"
#include "options/BehaviorOptions.h"
#include "options/KeyboardOptions.h"

/*! Constructs OptionsWindow. */
OptionsWindow::OptionsWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::OptionsWindow)
{
	ui->setupUi(this);
	if(Settings::settingsLockEnabled())
		ui->widget->setEnabled(false);
	else
	{
		ui->settingsLockLabel->hide();
		ui->unlockSettingsButton->hide();
	}
	setupList();
	// Disable focus of cancel button so that it doesn't react to return/enter key
	ui->buttonBox->button(QDialogButtonBox::Close)->setFocusPolicy(Qt::NoFocus);
	// Connections
	connect(ui->unlockSettingsButton, &QPushButton::clicked, this, &OptionsWindow::unlockSettings);
	connect(ui->list, &QListWidget::currentRowChanged, this, &OptionsWindow::changeOptionWidget);
	connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &OptionsWindow::accept);
}

/*! Sets up list of categories. */
void OptionsWindow::setupList(void)
{
	const int oldIndex = ui->list->currentRow();
	ui->list->clear();
	ui->list->setIconSize(QSize(36, 36));
	// Register category classes
	qRegisterMetaType<BehaviorOptions *>("BehaviorOptions");
	qRegisterMetaType<KeyboardOptions *>("KeyboardOptions");
	// Add default categories
	AddonApi::clearSettingsCategories();
	AddonApi::addSettingsCategory(tr("Behavior"), QIcon(":res/images/BehaviorOptions.svg"), "BehaviorOptions");
	AddonApi::addSettingsCategory(tr("Keyboard"), QIcon(":res/images/KeyboardOptions.svg"), "KeyboardOptions");
	// Add addon categories
	AddonApi::initSettingsCategories();
	QList<QVariantMap> categories = AddonApi::settingsCategories();
	for(const auto &category : categories)
		ui->list->addItem(new QListWidgetItem(category["icon"].value<QIcon>(), category["name"].toString()));
	ui->list->setCurrentRow(oldIndex);
}

/*! Destroys the OptionsWindow object. */
OptionsWindow::~OptionsWindow()
{
	delete ui;
}

/*! Opens selected category. */
void OptionsWindow::changeOptionWidget(int index)
{
	if(index < 0)
		return;
	QWidget *options = nullptr;
	Q_ASSERT(index >= 0 && index < AddonApi::settingsCategories().count());
	QVariantMap category = AddonApi::settingsCategories().at(index);
	QString categoryName = category["name"].toString();
	QString className = category["class"].toString();
	int id = QMetaType::type(QString(className + "*").toStdString().c_str());
	if(id != QMetaType::UnknownType)
	{
		const QMetaObject *metaObject = QMetaType::metaObjectForType(id);
		options = qobject_cast<QWidget *>(metaObject->newInstance());
		if(options == nullptr)
			QMessageBox::critical(nullptr, "Error", QString("Could not create settings page \"%1\": is %2's constructor declared with Q_INVOKABLE?").arg(categoryName, className));
	}
	if(options == nullptr)
		return;
	options->setParent(ui->currentOptions);
	if(currentWidget != nullptr)
	{
		ui->currentOptionsLayout->removeWidget(currentWidget);
		currentWidget->deleteLater();
	}
	currentWidget = options;
	ui->currentOptionsLayout->addWidget(currentWidget);
}

/*! Selects default category. */
void OptionsWindow::init(void)
{
	ui->list->setCurrentRow(0);
}

/*! Overrides QWidget#changeEvent().
 * Retranslates UI when the display language changes.
 */
void OptionsWindow::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::LanguageChange)
	{
		ui->retranslateUi(this);
		setupList();
	}
	else
		QWidget::changeEvent(event);
}

/*! Asks for a password and unlocks settings. */
void OptionsWindow::unlockSettings(void)
{
	if(Settings::containsSettingsLockPasswd() && (Settings::settingsLockPasswd() != "-")) // "-" means that there's no password set
	{
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
				ui->settingsLockLabel->hide();
				ui->unlockSettingsButton->hide();
				ui->widget->setEnabled(true);
			}
			else
				QMessageBox::warning(this, QString(), tr("Incorrect password!"));
		});
		dialog->open();
	}
}
