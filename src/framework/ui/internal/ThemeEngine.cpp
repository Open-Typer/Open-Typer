/*
 * ThemeEngine.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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

#include <QApplication>
#include <QPalette>
#include "ThemeEngine.h"
#include "QmlUtils.h"

static const QString module = "ui";
static const ISettings::Key FONT_FAMILY(module, "fontFamily");
static const ISettings::Key FONT_SIZE(module, "fontSize");
static const ISettings::Key CUSTOM_EX_TEXT_COLOR(module, "customExerciseTextColor");
static const ISettings::Key EX_TEXT_R(module, "exerciseTextColorR");
static const ISettings::Key EX_TEXT_G(module, "exerciseTextColorG");
static const ISettings::Key EX_TEXT_B(module, "exerciseTextColorB");
static const ISettings::Key CUSTOM_IN_TEXT_COLOR(module, "customInputTextColor");
static const ISettings::Key IN_TEXT_R(module, "inputTextColorR");
static const ISettings::Key IN_TEXT_G(module, "inputTextColorG");
static const ISettings::Key IN_TEXT_B(module, "inputTextColorB");
static const ISettings::Key CUSTOM_BG_COLOR(module, "customBgColor");
static const ISettings::Key BG_R(module, "bgColorR");
static const ISettings::Key BG_G(module, "bgColorG");
static const ISettings::Key BG_B(module, "bgColorB");
static const ISettings::Key CUSTOM_PAPER_COLOR(module, "customPaperColor");
static const ISettings::Key PAPER_R(module, "paperColorR");
static const ISettings::Key PAPER_G(module, "paperColorG");
static const ISettings::Key PAPER_B(module, "paperColorB");
static const ISettings::Key CUSTOM_PANEL_COLOR(module, "customPanelColor");
static const ISettings::Key PANEL_R(module, "panelColorR");
static const ISettings::Key PANEL_G(module, "panelColorG");
static const ISettings::Key PANEL_B(module, "panelColorB");
static const ISettings::Key ACCENT_ID(module, "accentColorId");
static const ISettings::Key APP_THEME(module, "appTheme");

std::shared_ptr<ThemeEngine> ThemeEngine::m_instance = std::make_shared<ThemeEngine>();

/*! Constructs ThemeEngine. */
ThemeEngine::ThemeEngine()
{
	// Connections
	connect(this, &ThemeEngine::fontFamilyChanged, this, &ThemeEngine::fontChanged);
	connect(this, &ThemeEngine::fontSizeChanged, this, &ThemeEngine::fontChanged);
	connect(this, &ThemeEngine::exerciseTextColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::inputTextColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::bgColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::paperColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::panelColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::currentAccentColorChanged);
}

/*! Returns the static instance of ThemeEngine. */
std::shared_ptr<ThemeEngine> ThemeEngine::instance()
{
	return m_instance;
}

/*!
 * The selected font.\n
 * Note: Font style such as bold, italic or underlined is not supported.
 */
QFont ThemeEngine::font(void)
{
	if(!settings()->containsKey(FONT_FAMILY) || !settings()->containsKey(FONT_SIZE))
	{
		setFontFamily("");
		setFontSize(20);
	}
	QFont _font;
	_font.setFamily(settings()->getValue(FONT_FAMILY).toString());
	_font.setPointSize(settings()->getValue(FONT_SIZE).toInt());
	_font.setBold(true);
	_font.setItalic(false);
	_font.setUnderline(false);
	return _font;
}

void ThemeEngine::setFont(QFont newFont)
{
	setFontFamily(newFont.family());
	setFontSize(newFont.pointSize());
}

/*! Font family of the selected font. */
QString ThemeEngine::fontFamily(void)
{
	return font().family();
}

void ThemeEngine::setFontFamily(QString family)
{
	QStringList families = QmlUtils::fontFamilies(true);
	if(!families.contains(family))
		family = families[0];
	settings()->setValue(FONT_FAMILY, family);
	emit fontFamilyChanged();
}

/*! Point size of the selected font. */
int ThemeEngine::fontSize(void)
{
	return font().pointSize();
}

void ThemeEngine::setFontSize(int size)
{
	settings()->setValue(FONT_SIZE, size);
	emit fontSizeChanged();
}

/*! Increases (or decreases) font point size. */
void ThemeEngine::increaseFontSize(int delta)
{
	int currentSize = fontSize();
	currentSize += delta;
	if(currentSize > maxFontSize())
		currentSize = maxFontSize();
	if(currentSize <= 2)
		currentSize = 2;
	setFontSize(currentSize);
}

/*! Returns maximum font point size. */
int ThemeEngine::maxFontSize(void)
{
	return 24;
}

/*! Returns true if there's a custom exercise text color set. */
bool ThemeEngine::customExerciseTextColor(void)
{
	return settings()->getValue(CUSTOM_EX_TEXT_COLOR).toBool();
}

