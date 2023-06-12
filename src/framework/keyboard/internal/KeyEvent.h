/*
 * KeyEvent.h
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

#ifndef KEYEVENT_H
#define KEYEVENT_H

#include <QObject>

class QKeyEvent;

class KeyEvent
{
		Q_GADGET
		Q_PROPERTY(Qt::Key key READ key)
		Q_PROPERTY(QString text READ text)
		Q_PROPERTY(Qt::KeyboardModifiers modifiers READ modifiers)
		Q_PROPERTY(bool autoRepeat READ isAutoRepeat)
		Q_PROPERTY(quint32 nativeScanCode READ nativeScanCode)
		Q_PROPERTY(quint32 nativeVirtualKey READ nativeVirtualKey)
		Q_PROPERTY(bool rightShift READ rightShift WRITE setRightShift)
	public:
		KeyEvent(Qt::Key key = Qt::Key_unknown, Qt::KeyboardModifiers modifiers = Qt::NoModifier, quint32 nativeScanCode = 0, quint32 nativeVirtualKey = 0, const QString &text = QString());
		KeyEvent(QKeyEvent *keyEvent);

		Qt::Key key() const;
		const QString &text() const;
		Qt::KeyboardModifiers modifiers() const;
		bool isAutoRepeat() const;
		quint32 nativeScanCode() const;
		quint32 nativeVirtualKey() const;

		bool rightShift() const;
		void setRightShift(bool newRightShift);

	private:
		Qt::Key m_key = Qt::Key_unknown;
		QString m_text;
		Qt::KeyboardModifiers m_modifiers = Qt::NoModifier;
		bool m_autoRepeat = false;
		quint32 m_nativeScanCode = 0;
		quint32 m_nativeVirtualKey = 0;
		bool m_rightShift = false;
};

#endif // KEYEVENT_H
