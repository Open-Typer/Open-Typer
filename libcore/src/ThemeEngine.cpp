/*
 * ThemeEngine.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

ThemeEngine globalThemeEngine;

/*! Constructs ThemeEngine. */
ThemeEngine::ThemeEngine(QObject *parent) :
	QObject(parent)
{
	updateThemeList();
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
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::styleChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::fontBoldChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::fontItalicChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::fontUnderlineChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::fontFamilyChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::fontSizeChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::exerciseTextColorChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::inputTextColorChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::bgColorChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::paperColorChanged);
	connect(this, &ThemeEngine::themeChanged, this, &ThemeEngine::panelColorChanged);
}

/*! Regenerates the list of themes. */
void ThemeEngine::updateThemeList(void)
{
	themes.clear();
	QVariantMap themeMap;
	// Default (obsolete)
	themeMap.clear();
	themes += themeMap;
	// Dark
	themeMap.clear();
	themeMap.insert("name", tr("Dark"));
	themeMap.insert("id", "dark");
	themeMap.insert("icon", "dark.png");
	themeMap.insert("style", static_cast<int>(Style::DarkStyle));
	themes += themeMap;
	// Light
	themeMap.clear();
	themeMap.insert("name", tr("Light"));
	themeMap.insert("id", "light");
	themeMap.insert("icon", "light.png");
	themeMap.insert("style", static_cast<int>(Style::LightStyle));
	themes += themeMap;
	// Green
	themeMap.clear();
	themeMap.insert("name", tr("Green"));
	themeMap.insert("id", "green");
	themeMap.insert("icon", "green.png");
	themeMap.insert("style", static_cast<int>(Style::LightStyle));
	themeMap.insert("bgColor", qRgb(0, 108, 0));
	themeMap.insert("panelColor", qRgb(175, 175, 175));
	themes += themeMap;
	// Light blue
	themeMap.clear();
	themeMap.insert("name", tr("Light blue"));
	themeMap.insert("id", "light_blue");
	themeMap.insert("icon", "light_blue.png");
	themeMap.insert("style", static_cast<int>(Style::LightStyle));
	themeMap.insert("bgColor", qRgb(228, 245, 255));
	themeMap.insert("panelColor", qRgb(180, 229, 255));
	themes += themeMap;
	// Custom
	themeMap.clear();
	themeMap.insert("name", tr("Custom"));
	themeMap.insert("id", "custom");
	themeMap.insert("icon", "custom.png");
	themes += themeMap;
}

/*! Returns selected font. */
QFont ThemeEngine::font(void)
{
	QFont _font;
	_font.setStyleHint(QFont::TypeWriter);
	_font.setFixedPitch(true);
	QString fontFamily = Settings::themeFont();
	QString oldFamily = fontFamily;
	if(fontFamily == "")
		fontFamily = "Courier New";
	_font.setFamily(fontFamily);
	QFontDatabase fontDB;
	if(!fontDB.families().contains(fontFamily))
		fontFamily = _font.defaultFamily();
	_font.setPointSize(Settings::themeFontSize());
	_font.setBold(Settings::themeFontBold());
	_font.setItalic(Settings::themeFontItalic());
	_font.setUnderline(Settings::themeFontUnderline());
	if(fontFamily != oldFamily)
		Settings::setThemeFont(fontFamily);
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
	QFont _font;
	_font.setStyleHint(QFont::TypeWriter);
	_font.setFixedPitch(true);
	if(family == "")
		family = "Courier New";
	_font.setFamily(family);
	QFontDatabase fontDB;
	if(!fontDB.families().contains(family))
		family = _font.defaultFamily();
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
	setExerciseTextColor(QColor(0, 125, 175));
	Settings::setCustomExerciseTextColor(false);
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
	QPalette palette;
	if(parent() == nullptr)
		palette = qApp->palette();
	else
		palette = ((QWidget *) parent())->palette();
	setInputTextColor(palette.color(QPalette::Text));
	Settings::setCustomInputTextColor(false);
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
	QPalette palette;
	if(parent() == nullptr)
		palette = qApp->palette();
	else
		palette = ((QWidget *) parent())->palette();
	setBgColor(palette.color(QPalette::Window));
	Settings::setCustomBgColor(false);
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
	switch(style())
	{
		case Style::DarkStyle:
			setPaperColor(QColor(15, 25, 35));
			break;
		case Style::LightStyle:
			setPaperColor(QColor(255, 255, 255));
			break;
		default:
			QPalette palette;
			if(parent() == nullptr)
				palette = qApp->palette();
			else
				palette = ((QWidget *) parent())->palette();
			setPaperColor(palette.color(QPalette::Base));
			break;
	}
	Settings::setCustomPaperColor(false);
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
	switch(style())
	{
		case Style::DarkStyle:
			setPanelColor(QColor(20, 33, 47));
			break;
		case Style::LightStyle:
			setPanelColor(QColor(255, 255, 255));
			break;
		default:
			QPalette palette;
			if(parent() == nullptr)
				palette = qApp->palette();
			else
				palette = ((QWidget *) parent())->palette();
			setPanelColor(palette.color(QPalette::Midlight));
			break;
	}
	Settings::setCustomPanelColor(false);
}

