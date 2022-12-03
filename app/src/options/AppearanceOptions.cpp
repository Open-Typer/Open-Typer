/*
 * AppearanceOptions.cpp
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

#include "options/AppearanceOptions.h"
#include "ui_AppearanceOptions.h"

#include <QColorDialog>
#include <QPropertyAnimation>
#include "Settings.h"
#include "ThemeEngine.h"

/*! Constructs AppearanceOptions. */
AppearanceOptions::AppearanceOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AppearanceOptions)
{
	ui->setupUi(this);
	ui->themeCustomizationFrame->hide();
	ui->themesFrame->show();
	const bool advancedMode = Settings::advancedTheme();
	ui->simpleModeButton->setChecked(!advancedMode);
	ui->advancedModeButton->setChecked(advancedMode);
	ui->advancedControls->setVisible(advancedMode);
	ui->simpleControls->setVisible(!advancedMode);
	updateFont();
	// Load built-in themes
	const auto &themes = globalThemeEngine.themeList();
	for(int i = 0; i < themes.count(); i++)
	{
		QListWidgetItem *item = new QListWidgetItem(QIcon(":res/images/themes/" + themes[i]["icon"].toString()), themes[i]["name"].toString());
		if(i == 0)
			item->setFlags(item->flags() & ~Qt::ItemIsEnabled); // block interaction with obsolete "default" theme
		ui->themeList->addItem(item);
	}
	selectCurrentFullTheme();
	// Connections
	connect(ui->advancedModeButton, &QPushButton::toggled, this, &AppearanceOptions::changeThemeMode);
	connect(ui->lightThemeButton, &QPushButton::clicked, this, [this](bool checked) {
		setSimpleTheme(checked ? SimpleTheme::Light : SimpleTheme::Dark);
	});
	connect(ui->darkThemeButton, &QPushButton::clicked, this, [this](bool checked) {
		setSimpleTheme(checked ? SimpleTheme::Dark : SimpleTheme::Light);
	});
	connect(ui->themeList, &QListWidget::itemClicked, this, &AppearanceOptions::changeFullTheme);
	connect(ui->themeList, &QListWidget::currentItemChanged, this, &AppearanceOptions::changeFullTheme);
	connect(ui->backButton, &QPushButton::clicked, this, &AppearanceOptions::goBack);
	connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this, &AppearanceOptions::changeFont);
	connect(ui->fontSizeBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &AppearanceOptions::changeFontSize);
	connect(ui->boldTextBox, &QCheckBox::clicked, this, &AppearanceOptions::setBoldText);
	connect(ui->italicTextBox, &QCheckBox::clicked, this, &AppearanceOptions::setItalicText);
	connect(ui->underlineTextBox, &QCheckBox::clicked, this, &AppearanceOptions::setUnderlineText);
	connect(ui->levelTextColorButton, &QPushButton::clicked, this, &AppearanceOptions::changeLevelTextColor);
	connect(ui->inputTextColorButton, &QPushButton::clicked, this, &AppearanceOptions::changeInputTextColor);
	connect(ui->bgColorButton, &QPushButton::clicked, this, &AppearanceOptions::changeBgColor);
	connect(ui->paperColorButton, &QPushButton::clicked, this, &AppearanceOptions::changePaperColor);
	connect(ui->panelColorButton, &QPushButton::clicked, this, &AppearanceOptions::changePanelColor);
	connect(ui->themeBox, QOverload<int>::of(&QComboBox::activated), this, &AppearanceOptions::changeTheme);
	// Set up simple themes
	updateSimpleTheme();
}

/*! Destroys the AppearanceOptions object. */
AppearanceOptions::~AppearanceOptions()
{
	delete ui;
}

/*! Initializes widgets and loads settings. */
void AppearanceOptions::init(void)
{
	ui->themeBox->setCurrentIndex(static_cast<int>(ThemeEngine::style()) - 1);
	// Colors
	setColors();
	// Font
	updateFont();
}

/*! Hides mode selector (simple and advanced mode). */
void AppearanceOptions::hideModeSelector(void)
{
	ui->modeSelectWidget->hide();
}

