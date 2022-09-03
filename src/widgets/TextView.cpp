/*
 * TextView.cpp
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

#include "widgets/TextView.h"

/*! Constructs TextView. */
TextView::TextView(QWidget *parent) :
	QTextEdit(parent)
{
	connect(this, &TextView::textChanged, this, &TextView::updateWidgetSize);
}

/*! Destroys the TextView object. */
TextView::~TextView() { }

/*! Sets horizontal adjustment (according to the text document) of the widget. */
void TextView::setHorizontalAdjust(bool value)
{
	horizontalAdjust = value;
}

/*! Sets vertical adjustment (according to the text document) of the widget. */
void TextView::setVerticalAdjust(bool value)
{
	verticalAdjust = value;
}

/*! Sets widget fixed size according to the text document. */
void TextView::updateWidgetSize(void)
{
	QTextDocument *targetDocument;
	if(toPlainText() == "")
	{
		targetDocument = document()->clone(this);
		targetDocument->setPlainText("A");
	}
	else
		targetDocument = document();
	if(horizontalAdjust)
		setFixedWidth(targetDocument->size().width());
	else
	{
		setMinimumWidth(0);
		setMaximumWidth(QWIDGETSIZE_MAX);
	}
	if(verticalAdjust)
		setFixedHeight(targetDocument->size().height());
	else
	{
		setMinimumHeight(0);
		setMaximumHeight(QWIDGETSIZE_MAX);
	}
	if(toPlainText() == "")
		targetDocument->deleteLater();
}

/*! Overrides QTextEdit#changeEvent(). */
void TextView::changeEvent(QEvent *event)
{
	QTextEdit::changeEvent(event);
	if(event->type() == QEvent::FontChange)
		updateWidgetSize();
}

/*! Overrides QTextEdit#wheelEvent(). */
void TextView::wheelEvent(QWheelEvent *event)
{
	event->accept();
}

/*! Overrides QTextEdit#mouseMoveEvent(). */
void TextView::mouseMoveEvent(QMouseEvent *event)
{
	event->accept();
}