/*! Exercise text color. */
QColor ThemeEngine::exerciseTextColor(void)
{
	return QColor(settings()->getValue(EX_TEXT_R).toInt(),
		settings()->getValue(EX_TEXT_G).toInt(),
		settings()->getValue(EX_TEXT_B).toInt());
}

void ThemeEngine::setExerciseTextColor(QColor color)
{
	settings()->setValue(EX_TEXT_R, color.red());
	settings()->setValue(EX_TEXT_G, color.green());
	settings()->setValue(EX_TEXT_B, color.blue());
	settings()->setValue(CUSTOM_EX_TEXT_COLOR, true);
	emit exerciseTextColorChanged();
}

/*! Resets exercise text color. */
void ThemeEngine::resetExerciseTextColor(void)
{
	setExerciseTextColor(defaultExerciseTextColor(theme() == Theme::DarkTheme));
	settings()->setValue(CUSTOM_EX_TEXT_COLOR, false);
}

/*! Returns default exercise text color. */
QColor ThemeEngine::defaultExerciseTextColor(void)
{
	return defaultExerciseTextColor(theme() == Theme::DarkTheme);
}

/*! Returns default exercise text color for light or dark theme. */
QColor ThemeEngine::defaultExerciseTextColor(bool dark)
{
	return defaultExerciseTextColor(currentAccentColor(), dark);
}

/*! Returns default exercise text color for the given accent color. */
QColor ThemeEngine::defaultExerciseTextColor(QColor accent)
{
	return defaultExerciseTextColor(accent, theme() == Theme::DarkTheme);
}

/*! Returns default exercise text color for the given accent color and light or dark theme. */
QColor ThemeEngine::defaultExerciseTextColor(QColor accent, bool dark)
{
	if(dark)
		return accent.lighter(110);
	else
		return accent.darker(150);
}

/*! Returns true if there's a custom input text color set. */
bool ThemeEngine::customInputTextColor(void)
{

	return settings()->getValue(CUSTOM_IN_TEXT_COLOR).toBool();
}

/*! Input text color. */
QColor ThemeEngine::inputTextColor(void)
{
	return QColor(settings()->getValue(IN_TEXT_R).toInt(),
		settings()->getValue(IN_TEXT_G).toInt(),
		settings()->getValue(IN_TEXT_B).toInt());
}

void ThemeEngine::setInputTextColor(QColor color)
{
	settings()->setValue(IN_TEXT_R, color.red());
	settings()->setValue(IN_TEXT_G, color.green());
	settings()->setValue(IN_TEXT_B, color.blue());
	settings()->setValue(CUSTOM_IN_TEXT_COLOR, true);
	emit inputTextColorChanged();
}

/*! Resets input text color. */
void ThemeEngine::resetInputTextColor(void)
{
	setInputTextColor(defaultInputTextColor(theme() == Theme::DarkTheme));
	settings()->setValue(CUSTOM_IN_TEXT_COLOR, false);
}

/*! Returns default input text color. */
QColor ThemeEngine::defaultInputTextColor(bool dark)
{
	if(dark)
		return QColor(255, 255, 255);
	else
		return QColor(0, 0, 0);
}

/*! Returns true if there's a custom background color set. */
bool ThemeEngine::customBgColor(void)
{
	return settings()->getValue(CUSTOM_BG_COLOR).toBool();
}

/*! Background color. */
QColor ThemeEngine::bgColor(void)
{
	return QColor(settings()->getValue(BG_R).toInt(),
		settings()->getValue(BG_G).toInt(),
		settings()->getValue(BG_B).toInt());
}

void ThemeEngine::setBgColor(QColor color)
{
	settings()->setValue(BG_R, color.red());
	settings()->setValue(BG_G, color.green());
	settings()->setValue(BG_B, color.blue());
	settings()->setValue(CUSTOM_BG_COLOR, true);
	emit bgColorChanged();
}

/*! Resets background color. */
void ThemeEngine::resetBgColor(void)
{
	setBgColor(defaultBgColor(theme() == Theme::DarkTheme));
	settings()->setValue(CUSTOM_BG_COLOR, false);
}

/*! Returns default background color. */
QColor ThemeEngine::defaultBgColor(bool dark)
{
	if(dark)
		return QColor(31, 30, 28);
	else
		return QColor(255, 255, 255);
}

/*! Returns true if there's a custom paper color set. */
bool ThemeEngine::customPaperColor(void)
{
	return settings()->getValue(CUSTOM_PAPER_COLOR).toBool();
}

/*! Paper color. */
QColor ThemeEngine::paperColor(void)
{
	return QColor(settings()->getValue(PAPER_R).toInt(),
		settings()->getValue(PAPER_G).toInt(),
		settings()->getValue(PAPER_B).toInt());
}

void ThemeEngine::setPaperColor(QColor color)
{
	settings()->setValue(PAPER_R, color.red());
	settings()->setValue(PAPER_G, color.green());
	settings()->setValue(PAPER_B, color.blue());
	settings()->setValue(CUSTOM_PAPER_COLOR, true);
	emit paperColorChanged();
}