/*! Toggles advanced theme mode. */
void AppearanceOptions::changeThemeMode(bool advanced)
{
	Settings::setAdvancedTheme(advanced);
	ui->advancedControls->setVisible(advanced);
	ui->simpleControls->setVisible(!advanced);
}

/*! Sets simple theme (0 for light, 1 for dark). */
void AppearanceOptions::setSimpleTheme(SimpleTheme theme)
{
	Settings::setSimpleThemeId(static_cast<int>(theme));
	ui->lightThemeButton->setChecked(theme == SimpleTheme::Light);
	ui->darkThemeButton->setChecked(theme == SimpleTheme::Dark);
	switch(theme)
	{
		case SimpleTheme::Light:
			ui->themeList->setCurrentRow(4); // light blue
			break;
		case SimpleTheme::Dark:
			ui->themeList->setCurrentRow(1); // dark
			break;
		default:
			break;
	}
}

/*! Loads selected simple theme. */
void AppearanceOptions::updateSimpleTheme(void)
{
	SimpleTheme simpleTheme = static_cast<SimpleTheme>(Settings::simpleThemeId());
	if((simpleTheme == SimpleTheme::Light) && (globalThemeEngine.theme() != 4)) // default theme for "light" - light blue
		setSimpleTheme(SimpleTheme::Undefined);
	if((simpleTheme == SimpleTheme::Dark) && (globalThemeEngine.theme() != 1)) // default theme for "dark" - dark
		setSimpleTheme(SimpleTheme::Undefined);
	ui->lightThemeButton->setChecked(simpleTheme == SimpleTheme::Light);
	ui->darkThemeButton->setChecked(simpleTheme == SimpleTheme::Dark);
}

/*!
 * Connected from themeList->itemClicked() and themeList->itemActivated().\n
 * Switches built-in theme.
 */
void AppearanceOptions::changeFullTheme(QListWidgetItem *item)
{
	int index = ui->themeList->row(item);
	if(index == 0) // block obsolete "default" theme
		return;
	if(item == lastItem)
		return;
	lastItem = item;
	if(globalThemeEngine.themeName(index) == "custom")
	{
		init();
		ui->themeList->hide();
		QPropertyAnimation *animation1 = new QPropertyAnimation(ui->themesFrame, "geometry");
		int oldWidth2 = geometry().width();
		// Animation 1
		animation1->setEasingCurve(QEasingCurve::InQuad);
		QRect widgetGeometry = ui->themesFrame->geometry();
		int oldWidth = widgetGeometry.width();
		animation1->setDuration(128);
		animation1->setStartValue(widgetGeometry);
		widgetGeometry.setWidth(0);
		animation1->setEndValue(widgetGeometry);
		animation1->start(QAbstractAnimation::DeleteWhenStopped);
		connect(animation1, &QPropertyAnimation::finished, this, [oldWidth, oldWidth2, index, this]() {
			ui->themesFrame->hide();
			QRect widgetGeometry = ui->themesFrame->geometry();
			widgetGeometry.setWidth(oldWidth);
			ui->themesFrame->setGeometry(widgetGeometry);
			// Animation 2
			QPropertyAnimation *animation2 = new QPropertyAnimation(ui->themeCustomizationFrame, "geometry");
			animation2->setEasingCurve(QEasingCurve::OutQuad);
			animation2->setDuration(128);
			ui->themeCustomizationFrame->show();
			widgetGeometry = ui->themeCustomizationFrame->geometry();
			widgetGeometry.setX(oldWidth2);
			animation2->setStartValue(widgetGeometry);
			animation2->setEndValue(ui->themeCustomizationFrame->geometry());
			animation2->start(QAbstractAnimation::DeleteWhenStopped);
			globalThemeEngine.setTheme(index);
		});
	}
	else
	{
		globalThemeEngine.setTheme(index);
		updateFont();
		setColors();
	}
	updateSimpleTheme();
}

