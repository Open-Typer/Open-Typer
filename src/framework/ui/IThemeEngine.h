/*
 * IThemeEngine.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

#ifndef ITHEMEENGINE_H
#define ITHEMEENGINE_H

#include <QObject>
#include "global/modularity/ioc.h"

class IThemeEngine : public QObject, MODULE_EXPORT_INTERFACE
{
		Q_OBJECT
	public:
		virtual ~IThemeEngine() { }

		// Note: Default accent color is defined in the default value of the accentColorId settings key
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

		virtual QFont font(void) = 0;
		virtual void setFont(QFont newFont) = 0;
		virtual QString fontFamily(void) = 0;
		virtual void setFontFamily(QString family) = 0;
		virtual int fontSize(void) = 0;
		virtual void setFontSize(int size) = 0;
		virtual void increaseFontSize(int delta) = 0;
		virtual int maxFontSize(void) = 0;

		virtual bool customExerciseTextColor(void) = 0;
		virtual QColor exerciseTextColor(void) = 0;
		virtual void setExerciseTextColor(QColor color) = 0;
		virtual void resetExerciseTextColor(void) = 0;
		virtual QColor defaultExerciseTextColor(void) = 0;
		virtual QColor defaultExerciseTextColor(bool dark) = 0;
		virtual QColor defaultExerciseTextColor(QColor accent) = 0;
		virtual QColor defaultExerciseTextColor(QColor accent, bool dark) = 0;

		virtual bool customInputTextColor(void) = 0;
		virtual QColor inputTextColor(void) = 0;
		virtual void setInputTextColor(QColor color) = 0;
		virtual void resetInputTextColor(void) = 0;
		virtual QColor defaultInputTextColor(bool dark = false) = 0;

		virtual bool customBgColor(void) = 0;
		virtual QColor bgColor(void) = 0;
		virtual void setBgColor(QColor color) = 0;
		virtual void resetBgColor(void) = 0;
		virtual QColor defaultBgColor(bool dark = false) = 0;

		virtual bool customPaperColor(void) = 0;
		virtual QColor paperColor(void) = 0;
		virtual void setPaperColor(QColor color) = 0;
		virtual void resetPaperColor(void) = 0;
		virtual QColor defaultPaperColor(bool dark = false) = 0;

		virtual bool customPanelColor(void) = 0;
		virtual QColor panelColor(void) = 0;
		virtual void setPanelColor(QColor color) = 0;
		virtual void resetPanelColor(void) = 0;
		virtual QColor defaultPanelColor(bool dark = false) = 0;

		virtual AccentColor accentColor(void) = 0;
		virtual void setAccentColor(AccentColor color) = 0;
		virtual QColor getAccentColor(AccentColor colorId) = 0;
		virtual QColor currentAccentColor(void) = 0;

		virtual Theme theme(void) const = 0;
		virtual void setTheme(Theme newTheme) = 0;
		virtual void updateTheme(void) = 0;
		virtual void setDefaultTheme(void) = 0;

		virtual QColor borderColor() const = 0;

		virtual QColor foregroundColor() const = 0;

	signals:
		/*! A signal, which is emitted when the font changes. */
		void fontChanged();
		/*! A signal, which is emitted when the font family changes. */
		void fontFamilyChanged();
		/*! A signal, which is emitted when the font point size changes. */
		void fontSizeChanged();
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
		void borderColorChanged();
		void foregroundColorChanged();
};

#endif // ITHEMEENGINE_H
