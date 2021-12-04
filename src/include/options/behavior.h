/*
 * behavior.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <QWidget>
#include <QSettings>
#include "core/utils.h"

namespace Ui {
	class behaviorOptions;
}

class behaviorOptions : public QWidget
{
	Q_OBJECT
	public:
		explicit behaviorOptions(QWidget *parent = nullptr);
		~behaviorOptions();

	private:
		Ui::behaviorOptions *ui;
		QSettings *settings;

	private slots:
		void setSpaceNewline(bool value);
		void setErrorPenalty(int value);
		void setTypingMode(int mode);
};

#endif // BEHAVIOR_H
