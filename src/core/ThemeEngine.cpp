/*
 * ThemeEngine.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#include "core/ThemeEngine.h"

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
	themeMap.insert("style", 1);
	themes += themeMap;
	// Light
	themeMap.clear();
	themeMap.insert("name", tr("Light"));
	themeMap.insert("id", "light");
	themeMap.insert("icon", "light.png");
	themeMap.insert("style", 2);
	themes += themeMap;
	// Green
	themeMap.clear();
	themeMap.insert("name", tr("Green"));
	themeMap.insert("id", "green");
	themeMap.insert("icon", "green.png");
	themeMap.insert("style", 2);
	themeMap.insert("bgColor", qRgb(0, 108, 0));
	themeMap.insert("panelColor", qRgb(175, 175, 175));
	themes += themeMap;
	// Light blue
	themeMap.clear();
	themeMap.insert("name", tr("Light blue"));
	themeMap.insert("id", "light_blue");
	themeMap.insert("icon", "light_blue.png");
	themeMap.insert("style", 2);
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
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QFont _font;
	_font.setStyleHint(QFont::TypeWriter);
	_font.setFixedPitch(true);
	QString fontFamily = settings.value("theme/font", "").toString();
	QString oldFamily = fontFamily;
	if(fontFamily == "")
		fontFamily = "Courier New";
	_font.setFamily(fontFamily);
	QFontDatabase fontDB;
	if(!fontDB.families().contains(fontFamily))
		fontFamily = _font.defaultFamily();
	_font.setPointSize(settings.value("theme/fontsize", "20").toInt());
	_font.setBold(settings.value("theme/fontbold", "true").toBool());
	_font.setItalic(settings.value("theme/fontitalic", "false").toBool());
	_font.setUnderline(settings.value("theme/fontunderline", "false").toBool());
	if(fontFamily != oldFamily)
		settings.setValue("theme/font", fontFamily);
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
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QFont _font;
	_font.setStyleHint(QFont::TypeWriter);
	_font.setFixedPitch(true);
	if(family == "")
		family = "Courier New";
	_font.setFamily(family);
	QFontDatabase fontDB;
	if(!fontDB.families().contains(family))
		family = _font.defaultFamily();
	settings.setValue("theme/font", family);
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
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	settings.setValue("theme/fontsize", size);
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
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	settings.setValue("theme/fontbold", value);
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
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	settings.setValue("theme/fontitalic", value);
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
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	settings.setValue("theme/fontunderline", value);
	emit fontUnderlineChanged();
}

/*! Returns true if there's a custom exercise text color set. */
bool ThemeEngine::customExerciseTextColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return settings.value("theme/customleveltextcolor", "false").toBool();
}

/*! Returns exercise text color. */
QColor ThemeEngine::exerciseTextColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return QColor(settings.value("theme/leveltextred", "0").toInt(),
		settings.value("theme/leveltextgreen", "0").toInt(),
		settings.value("theme/leveltextblue", "0").toInt());
}

/*! Sets exercise text color. */
void ThemeEngine::setExerciseTextColor(QColor color)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	settings.setValue("theme/leveltextred", color.red());
	settings.setValue("theme/leveltextgreen", color.green());
	settings.setValue("theme/leveltextblue", color.blue());
	settings.setValue("theme/customleveltextcolor", true);
	emit exerciseTextColorChanged();
}

/*! Resets exercise text color. */
void ThemeEngine::resetExerciseTextColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	setExerciseTextColor(QColor(0, 125, 175));
	settings.setValue("theme/customleveltextcolor", false);
}

/*! Returns exercise text style sheet. */
QString ThemeEngine::exerciseTextStyleSheet(void)
{
	return "QWidget { color: rgb(" + QString::number(exerciseTextColor().red()) + ", " + QString::number(exerciseTextColor().green()) + ", " + QString::number(exerciseTextColor().blue()) + "); margin: 0px; padding: 0px; }";
}

/*! Returns true if there's a custom input text color set. */
bool ThemeEngine::customInputTextColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return settings.value("theme/custominputtextcolor", "false").toBool();
}

/*! Returns input text color. */
QColor ThemeEngine::inputTextColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return QColor(settings.value("theme/inputtextred", "0").toInt(),
		settings.value("theme/inputtextgreen", "0").toInt(),
		settings.value("theme/inputtextblue", "0").toInt());
}

/*! Sets input text color. */
void ThemeEngine::setInputTextColor(QColor color)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	settings.setValue("theme/inputtextred", color.red());
	settings.setValue("theme/inputtextgreen", color.green());
	settings.setValue("theme/inputtextblue", color.blue());
	settings.setValue("theme/custominputtextcolor", true);
	emit inputTextColorChanged();
}

/*! Resets input text color. */
void ThemeEngine::resetInputTextColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QPalette palette;
	if(parent() == nullptr)
		palette = qApp->palette();
	else
		palette = ((QWidget *) parent())->palette();
	setInputTextColor(palette.color(QPalette::Text));
	settings.setValue("theme/custominputtextcolor", false);
}

/*! Returns input text style sheet. */
QString ThemeEngine::inputTextStyleSheet(void)
{
	return "QWidget { color: rgb(" + QString::number(inputTextColor().red()) + ", " + QString::number(inputTextColor().green()) + ", " + QString::number(inputTextColor().blue()) + "); margin: 0px; padding: 0px; background-color: rgba(0,0,0,0); }";
}

/*! Returns true if there's a custom background color set. */
bool ThemeEngine::customBgColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return settings.value("theme/custombgcolor", "false").toBool();
}

/*! Returns background color. */
QColor ThemeEngine::bgColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return QColor(settings.value("theme/bgred", "0").toInt(),
		settings.value("theme/bggreen", "0").toInt(),
		settings.value("theme/bgblue", "0").toInt());
}

