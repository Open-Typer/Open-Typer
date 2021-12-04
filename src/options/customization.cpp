/*
 * customization.cpp
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

#include "options/customization.h"
#include "ui_customization.h"

customizationOptions::customizationOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::customizationOptions)
{
	ui->setupUi(this);
	settings = new QSettings(fileUtils::configLocation()+"/config.ini",QSettings::IniFormat);
	// Font
	setFont(settings->value("theme/font","Courier").toString(),
		settings->value("theme/fontsize","14").toInt(),
		settings->value("theme/fontbold","false").toBool(),
		settings->value("theme/fontitalic","false").toBool(),
		settings->value("theme/fontunderline","false").toBool());
	// Colors
	// Level text
	customLevelTextColor = settings->value("theme/customleveltextcolor","false").toBool();
	levelTextRedColor = settings->value("theme/leveltextred","0").toInt();
	levelTextGreenColor = settings->value("theme/leveltextgreen","0").toInt();
	levelTextBlueColor = settings->value("theme/leveltextblue","0").toInt();
	// Input text
	customInputTextColor = settings->value("theme/custominputtextcolor","false").toBool();
	inputTextRedColor = settings->value("theme/inputtextred","0").toInt();
	inputTextGreenColor = settings->value("theme/inputtextgreen","0").toInt();
	inputTextBlueColor = settings->value("theme/inputtextblue","0").toInt();
	// Background
	customBgColor = settings->value("theme/custombgcolor","false").toBool();
	bgRedColor = settings->value("theme/bgred","0").toInt();
	bgGreenColor = settings->value("theme/bggreen","0").toInt();
	bgBlueColor = settings->value("theme/bgblue","0").toInt();
	// Paper
	customPaperColor = settings->value("theme/custompapercolor","false").toBool();
	paperRedColor = settings->value("theme/paperred","0").toInt();
	paperGreenColor = settings->value("theme/papergreen","0").toInt();
	paperBlueColor = settings->value("theme/paperblue","0").toInt();
	// Panel
	customPanelColor = settings->value("theme/custompanelcolor","false").toBool();
	panelRedColor = settings->value("theme/panelred","0").toInt();
	panelGreenColor = settings->value("theme/panelgreen","0").toInt();
	panelBlueColor = settings->value("theme/panelblue","0").toInt();
	// Font selector
	connect(ui->fontComboBox,
		SIGNAL(currentFontChanged(QFont)),
		this,
		SLOT(changeFont(QFont)));
	// Font size box
	connect(ui->fontSizeBox,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(changeFontSize(int)));
	// Bold text button
	connect(ui->boldTextBox,
		SIGNAL(clicked()),
		this,
		SLOT(setBoldText()));
	// Italic text button
	connect(ui->italicTextBox,
		SIGNAL(clicked()),
		this,
		SLOT(setItalicText()));
	// Underline text button
	connect(ui->underlineTextBox,
		SIGNAL(clicked()),
		this,
		SLOT(setUnderlineText()));
	// Reset font button
	connect(ui->fontResetButton,
		SIGNAL(clicked()),
		this,
		SLOT(resetFont()));
	// Change level text color button
	connect(ui->levelTextColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(changeLevelTextColor()));
	// Change input text color button
	connect(ui->inputTextColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(changeInputTextColor()));
	// Reset text color button
	connect(ui->resetTextColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(resetTextColors()));
	// Change background color button
	connect(ui->bgColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(changeBgColor()));
	// Change paper color button
	connect(ui->paperColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(changePaperColor()));
	// Change panel color button
	connect(ui->panelColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(changePanelColor()));
	// Reset background, paper and panel color button
	connect(ui->resetBgPaperColorButton,
		SIGNAL(clicked()),
		this,
		SLOT(resetBgPaperColors()));
	// Theme selector
	connect(ui->themeBox,
		SIGNAL(activated(int)),
		this,
		SLOT(changeTheme(int)));
}

customizationOptions::~customizationOptions()
{
	delete ui;
}

void customizationOptions::init(void)
{
	ui->themeBox->setCurrentIndex(settings->value("theme/theme","0").toInt());
	setColors();
}

void customizationOptions::setFont(QString fontFamily, int fontSize, bool fontBold, bool fontItalic, bool fontUnderline)
{
	QFont newFont, mistakeLabelFont;
	// Set font
	newFont.setFamily(fontFamily);
	newFont.setPointSize(fontSize);
	newFont.setBold(fontBold);
	newFont.setItalic(fontItalic);
	newFont.setUnderline(fontUnderline);
	// Update input widgets
	ui->fontComboBox->setCurrentFont(newFont);
	ui->fontSizeBox->setValue(fontSize);
	ui->boldTextBox->setChecked(fontBold);
	ui->italicTextBox->setChecked(fontItalic);
	ui->underlineTextBox->setChecked(fontUnderline);
	// Update labels
	ui->levelLabel->setFont(newFont);
	ui->inputLabel->setFont(newFont);
	// Save settings
	settings->setValue("theme/font",fontFamily);
	settings->setValue("theme/fontsize",fontSize);
	settings->setValue("theme/fontbold",fontBold);
	settings->setValue("theme/fontitalic",fontItalic);
	settings->setValue("theme/fontunderline",fontUnderline);
}

void customizationOptions::changeFont(QFont font)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setFamily(font.family());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	settings->setValue("theme/font",font.family());
}

void customizationOptions::changeFontSize(int size)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setPointSize(size);
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	settings->setValue("theme/fontsize",size);
}

void customizationOptions::setBoldText(void)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setBold(ui->boldTextBox->isChecked());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	settings->setValue("theme/fontbold",ui->boldTextBox->isChecked());
}

void customizationOptions::setItalicText(void)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setItalic(ui->italicTextBox->isChecked());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	settings->setValue("theme/fontitalic",ui->italicTextBox->isChecked());
}

void customizationOptions::setUnderlineText(void)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setUnderline(ui->underlineTextBox->isChecked());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	settings->setValue("theme/fontunderline",ui->underlineTextBox->isChecked());
}

void customizationOptions::resetFont(void)
{
	// Default font
	setFont("Courier", 	// Family
		14,		// Point size
		false,		// Bold
		false,		// Italic
		false);	// Underline
}

void customizationOptions::saveColorSettings(void)
{
	// Level text
	settings->setValue("theme/customleveltextcolor",customLevelTextColor);
	settings->setValue("theme/leveltextred",levelTextRedColor);
	settings->setValue("theme/leveltextgreen",levelTextGreenColor);
	settings->setValue("theme/leveltextblue",levelTextBlueColor);
	// Input text
	settings->setValue("theme/custominputtextcolor",customInputTextColor);
	settings->setValue("theme/inputtextred",inputTextRedColor);
	settings->setValue("theme/inputtextgreen",inputTextGreenColor);
	settings->setValue("theme/inputtextblue",inputTextBlueColor);
	// Background
	settings->setValue("theme/custombgcolor",customBgColor);
	settings->setValue("theme/bgred",bgRedColor);
	settings->setValue("theme/bggreen",bgGreenColor);
	settings->setValue("theme/bgblue",bgBlueColor);
	// Paper
	settings->setValue("theme/custompapercolor",customPaperColor);
	settings->setValue("theme/paperred",paperRedColor);
	settings->setValue("theme/papergreen",paperGreenColor);
	settings->setValue("theme/paperblue",paperBlueColor);
	// Panel
	settings->setValue("theme/custompanelcolor",customPanelColor);
	settings->setValue("theme/panelred",panelRedColor);
	settings->setValue("theme/panelgreen",panelGreenColor);
	settings->setValue("theme/panelblue",panelBlueColor);
}

void customizationOptions::setColors()
{
	// Reset style sheets
	ui->inputLabel->setStyleSheet("");
	ui->previewFrame->setStyleSheet("");
	ui->paper->setStyleSheet("");
	// Update theme
	updateTheme();
	// Set level text color
	if(customLevelTextColor)
		ui->levelLabel->setStyleSheet("color: rgb(" + QString::number(levelTextRedColor) + ", " + QString::number(levelTextGreenColor) + ", " + QString::number(levelTextBlueColor) + ")");
	else
	{
		// Default level text color
		levelTextRedColor = 0;
		levelTextGreenColor = 125;
		levelTextBlueColor = 175;
		ui->levelLabel->setStyleSheet("color: rgb(0, 125, 175)");
	}
	// Set input text color
	if(customInputTextColor)
		ui->inputLabel->setStyleSheet("color: rgb(" + QString::number(inputTextRedColor) + ", " + QString::number(inputTextGreenColor) + ", " + QString::number(inputTextBlueColor) + ")");
	else
	{
		// Default input text color
		inputTextRedColor = ui->inputLabel->palette().color(QPalette::Text).red();
		inputTextGreenColor = ui->inputLabel->palette().color(QPalette::Text).green();
		inputTextBlueColor = ui->inputLabel->palette().color(QPalette::Text).blue();
		ui->inputLabel->setStyleSheet("color: rgb(" + QString::number(inputTextRedColor) + ", " + QString::number(inputTextGreenColor) + ", " + QString::number(inputTextBlueColor) + ")");
	}
	// Set paper color
	if(customPaperColor)
		ui->paper->setStyleSheet("background-color: rgb(" + QString::number(paperRedColor) + ", " + QString::number(paperGreenColor) + ", " + QString::number(paperBlueColor) + ")");
	else
	{
		// Reset background color before setting paper color
		ui->previewFrame->setStyleSheet("");
		// Default paper color
		paperRedColor = ui->paper->palette().color(QPalette::Base).red();
		paperGreenColor = ui->paper->palette().color(QPalette::Base).green();
		paperBlueColor = ui->paper->palette().color(QPalette::Base).blue();
		ui->paper->setStyleSheet("background-color: rgb(" + QString::number(paperRedColor) + ", " + QString::number(paperGreenColor) + ", " + QString::number(paperBlueColor) + ")");
		// Fix inputLabel automatically set background color
		ui->inputLabel->setStyleSheet(
			ui->inputLabel->styleSheet() + ";\nbackground-color: rgba(0,0,0,0)");
	}
	// Set background color
	if(customBgColor)
		ui->previewFrame->setStyleSheet("background-color: rgb(" + QString::number(bgRedColor) + ", " + QString::number(bgGreenColor) + ", " + QString::number(bgBlueColor) + ")");
	else
	{
		// Default background color
		bgRedColor = ui->previewFrame->palette().color(QPalette::Window).red();
		bgGreenColor = ui->previewFrame->palette().color(QPalette::Window).green();
		bgBlueColor = ui->previewFrame->palette().color(QPalette::Window).blue();
	}
}

void customizationOptions::updateTheme()
{
	QFile darkSheet(":/dark-theme/style.qss");
	QFile lightSheet(":/light-theme/style.qss");
	char *paperStyleSheet;
	switch(settings->value("theme/theme","0").toInt()) {
		case 0:
			// System (default)
			setStyleSheet("");
			paperStyleSheet = (char*) malloc(128);
			sprintf(paperStyleSheet,"background-color: rgb(%d,%d,%d)",
				palette().color(QPalette::Base).red(),
				palette().color(QPalette::Base).green(),
				palette().color(QPalette::Base).blue());
			ui->paper->setStyleSheet(paperStyleSheet);
			break;
		case 1:
			// Dark
			if(darkSheet.exists())
			{
				darkSheet.open(QFile::ReadOnly | QFile::Text);
				QTextStream ts(&darkSheet);
				setStyleSheet(ts.readAll());
			}
			else
			{
				printf("D: Failed to open dark style\n");
				darkSheet.open(QFile::ReadOnly | QFile::Text);
				qDebug() << darkSheet.errorString();
			}
			ui->paper->setStyleSheet("background-color: rgb(15, 25, 35)");
			break;
		case 2:
			// Light
			if(lightSheet.exists())
			{
				lightSheet.open(QFile::ReadOnly | QFile::Text);
				QTextStream ts(&lightSheet);
				setStyleSheet(ts.readAll());
			}
			else
			{
				printf("D: Failed to open light style\n");
			}
			ui->paper->setStyleSheet("background-color: rgb(255, 255, 255)");
			break;
	}
	parentWidget()->parentWidget()->parentWidget()->setStyleSheet(styleSheet());
}

void customizationOptions::changeLevelTextColor(void)
{
	SimpleColorDialog colorDialog;
	colorDialog.setColor(levelTextRedColor,
		levelTextGreenColor,
		levelTextBlueColor);
	colorDialog.setStyleSheet(styleSheet());
	if(colorDialog.exec() == QDialog::Accepted)
	{
		levelTextRedColor = colorDialog.redColor;
		levelTextGreenColor = colorDialog.greenColor;
		levelTextBlueColor = colorDialog.blueColor;
		customLevelTextColor = true;
		saveColorSettings();
		setColors();
	}
}

void customizationOptions::changeInputTextColor(void)
{
	SimpleColorDialog colorDialog;
	colorDialog.setColor(inputTextRedColor,
		inputTextGreenColor,
		inputTextBlueColor);
	colorDialog.setStyleSheet(styleSheet());
	if(colorDialog.exec() == QDialog::Accepted)
	{
		inputTextRedColor = colorDialog.redColor;
		inputTextGreenColor = colorDialog.greenColor;
		inputTextBlueColor = colorDialog.blueColor;
		customInputTextColor = true;
		saveColorSettings();
		setColors();
	}
}

void customizationOptions::resetTextColors(void)
{
	// There's no need to set RGB values because they're defined in setColors()
	customLevelTextColor = false;
	customInputTextColor = false;
	saveColorSettings();
	setColors();
}

void customizationOptions::changeBgColor(void)
{
	SimpleColorDialog colorDialog;
	colorDialog.setColor(bgRedColor,
		bgGreenColor,
		bgBlueColor);
	colorDialog.setStyleSheet(styleSheet());
	if(colorDialog.exec() == QDialog::Accepted)
	{
		bgRedColor = colorDialog.redColor;
		bgGreenColor = colorDialog.greenColor;
		bgBlueColor = colorDialog.blueColor;
		customBgColor = true;
		saveColorSettings();
		setColors();
	}
}

void customizationOptions::changePaperColor(void)
{
	SimpleColorDialog colorDialog;
	colorDialog.setColor(paperRedColor,
		paperGreenColor,
		paperBlueColor);
	colorDialog.setStyleSheet(styleSheet());
	if(colorDialog.exec() == QDialog::Accepted)
	{
		paperRedColor = colorDialog.redColor;
		paperGreenColor = colorDialog.greenColor;
		paperBlueColor = colorDialog.blueColor;
		customPaperColor = true;
		saveColorSettings();
		setColors();
	}
}

void customizationOptions::changePanelColor(void)
{
	SimpleColorDialog colorDialog;
	colorDialog.setColor(panelRedColor,
		panelGreenColor,
		panelBlueColor);
	colorDialog.setStyleSheet(styleSheet());
	if(colorDialog.exec() == QDialog::Accepted)
	{
		panelRedColor = colorDialog.redColor;
		panelGreenColor = colorDialog.greenColor;
		panelBlueColor = colorDialog.blueColor;
		customPanelColor = true;
		saveColorSettings();
		setColors();
	}
}

void customizationOptions::resetBgPaperColors(void)
{
	// There's no need to set RGB values because they're defined in setColors()
	customBgColor = false;
	customPaperColor = false;
	customPanelColor = false;
	saveColorSettings();
	setColors();
}

void customizationOptions::changeTheme(int index)
{
	settings->setValue("theme/theme",index);
	updateTheme();
	setColors();
}
