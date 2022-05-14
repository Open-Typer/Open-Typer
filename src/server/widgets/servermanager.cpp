/*
 * servermanager.cpp
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

#include "server/widgets/servermanager.h"
#include "ui_servermanager.h"

/*! Constructs serverManager. */
serverManager::serverManager(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::serverManager)
{
	ui->setupUi(this);
	init();
	collapse();
	// Connections
	connect(ui->toggleButton, &QToolButton::clicked, this, [this]() {
		if(expanded)
			collapse();
		else
			expand();
	});
	connect(ui->usersButton, &QToolButton::clicked, this, &serverManager::openUserManager);
	connect(ui->changeSchoolNameButton, &QToolButton::clicked, this, &serverManager::changeSchoolName);
	connect(ui->addClassButton, &QToolButton::clicked, this, &serverManager::addClass);
	connect(ui->classBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &serverManager::openClass);
	connect(ui->removeClassButton, &QToolButton::clicked, this, &serverManager::removeClass);
	connect(ui->editClassButton, &QToolButton::clicked, this, &serverManager::editClass);
}

/*! Destroys serverManager. */
serverManager::~serverManager()
{
	delete ui;
}

/*! Initializes server manager. */
bool serverManager::init(void)
{
	bool ret = true;
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	if(settings.contains("server/schoolname"))
	{
		if(settings.value("server/schoolname").toString() == "")
			settings.setValue("server/schoolname", "?");
		ui->schoolName->setText(settings.value("server/schoolname").toString());
	}
	else
		ui->schoolName->setText("");
	if(dbMgr.administratorIDs().count() == 0)
	{
		serverSetup *dialog = new serverSetup(this);
		dialog->setWindowModality(Qt::WindowModal);
		dialog->open();
		connect(dialog, &QDialog::rejected, this, [this]() { ui->mainControlsFrame->setEnabled(false); });
		connect(dialog, &QDialog::accepted, this, [this]() { ui->mainControlsFrame->setEnabled(true); });
		ret = false;
	}
	QDirIterator icons(":/class-icons",QDirIterator::NoIteratorFlags);
	QStringList iconNames;
	while(icons.hasNext())
		iconNames += icons.next();
	iconNames.sort();
	int oldClass = 0, currentIndex = ui->classBox->currentIndex();
	if(currentIndex > 0)
		oldClass = classes[currentIndex-1];
	disableClassOpening = true;
	ui->classBox->clear();
	ui->classBox->addItem(tr("No class selected", "Displayed in the class selection combo box."));
	classes = dbMgr.classIDs(true);
	for(int i = 0; i < classes.count(); i++)
	{
		ui->classBox->addItem(dbMgr.className(classes[i]));
		ui->classBox->setItemIcon(i+1, QIcon(iconNames[dbMgr.classIcon(classes[i])]));
	}
	if(oldClass != 0)
		ui->classBox->setCurrentIndex(classes.indexOf(oldClass)+1);
	disableClassOpening = false;
	if((classes.count() == 0) || (ui->classBox->currentIndex() == 0))
	{
		ui->classBox->setVisible(classes.count() != 0);
		ui->removeClassButton->setEnabled(false);
		ui->editClassButton->setEnabled(false);
		ui->toggleButton->setEnabled(false);
		collapse();
	}
	else
	{
		ui->classBox->show();
		ui->removeClassButton->setEnabled(true);
		ui->editClassButton->setEnabled(true);
		ui->toggleButton->setEnabled(true);
	}
	return ret;
}

/*! Opens userManager. */
void serverManager::openUserManager(void)
{
	if(exerciseProgressDialogConfig::dialogCount > 0)
	{
		showCloseExercisesMessage();
		return;
	}
	adminSelector *selectDialog = new adminSelector(this);
	selectDialog->setWindowModality(Qt::WindowModal);
	selectDialog->open();
	connect(selectDialog, &QDialog::accepted, this, [selectDialog, this]() {
		if(dbMgr.auth(selectDialog->userID))
		{
			userManager *dialog = new userManager(this);
			dialog->setWindowModality(Qt::WindowModal);
			dialog->open();
			connect(dialog, &QDialog::finished, this, &serverManager::init);
		}
	});
}

/*! Hides class controls. */
void serverManager::collapse(void)
{
	ui->toggleButton->setIcon(QIcon(":/res/images/up.png"));
	if(!expanded)
		return;
	expanded = false;
	ui->classControlsFrame->hide();
	emit widgetCollapsed();
}

/*! Shows class controls. */
void serverManager::expand(void)
{
	ui->toggleButton->setIcon(QIcon(":/res/images/down.png"));
	if(expanded)
		return;
	expanded = true;
	ui->classControlsFrame->show();
	emit widgetExpanded();
}

