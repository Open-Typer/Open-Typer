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

#include "ThemeEngine.h"
#include "Settings.h"
#include "QmlUtils.h"

ThemeEngine globalThemeEngine;

/*! Constructs ThemeEngine. */
ThemeEngine::ThemeEngine(QObject *parent) :
	QObject(parent)
{
	// Connections
	connect(this, &ThemeEngine::fontBoldChanged, this, &ThemeEngine::fontStyleChanged);
	connect(this, &ThemeEngine::fontFamilyChanged, this, &ThemeEngine::fontChanged);
	connect(this, &ThemeEngine::fontSizeChanged, this, &ThemeEngine::fontChanged);
	connect(this, &ThemeEngine::fontStyleChanged, this, &ThemeEngine::fontChanged);
	connect(this, &ThemeEngine::exerciseTextColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::inputTextColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::bgColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::paperColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::panelColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::currentAccentColorChanged);
}

/*! Returns selected font. */
QFont ThemeEngine::font(void)
{
	if(!Settings::containsThemeFont() || !Settings::containsThemeFontSize() || !Settings::containsThemeFontBold())
	{
		setFontFamily("");
		setFontSize(20);
		setFontBold(true);
	}
	QFont _font;
	_font.setFamily(Settings::themeFont());
	_font.setPointSize(Settings::themeFontSize());
	_font.setBold(Settings::themeFontBold());
	_font.setItalic(Settings::themeFontItalic());
	_font.setUnderline(Settings::themeFontUnderline());
	return _font;
}

/*! Sets font. */
void ThemeEngine::setFont(QFont newFont)
{
	setFontFamily(newFont.family());
	setFontSize(newFont.pointSize());
	setFontBold(newFont.bold());
}

/*! Returns font family of selected font. */
QString ThemeEngine::fontFamily(void)
{
	return font().family();
}

/*! Sets font family. */
void ThemeEngine::setFontFamily(QString family)
{
	QStringList families = QmlUtils::fontFamilies(true);
	if(!families.contains(family))
		family = families[0];
	Settings::setThemeFont(family);
	emit fontFamilyChanged();
}

/*! Returns point size of selected font. */
int ThemeEngine::fontSize(void)
{
	return font().pointSize();
}

/*! Sets font point size. */
void ThemeEngine::setFontSize(int size)
{
	Settings::setThemeFontSize(size);
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

/*! Returns true if selected font is bold. */
bool ThemeEngine::fontBold(void)
{
	return font().bold();
}

/*! Sets font weight. */
void ThemeEngine::setFontBold(bool value)
{
	Settings::setThemeFontBold(value);
	emit fontBoldChanged();
}

/*! Returns true if there's a custom exercise text color set. */
bool ThemeEngine::customExerciseTextColor(void)
{
	return Settings::customExerciseTextColor();
}

/*! Returns exercise text color. */
QColor ThemeEngine::exerciseTextColor(void)
{
	return QColor(Settings::exerciseTextColor());
}

/*! Sets exercise text color. */
void ThemeEngine::setExerciseTextColor(QColor color)
{
	Settings::setExerciseTextColor(color.rgb());
	Settings::setCustomExerciseTextColor(true);
	emit exerciseTextColorChanged();
}

/*! Resets exercise text color. */
void ThemeEngine::resetExerciseTextColor(void)
{
	setExerciseTextColor(defaultExerciseTextColor(theme() == Theme::DarkTheme));
	Settings::setCustomExerciseTextColor(false);
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

	return Settings::customInputTextColor();
}

/*! Returns input text color. */
QColor ThemeEngine::inputTextColor(void)
{
	return QColor(Settings::inputTextColor());
}

/*! Sets input text color. */
void ThemeEngine::setInputTextColor(QColor color)
{
	Settings::setInputTextColor(color.rgb());
	Settings::setCustomInputTextColor(true);
	emit inputTextColorChanged();
}

/*! Resets input text color. */
void ThemeEngine::resetInputTextColor(void)
{
	setInputTextColor(defaultInputTextColor(theme() == Theme::DarkTheme));
	Settings::setCustomInputTextColor(false);
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
	return Settings::customBgColor();
}

/*! Returns background color. */
QColor ThemeEngine::bgColor(void)
{
	return QColor(Settings::bgColor());
}

/*! Sets background color. */
void ThemeEngine::setBgColor(QColor color)
{
	Settings::setBgColor(color.rgb());
	Settings::setCustomBgColor(true);
	emit bgColorChanged();
}

/*! Resets background color. */
void ThemeEngine::resetBgColor(void)
{
	setBgColor(defaultBgColor(theme() == Theme::DarkTheme));
	Settings::setCustomBgColor(false);
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
	return Settings::customPaperColor();
}

/*! Returns paper color. */
QColor ThemeEngine::paperColor(void)
{
	return QColor(Settings::paperColor());
}

/*! Sets paper color. */
void ThemeEngine::setPaperColor(QColor color)
{
	Settings::setPaperColor(color.rgb());
	Settings::setCustomPaperColor(true);
	emit paperColorChanged();
}

/*! Resets paper color. */
void ThemeEngine::resetPaperColor(void)
{
	setPaperColor(defaultPaperColor(theme() == Theme::DarkTheme));
	Settings::setCustomPaperColor(false);
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
	return Settings::customPanelColor();
}

/*! Returns panel color. */
QColor ThemeEngine::panelColor(void)
{
	return QColor(Settings::panelColor());
}

/*! Sets panel color. */
void ThemeEngine::setPanelColor(QColor color)
{
	Settings::setPanelColor(color.rgb());
	Settings::setCustomPanelColor(true);
	emit panelColorChanged();
}

/*! Resets panel color. */
void ThemeEngine::resetPanelColor(void)
{
	setPanelColor(defaultPanelColor(theme() == Theme::DarkTheme));
	Settings::setCustomPanelColor(false);
}

/*! Returns default panel color. */
QColor ThemeEngine::defaultPanelColor(bool dark)
{
	if(dark)
		return QColor(41, 41, 41);
	else
		return QColor(255, 255, 255);
}

/*! Returns current accent color ID. */
ThemeEngine::AccentColor ThemeEngine::accentColor(void)
{
	return Settings::accentColorId();
}

/*! Sets accent color. */
void ThemeEngine::setAccentColor(ThemeEngine::AccentColor color)
{
	Settings::setAccentColorId(color);
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

/*! Returns current accent color */
QColor ThemeEngine::currentAccentColor(void)
{
	return getAccentColor(accentColor());
}

/*! Returns current application theme. */
ThemeEngine::Theme ThemeEngine::theme(void)
{
	return Settings::appTheme();
}

/*! Sets application theme. */
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
	Settings::setAppTheme(newTheme);
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
	setTheme(Settings::appTheme());
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
