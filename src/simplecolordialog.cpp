/*
 * simplecolordialog.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

#include "simplecolordialog.h"
#include "ui_simplecolordialog.h"

SimpleColorDialog::SimpleColorDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SimpleColorDialog)
{
	ui->setupUi(this);
	// Quick selection
	connect(ui->redColorButton,SIGNAL(clicked()),this,SLOT(setRedColor()));
	connect(ui->brownColorButton,SIGNAL(clicked()),this,SLOT(setBrownColor()));
	connect(ui->orangeColorButton,SIGNAL(clicked()),this,SLOT(setOrangeColor()));
	connect(ui->yellowColorButton,SIGNAL(clicked()),this,SLOT(setYellowColor()));
	connect(ui->greenColorButton,SIGNAL(clicked()),this,SLOT(setGreenColor()));
	connect(ui->lightBlueColorButton,SIGNAL(clicked()),this,SLOT(setLightBlueColor()));
	connect(ui->blueColorButton,SIGNAL(clicked()),this,SLOT(setBlueColor()));
	connect(ui->pinkColorButton,SIGNAL(clicked()),this,SLOT(setPinkColor()));
	connect(ui->purpleColorButton,SIGNAL(clicked()),this,SLOT(setPurpleColor()));
	connect(ui->blackColorButton,SIGNAL(clicked()),this,SLOT(setBlackColor()));
	connect(ui->grayColorButton,SIGNAL(clicked()),this,SLOT(setGrayColor()));
	connect(ui->whiteColorButton,SIGNAL(clicked()),this,SLOT(setWhiteColor()));
	// Advanced
	connect(ui->redBox,SIGNAL(valueChanged(int)),this,SLOT(setRedColorValue(int)));
	connect(ui->greenBox,SIGNAL(valueChanged(int)),this,SLOT(setGreenColorValue(int)));
	connect(ui->blueBox,SIGNAL(valueChanged(int)),this,SLOT(setBlueColorValue(int)));
	// Buttons
	connect(ui->applyButton,SIGNAL(clicked()),this,SLOT(accept()));
	connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
}

SimpleColorDialog::~SimpleColorDialog()
{
	delete ui;
}

void SimpleColorDialog::setColor(int r, int g, int b)
{
	redColor = r;
	greenColor = g;
	blueColor = b;
	updateColor();
}

void SimpleColorDialog::updateColor(void)
{
	char *styleSheet = (char*) malloc(128);
	sprintf(styleSheet,"background-color: rgb(%d, %d, %d)",redColor,greenColor,blueColor);
	ui->colorFrame->setStyleSheet(styleSheet);
	ui->redBox->setValue(redColor);
	ui->greenBox->setValue(greenColor);
	ui->blueBox->setValue(blueColor);
}

void SimpleColorDialog::setRedColorValue(int val)
{
	redColor = val;
	updateColor();
}

void SimpleColorDialog::setGreenColorValue(int val)
{
	greenColor = val;
	updateColor();
}

void SimpleColorDialog::setBlueColorValue(int val)
{
	blueColor = val;
	updateColor();
}

void SimpleColorDialog::setRedColor(void)
{
	redColor = 255;
	greenColor = 0;
	blueColor = 0;
	updateColor();
}

void SimpleColorDialog::setBrownColor(void)
{
	redColor = 62;
	greenColor = 20;
	blueColor = 0;
	updateColor();
}

void SimpleColorDialog::setOrangeColor(void)
{
	redColor = 255;
	greenColor = 85;
	blueColor = 0;
	updateColor();
}

void SimpleColorDialog::setYellowColor(void)
{
	redColor = 255;
	greenColor = 255;
	blueColor = 0;
	updateColor();
}

void SimpleColorDialog::setGreenColor(void)
{
	redColor = 0;
	greenColor = 255;
	blueColor = 0;
	updateColor();
}

void SimpleColorDialog::setLightBlueColor(void)
{
	redColor = 0;
	greenColor = 170;
	blueColor = 255;
	updateColor();
}

void SimpleColorDialog::setBlueColor(void)
{
	redColor = 0;
	greenColor = 0;
	blueColor = 255;
	updateColor();
}

void SimpleColorDialog::setPinkColor(void)
{
	redColor = 255;
	greenColor = 0;
	blueColor = 255;
	updateColor();
}

void SimpleColorDialog::setPurpleColor(void)
{
	redColor = 85;
	greenColor = 0;
	blueColor = 127;
	updateColor();
}

void SimpleColorDialog::setBlackColor(void)
{
	redColor = 0;
	greenColor = 0;
	blueColor = 0;
	updateColor();
}

void SimpleColorDialog::setGrayColor(void)
{
	redColor = 100;
	greenColor = 100;
	blueColor = 100;
	updateColor();
}

void SimpleColorDialog::setWhiteColor(void)
{
	redColor = 255;
	greenColor = 255;
	blueColor = 255;
	updateColor();
}
