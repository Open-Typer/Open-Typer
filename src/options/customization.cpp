/*
 * customization.cpp
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

#include "options/customization.h"
#include "ui_customization.h"

/*! Constructs customizationOptions. */
customizationOptions::customizationOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::customizationOptions)
{
	ui->setupUi(this);
	ui->themeCustomizationFrame->hide();
	ui->themesFrame->show();
	blockThemeSignal = true;
	// Built-in themes
	themes.clear();
	QVariantMap themeMap;
	// Default
	themeMap.clear();
	themeMap.insert("name",tr("Default"));
	themeMap.insert("id","default");
	themeMap.insert("icon","default.png");
	themes += themeMap;
	// Dark
	themeMap.clear();
	themeMap.insert("name",tr("Dark"));
	themeMap.insert("id","dark");
	themeMap.insert("icon","dark.png");
	themeMap.insert("baseTheme",1);
	themes += themeMap;
	// Light
	themeMap.clear();
	themeMap.insert("name",tr("Light"));
	themeMap.insert("id","light");
	themeMap.insert("icon","light.png");
	themeMap.insert("baseTheme",2);
	themes += themeMap;
	// Green
	themeMap.clear();
	themeMap.insert("name",tr("Green"));
	themeMap.insert("id","green");
	themeMap.insert("icon","green.png");
	themeMap.insert("baseTheme",2);
	themeMap.insert("bgColor",qRgb(0,108,0));
	themeMap.insert("panelColor",qRgb(175,175,175));
	themes += themeMap;
	// Custom
	themeMap.clear();
	themeMap.insert("name",tr("Custom"));
	themeMap.insert("id","custom");
	themeMap.insert("icon","custom.png");
	themes += themeMap;
	// Load built-in themes
	for(int i=0; i < themes.count(); i++)
	{
		QListWidgetItem *item = new QListWidgetItem(QIcon(":res/images/themes/" + themes[i]["icon"].toString()), themes[i]["name"].toString());
		ui->themeList->addItem(item);
	}
	selectCurrentFullTheme();
	// Connections
	// Theme list
	connect(ui->themeList,
		SIGNAL(itemClicked(QListWidgetItem*)),
		this,
		SLOT(changeFullTheme(QListWidgetItem*)));
	connect(ui->themeList,
		SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
		this,
		SLOT(changeFullTheme(QListWidgetItem*)));
	// Back button
	connect(ui->backButton,
		SIGNAL(clicked()),
		this,
		SLOT(goBack()));
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
	// Theme selector
	connect(ui->themeBox,
		SIGNAL(activated(int)),
		this,
		SLOT(changeTheme(int)));
}

/*! Destroys the customizationOptions object. */
customizationOptions::~customizationOptions()
{
	delete ui;
}

/*! Initializes widgets and loads settings. */
void customizationOptions::init(void)
{
	blockThemeSignal = true;
	ui->themeBox->setCurrentIndex(settings.value("theme/theme","0").toInt());
	// Colors
	// Level text
	customLevelTextColor = settings.value("theme/customleveltextcolor","false").toBool();
	levelTextRedColor = settings.value("theme/leveltextred","0").toInt();
	levelTextGreenColor = settings.value("theme/leveltextgreen","0").toInt();
	levelTextBlueColor = settings.value("theme/leveltextblue","0").toInt();
	// Input text
	customInputTextColor = settings.value("theme/custominputtextcolor","false").toBool();
	inputTextRedColor = settings.value("theme/inputtextred","0").toInt();
	inputTextGreenColor = settings.value("theme/inputtextgreen","0").toInt();
	inputTextBlueColor = settings.value("theme/inputtextblue","0").toInt();
	// Background
	customBgColor = settings.value("theme/custombgcolor","false").toBool();
	bgRedColor = settings.value("theme/bgred","0").toInt();
	bgGreenColor = settings.value("theme/bggreen","0").toInt();
	bgBlueColor = settings.value("theme/bgblue","0").toInt();
	// Paper
	customPaperColor = settings.value("theme/custompapercolor","false").toBool();
	paperRedColor = settings.value("theme/paperred","0").toInt();
	paperGreenColor = settings.value("theme/papergreen","0").toInt();
	paperBlueColor = settings.value("theme/paperblue","0").toInt();
	// Panel
	customPanelColor = settings.value("theme/custompanelcolor","false").toBool();
	panelRedColor = settings.value("theme/panelred","0").toInt();
	panelGreenColor = settings.value("theme/panelgreen","0").toInt();
	panelBlueColor = settings.value("theme/panelblue","0").toInt();
	setColors();
	// Font
	setFont(settings.value("theme/font","").toString(),
		settings.value("theme/fontsize","20").toInt(),
		settings.value("theme/fontbold","true").toBool(),
		settings.value("theme/fontitalic","false").toBool(),
		settings.value("theme/fontunderline","false").toBool());
	blockThemeSignal = false;
}

