/*
 * Key.h
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

#ifndef KEY_H
#define KEY_H

#include <QObject>

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

class CORE_LIB_EXPORT Key
{
		Q_GADGET
		Q_PROPERTY(QString text READ text WRITE setText)
		Q_PROPERTY(QString shiftText READ shiftText WRITE setShiftText)
		Q_PROPERTY(QString type READ type)
	public:
		enum Type
		{
			Type_Any = 0,
			Type_Tab = 1,
			Type_CapsLock = 2,
			Type_Return = 3,
			Type_LShift = 4,
			Type_RShift = 5,
			Type_Ctrl = 6,
			Type_Alt = 7,
			Type_Space = 8
		};
		Q_ENUM(Type)

		explicit Key();
		Key(QString text, QString shiftText);
		QString text(void);
		void setText(QString text);
		QString shiftText(void);
		void setShiftText(QString text);
		QString type(void);
		void setTypeFromEnum(Type type);

	private:
		QString m_text;
		QString m_shiftText;
		Type m_type;
};

typedef QList<Key> KeyboardRow;

Q_DECLARE_METATYPE(Key)
Q_DECLARE_METATYPE(KeyboardRow)

#endif // KEY_H
