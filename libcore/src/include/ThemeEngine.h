/*
 * ThemeEngine.h
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

#ifndef THEMEENGINE_H
#define THEMEENGINE_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QFont>
#include <QFontDatabase>
#include <QColor>
#include <QFile>
#include "FileUtils.h"

class Settings;

/*! \brief The ThemeEngine class provides theme functions. */
class CORE_LIB_EXPORT ThemeEngine : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
		Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
		Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
		Q_PROPERTY(bool fontBold READ fontBold WRITE setFontBold NOTIFY fontBoldChanged)
		Q_PROPERTY(bool fontItalic READ fontItalic WRITE setFontItalic NOTIFY fontItalicChanged)
		Q_PROPERTY(bool fontUnderline READ fontUnderline WRITE setFontUnderline NOTIFY fontUnderlineChanged)
		Q_PROPERTY(QColor exerciseTextColor READ exerciseTextColor WRITE setExerciseTextColor NOTIFY exerciseTextColorChanged)
		Q_PROPERTY(QColor inputTextColor READ inputTextColor WRITE setInputTextColor NOTIFY inputTextColorChanged)
		Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor NOTIFY bgColorChanged)
		Q_PROPERTY(QColor paperColor READ paperColor WRITE setPaperColor NOTIFY paperColorChanged)
		Q_PROPERTY(QColor panelColor READ panelColor WRITE setPanelColor NOTIFY panelColorChanged)
		Q_PROPERTY(Style style READ style WRITE setStyle NOTIFY styleChanged)
		Q_PROPERTY(int theme READ theme WRITE setTheme NOTIFY themeChanged())
	public:
		ThemeEngine(QObject *parent = nullptr);
		void updateThemeList(void);

		enum class Style
		{
			SystemStyle = 0,
			DarkStyle = 1,
			LightStyle = 2
		};
		Q_ENUM(Style)

		// Font
		Q_INVOKABLE static QFont font(void);
		Q_INVOKABLE void setFont(QFont newFont);
		Q_INVOKABLE static QFont errorFont(void);
		Q_INVOKABLE static QString fontFamily(void);
		Q_INVOKABLE void setFontFamily(QString family);
		Q_INVOKABLE static int fontSize(void);
		Q_INVOKABLE void setFontSize(int size);
		Q_INVOKABLE void increaseFontSize(int delta);
		Q_INVOKABLE static int maxFontSize(void);
		Q_INVOKABLE static bool fontBold(void);
		Q_INVOKABLE void setFontBold(bool value);
		Q_INVOKABLE static bool fontItalic(void);
		Q_INVOKABLE void setFontItalic(bool value);
		Q_INVOKABLE static bool fontUnderline(void);
		Q_INVOKABLE void setFontUnderline(bool value);
		// Exercise text color
		Q_INVOKABLE static bool customExerciseTextColor(void);
		Q_INVOKABLE static QColor exerciseTextColor(void);
		Q_INVOKABLE void setExerciseTextColor(QColor color);
		Q_INVOKABLE void resetExerciseTextColor(void);
		Q_INVOKABLE static QString exerciseTextStyleSheet(void);
		// Input text color
		Q_INVOKABLE static bool customInputTextColor(void);
		Q_INVOKABLE static QColor inputTextColor(void);
		Q_INVOKABLE void setInputTextColor(QColor color);
		Q_INVOKABLE void resetInputTextColor(void);
		Q_INVOKABLE static QString inputTextStyleSheet(void);
		// Background color
		Q_INVOKABLE static bool customBgColor(void);
		Q_INVOKABLE static QColor bgColor(void);
		Q_INVOKABLE void setBgColor(QColor color);
		Q_INVOKABLE void resetBgColor(void);
		Q_INVOKABLE static QString bgStyleSheet(void);
		// Paper color
		Q_INVOKABLE static bool customPaperColor(void);
		Q_INVOKABLE static QColor paperColor(void);
		Q_INVOKABLE void setPaperColor(QColor color);
		Q_INVOKABLE void resetPaperColor(void);
		Q_INVOKABLE static QString paperStyleSheet(void);
		// Panel color
		Q_INVOKABLE static bool customPanelColor(void);
		Q_INVOKABLE static QColor panelColor(void);
		Q_INVOKABLE void setPanelColor(QColor color);
		Q_INVOKABLE void resetPanelColor(void);
		Q_INVOKABLE static QString panelStyleSheet(void);
		// Style
		Q_INVOKABLE static ThemeEngine::Style style(void);
		Q_INVOKABLE void setStyle(ThemeEngine::Style newStyle);
		Q_INVOKABLE void updateStyle(void);
		// Theme
		Q_INVOKABLE int theme(void);
		Q_INVOKABLE void setTheme(int index);
		Q_INVOKABLE QString themeName(int index);
		Q_INVOKABLE const QList<QVariantMap> &themeList(void);

	private:
		void applyStyleSheetFromFile(const QString &stylePath);

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

extern ThemeEngine CORE_LIB_EXPORT globalThemeEngine;

#endif // THEMEENGINE_H