/*!
 * Connected from themeList->itemClicked() and themeList->itemActivated().\n
 * Switches built-in theme.
 */
void customizationOptions::changeFullTheme(QListWidgetItem* item)
{
	int index = ui->themeList->row(item);
	QVariantMap themeMap = themes[index];
	if(item == lastItem)
		return;
	lastItem = item;
	if(themeMap["id"].toString() == "custom")
	{
		init();
		ui->themeList->hide();
		QPropertyAnimation *animation1 = new QPropertyAnimation(ui->themesFrame,"geometry");
		int oldWidth2 = geometry().width();
		// Animation 1
		animation1->setEasingCurve(QEasingCurve::InQuad);
		QRect widgetGeometry = ui->themesFrame->geometry();
		int oldWidth = widgetGeometry.width();
		animation1->setDuration(128);
		animation1->setStartValue(widgetGeometry);
		widgetGeometry.setWidth(0);
		animation1->setEndValue(widgetGeometry);
		animation1->start();
		connect(animation1,&QPropertyAnimation::finished,this,[oldWidth, oldWidth2, themeMap, this]() {
			ui->themesFrame->hide();
			QRect widgetGeometry = ui->themesFrame->geometry();
			widgetGeometry.setWidth(oldWidth);
			ui->themesFrame->setGeometry(widgetGeometry);
			// Animation 2
			QPropertyAnimation *animation2 = new QPropertyAnimation(ui->themeCustomizationFrame,"geometry");
			animation2->setEasingCurve(QEasingCurve::OutQuad);
			animation2->setDuration(128);
			ui->themeCustomizationFrame->show();
			widgetGeometry = ui->themeCustomizationFrame->geometry();
			widgetGeometry.setX(oldWidth2);
			animation2->setStartValue(widgetGeometry);
			animation2->setEndValue(ui->themeCustomizationFrame->geometry());
			animation2->start();
			settings.setValue("theme/fulltheme",themeMap["id"]);
		});
	}
	else
	{
		blockThemeSignal = true;
		// Base theme
		if(themeMap.contains("baseTheme"))
			changeTheme(themeMap["baseTheme"].toInt());
		else
			changeTheme(0);
		// Font
		QString fontFamily;
		int fontSize;
		bool fontBold, fontItalic, fontUnderline;
		if(themeMap.contains("font"))
			fontFamily = themeMap["font"].toString();
		else
			fontFamily = "";
		if(themeMap.contains("fontSize"))
			fontSize = themeMap["fontSize"].toInt();
		else
			fontSize = settings.value("theme/fontsize","20").toInt();
		if(themeMap.contains("fontBold"))
			fontBold = themeMap["fontBold"].toBool();
		else
			fontBold = true;
		if(themeMap.contains("fontItalic"))
			fontItalic = themeMap["fontItalic"].toBool();
		else
			fontItalic = false;
		if(themeMap.contains("fontUnderline"))
			fontUnderline = themeMap["fontUnderline"].toBool();
		else
			fontUnderline = false;
		setFont(fontFamily,fontSize,fontBold,fontItalic,fontUnderline);
		// Colors
		resetTextColors();
		if(themeMap.contains("levelTextColor"))
		{
			QRgb color = themeMap["levelTextColor"].toUInt();
			levelTextRedColor = qRed(color);
			levelTextGreenColor = qGreen(color);
			levelTextBlueColor = qBlue(color);
			customLevelTextColor = true;
		}
		if(themeMap.contains("inputTextColor"))
		{
			QRgb color = themeMap["inputTextColor"].toUInt();
			inputTextRedColor = qRed(color);
			inputTextGreenColor = qGreen(color);
			inputTextBlueColor = qBlue(color);
			customInputTextColor = true;
		}
		resetBgPaperColors();
		if(themeMap.contains("bgColor"))
		{
			QRgb color = themeMap["bgColor"].toUInt();
			bgRedColor = qRed(color);
			bgGreenColor = qGreen(color);
			bgBlueColor = qBlue(color);
			customBgColor = true;
		}
		if(themeMap.contains("paperColor"))
		{
			QRgb color = themeMap["paperColor"].toUInt();
			paperRedColor = qRed(color);
			paperGreenColor = qGreen(color);
			paperBlueColor = qBlue(color);
			customPaperColor = true;
		}
		if(themeMap.contains("panelColor"))
		{
			QRgb color = themeMap["panelColor"].toUInt();
			panelRedColor = qRed(color);
			panelGreenColor = qGreen(color);
			panelBlueColor = qBlue(color);
			customPanelColor = true;
		}
		saveColorSettings();
		setColors();
		settings.setValue("theme/fulltheme",themeMap["id"]);
		blockThemeSignal = false;
		emit themeChanged();
	}
}

