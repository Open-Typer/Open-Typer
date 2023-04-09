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
#include <QFont>
#include <QFontDatabase>
#include <QColor>
#include <QFile>
#include "global/ISettings.h"
#include "../IThemeEngine.h"

/*! \brief The ThemeEngine class provides theme functions. */
class Q_DECL_EXPORT ThemeEngine : public IThemeEngine
{
		Q_OBJECT
		INJECT(ISettings, settings)
		Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
		Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
		Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
		Q_PROPERTY(QColor exerciseTextColor READ exerciseTextColor WRITE setExerciseTextColor NOTIFY exerciseTextColorChanged)
		Q_PROPERTY(QColor inputTextColor READ inputTextColor WRITE setInputTextColor NOTIFY inputTextColorChanged)
		Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor NOTIFY bgColorChanged)
		Q_PROPERTY(QColor paperColor READ paperColor WRITE setPaperColor NOTIFY paperColorChanged)
		Q_PROPERTY(QColor panelColor READ panelColor WRITE setPanelColor NOTIFY panelColorChanged)
		Q_PROPERTY(AccentColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)
		Q_PROPERTY(QColor currentAccentColor READ currentAccentColor NOTIFY currentAccentColorChanged)
		Q_PROPERTY(Theme theme READ theme WRITE setTheme NOTIFY themeChanged)
	public:
		ThemeEngine();
		static std::shared_ptr<ThemeEngine> instance();

		// Font
		QFont font(void) override;
		void setFont(QFont newFont) override;
		QString fontFamily(void) override;
		void setFontFamily(QString family) override;
		int fontSize(void) override;
		void setFontSize(int size) override;
		Q_INVOKABLE void increaseFontSize(int delta) override;
		Q_INVOKABLE int maxFontSize(void) override;
		// Exercise text color
		Q_INVOKABLE bool customExerciseTextColor(void) override;
		QColor exerciseTextColor(void) override;
		void setExerciseTextColor(QColor color) override;
		Q_INVOKABLE void resetExerciseTextColor(void) override;
		Q_INVOKABLE QColor defaultExerciseTextColor(void) override;
		Q_INVOKABLE QColor defaultExerciseTextColor(bool dark) override;
		Q_INVOKABLE QColor defaultExerciseTextColor(QColor accent) override;
		Q_INVOKABLE QColor defaultExerciseTextColor(QColor accent, bool dark) override;
		// Input text color
		Q_INVOKABLE bool customInputTextColor(void) override;
		QColor inputTextColor(void) override;
		void setInputTextColor(QColor color) override;
		Q_INVOKABLE void resetInputTextColor(void) override;
		Q_INVOKABLE QColor defaultInputTextColor(bool dark = false) override;
		// Background color
		Q_INVOKABLE bool customBgColor(void) override;
		QColor bgColor(void) override;
		void setBgColor(QColor color) override;
		Q_INVOKABLE void resetBgColor(void) override;
		Q_INVOKABLE QColor defaultBgColor(bool dark = false) override;
		// Paper color
		Q_INVOKABLE bool customPaperColor(void) override;
		QColor paperColor(void) override;
		void setPaperColor(QColor color) override;
		Q_INVOKABLE void resetPaperColor(void) override;
		Q_INVOKABLE QColor defaultPaperColor(bool dark = false) override;
		// Panel color
		Q_INVOKABLE bool customPanelColor(void) override;
		QColor panelColor(void) override;
		void setPanelColor(QColor color) override;
		Q_INVOKABLE void resetPanelColor(void) override;
		Q_INVOKABLE QColor defaultPanelColor(bool dark = false) override;
		// Accent color
		AccentColor accentColor(void) override;
		void setAccentColor(AccentColor color) override;
		Q_INVOKABLE QColor getAccentColor(AccentColor colorId) override;
		QColor currentAccentColor(void) override;
		// Theme
		ThemeEngine::Theme theme(void) override;
		void setTheme(ThemeEngine::Theme newTheme) override;
		Q_INVOKABLE void updateTheme(void) override;
		Q_INVOKABLE void setDefaultTheme(void) override;

	private:
		static std::shared_ptr<ThemeEngine> m_instance;
		void applyStyleSheetFromFile(const QString &stylePath);
};

#endif // THEMEENGINE_H
