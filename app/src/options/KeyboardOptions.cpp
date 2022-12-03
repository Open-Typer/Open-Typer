/*
 * KeyboardOptions.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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

#include "options/KeyboardOptions.h"
#include "ui_KeyboardOptions.h"

#include <QDirIterator>
#include "BuiltInPacks.h"
#include "Settings.h"

/*! Constructs KeyboardOptions object. */
KeyboardOptions::KeyboardOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::KeyboardOptions)
{
	ui->setupUi(this);
	// Load packs
	QDirIterator it(":/res/configs/", QDirIterator::NoIteratorFlags);
	int i;
	QString item, current;
	QStringList items;
	rawItems.clear();
	while(it.hasNext())
	{
		item = it.next();
		current = "";
		for(i = 14; i < item.count(); i++)
			current += item[i];
		rawItems += current;
		items += BuiltInPacks::packName(current);
	}
	ui->packList->addItems(items);
	// Get current pack
	if(!Settings::customLessonPack())
		ui->packList->setCurrentItem(ui->packList->item(rawItems.indexOf(Settings::lessonPack())));
	// Connections
	connect(ui->packList, &QListWidget::currentRowChanged, this, &KeyboardOptions::changePack);
}

/*! Destroys the KeyboardOptions object. */
KeyboardOptions::~KeyboardOptions()
{
	delete ui;
}

/*!
 * Connected from packList->currentRowChanged().\n
 * Sets selected pack based on the value in packList and saves it in the settings.
 */
void KeyboardOptions::changePack(int index)
{
	Settings::setLessonPack(rawItems[index]);
	Settings::setCustomLessonPack(false);
}