/*! Sets background color. */
void ThemeEngine::setBgColor(QColor color)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	settings.setValue("theme/bgred", color.red());
	settings.setValue("theme/bggreen", color.green());
	settings.setValue("theme/bgblue", color.blue());
	settings.setValue("theme/custombgcolor", true);
	emit bgColorChanged();
}

/*! Resets background color. */
void ThemeEngine::resetBgColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QPalette palette;
	if(parent() == nullptr)
		palette = qApp->palette();
	else
		palette = ((QWidget *) parent())->palette();
	setBgColor(palette.color(QPalette::Window));
	settings.setValue("theme/custombgcolor", false);
}

/*! Returns background style sheet. */
QString ThemeEngine::bgStyleSheet(void)
{
	return "QFrame, #centralwidget { background-color: rgb(" + QString::number(bgColor().red()) + ", " + QString::number(bgColor().green()) + ", " + QString::number(bgColor().blue()) + ");}";
}

/*! Returns true if there's a custom paper color set. */
bool ThemeEngine::customPaperColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return settings.value("theme/custompapercolor", "false").toBool();
}

/*! Returns paper color. */
QColor ThemeEngine::paperColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return QColor(settings.value("theme/paperred", "0").toInt(),
		settings.value("theme/papergreen", "0").toInt(),
		settings.value("theme/paperblue", "0").toInt());
}

/*! Sets paper color. */
void ThemeEngine::setPaperColor(QColor color)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	settings.setValue("theme/paperred", color.red());
	settings.setValue("theme/papergreen", color.green());
	settings.setValue("theme/paperblue", color.blue());
	settings.setValue("theme/custompapercolor", true);
	emit paperColorChanged();
}

/*! Resets paper color. */
void ThemeEngine::resetPaperColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	switch(style())
	{
		case DarkStyle:
			setPaperColor(QColor(15, 25, 35));
			break;
		case LightStyle:
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
	settings.setValue("theme/custompapercolor", false);
}

/*! Returns paper style sheet. */
QString ThemeEngine::paperStyleSheet(void)
{
	return "QFrame { background-color: rgb(" + QString::number(paperColor().red()) + ", " + QString::number(paperColor().green()) + ", " + QString::number(paperColor().blue()) + ");}";
}

/*! Returns true if there's a custom panel color set. */
bool ThemeEngine::customPanelColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return settings.value("theme/custompanelcolor", "false").toBool();
}

/*! Returns panel color. */
QColor ThemeEngine::panelColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return QColor(settings.value("theme/panelred", "0").toInt(),
		settings.value("theme/panelgreen", "0").toInt(),
		settings.value("theme/panelblue", "0").toInt());
}

/*! Sets panel color. */
void ThemeEngine::setPanelColor(QColor color)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	settings.setValue("theme/panelred", color.red());
	settings.setValue("theme/panelgreen", color.green());
	settings.setValue("theme/panelblue", color.blue());
	settings.setValue("theme/custompanelcolor", true);
	emit panelColorChanged();
}

/*! Resets panel color. */
void ThemeEngine::resetPanelColor(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	switch(style())
	{
		case DarkStyle:
			setPanelColor(QColor(20, 33, 47));
			break;
		case LightStyle:
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
	settings.setValue("theme/custompanelcolor", false);
}

/*! Returns panel style sheet. */
QString ThemeEngine::panelStyleSheet(void)
{
	return "QMenuBar, QFrame, QCheckBox { background-color: rgb(" + QString::number(panelColor().red()) + ", " + QString::number(panelColor().green()) + ", " + QString::number(panelColor().blue()) + ");}";
}

/*! Returns current application style. */
ThemeEngine::Style ThemeEngine::style(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	return (ThemeEngine::Style) settings.value("theme/theme", "0").toInt();
}

/*! Sets application style. */
void ThemeEngine::setStyle(ThemeEngine::Style newStyle)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QFile styleFile;
	switch(newStyle)
	{
		case SystemStyle:
			// System (default)
			qApp->setStyleSheet("");
			break;
		case DarkStyle:
			// Dark
			styleFile.setFileName(":/dark-theme/style.qss");
			if(styleFile.open(QFile::ReadOnly | QFile::Text))
				qApp->setStyleSheet(styleFile.readAll());
			break;
		case LightStyle:
			// Light
			styleFile.setFileName(":/light-theme/style.qss");
			if(styleFile.open(QFile::ReadOnly | QFile::Text))
				qApp->setStyleSheet(styleFile.readAll());
			break;
	}
	settings.setValue("theme/theme", (int) newStyle);
	emit styleChanged();
}

/*! Loads and sets application style. */
void ThemeEngine::updateStyle(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	setStyle((ThemeEngine::Style) settings.value("theme/theme", "0").toInt());
}

/*! Returns current application theme. */
int ThemeEngine::theme(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	QString id = settings.value("theme/fulltheme", "default").toString();
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
	QSettings settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat);
	blockSignals(true);
	if(themeName(index) != "custom")
	{
		QVariantMap themeMap = themes[index];
		// Style
		if(themeMap.contains("style"))
			setStyle((Style) themeMap["style"].toInt());
		else
			setStyle(SystemStyle);
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
	settings.setValue("theme/fulltheme", themeName(index));
	blockSignals(false);
	if(themeName(index) != "custom")
		emit themeChanged();
}

/*! Returns the name of the theme. */
QString ThemeEngine::themeName(int index)
{
	return themes[index]["id"].toString();
}

/*! Returns list of themes. */
QList<QVariantMap> ThemeEngine::themeList(void)
{
	return themes;
}
