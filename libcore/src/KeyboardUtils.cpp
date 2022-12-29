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

#include <QMetaEnum>
#include "KeyboardUtils.h"

/*!
 * Returns true if the key is a special key.\n
 * Special keys are ignored by default.
 */
bool KeyboardUtils::isSpecialKey(QKeyEvent *event)
{
	return isSpecialKey(QmlKeyboardHandler::convertEvent(event));
}

/*! Implementation of isSpecialKey() for QVariantMap events. */
bool KeyboardUtils::isSpecialKey(QVariantMap event)
{
	int keyID = event["key"].toInt();
	if((event["text"].toString() == "") && (keyID != Qt::Key_Return) && (keyID != Qt::Key_Enter))
		return true;
	switch(keyID)
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

/*! Returns true if the key code belongs to a dead key. */
bool KeyboardUtils::isDeadKey(int key)
{
	QMetaEnum metaEnum = QMetaEnum::fromType<Qt::Key>();
	return QString(metaEnum.valueToKey(key)).contains("Dead");
}
