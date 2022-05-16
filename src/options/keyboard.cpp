/*
 * keyboard.cpp
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

#include "options/keyboard.h"
#include "ui_keyboard.h"

/*! Constructs keyboardOptions object. */
keyboardOptions::keyboardOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::keyboardOptions),
	settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat)
{
	ui->setupUi(this);
	// Load packs
	QDirIterator it(":/res/configs/",QDirIterator::NoIteratorFlags);
	int i;
	QString item, current;
	QStringList items;
	rawItems.clear();
	while(it.hasNext())
	{
		item = it.next();
		current = "";
		for(i=14; i < item.count(); i++)
			current += item[i];
		rawItems += current;
		items += builtinPacks::packName(current);
	}
	ui->packList->addItems(items);
	// Get current pack
	// TODO: Detect default config
	if(!settings.value("main/customconfig","false").toBool())
		ui->packList->setCurrentItem(ui->packList->item(rawItems.indexOf(settings.value("main/configfile","sk_SK-QWERTZ-B1").toString())));
	// Connections
	connect(ui->editorButton,SIGNAL(clicked()),this,SLOT(openEditor()));
	connect(ui->packList,SIGNAL(currentRowChanged(int)),this,SLOT(changePack(int)));
}

/*! Destroys the keyboardOptions object. */
keyboardOptions::~keyboardOptions()
{
	delete ui;
}

/*!
 * Connected from editorButton->clicked().\n
 * Opens the editor window.
 * \see packEditor
 */
void keyboardOptions::openEditor(void)
{
	packEditor *editorWindow = new packEditor(this);
	editorWindow->setWindowFlag(Qt::WindowMinimizeButtonHint,true);
	editorWindow->setWindowFlag(Qt::WindowMaximizeButtonHint,true);
	editorWindow->setWindowModality(Qt::WindowModal);
	editorWindow->show();
}

/*!
 * Connected from packList->currentRowChanged().\n
 * Sets selected pack based on the value in packList and saves it in the settings.
 */
void keyboardOptions::changePack(int index)
{
	settings.setValue("main/configfile",rawItems[index]);
	settings.setValue("main/customconfig","false");
}