/*! Select currently set full theme. */
void customizationOptions::selectCurrentFullTheme(void)
{
	QString id = settings.value("theme/fulltheme","default").toString();
	for(int i=0; i < themes.count(); i++)
	{
		if(themes[i]["id"] == id)
		{
			ui->themeList->setCurrentRow(i);
			break;
		}
	}
}

/*!
 * Connected from backButton->clicked().\n
 * Returns to theme selection.
 */
void customizationOptions::goBack(void)
{
	QPropertyAnimation *animation1 = new QPropertyAnimation(ui->themeCustomizationFrame,"geometry");
	// Animation 1
	animation1->setEasingCurve(QEasingCurve::InQuad);
	QRect widgetGeometry = ui->themeCustomizationFrame->geometry();
	animation1->setDuration(128);
	animation1->setStartValue(widgetGeometry);
	widgetGeometry.setX(geometry().width());
	animation1->setEndValue(widgetGeometry);
	animation1->start();
	connect(animation1, &QPropertyAnimation::finished, this, [this]() {
		ui->themeCustomizationFrame->hide();
		// Animation 2
		QPropertyAnimation *animation2 = new QPropertyAnimation(ui->themesFrame,"geometry");
		animation2->setEasingCurve(QEasingCurve::OutQuad);
		animation2->setDuration(128);
		ui->themesFrame->show();
		QRect widgetGeometry = ui->themesFrame->geometry();
		widgetGeometry.setWidth(0);
		animation2->setStartValue(widgetGeometry);
		animation2->setEndValue(ui->themesFrame->geometry());
		animation2->start();
		connect(animation2, &QPropertyAnimation::finished, this, [this]() {
			ui->themeList->setResizeMode(QListView::Adjust);
			ui->themeList->show();
			selectCurrentFullTheme();
			lastItem = nullptr;
		});
	});
}