/*! Resets paper color. */
void ThemeEngine::resetPaperColor(void)
{
	setPaperColor(defaultPaperColor(theme() == Theme::DarkTheme));
	settings()->setValue(CUSTOM_PAPER_COLOR, false);
}

/*! Returns default paper color. */
QColor ThemeEngine::defaultPaperColor(bool dark)
{
	if(dark)
		return QColor(30, 30, 30);
	else
		return QColor(255, 255, 255);
}

/*! Returns true if there's a custom panel color set. */
bool ThemeEngine::customPanelColor(void)
{
	return settings()->getValue(CUSTOM_PANEL_COLOR).toBool();
}

/*! Panel color. */
QColor ThemeEngine::panelColor(void)
{
	return QColor(settings()->getValue(PANEL_R).toInt(),
		settings()->getValue(PANEL_G).toInt(),
		settings()->getValue(PANEL_B).toInt());
}

void ThemeEngine::setPanelColor(QColor color)
{
	settings()->setValue(PANEL_R, color.red());
	settings()->setValue(PANEL_G, color.green());
	settings()->setValue(PANEL_B, color.blue());
	settings()->setValue(CUSTOM_PANEL_COLOR, true);
	emit panelColorChanged();
}

/*! Resets panel color. */
void ThemeEngine::resetPanelColor(void)
{
	setPanelColor(defaultPanelColor(theme() == Theme::DarkTheme));
	settings()->setValue(CUSTOM_PANEL_COLOR, false);
}

/*! Returns default panel color. */
QColor ThemeEngine::defaultPanelColor(bool dark)
{
	if(dark)
		return QColor(41, 41, 41);
	else
		return QColor(255, 255, 255);
}

/*! Current accent color ID. */
ThemeEngine::AccentColor ThemeEngine::accentColor(void)
{
	return static_cast<AccentColor>(settings()->getValue(ACCENT_ID).toInt());
}

void ThemeEngine::setAccentColor(ThemeEngine::AccentColor color)
{
	settings()->setValue(ACCENT_ID, color);
	emit accentColorChanged();
	emit currentAccentColorChanged();
}

/*! Returns the accent color of the given ID. */
QColor ThemeEngine::getAccentColor(AccentColor colorId)
{
	bool light = theme() == Theme::LightTheme;
	switch(colorId)
	{
		case Accent_Red:
			return light ? QColor(191, 19, 24) : QColor(217, 80, 84);
			break;
		case Accent_Orange:
			return light ? QColor(191, 119, 19) : QColor(217, 159, 80);
			break;
		case Accent_Yellow:
			return light ? QColor(191, 188, 19) : QColor(217, 214, 80);
			break;
		case Accent_Green:
			return light ? QColor(43, 191, 19) : QColor(99, 217, 80);
			break;
		case Accent_LightBlue:
			return light ? QColor(19, 160, 191) : QColor(80, 192, 217);
			break;
		case Accent_Purple:
			return light ? QColor(19, 19, 191) : QColor(81, 81, 217);
			break;
		case Accent_Pink:
			return light ? QColor(138, 19, 191) : QColor(174, 80, 217);
			break;
		default:
			return QColor(0, 0, 0);
			break;
	}
}

/*! Current accent color. */
QColor ThemeEngine::currentAccentColor(void)
{
	return getAccentColor(accentColor());
}

/*! Current application theme. */
ThemeEngine::Theme ThemeEngine::theme(void)
{
	return static_cast<Theme>(settings()->getValue(APP_THEME).toInt());
}

void ThemeEngine::setTheme(ThemeEngine::Theme newTheme)
{
	// TODO: Remove style sheets
	switch(newTheme)
	{
		case Theme::DarkTheme:
			// Dark
			applyStyleSheetFromFile(":/dark-theme/style.qss");
			break;
		case Theme::LightTheme:
			// Light
			applyStyleSheetFromFile(":/light-theme/style.qss");
			break;
	}
	settings()->setValue(APP_THEME, static_cast<int>(newTheme));
	resetExerciseTextColor();
	resetInputTextColor();
	resetBgColor();
	resetPaperColor();
	resetPanelColor();
	emit themeChanged();
}

/*! Loads and sets application style. */
void ThemeEngine::updateTheme(void)
{
	setTheme(static_cast<Theme>(settings()->getValue(APP_THEME).toInt()));
}

/*! Sets theme based on system theme. */
void ThemeEngine::setDefaultTheme(void)
{
	if(qApp->palette().color(QPalette::Base).toHsv().value() < 50)
		setTheme(Theme::DarkTheme);
	else
		setTheme(Theme::LightTheme);
}

void ThemeEngine::applyStyleSheetFromFile(const QString &stylePath)
{
	QFile styleFile;
	styleFile.setFileName(stylePath);
	if(styleFile.open(QFile::ReadOnly | QFile::Text))
		qApp->setStyleSheet(styleFile.readAll());
}
