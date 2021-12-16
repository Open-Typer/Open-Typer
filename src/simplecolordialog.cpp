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

/*! Constructs SimpleColorDialog. */
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

/*! Destroys the SimpleColorDialog object. */
SimpleColorDialog::~SimpleColorDialog()
{
	delete ui;
}

/*! Sets initial color. */
void SimpleColorDialog::setColor(int r, int g, int b)
{
	redColor = r;
	greenColor = g;
	blueColor = b;
	updateColor();
}

/*! Updates color using the redColor, greenColor and blueColor variables. */
void SimpleColorDialog::updateColor(void)
{
	ui->colorFrame->setStyleSheet("background-color: rgb(" + QString::number(redColor) + ", " + QString::number(greenColor) + ", " + QString::number(blueColor) + ")");
	ui->redBox->setValue(redColor);
	ui->greenBox->setValue(greenColor);
	ui->blueBox->setValue(blueColor);
}

/*! Connected from redBox->valueChanged(). */
void SimpleColorDialog::setRedColorValue(int val)
{
	redColor = val;
	updateColor();
}

/*! Connected from greenBox->valueChanged(). */
void SimpleColorDialog::setGreenColorValue(int val)
{
	greenColor = val;
	updateColor();
}

/*! Connected from blueBox->valueChanged(). */
void SimpleColorDialog::setBlueColorValue(int val)
{
	blueColor = val;
	updateColor();
}

/*! Connected from redColorButton->clicked(). */
void SimpleColorDialog::setRedColor(void)
{
	redColor = 255;
	greenColor = 0;
	blueColor = 0;
	updateColor();
}

/*! Connected from brownColorButton->clicked(). */
void SimpleColorDialog::setBrownColor(void)
{
	redColor = 62;
	greenColor = 20;
	blueColor = 0;
	updateColor();
}

/*! Connected from orangeColorButton->clicked(). */
void SimpleColorDialog::setOrangeColor(void)
{
	redColor = 255;
	greenColor = 85;
	blueColor = 0;
	updateColor();
}

/*! Connected from yellowColorButton->clicked(). */
void SimpleColorDialog::setYellowColor(void)
{
	redColor = 255;
	greenColor = 255;
	blueColor = 0;
	updateColor();
}

/*! Connected from greenColorButton->clicked(). */
void SimpleColorDialog::setGreenColor(void)
{
	redColor = 0;
	greenColor = 255;
	blueColor = 0;
	updateColor();
}

/*! Connected from lightBlueColorButton->clicked(). */
void SimpleColorDialog::setLightBlueColor(void)
{
	redColor = 0;
	greenColor = 170;
	blueColor = 255;
	updateColor();
}

/*! Connected from blueColorButton->clicked(). */
void SimpleColorDialog::setBlueColor(void)
{
	redColor = 0;
	greenColor = 0;
	blueColor = 255;
	updateColor();
}

/*! Connected from pinkColorButton->clicked(). */
void SimpleColorDialog::setPinkColor(void)
{
	redColor = 255;
	greenColor = 0;
	blueColor = 255;
	updateColor();
}

/*! Connected from purpleColorButton->clicked(). */
void SimpleColorDialog::setPurpleColor(void)
{
	redColor = 85;
	greenColor = 0;
	blueColor = 127;
	updateColor();
}

/*! Connected from blackColorButton->clicked(). */
void SimpleColorDialog::setBlackColor(void)
{
	redColor = 0;
	greenColor = 0;
	blueColor = 0;
	updateColor();
}

/*! Connected from grayColorButton->clicked(). */
void SimpleColorDialog::setGrayColor(void)
{
	redColor = 100;
	greenColor = 100;
	blueColor = 100;
	updateColor();
}

/*! Connected from whiteColorButton->clicked(). */
void SimpleColorDialog::setWhiteColor(void)
{
	redColor = 255;
	greenColor = 255;
	blueColor = 255;
	updateColor();
}
