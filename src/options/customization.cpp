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
	updateFont();
	// Load built-in themes
	QList<QVariantMap> themes = globalThemeEngine.themeList();
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
	ui->themeBox->setCurrentIndex((int) themeEngine::style());
	// Colors
	setColors();
	// Font
	updateFont();
}

/*!
 * Connected from themeList->itemClicked() and themeList->itemActivated().\n
 * Switches built-in theme.
 */
void customizationOptions::changeFullTheme(QListWidgetItem* item)
{
	int index = ui->themeList->row(item);
	if(item == lastItem)
		return;
	lastItem = item;
	if(globalThemeEngine.themeName(index) == "custom")
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
		connect(animation1,&QPropertyAnimation::finished,this,[oldWidth, oldWidth2, index, this]() {
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
			globalThemeEngine.setTheme(index);
		});
	}
	else
	{
		globalThemeEngine.setTheme(index);
		updateFont();
		setColors();
	}
}

/*! Select currently set full theme. */
void customizationOptions::selectCurrentFullTheme(void)
{
	QString id = settings.value("theme/fulltheme","default").toString();
	QList<QVariantMap> themes = globalThemeEngine.themeList();
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

/*! Updates text font. */
void customizationOptions::updateFont(void)
{
	QFont newFont = themeEngine::font();
	ui->fontComboBox->setCurrentFont(newFont);
	ui->fontSizeBox->setValue(newFont.pointSize());
	ui->boldTextBox->setChecked(newFont.bold());
	ui->italicTextBox->setChecked(newFont.italic());
	ui->underlineTextBox->setChecked(newFont.underline());
	// Update labels
	ui->levelLabel->setFont(newFont);
	ui->inputLabel->setFont(newFont);
}

/*!
 * Connected from fontComboBox->currentFontChanged().\n
 * Sets exercise text font family and saves it in the settings.
 * \see setFont()
 */
void customizationOptions::changeFont(QFont font)
{
	globalThemeEngine.setFontFamily(font.family());
	updateFont();
}

/*!
 * Connected from fontSizeBox->valueChanged().\n
 * Sets font size and saves it in the settings.
 * \see setFont()
 */
void customizationOptions::changeFontSize(int size)
{
	globalThemeEngine.setFontSize(size);
	updateFont();
}

/*!
 * Connected from boldTextBox->clicked().\n
 * Switches bold text based on the value of boldTextBox.
 * \see setFont()
 */
void customizationOptions::setBoldText(void)
{
	globalThemeEngine.setFontBold(ui->boldTextBox->isChecked());
	updateFont();
}

/*!
 * Connected from italicTextBox->clicked().\n
 * Switches italic text based on the value of italicTextBox.
 * \see setFont()
 */
void customizationOptions::setItalicText(void)
{
	globalThemeEngine.setFontItalic(ui->italicTextBox->isChecked());
	updateFont();
}

/*!
 * Connected from underlineTextBox->clicked().\n
 * Switches text underline based on the value of underlineTextBox.
 * \see setFont()
 */
void customizationOptions::setUnderlineText(void)
{
	globalThemeEngine.setFontUnderline(ui->underlineTextBox->isChecked());
	updateFont();
}

/*! Sets custom colors (if they are set) or default colors. */
void customizationOptions::setColors()
{
	// Set exercise text color
	ui->levelLabel->setStyleSheet(themeEngine::exerciseTextStyleSheet());
	// Set input text color
	ui->inputLabel->setStyleSheet(themeEngine::inputTextStyleSheet());
	// Set background color
	ui->previewFrame->setStyleSheet(themeEngine::bgStyleSheet());
	// Set paper color
	ui->paper->setStyleSheet(themeEngine::paperStyleSheet());
	// Set panel color
	ui->panelFrame->setStyleSheet(themeEngine::panelStyleSheet());
	// Update color buttons
	QColor exTextColor = themeEngine::exerciseTextColor();
	QColor inTextColor = themeEngine::inputTextColor();
	QColor bgColor = themeEngine::bgColor();
	QColor paperColor = themeEngine::paperColor();
	QColor panelColor = themeEngine::panelColor();
	QString styleSheetPart = "border: 2px solid gray; background-color: rgb(";
	ui->levelTextColorButton->setStyleSheet(styleSheetPart +
		QString::number(exTextColor.red()) + ", " +
		QString::number(exTextColor.green()) + ", " +
		QString::number(exTextColor.blue()) + ");");
	ui->inputTextColorButton->setStyleSheet(styleSheetPart +
		QString::number(inTextColor.red()) + ", " +
		QString::number(inTextColor.green()) + ", " +
		QString::number(inTextColor.blue()) + ");");
	ui->bgColorButton->setStyleSheet(styleSheetPart +
		QString::number(bgColor.red()) + ", " +
		QString::number(bgColor.green()) + ", " +
		QString::number(bgColor.blue()) + ");");
	ui->paperColorButton->setStyleSheet(styleSheetPart +
		QString::number(paperColor.red()) + ", " +
		QString::number(paperColor.green()) + ", " +
		QString::number(paperColor.blue()) + ");");
	ui->panelColorButton->setStyleSheet(styleSheetPart +
		QString::number(panelColor.red()) + ", " +
		QString::number(panelColor.green()) + ", " +
		QString::number(panelColor.blue()) + ");");
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
	QColor color = themeEngine::exerciseTextColor();
	colorDialog->setColor(color.red(), color.green(), color.blue());
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog,this]() {
		globalThemeEngine.setExerciseTextColor(QColor(colorDialog->redColor, colorDialog->greenColor, colorDialog->blueColor));
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
	QColor color = themeEngine::inputTextColor();
	colorDialog->setColor(color.red(), color.green(), color.blue());
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog,this]() {
		globalThemeEngine.setInputTextColor(QColor(colorDialog->redColor, colorDialog->greenColor, colorDialog->blueColor));
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
	globalThemeEngine.resetExerciseTextColor();
	globalThemeEngine.resetInputTextColor();
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
	QColor color = themeEngine::bgColor();
	colorDialog->setColor(color.red(), color.green(), color.blue());
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog,this]() {
		globalThemeEngine.setBgColor(QColor(colorDialog->redColor, colorDialog->greenColor, colorDialog->blueColor));
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
	QColor color = themeEngine::paperColor();
	colorDialog->setColor(color.red(), color.green(), color.blue());
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog,this]() {
		globalThemeEngine.setPaperColor(QColor(colorDialog->redColor, colorDialog->greenColor, colorDialog->blueColor));
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
	QColor color = themeEngine::panelColor();
	colorDialog->setColor(color.red(), color.green(), color.blue());
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog,this]() {
		globalThemeEngine.setPanelColor(QColor(colorDialog->redColor, colorDialog->greenColor, colorDialog->blueColor));
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
	globalThemeEngine.resetBgColor();
	globalThemeEngine.resetPaperColor();
	globalThemeEngine.resetPanelColor();
}

/*!
 * Connected from themeBox->activated().\n
 * Sets theme based on the value of themeBox.
 */
void customizationOptions::changeTheme(int index)
{
	globalThemeEngine.setStyle((themeEngine::Style) index);
	setColors();
}
