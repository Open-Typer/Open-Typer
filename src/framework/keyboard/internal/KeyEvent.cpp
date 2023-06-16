/*
 * KeyEvent.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

#include <QKeyEvent>
#include "KeyEvent.h"

KeyEvent::KeyEvent(Qt::Key key, Qt::KeyboardModifiers modifiers, quint32 nativeScanCode, quint32 nativeVirtualKey, const QString &text) :
	m_key(key),
	m_text(text),
	m_modifiers(modifiers),
	m_nativeScanCode(nativeScanCode),
	m_nativeVirtualKey(nativeVirtualKey)
{
}

KeyEvent::KeyEvent(QKeyEvent *keyEvent) :
	m_key(static_cast<Qt::Key>(keyEvent->key())),
	m_text(keyEvent->text()),
	m_modifiers(keyEvent->modifiers()),
	m_autoRepeat(keyEvent->isAutoRepeat()),
	m_nativeScanCode(keyEvent->nativeScanCode()),
	m_nativeVirtualKey(keyEvent->nativeVirtualKey())
{
}

Qt::Key KeyEvent::key() const
{
	return m_key;
}

const QString &KeyEvent::text() const
{
	return m_text;
}

Qt::KeyboardModifiers KeyEvent::modifiers() const
{
	return m_modifiers;
}

bool KeyEvent::isAutoRepeat() const
{
	return m_autoRepeat;
}

quint32 KeyEvent::nativeScanCode() const
{
	return m_nativeScanCode;
}

quint32 KeyEvent::nativeVirtualKey() const
{
	return m_nativeVirtualKey;
}

bool KeyEvent::rightShift() const
{
	return m_rightShift;
}

void KeyEvent::setRightShift(bool newRightShift)
{
	if(m_rightShift == newRightShift)
		return;
	m_rightShift = newRightShift;
}
