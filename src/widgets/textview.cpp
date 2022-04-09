/*
 * textview.cpp
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

#include "widgets/textview.h"

/*! Constructs textView. */
textView::textView(QWidget *parent) :
	QTextEdit(parent)
{
	connect(this, &textView::textChanged, this, &textView::updateWidgetSize);
}

/*! Destroys the textView object. */
textView::~textView() { }

/*! Sets horizontal adjustment (according to the text document) of the widget. */
void textView::setHorizontalAdjust(bool value)
{
	horizontalAdjust = value;
}

/*! Sets vertical adjustment (according to the text document) of the widget. */
void textView::setVerticalAdjust(bool value)
{
	verticalAdjust = value;
}

/*! Sets widget fixed size according to the text document. */
void textView::updateWidgetSize(void)
{
	QTextDocument *targetDocument;
	if(toPlainText() == "")
	{
		targetDocument = document()->clone();
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
void textView::changeEvent(QEvent *event)
{
	QTextEdit::changeEvent(event);
	if(event->type() == QEvent::FontChange)
		updateWidgetSize();
}

/*! Overrides QTextEdit#wheelEvent(). */
void textView::wheelEvent(QWheelEvent *event)
{
	event->accept();
}