/*! Sets exercise text font and saves it in the settings. \see OpenTyper#setFont */
void customizationOptions::setFont(QString fontFamily, int fontSize, bool fontBold, bool fontItalic, bool fontUnderline)
{
	QFont newFont, mistakeLabelFont;
	// Set font
	newFont.setStyleHint(QFont::TypeWriter);
	newFont.setFixedPitch(true);
	if(fontFamily == "")
		fontFamily = "Courier New";
	newFont.setFamily(fontFamily);
	QFontDatabase fontDB;
	if(!fontDB.families().contains(fontFamily))
		fontFamily = newFont.defaultFamily();
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
	settings.setValue("theme/font",fontFamily);
	settings.setValue("theme/fontsize",fontSize);
	settings.setValue("theme/fontbold",fontBold);
	settings.setValue("theme/fontitalic",fontItalic);
	settings.setValue("theme/fontunderline",fontUnderline);
	if(!blockThemeSignal)
		emit themeChanged();
}

/*!
 * Connected from fontComboBox->currentFontChanged().\n
 * Sets exercise text font family and saves it in the settings.
 * \see setFont()
 */
void customizationOptions::changeFont(QFont font)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setFamily(font.family());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	settings.setValue("theme/font",font.family());
	if(!blockThemeSignal)
		emit themeChanged();
}

/*!
 * Connected from fontSizeBox->valueChanged().\n
 * Sets font size and saves it in the settings.
 * \see setFont()
 */
void customizationOptions::changeFontSize(int size)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setPointSize(size);
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	settings.setValue("theme/fontsize",size);
	if(!blockThemeSignal)
		emit themeChanged();
}

/*!
 * Connected from boldTextBox->clicked().\n
 * Switches bold text based on the value of boldTextBox.
 * \see setFont()
 */
void customizationOptions::setBoldText(void)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setBold(ui->boldTextBox->isChecked());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	settings.setValue("theme/fontbold",ui->boldTextBox->isChecked());
	if(!blockThemeSignal)
		emit themeChanged();
}

/*!
 * Connected from italicTextBox->clicked().\n
 * Switches bold text based on the value of italicTextBox.
 * \see setFont()
 */
void customizationOptions::setItalicText(void)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setItalic(ui->italicTextBox->isChecked());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	settings.setValue("theme/fontitalic",ui->italicTextBox->isChecked());
	if(!blockThemeSignal)
		emit themeChanged();
}

/*!
 * Connected from underlineTextBox->clicked().\n
 * Switches bold text based on the value of underlineTextBox.
 * \see setFont()
 */
void customizationOptions::setUnderlineText(void)
{
	QFont oldFont = ui->levelLabel->font();
	oldFont.setUnderline(ui->underlineTextBox->isChecked());
	ui->levelLabel->setFont(oldFont);
	ui->inputLabel->setFont(oldFont);
	settings.setValue("theme/fontunderline",ui->underlineTextBox->isChecked());
	if(!blockThemeSignal)
		emit themeChanged();
}

/*! Saves all custom colors in the settings. */
void customizationOptions::saveColorSettings(void)
{
	// Level text
	settings.setValue("theme/customleveltextcolor",customLevelTextColor);
	settings.setValue("theme/leveltextred",levelTextRedColor);
	settings.setValue("theme/leveltextgreen",levelTextGreenColor);
	settings.setValue("theme/leveltextblue",levelTextBlueColor);
	// Input text
	settings.setValue("theme/custominputtextcolor",customInputTextColor);
	settings.setValue("theme/inputtextred",inputTextRedColor);
	settings.setValue("theme/inputtextgreen",inputTextGreenColor);
	settings.setValue("theme/inputtextblue",inputTextBlueColor);
	// Background
	settings.setValue("theme/custombgcolor",customBgColor);
	settings.setValue("theme/bgred",bgRedColor);
	settings.setValue("theme/bggreen",bgGreenColor);
	settings.setValue("theme/bgblue",bgBlueColor);
	// Paper
	settings.setValue("theme/custompapercolor",customPaperColor);
	settings.setValue("theme/paperred",paperRedColor);
	settings.setValue("theme/papergreen",paperGreenColor);
	settings.setValue("theme/paperblue",paperBlueColor);
	// Panel
	settings.setValue("theme/custompanelcolor",customPanelColor);
	settings.setValue("theme/panelred",panelRedColor);
	settings.setValue("theme/panelgreen",panelGreenColor);
	settings.setValue("theme/panelblue",panelBlueColor);
	if(!blockThemeSignal)
		emit themeChanged();
}