/*! Allows the user to change school name. */
void serverManager::changeSchoolName(void)
{
	ui->schoolName->setReadOnly(false);
	ui->schoolName->setFocus();
	ui->schoolName->setSelection(0, ui->schoolName->text().count());
	ui->changeSchoolNameButton->setEnabled(false);
	connect(qApp, &QApplication::focusChanged, this, [this](QWidget *old) {
		if(old == ui->schoolName)
		{
			ui->schoolName->setReadOnly(true);
			if(ui->schoolName->text() != "")
			{
				adminSelector *selectDialog = new adminSelector(this);
				selectDialog->setWindowModality(Qt::WindowModal);
				selectDialog->open();
				connect(selectDialog, &QDialog::finished, this, [selectDialog, this]() {
					if((selectDialog->result() == QDialog::Accepted) && dbMgr.auth(selectDialog->userID))
					{
						QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
						settings.setValue("server/schoolname", ui->schoolName->text());
					}
					ui->changeSchoolNameButton->setEnabled(true);
					init();
				});
			}
		}
		qApp->disconnect(this);
	});
}

/*! Opens classEdit and creates a class. */
void serverManager::addClass(void)
{
	if(exerciseProgressDialogConfig::dialogCount > 0)
	{
		showCloseExercisesMessage();
		return;
	}
	classEdit *dialog = new classEdit(true, 1, this);
	dialog->setWindowModality(Qt::WindowModal);
	dialog->open();
	connect(dialog, &QDialog::accepted, this, [this]() {
		init();
		disableClassOpening = true;
		ui->classBox->setCurrentIndex(1);
		disableClassOpening = false;
		openClass(false);
	});
}

/*! Opens selected class */
void serverManager::openClass(bool auth)
{
	if((exerciseProgressDialogConfig::dialogCount > 0) && !disableClassOpening)
	{
		showCloseExercisesMessage();
		disableClassOpening = true;
		ui->classBox->setCurrentIndex(lastClassBoxIndex);
		disableClassOpening = false;
		return;
	}
	lastClassBoxIndex = ui->classBox->currentIndex();
	if(disableClassOpening)
		return;
	int selected = ui->classBox->currentIndex()-1;
	if(selected == -1)
	{
		ui->classControlsFrame->hide();
		dbMgr.activeClass = 0;
	}
	else
	{
		if(!auth || dbMgr.auth(dbMgr.classOwner(classes[selected])))
		{
			ui->classControlsFrame->setVisible(expanded);
			dbMgr.updateClassTimestamp(classes[selected]);
			classControls *controlsWidget = new classControls(classes[selected], ui->classControlsFrame);
			controlsWidget->setAttribute(Qt::WA_DeleteOnClose);
			if(ui->classControlsLayout->count() > 0)
			{
				QWidget *oldWidget = ui->classControlsLayout->itemAt(0)->widget();
				oldWidget->close();
			}
			ui->classControlsLayout->addWidget(controlsWidget);
			connect(controlsWidget, &classControls::detailsClicked, this, &serverManager::openDetails);
			dbMgr.activeClass = classes[selected];
		}
		else
			ui->classBox->setCurrentIndex(0);
	}
	init();
}

/*! Opens student details. \see studentDetails */
void serverManager::openDetails(int studentID)
{
	studentDetails *detailsWidget = new studentDetails(classes[ui->classBox->currentIndex()-1], studentID, ui->classControlsFrame);
	detailsWidget->setAttribute(Qt::WA_DeleteOnClose);
	QWidget *oldWidget = ui->classControlsLayout->itemAt(0)->widget();
	oldWidget->close();
	ui->classControlsLayout->addWidget(detailsWidget);
	connect(detailsWidget, &studentDetails::backClicked, this, [this](){ openClass(false); });
}

/*! Removes selected class. */
void serverManager::removeClass(void)
{
	if(exerciseProgressDialogConfig::dialogCount > 0)
	{
		showCloseExercisesMessage();
		return;
	}
	int selectedClass = classes[ui->classBox->currentIndex()-1];
	if(QMessageBox::question(this, QString(), tr("Are you sure you want to remove class %1?").arg(dbMgr.className(selectedClass))) == QMessageBox::Yes)
	{
		if(dbMgr.auth(dbMgr.loginID()))
		{
			dbMgr.removeClass(selectedClass);
			init();
			openClass();
		}
	}
}

/*! Edits selected class. */
void serverManager::editClass(void)
{
	if(exerciseProgressDialogConfig::dialogCount > 0)
	{
		showCloseExercisesMessage();
		return;
	}
	classEdit *dialog = new classEdit(false, 1, this);
	dialog->setWindowModality(Qt::WindowModal);
	dialog->open();
	connect(dialog, &QDialog::accepted, this, [this]() { init(); openClass(false); });
}

/*! Shows a warning about opened exerciseProgressDialog dialogs. */
void serverManager::showCloseExercisesMessage(void)
{
	QMessageBox::warning(this, QString(), tr("Close opened windows first."));
}