/*! Select currently set full theme. */
void AppearanceOptions::selectCurrentFullTheme(void)
{
	QString id = Settings::applicationTheme();
	const auto &themes = globalThemeEngine.themeList();
	for(int i = 0; i < themes.count(); i++)
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
void AppearanceOptions::goBack(void)
{
	QPropertyAnimation *animation1 = new QPropertyAnimation(ui->themeCustomizationFrame, "geometry");
	// Animation 1
	animation1->setEasingCurve(QEasingCurve::InQuad);
	QRect widgetGeometry = ui->themeCustomizationFrame->geometry();
	animation1->setDuration(128);
	animation1->setStartValue(widgetGeometry);
	widgetGeometry.setX(geometry().width());
	animation1->setEndValue(widgetGeometry);
	animation1->start(QAbstractAnimation::DeleteWhenStopped);
	connect(animation1, &QPropertyAnimation::finished, this, [this]() {
		ui->themeCustomizationFrame->hide();
		// Animation 2
		QPropertyAnimation *animation2 = new QPropertyAnimation(ui->themesFrame, "geometry");
		animation2->setEasingCurve(QEasingCurve::OutQuad);
		animation2->setDuration(128);
		ui->themesFrame->show();
		QRect widgetGeometry = ui->themesFrame->geometry();
		widgetGeometry.setWidth(0);
		animation2->setStartValue(widgetGeometry);
		animation2->setEndValue(ui->themesFrame->geometry());
		animation2->start(QAbstractAnimation::DeleteWhenStopped);
		connect(animation2, &QPropertyAnimation::finished, this, [this]() {
			ui->themeList->setResizeMode(QListView::Adjust);
			ui->themeList->show();
			selectCurrentFullTheme();
			lastItem = nullptr;
		});
	});
}

/*! Updates text font. */
void AppearanceOptions::updateFont(void)
{
	QFont newFont = ThemeEngine::font();
	ui->fontComboBox->blockSignals(true);
	ui->fontComboBox->setCurrentFont(newFont);
	ui->fontComboBox->blockSignals(false);
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
void AppearanceOptions::changeFont(QFont font)
{
	globalThemeEngine.setFontFamily(font.family());
	updateFont();
}

/*!
 * Connected from fontSizeBox->valueChanged().\n
 * Sets font size and saves it in the settings.
 * \see setFont()
 */
void AppearanceOptions::changeFontSize(int size)
{
	globalThemeEngine.setFontSize(size);
	updateFont();
}

/*!
 * Connected from boldTextBox->clicked().\n
 * Switches bold text based on the value of boldTextBox.
 * \see setFont()
 */
void AppearanceOptions::setBoldText(void)
{
	globalThemeEngine.setFontBold(ui->boldTextBox->isChecked());
	updateFont();
}

/*!
 * Connected from italicTextBox->clicked().\n
 * Switches italic text based on the value of italicTextBox.
 * \see setFont()
 */
void AppearanceOptions::setItalicText(void)
{
	globalThemeEngine.setFontItalic(ui->italicTextBox->isChecked());
	updateFont();
}

/*!
 * Connected from underlineTextBox->clicked().\n
 * Switches text underline based on the value of underlineTextBox.
 * \see setFont()
 */
void AppearanceOptions::setUnderlineText(void)
{
	globalThemeEngine.setFontUnderline(ui->underlineTextBox->isChecked());
	updateFont();
}

/*! Sets custom colors (if they are set) or default colors. */
void AppearanceOptions::setColors()
{
	// Set exercise text color
	ui->levelLabel->setStyleSheet(ThemeEngine::exerciseTextStyleSheet());
	// Set input text color
	ui->inputLabel->setStyleSheet(ThemeEngine::inputTextStyleSheet());
	// Set background color
	ui->previewFrame->setStyleSheet(ThemeEngine::bgStyleSheet());
	// Set paper color
	ui->paper->setStyleSheet(ThemeEngine::paperStyleSheet());
	// Set panel color
	ui->panelFrame->setStyleSheet(ThemeEngine::panelStyleSheet());
	// Update color buttons
	QColor exTextColor = ThemeEngine::exerciseTextColor();
	QColor inTextColor = ThemeEngine::inputTextColor();
	QColor bgColor = ThemeEngine::bgColor();
	QColor paperColor = ThemeEngine::paperColor();
	QColor panelColor = ThemeEngine::panelColor();
	QString styleSheetPart = "border: 2px solid gray; background-color: rgb(";
	ui->levelTextColorButton->setStyleSheet(styleSheetPart + QString::number(exTextColor.red()) + ", " + QString::number(exTextColor.green()) + ", " + QString::number(exTextColor.blue()) + ");");
	ui->inputTextColorButton->setStyleSheet(styleSheetPart + QString::number(inTextColor.red()) + ", " + QString::number(inTextColor.green()) + ", " + QString::number(inTextColor.blue()) + ");");
	ui->bgColorButton->setStyleSheet(styleSheetPart + QString::number(bgColor.red()) + ", " + QString::number(bgColor.green()) + ", " + QString::number(bgColor.blue()) + ");");
	ui->paperColorButton->setStyleSheet(styleSheetPart + QString::number(paperColor.red()) + ", " + QString::number(paperColor.green()) + ", " + QString::number(paperColor.blue()) + ");");
	ui->panelColorButton->setStyleSheet(styleSheetPart + QString::number(panelColor.red()) + ", " + QString::number(panelColor.green()) + ", " + QString::number(panelColor.blue()) + ");");
}

/*!
 * Connected from levelTextColorButton->clicked().\n
 * Opens QColorDialog and sets exercise text color.
 * \see changeInputTextColor()
 */
void AppearanceOptions::changeLevelTextColor(void)
{
	QColorDialog *colorDialog = new QColorDialog(globalThemeEngine.exerciseTextColor(), this);
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog, this]() {
		globalThemeEngine.setExerciseTextColor(colorDialog->selectedColor());
		setColors();
	});
	colorDialog->open();
}

