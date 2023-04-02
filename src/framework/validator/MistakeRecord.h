/*
 * MistakeRecord.h
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

#ifndef MISTAKERECORD_H
#define MISTAKERECORD_H

#include <QObject>
#include <QVariant>

/*! \brief The MistakeRecord class can be used to store mistake records. */
class Q_DECL_EXPORT MistakeRecord
{
		Q_GADGET
		Q_PROPERTY(int position READ position WRITE setPosition)
		Q_PROPERTY(Type type READ type WRITE setType)
		Q_PROPERTY(QString previousText READ previousText WRITE setPreviousText)
		Q_PROPERTY(QVariant previousVariant READ previousVariant WRITE setPreviousVariant)
		Q_PROPERTY(int previousPosition READ previousPosition WRITE setPreviousPosition)
		Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
		Q_PROPERTY(bool merged READ merged WRITE setMerged)
	public:
		enum Type
		{
			Type_Addition = 0,
			Type_Deletion = 1,
			Type_Change = 2
		};
		Q_ENUM(Type)

		int position(void);
		void setPosition(int pos);
		Type type(void);
		void setType(Type type);
		QString previousText(void);
		void setPreviousText(QString text);
		QVariant previousVariant(void);
		void setPreviousVariant(QVariant value);
		int previousPosition(void);
		void setPreviousPosition(int pos);
		bool enabled(void);
		bool isEnabled(void);
		void setEnabled(bool enabled);
		bool merged(void);
		bool isMerged(void);
		void setMerged(bool merged);

	private:
		int m_position;
		Type m_type;
		QString m_previousText;
		QVariant m_previousVariant;
		int m_previousPosition;
		bool m_enabled = true;
		bool m_merged = false;

		friend inline bool operator==(const MistakeRecord &r1, const MistakeRecord &r2)
		{
			bool posCheck = r1.m_position == r2.m_position;
			bool typeCheck = r1.m_type == r2.m_type;
			bool previousTextCheck = r1.m_previousText == r2.m_previousText;
			bool previousVariantCheck = r1.m_previousVariant == r2.m_previousVariant;
			bool previousPositionCheck = r1.m_previousPosition == r2.m_previousPosition;
			bool enabledCheck = r1.m_enabled == r2.m_enabled;
			bool mergedCheck = r1.m_merged == r2.m_merged;
			return (posCheck && typeCheck && previousTextCheck && previousVariantCheck && previousPositionCheck && enabledCheck && mergedCheck);
		}
};

#endif // MISTAKERECORD_H
