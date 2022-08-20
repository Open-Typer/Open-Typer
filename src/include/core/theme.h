/*
 * theme.h
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

#ifndef THEME_H
#define THEME_H

#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QFont>
#include <QFontDatabase>
#include <QColor>
#include <QPalette>
#include <QSettings>
#include <QFile>
#include "core/utils.h"

/*! \brief The themeEngine class provides theme functions. */
class themeEngine : public QObject
{
		Q_OBJECT
	public:
		themeEngine(QObject *parent = nullptr);
		void updateThemeList(void);

		enum Style
		{
			SystemStyle = 0,
			DarkStyle = 1,
			LightStyle = 2
		};

		// Font
		static QFont font(void);
		void setFont(QFont newFont);
		static QFont errorFont(void);
		static QString fontFamily(void);
		void setFontFamily(QString family);
		static int fontSize(void);
		void setFontSize(int size);
		void increaseFontSize(int delta);
		static int maxFontSize(void);
		static bool fontBold(void);
		void setFontBold(bool value);
		static bool fontItalic(void);
		void setFontItalic(bool value);
		static bool fontUnderline(void);
		void setFontUnderline(bool value);
		// Exercise text color
		static bool customExerciseTextColor(void);
		static QColor exerciseTextColor(void);
		void setExerciseTextColor(QColor color);
		void resetExerciseTextColor(void);
		static QString exerciseTextStyleSheet(void);
		// Input text color
		static bool customInputTextColor(void);
		static QColor inputTextColor(void);
		void setInputTextColor(QColor color);
		void resetInputTextColor(void);
		static QString inputTextStyleSheet(void);
		// Background color
		static bool customBgColor(void);
		static QColor bgColor(void);
		void setBgColor(QColor color);
		void resetBgColor(void);
		static QString bgStyleSheet(void);
		// Paper color
		static bool customPaperColor(void);
		static QColor paperColor(void);
		void setPaperColor(QColor color);
		void resetPaperColor(void);
		static QString paperStyleSheet(void);
		// Panel color
		static bool customPanelColor(void);
		static QColor panelColor(void);
		void setPanelColor(QColor color);
		void resetPanelColor(void);
		static QString panelStyleSheet(void);
		// Style
		static Style style(void);
		void setStyle(Style newStyle);
		void updateStyle(void);
		// Theme
		int theme(void);
		void setTheme(int index);
		QString themeName(int index);
		QList<QVariantMap> themeList(void);

	private:
		QList<QVariantMap> themes;

	signals:
		/*! A signal, which is emitted when the font changes. */
		void fontChanged();
		/*! A signal, which is emitted when the font family changes. */
		void fontFamilyChanged();
		/*! A signal, which is emitted when the font point size changes. */
		void fontSizeChanged();
		/*! A signal, which is emitted when the font style (bold, italic or underline) changes. */
		void fontStyleChanged();
		/*! A signal, which is emitted when the font weight changes. */
		void fontBoldChanged();
		/*! A signal, which is emitted when the font italic style changes. */
		void fontItalicChanged();
		/*! A signal, which is emitted when the font underline changes. */
		void fontUnderlineChanged();
		/*! A signal, which is emitted when a color changes. */
		void colorChanged();
		/*! A signal, which is emitted when exercise text color changes. */
		void exerciseTextColorChanged();
		/*! A signal, which is emitted when input text color changes. */
		void inputTextColorChanged();
		/*! A signal, which is emitted when background color changes. */
		void bgColorChanged();
		/*! A signal, which is emitted when paper color changes. */
		void paperColorChanged();
		/*! A signal, which is emitted when panel color changes. */
		void panelColorChanged();
		/*! A signal, which is emitted when the application style changes. */
		void styleChanged();
		/*! A signal, which is emitted when the application theme changes. */
		void themeChanged();
};

extern themeEngine globalThemeEngine;

#endif // THEME_H
