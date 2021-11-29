/*
 * inputlabel.cpp
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

#include "widgets/inputlabel.h"

inputLabelWidget::inputLabelWidget(QWidget *parent) :
	QTextEdit(parent)
{
	parentWidget = parent;
	setAttribute(Qt::WA_InputMethodEnabled,true);
}

inputLabelWidget::~inputLabelWidget() { }

bool inputLabelWidget::isDeadKey(int key)
{
	return ((key >= Qt::Key_Dead_Grave) && (key <= Qt::Key_Dead_Longsolidusoverlay));
}

void inputLabelWidget::inputMethodEvent(QInputMethodEvent *event)
{
	if (!event->commitString().isEmpty())
	{
		QKeyEvent keyEvent(QEvent::KeyPress,0,Qt::NoModifier,event->commitString());
		keyPressEvent(&keyEvent);
	}
	event->accept();
}

void inputLabelWidget::keyPressEvent(QKeyEvent *event)
{
	emit keyPressed(event);
}
