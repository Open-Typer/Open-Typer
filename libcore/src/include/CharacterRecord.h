/*
 * CharacterRecord.h
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

#ifndef CHARACTERRECORD_H
#define CHARACTERRECORD_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>

/*!
 * \brief The CharacterRecord class can be used to store character records.
 * A character record consists of key text (usually a single character) and number of key hits used to compose the character.
 */
class CORE_LIB_EXPORT CharacterRecord : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString keyText READ keyText WRITE setKeyText NOTIFY keyTextChanged)
		Q_PROPERTY(int hits READ hits WRITE setHits NOTIFY hitsChanged)
	public:
		void setKeyText(QString text);
		QString keyText(void);
		void setHits(int count);
		int hits(void);

	private:
		QString m_keyText;
		int m_hits;

	signals:
		void keyTextChanged(QString text);
		void hitsChanged(int count);
};

#endif // CHARACTERRECORD_H
