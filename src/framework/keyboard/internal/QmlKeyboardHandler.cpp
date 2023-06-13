/*
 * QmlKeyboardHandler.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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

#include "QmlKeyboardHandler.h"

QmlKeyboardHandler::QmlKeyboardHandler(QQuickItem *parent) :
	QQuickItem(parent)
{
	setFlag(ItemAcceptsInputMethod, true);
}

/*!
 * Overrides QQuickItem#inputMethodEvent.
 * Handles characters composed using dead keys.
 * \see keyReleaseEvent()
 */
void QmlKeyboardHandler::inputMethodEvent(QInputMethodEvent *event)
{
	if(!event->commitString().isEmpty())
	{
		QKeyEvent keyEvent(QEvent::KeyPress, 0, Qt::NoModifier, event->commitString());
		keyPressEvent(&keyEvent);
	}
	event->accept();
}

/*!
 * Overrides QQuickItem#keyPressEvent.
 * Handles all key presses, including dead keys (using inputMethodEvent() and keyReleaseEvent()).
 * \see inputMethodEvent()
 * \see keyReleaseEvent()
 */
void QmlKeyboardHandler::keyPressEvent(QKeyEvent *event)
{
	emit keyPressed(event);
}

/*!
 * Overrides QQuickItem#keyReleaseEvent.
 * Handles characters generated from 2 same dead keys.\n
 * \see inputMethodEvent()
 */
void QmlKeyboardHandler::keyReleaseEvent(QKeyEvent *event)
{
	if(KeyboardUtils::isDeadKey(event->key()))
		emit keyPressed(event);
	emit keyReleased(event);
}
