/*
 * initialsetup.cpp
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

#include "initialsetup.h"
#include "ui_initialsetup.h"

/*! Constructs initialSetup. */
initialSetup::initialSetup(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::initialSetup)
{
	ui->setupUi(this);
	loadList();
	customizationOptions *options = new customizationOptions(ui->themeFrame);
	ui->customizationLayout->addWidget(options);
	// Connections
	connect(ui->okButton,&QPushButton::clicked,this,&initialSetup::finish);
	connect(ui->showAllLayoutsCheckBox,&QCheckBox::stateChanged,this,&initialSetup::loadList);
}

/*! Destroys the initialSetup object. */
initialSetup::~initialSetup()
{
	delete ui;
}

/*! Saves settings and closes the dialog. */
void initialSetup::finish(void)
{
	settings.setValue("main/configfile",rawItems[ui->packList->currentRow()]);
	settings.setValue("main/customconfig",false);
	accept();
}

/*! Loads list of packs. */
void initialSetup::loadList(void)
{
	ui->packList->clear();
	QDirIterator it(":/res/configs/",QDirIterator::NoIteratorFlags);
	QString item, current;
	QStringList items;
	rawItems.clear();
	while(it.hasNext())
	{
		item = it.next();
		current = "";
		for(int i=14; i < item.count(); i++)
			current += item[i];
		rawItems += current;
	}
	if(!ui->showAllLayoutsCheckBox->isChecked())
	{
		QStringList itemsToRemove;
		itemsToRemove.clear();
		for(int i=0; i < rawItems.count(); i++)
		{
			QString localeStr = "";
			for(int i2=0; i2 < rawItems[i].count(); i2++)
			{
				if(rawItems[i][i2] == "-")
					break;
				else
					localeStr += rawItems[i][i2];
			}
			QLocale packLocale(localeStr);
			QLocale systemLocale = QLocale::system();
			if(!((packLocale.language() == systemLocale.language()) && (packLocale.country() == systemLocale.country())))
				itemsToRemove += rawItems[i];
		}
		for(int i=0; i < itemsToRemove.count(); i++)
			rawItems.removeAll(itemsToRemove[i]);
	}
	items.clear();
	for(int i=0; i < rawItems.count(); i++)
		items += builtinPacks::packName(rawItems[i]);
	ui->packList->addItems(items);
	if(ui->packList->count() > 0)
		ui->packList->setCurrentRow(0);
	else
	{
		ui->showAllLayoutsCheckBox->setChecked(true);
		ui->showAllLayoutsCheckBox->hide();
		loadList();
	}
}
