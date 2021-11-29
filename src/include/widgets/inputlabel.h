/*
 * inputlabel.h
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

#ifndef INPUTLABEL_H
#define INPUTLABEL_H

#include <QWidget>
#include <QTextEdit>
#include <QMainWindow>
#include <QInputMethodEvent>

class inputLabelWidget : public QTextEdit
{
	Q_OBJECT
	public:
		explicit inputLabelWidget(QWidget *parent = nullptr);
		~inputLabelWidget();
		QWidget *parentWidget;
		static bool isDeadKey(int key);

	protected:
		void inputMethodEvent(QInputMethodEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);

	signals:
		void keyPressed(QKeyEvent *event);
};

#endif // INPUTLABEL_H