/*! Sets custom colors (if they are set) or default colors. \see OpenTyper#setColors */
void customizationOptions::setColors()
{
	// Reset style sheets
	ui->inputLabel->setStyleSheet("");
	ui->previewFrame->setStyleSheet("");
	ui->paper->setStyleSheet("");
	// Update theme
	updateTheme();
	// Set level text color
	QString levelTextStyleSheet = "rgb(" + QString::number(levelTextRedColor) + ", " + QString::number(levelTextGreenColor) + ", " + QString::number(levelTextBlueColor) + ")";
	ui->levelLabel->setStyleSheet("color: " + levelTextStyleSheet);
	// Set input text color
	QString inputTextStyleSheet = "rgb(" + QString::number(inputTextRedColor) + ", " + QString::number(inputTextGreenColor) + ", " + QString::number(inputTextBlueColor) + ")";
	ui->inputLabel->setStyleSheet("color: " + inputTextStyleSheet);
	// Set paper color
	QString paperStyleSheet = "rgb(" + QString::number(paperRedColor) + ", " + QString::number(paperGreenColor) + ", " + QString::number(paperBlueColor) + ")";
	ui->paper->setStyleSheet("background-color: " + paperStyleSheet);
	// Set background color
	QString bgStyleSheet = "rgb(" + QString::number(bgRedColor) + ", " + QString::number(bgGreenColor) + ", " + QString::number(bgBlueColor) + ")";
	ui->previewFrame->setStyleSheet("background-color: " + bgStyleSheet);
	// Set panel color
	QString panelStyleSheet = "rgb(" + QString::number(panelRedColor) + ", " + QString::number(panelGreenColor) + ", " + QString::number(panelBlueColor) + ")";
	ui->panelFrame->setStyleSheet("background-color: " + panelStyleSheet);
	// Update color buttons
	ui->levelTextColorButton->setStyleSheet("border: 2px solid gray; background-color: " + levelTextStyleSheet);
	ui->inputTextColorButton->setStyleSheet("border: 2px solid gray; background-color: " + inputTextStyleSheet);
	ui->paperColorButton->setStyleSheet("border: 2px solid gray; background-color: " + paperStyleSheet);
	ui->bgColorButton->setStyleSheet("border: 2px solid gray; background-color: " + bgStyleSheet);
	ui->panelColorButton->setStyleSheet("border: 2px solid gray; background-color: " + panelStyleSheet);
}

/*! Loads the style sheet of the selected theme. \see OpenTyper#updateTheme */
void customizationOptions::updateTheme()
{
	QFile darkSheet(":/dark-theme/style.qss");
	QFile lightSheet(":/light-theme/style.qss");
	char *paperStyleSheet;
	switch(settings.value("theme/theme","0").toInt()) {
		case 0:
			// System (default)
			qApp->setStyleSheet("");
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
				qApp->setStyleSheet(ts.readAll());
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
				qApp->setStyleSheet(ts.readAll());
			}
			else
			{
				printf("D: Failed to open light style\n");
			}
			ui->paper->setStyleSheet("background-color: rgb(255, 255, 255)");
			break;
	}
	if(!blockThemeSignal)
		emit themeChanged();
}

/*!
 * Connected from levelTextColorButton->clicked().\n
 * Opens SimpleColorDialog and sets exercise text color.
 * \see SimpleColorDialog
 * \see changeInputTextColor()
 */
void customizationOptions::changeLevelTextColor(void)
{
	SimpleColorDialog *colorDialog = new SimpleColorDialog(this);
	colorDialog->setColor(levelTextRedColor,
		levelTextGreenColor,
		levelTextBlueColor);
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog,this]() {
		levelTextRedColor = colorDialog->redColor;
		levelTextGreenColor = colorDialog->greenColor;
		levelTextBlueColor = colorDialog->blueColor;
		customLevelTextColor = true;
		saveColorSettings();
		setColors();
	});
	colorDialog->open();
}