/*! Returns panel style sheet. */
QString ThemeEngine::panelStyleSheet(void)
{
	return "QMenuBar, QFrame, QCheckBox { background-color: rgb(" + QString::number(panelColor().red()) + ", " + QString::number(panelColor().green()) + ", " + QString::number(panelColor().blue()) + ");}";
}

/*! Returns current application style. */
ThemeEngine::Style ThemeEngine::style(void)
{
	return Settings::applicationStyle();
}

/*! Sets application style. */
void ThemeEngine::setStyle(ThemeEngine::Style newStyle)
{
	switch(newStyle)
	{
		case Style::SystemStyle:
			// System (default)
			qApp->setStyleSheet("");
			break;
		case Style::DarkStyle:
			// Dark
			applyStyleSheetFromFile(":/dark-theme/style.qss");
			break;
		case Style::LightStyle:
			// Light
			applyStyleSheetFromFile(":/light-theme/style.qss");
			break;
	}
	Settings::setApplicationStyle(newStyle);
	emit styleChanged();
}

/*! Loads and sets application style. */
void ThemeEngine::updateStyle(void)
{
	setStyle(Settings::applicationStyle());
}

/*! Returns current application theme. */
int ThemeEngine::theme(void)
{
	QString id = Settings::applicationTheme();
	for(int i = 0; i < themes.count(); i++)
	{
		if(themes[i]["id"].toString() == id)
			return i;
	}
	return 0;
}

/*! Sets application theme. */
void ThemeEngine::setTheme(int index)
{
	blockSignals(true);
	if(themeName(index) != "custom")
	{
		QVariantMap themeMap = themes[index];
		// Style
		if(themeMap.contains("style"))
			setStyle(static_cast<Style>(themeMap["style"].toInt()));
		else
			setStyle(Style::SystemStyle);
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
			fontSize = 20;
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
		setFontFamily(fontFamily);
		setFontSize(fontSize);
		setFontBold(fontBold);
		setFontItalic(fontItalic);
		setFontUnderline(fontUnderline);
		// Colors
		if(themeMap.contains("exerciseTextColor"))
		{
			QRgb color = themeMap["levelTextColor"].toUInt();
			setExerciseTextColor(QColor(color));
		}
		else
			resetExerciseTextColor();
		if(themeMap.contains("inputTextColor"))
		{
			QRgb color = themeMap["inputTextColor"].toUInt();
			setInputTextColor(QColor(color));
		}
		else
			resetInputTextColor();
		if(themeMap.contains("bgColor"))
		{
			QRgb color = themeMap["bgColor"].toUInt();
			setBgColor(QColor(color));
		}
		else
			resetBgColor();
		if(themeMap.contains("paperColor"))
		{
			QRgb color = themeMap["paperColor"].toUInt();
			setPaperColor(QColor(color));
		}
		else
			resetPaperColor();
		if(themeMap.contains("panelColor"))
		{
			QRgb color = themeMap["panelColor"].toUInt();
			setPanelColor(QColor(color));
		}
		else
			resetPanelColor();
	}
	Settings::setApplicationTheme(themeName(index));
	blockSignals(false);
	if(themeName(index) != "custom")
		emit themeChanged();
}

void ThemeEngine::applyStyleSheetFromFile(const QString &stylePath)
{
	QFile styleFile;
	styleFile.setFileName(stylePath);
	if(styleFile.open(QFile::ReadOnly | QFile::Text))
		qApp->setStyleSheet(styleFile.readAll());
}

/*! Returns the name of the theme. */
QString ThemeEngine::themeName(int index)
{
	return themes[index]["id"].toString();
}

/*! Returns list of themes. */
const QList<QVariantMap> &ThemeEngine::themeList(void)
{
	return themes;
}
