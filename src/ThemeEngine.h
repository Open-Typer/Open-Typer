/*
 * ThemeEngine.h
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

#ifndef THEMEENGINE_H
#define THEMEENGINE_H

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
class Q_DECL_EXPORT ThemeEngine : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
		Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
		Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
		Q_PROPERTY(bool fontBold READ fontBold WRITE setFontBold NOTIFY fontBoldChanged) // deprecated
		Q_PROPERTY(QColor exerciseTextColor READ exerciseTextColor WRITE setExerciseTextColor NOTIFY exerciseTextColorChanged)
		Q_PROPERTY(QColor inputTextColor READ inputTextColor WRITE setInputTextColor NOTIFY inputTextColorChanged)
		Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor NOTIFY bgColorChanged)
		Q_PROPERTY(QColor paperColor READ paperColor WRITE setPaperColor NOTIFY paperColorChanged)
		Q_PROPERTY(QColor panelColor READ panelColor WRITE setPanelColor NOTIFY panelColorChanged)
		Q_PROPERTY(AccentColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)
		Q_PROPERTY(QColor currentAccentColor READ currentAccentColor NOTIFY currentAccentColorChanged)
		Q_PROPERTY(Theme theme READ theme WRITE setTheme NOTIFY themeChanged)
	public:
		ThemeEngine(QObject *parent = nullptr);

		// Note: Default accent color is defined in Settings::accentColorId()
		// The actual colors are defined in getAccentColor()
		enum AccentColor
		{
			Accent_Red = 0,
			Accent_Orange = 1,
			Accent_Yellow = 2,
			Accent_Green = 3,
			Accent_LightBlue = 4,
			Accent_Purple = 5,
			Accent_Pink = 6
		};
		Q_ENUM(AccentColor)

		enum class Theme
		{
			LightTheme = 1,
			DarkTheme = 0
		};
		Q_ENUM(Theme)

		// Font
		QFont font(void);
		void setFont(QFont newFont);
		QString fontFamily(void);
		void setFontFamily(QString family);
		int fontSize(void);
		void setFontSize(int size);
		Q_INVOKABLE void increaseFontSize(int delta);
		Q_INVOKABLE int maxFontSize(void);
		Q_DECL_DEPRECATED bool fontBold(void);
		Q_DECL_DEPRECATED void setFontBold(bool value);
		// Exercise text color
		Q_INVOKABLE bool customExerciseTextColor(void);
		QColor exerciseTextColor(void);
		void setExerciseTextColor(QColor color);
		Q_INVOKABLE void resetExerciseTextColor(void);
		Q_INVOKABLE QColor defaultExerciseTextColor(void);
		Q_INVOKABLE QColor defaultExerciseTextColor(bool dark);
		Q_INVOKABLE QColor defaultExerciseTextColor(QColor accent);
		Q_INVOKABLE QColor defaultExerciseTextColor(QColor accent, bool dark);
		// Input text color
		Q_INVOKABLE bool customInputTextColor(void);
		QColor inputTextColor(void);
		void setInputTextColor(QColor color);
		Q_INVOKABLE void resetInputTextColor(void);
		Q_INVOKABLE QColor defaultInputTextColor(bool dark = false);
		// Background color
		Q_INVOKABLE bool customBgColor(void);
		QColor bgColor(void);
		void setBgColor(QColor color);
		Q_INVOKABLE void resetBgColor(void);
		Q_INVOKABLE QColor defaultBgColor(bool dark = false);
		// Paper color
		Q_INVOKABLE bool customPaperColor(void);
		QColor paperColor(void);
		void setPaperColor(QColor color);
		Q_INVOKABLE void resetPaperColor(void);
		Q_INVOKABLE QColor defaultPaperColor(bool dark = false);
		// Panel color
		Q_INVOKABLE bool customPanelColor(void);
		QColor panelColor(void);
		void setPanelColor(QColor color);
		Q_INVOKABLE void resetPanelColor(void);
		Q_INVOKABLE QColor defaultPanelColor(bool dark = false);
		// Accent color
		AccentColor accentColor(void);
		void setAccentColor(AccentColor color);
		Q_INVOKABLE QColor getAccentColor(AccentColor colorId);
		QColor currentAccentColor(void);
		// Theme
		ThemeEngine::Theme theme(void);
		void setTheme(ThemeEngine::Theme newTheme);
		Q_INVOKABLE void updateTheme(void);
		Q_INVOKABLE void setDefaultTheme(void);

	private:
		void applyStyleSheetFromFile(const QString &stylePath);

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
		/*! A signal, which is emitted when accent color ID changes. */
		void accentColorChanged();
		/*! A signal, which is emitted when current accent color changes. */
		void currentAccentColorChanged();
		/*! A signal, which is emitted when the application theme changes. */
		void themeChanged();
};

extern ThemeEngine Q_DECL_EXPORT globalThemeEngine;

#endif // THEMEENGINE_H
