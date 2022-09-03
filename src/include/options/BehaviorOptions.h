/*
 * BehaviorOptions.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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

#ifndef BEHAVIOROPTIONS_H
#define BEHAVIOROPTIONS_H

#include <QWidget>
#include <QSettings>
#include "core/StringUtils.h"

namespace Ui {
	class BehaviorOptions;
}

/*!
 * \brief The BehaviorOptions class is the behavior category in the options.
 * \image html BehaviorOptions.png
 * \see OptionsWindow
 */
class BehaviorOptions : public QWidget
{
		Q_OBJECT
	public:
		explicit BehaviorOptions(QWidget *parent = nullptr);
		~BehaviorOptions();

	private:
		Ui::BehaviorOptions *ui;
		QSettings settings;

	private slots:
		void setSpaceNewline(bool value);
		void setErrorPenalty(int value);
		void toggleMistakeLimit(bool checked);
		void setMistakeChars(int value);
};

#endif // BEHAVIOROPTIONS_H
