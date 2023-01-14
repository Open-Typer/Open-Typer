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
	connect(this, &ThemeEngine::fontItalicChanged, this, &ThemeEngine::fontStyleChanged);
	connect(this, &ThemeEngine::fontUnderlineChanged, this, &ThemeEngine::fontStyleChanged);
	connect(this, &ThemeEngine::fontFamilyChanged, this, &ThemeEngine::fontChanged);
	connect(this, &ThemeEngine::fontSizeChanged, this, &ThemeEngine::fontChanged);
	connect(this, &ThemeEngine::fontStyleChanged, this, &ThemeEngine::fontChanged);
	connect(this, &ThemeEngine::exerciseTextColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::inputTextColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::bgColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::paperColorChanged, this, &ThemeEngine::colorChanged);
	connect(this, &ThemeEngine::panelColorChanged, this, &ThemeEngine::colorChanged);
}

/*! Returns selected font. */
QFont ThemeEngine::font(void)
{
	if(!Settings::containsThemeFont() || !Settings::containsThemeFontSize() || !Settings::containsThemeFontBold() || !Settings::containsThemeFontItalic() || !Settings::containsThemeFontUnderline())
	{
		setFontFamily("");
		setFontSize(20);
		setFontBold(true);
		setFontItalic(false);
		setFontUnderline(false);
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
	setFontItalic(newFont.italic());
	setFontUnderline(newFont.underline());
}

/*! Returns selected font optimized for error text. */
QFont ThemeEngine::errorFont(void)
{
	QFont _font = font();
	_font.setUnderline(false);
	return _font;
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

/*! Returns true if selected font style is set to italic. */
bool ThemeEngine::fontItalic(void)
{
	return font().italic();
}

/*! Sets font style to italic if value is true. */
void ThemeEngine::setFontItalic(bool value)
{
	Settings::setThemeFontItalic(value);
	emit fontItalicChanged();
}

/*! Returns true if selected font's underline is enabled. */
bool ThemeEngine::fontUnderline(void)
{
	return font().underline();
}

/*! Sets font underline. */
void ThemeEngine::setFontUnderline(bool value)
{
	Settings::setThemeFontUnderline(value);
	emit fontUnderlineChanged();
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
QColor ThemeEngine::defaultExerciseTextColor(bool dark)
{
	Q_UNUSED(dark);
	return QColor(0, 125, 175);
}

/*! Returns exercise text style sheet. */
QString ThemeEngine::exerciseTextStyleSheet(void)
{
	return "QWidget { color: rgb(" + QString::number(exerciseTextColor().red()) + ", " + QString::number(exerciseTextColor().green()) + ", " + QString::number(exerciseTextColor().blue()) + "); margin: 0px; padding: 0px; }";
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

/*! Returns input text style sheet. */
QString ThemeEngine::inputTextStyleSheet(void)
{
	return "QWidget { color: rgb(" + QString::number(inputTextColor().red()) + ", " + QString::number(inputTextColor().green()) + ", " + QString::number(inputTextColor().blue()) + "); margin: 0px; padding: 0px; background-color: rgba(0,0,0,0); }";
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

/*! Returns background style sheet. */
QString ThemeEngine::bgStyleSheet(void)
{
	return "QFrame, #centralwidget { background-color: rgb(" + QString::number(bgColor().red()) + ", " + QString::number(bgColor().green()) + ", " + QString::number(bgColor().blue()) + ");}";
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

/*! Returns paper style sheet. */
QString ThemeEngine::paperStyleSheet(void)
{
	return "QFrame { background-color: rgb(" + QString::number(paperColor().red()) + ", " + QString::number(paperColor().green()) + ", " + QString::number(paperColor().blue()) + ");}";
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

/*! Returns panel style sheet. */
QString ThemeEngine::panelStyleSheet(void)
{
	return "QMenuBar, QFrame, QCheckBox { background-color: rgb(" + QString::number(panelColor().red()) + ", " + QString::number(panelColor().green()) + ", " + QString::number(panelColor().blue()) + ");}";
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
