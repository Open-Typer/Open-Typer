/*
 * InputLabelWidget.h
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

#ifndef INPUTLABEL_H
#define INPUTLABEL_H

#include <QWidget>
#include <QMainWindow>
#include <QInputMethodEvent>
#include "widgets/TextView.h"
#include "core/KeyboardUtils.h"

/*!
 * \brief The InputLabelWidget class is a TextView, which handles all key presses.
 *
 * The main window (OpenTyper) uses it to display the input text and receive key presses.
 * \image html InputLabelWidget.png
 *
 * Usage example:
 * \code
 * class myClass : public QWidget
 * {
 * 	Q_OBJECT
 * 	private:
 * 		explicit myClass(QWidget *parent = nullptr);
 * 		InputLabelWidget *inputText;
 *
 * 	private slots:
 * 		void keyPressed(QKeyEvent* event);
 * };
 *
 * void myClass::myClass(QWidget *parent)
 * {
 * 	inputText = new InputLabelWidget;
 * 	connect(InputLabelWidget,SIGNAL(keyPressed(QKeyEvent*)),this,SLOT(keyPressed(QKeyEvent*)));
 * }
 *
 * void myClass::keyPressed(QKeyEvent* event)
 * {
 * 	inputText->setText(inputText->text() + event->text());
 * }
 * \endcode
 */
class InputLabelWidget : public TextView
{
		Q_OBJECT
	public:
		explicit InputLabelWidget(QWidget *parent = nullptr);
		~InputLabelWidget();
		QWidget *parentWidget;

	protected:
		void inputMethodEvent(QInputMethodEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);

	signals:
		void keyPressed(QKeyEvent *event);
		void keyReleased(QKeyEvent *event);
};

#endif // INPUTLABEL_H
