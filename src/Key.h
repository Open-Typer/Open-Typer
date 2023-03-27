/*
 * Key.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
 * Copyright (C) 2023 - Roker2
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
#include "KeyboardUtils.h"

/*! \brief The Key class is used for the keys on the on screen keyboard. */
class Q_DECL_EXPORT Key
{
		Q_GADGET
		Q_PROPERTY(QString text READ text WRITE setText)
		Q_PROPERTY(QString shiftText READ shiftText WRITE setShiftText)
		Q_PROPERTY(KeyboardUtils::KeyType type READ type WRITE setType)
		Q_PROPERTY(QString displayText READ displayText WRITE setDisplayText)
		Q_PROPERTY(QString displayShiftText READ displayShiftText WRITE setDisplayShiftText)
		Q_PROPERTY(bool dead READ dead WRITE setDead)
		Q_PROPERTY(bool shiftDead READ shiftDead WRITE setShiftDead)
	public:
		explicit Key();
		Key(QString text, QString shiftText);
		QString text(void);
		void setText(QString text);
		QString shiftText(void);
		void setShiftText(QString text);
		KeyboardUtils::KeyType type(void);
		void setType(KeyboardUtils::KeyType type);
		QString displayText(void);
		void setDisplayText(QString text);
		QString displayShiftText(void);
		void setDisplayShiftText(QString text);
		bool dead(void);
		bool isDead(void);
		void setDead(bool dead);
		bool shiftDead(void);
		bool isShiftDead(void);
		void setShiftDead(bool dead);

	private:
		QString m_text;
		QString m_shiftText;
		KeyboardUtils::KeyType m_type = KeyboardUtils::KeyType_Any;
		QString m_displayText;
		QString m_displayShiftText;
		bool m_dead;
		bool m_shiftDead;
};

using KeyboardRow = QList<Key>;

Q_DECLARE_METATYPE(Key)
Q_DECLARE_METATYPE(KeyboardRow)

#endif // KEY_H