/*!
 * Connected from inputTextColorButton->clicked().\n
 * Opens QColorDialog and sets input text color.
 * \see changeLevelTextColor()
 */
void AppearanceOptions::changeInputTextColor(void)
{
	QColorDialog *colorDialog = new QColorDialog(globalThemeEngine.inputTextColor(), this);
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog, this]() {
		globalThemeEngine.setInputTextColor(colorDialog->selectedColor());
		setColors();
	});
	colorDialog->open();
}

/*!
 * Connected from resetTextColorButton->clicked().\n
 * Sets the default text colors.
 */
void AppearanceOptions::resetTextColors(void)
{
	globalThemeEngine.resetExerciseTextColor();
	globalThemeEngine.resetInputTextColor();
}

/*!
 * Connected from bgColorButton->clicked().\n
 * Opens QColorDialog and sets background color.
 * \see changePaperColor()
 * \see changePanelColor()
 */
void AppearanceOptions::changeBgColor(void)
{
	QColorDialog *colorDialog = new QColorDialog(globalThemeEngine.bgColor(), this);
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog, this]() {
		globalThemeEngine.setBgColor(colorDialog->selectedColor());
		setColors();
	});
	colorDialog->open();
}

/*!
 * Connected from paperColorButton->clicked().\n
 * Opens QColorDialog and sets paper color.
 * \see changeBgColor()
 * \see changePanelColor()
 */
void AppearanceOptions::changePaperColor(void)
{
	QColorDialog *colorDialog = new QColorDialog(globalThemeEngine.paperColor(), this);
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog, this]() {
		globalThemeEngine.setPaperColor(colorDialog->selectedColor());
		setColors();
	});
	colorDialog->open();
}

/*!
 * Connected from panelColorButton->clicked().\n
 * Opens QColorDialog and sets panel color.
 * \see changeBgColor()
 * \see changePaperColor()
 */
void AppearanceOptions::changePanelColor(void)
{
	QColorDialog *colorDialog = new QColorDialog(globalThemeEngine.panelColor(), this);
	colorDialog->setWindowModality(Qt::WindowModal);
	connect(colorDialog, &QDialog::accepted, this, [colorDialog, this]() {
		globalThemeEngine.setPanelColor(colorDialog->selectedColor());
		setColors();
	});
	colorDialog->open();
}

/*!
 * Connected from resetBgPaperColorButton->clicked().\n
 * Sets the default background, paper and panel colors.
 */
void AppearanceOptions::resetBgPaperColors(void)
{
	globalThemeEngine.resetBgColor();
	globalThemeEngine.resetPaperColor();
	globalThemeEngine.resetPanelColor();
}

/*!
 * Connected from themeBox->activated().\n
 * Sets theme based on the value of themeBox.
 */
void AppearanceOptions::changeTheme(int index)
{
	globalThemeEngine.setStyle((ThemeEngine::Style)(index + 1));
	setColors();
}
