/*
 * KeyboardUtils.cpp
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

#include "KeyboardUtils.h"

/*!
 * Returns true if the key is a special key.\n
 * Special keys are ignored by OpenTyper#keyPress().
 * \see OpenTyper#keyPress()
 */
bool KeyboardUtils::isSpecialKey(QKeyEvent *event)
{
	if((event->text() == "") && (event->key() != Qt::Key_Return) && (event->key() != Qt::Key_Enter))
		return true;
	switch(event->key())
	{
		case Qt::Key_Delete:
			return true;
			break;
		case Qt::Key_Backspace:
			return true;
			break;
		case Qt::Key_Backtab:
			return true;
			break;
		case Qt::Key_Escape:
			return true;
			break;
		default:
			return false;
			break;
	}
}

#if QT_VERSION <= QT_VERSION_CHECK(5, 10, 0)
#define LAST_DEAD_KEY Qt::Key_Dead_Horn
#else
#define LAST_DEAD_KEY Qt::Key_Dead_Longsolidusoverlay
#endif

/*! Returns true if the key code belongs to a dead key. */
bool KeyboardUtils::isDeadKey(int key)
{
	return ((key >= Qt::Key_Dead_Grave) && (key <= LAST_DEAD_KEY));
}