/*!
 * Connected from inputTextColorButton->clicked().\n
 * Opens SimpleColorDialog and sets input text color.
 * \see SimpleColorDialog
 * \see changeLevelTextColor()
 */
void customizationOptions::changeInputTextColor(void)
{
	SimpleColorDialog *colorDialog = new SimpleColorDialog(this);
	colorDialog->setColor(inputTextRedColor,
		inputTextGreenColor,
		inputTextBlueColor);
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog,this]() {
		inputTextRedColor = colorDialog->redColor;
		inputTextGreenColor = colorDialog->greenColor;
		inputTextBlueColor = colorDialog->blueColor;
		customInputTextColor = true;
		saveColorSettings();
		setColors();
	});
	colorDialog->open();
}

/*!
 * Connected from resetTextColorButton->clicked().\n
 * Sets the default text colors.
 */
void customizationOptions::resetTextColors(void)
{
	// There's no need to set RGB values because they're defined in setColors()
	customLevelTextColor = false;
	customInputTextColor = false;
	saveColorSettings();
}

/*!
 * Connected from bgColorButton->clicked().\n
 * Opens SimpleColorDialog and sets background color.
 * \see SimpleColorDialog
 * \see changePaperColor()
 * \see changePanelColor()
 */
void customizationOptions::changeBgColor(void)
{
	SimpleColorDialog *colorDialog = new SimpleColorDialog(this);
	colorDialog->setColor(bgRedColor,
		bgGreenColor,
		bgBlueColor);
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog,this]() {
		bgRedColor = colorDialog->redColor;
		bgGreenColor = colorDialog->greenColor;
		bgBlueColor = colorDialog->blueColor;
		customBgColor = true;
		saveColorSettings();
		setColors();
	});
	colorDialog->open();
}

/*!
 * Connected from paperColorButton->clicked().\n
 * Opens SimpleColorDialog and sets paper color.
 * \see SimpleColorDialog
 * \see changeBgColor()
 * \see changePanelColor()
 */
void customizationOptions::changePaperColor(void)
{
	SimpleColorDialog *colorDialog = new SimpleColorDialog(this);
	colorDialog->setColor(paperRedColor,
		paperGreenColor,
		paperBlueColor);
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog,this]() {
		paperRedColor = colorDialog->redColor;
		paperGreenColor = colorDialog->greenColor;
		paperBlueColor = colorDialog->blueColor;
		customPaperColor = true;
		saveColorSettings();
		setColors();
	});
	colorDialog->open();
}

/*!
 * Connected from panelColorButton->clicked().\n
 * Opens SimpleColorDialog and sets panel color.
 * \see SimpleColorDialog
 * \see changeBgColor()
 * \see changePaperColor()
 */
void customizationOptions::changePanelColor(void)
{
	SimpleColorDialog *colorDialog = new SimpleColorDialog(this);
	colorDialog->setColor(panelRedColor,
		panelGreenColor,
		panelBlueColor);
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog,this]() {
		panelRedColor = colorDialog->redColor;
		panelGreenColor = colorDialog->greenColor;
		panelBlueColor = colorDialog->blueColor;
		customPanelColor = true;
		saveColorSettings();
		setColors();
	});
	colorDialog->open();
}

/*!
 * Connected from resetBgPaperColorButton->clicked().\n
 * Sets the default background, paper and panel colors.
 */
void customizationOptions::resetBgPaperColors(void)
{
	// There's no need to set RGB values because they're defined in setColors()
	customBgColor = false;
	customPaperColor = false;
	customPanelColor = false;
	saveColorSettings();
}

/*!
 * Connected from themeBox->activated().\n
 * Sets theme based on the value of themeBox.
 */
void customizationOptions::changeTheme(int index)
{
	settings.setValue("theme/theme",index);
	setColors();
	if(!blockThemeSignal)
		init();
}
