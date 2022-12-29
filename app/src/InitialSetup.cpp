/*
 * InitialSetup.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#include "InitialSetup.h"
#include "ui_InitialSetup.h"

/*! Constructs InitialSetup. */
InitialSetup::InitialSetup(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InitialSetup)
{
	ui->setupUi(this);
	loadList();
	AppearanceOptions *options = new AppearanceOptions(ui->themeFrame);
	options->hideModeSelector();
	if(palette().color(QPalette::Base).toHsv().value() < 50)
		options->setSimpleTheme(AppearanceOptions::SimpleTheme::Dark);
	else
		options->setSimpleTheme(AppearanceOptions::SimpleTheme::Light);
	ui->customizationLayout->addWidget(options);
	// Connections
	connect(ui->okButton, &QPushButton::clicked, this, &InitialSetup::finish);
	connect(ui->showAllLayoutsCheckBox, &QCheckBox::stateChanged, this, &InitialSetup::loadList);
}

/*! Destroys the InitialSetup object. */
InitialSetup::~InitialSetup()
{
	delete ui;
}

/*! Saves settings and closes the dialog. */
void InitialSetup::finish(void)
{
	Settings::setLessonPack(rawItems[ui->packList->currentRow()]);
	Settings::setCustomLessonPack(false);
	accept();
}

/*! Loads list of packs. */
void InitialSetup::loadList(void)
{
	ui->packList->clear();
	QDirIterator it(":/res/configs/", QDirIterator::NoIteratorFlags);
	QString item, current;
	QStringList items;
	rawItems.clear();
	while(it.hasNext())
	{
		item = it.next();
		current = "";
		for(int i = 14; i < item.count(); i++)
			current += item[i];
		rawItems += current;
	}
	if(!ui->showAllLayoutsCheckBox->isChecked())
	{
		QStringList itemsToRemove;
		itemsToRemove.clear();
		for(int i = 0; i < rawItems.count(); i++)
		{
			QString localeStr = "";
			for(int i2 = 0; i2 < rawItems[i].count(); i2++)
			{
				if(rawItems[i][i2] == '-')
					break;
				else
					localeStr += rawItems[i][i2];
			}
			QLocale packLocale(localeStr);
			QLocale systemLocale = QLocale::system();
			if(!((packLocale.language() == systemLocale.language()) && (packLocale.country() == systemLocale.country())))
				itemsToRemove += rawItems[i];
		}
		for(int i = 0; i < itemsToRemove.count(); i++)
			rawItems.removeAll(itemsToRemove[i]);
	}
	items.clear();
	for(int i = 0; i < rawItems.count(); i++)
		items += BuiltInPacks::packName(rawItems[i]);
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
