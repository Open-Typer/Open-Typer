/*
 * KeyboardUtils.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2023 - adazem009
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

#ifndef KEYBOARDUTILS_H
#define KEYBOARDUTILS_H

#include <QObject>
#include <QKeyEvent>
#include "QmlKeyboardHandler.h"
#include "KeyEvent.h"

/*! \brief The KeyboardUtils class provides functions related to keyboard and input method. */
class Q_DECL_EXPORT KeyboardUtils : public QObject
{
		Q_OBJECT
	public:
		enum KeyType
		{
			KeyType_Any = 0,
			KeyType_Tab = 1,
			KeyType_CapsLock = 2,
			KeyType_Return = 3,
			KeyType_LShift = 4,
			KeyType_RShift = 5,
			KeyType_Ctrl = 6,
			KeyType_LAlt = 7,
			KeyType_RAlt = 8,
			KeyType_Space = 9,
			KeyType_Backspace = 10
		};
		Q_ENUM(KeyType)

		static bool isSpecialKey(QKeyEvent *event);
		Q_INVOKABLE static bool isSpecialKey(QVariantMap event);
		Q_INVOKABLE static bool isDeadKey(int key);
		Q_INVOKABLE static QString deadKeyToString(Qt::Key key);
		Q_INVOKABLE static QString deadKeyToReadableString(Qt::Key key);
		Q_INVOKABLE static bool isRShift(int nativeScanCode, int nativeVirtualKey);
		Q_INVOKABLE static bool isRControl(int nativeScanCode, int nativeVirtualKey);

		Q_INVOKABLE static KeyEvent createKeyEvent(Qt::Key key = Qt::Key_unknown, Qt::KeyboardModifiers modifiers = Qt::NoModifier, quint32 nativeScanCode = 0, quint32 nativeVirtualKey = 0, const QString &text = QString());
};

#endif // KEYBOARDUTILS_H
