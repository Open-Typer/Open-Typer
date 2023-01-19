/*
 * KeyboardLayout.h
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

#ifndef KEYBOARDLAYOUT_H
#define KEYBOARDLAYOUT_H

#include <QObject>
#include <QVariant>
#include <QPoint>
#include "Key.h"

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

/*! \brief The KeyboardLayout class provides information about a keyboard layout. */
class CORE_LIB_EXPORT KeyboardLayout : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString layoutId READ layoutId WRITE setLayoutId NOTIFY layoutIdChanged)
		Q_PROPERTY(QString variant READ variant WRITE setVariant NOTIFY variantChanged)
		Q_PROPERTY(KeyboardRow rowA READ rowA NOTIFY rowAChanged)
		Q_PROPERTY(KeyboardRow rowB READ rowB NOTIFY rowBChanged)
		Q_PROPERTY(KeyboardRow rowC READ rowC NOTIFY rowCChanged)
		Q_PROPERTY(KeyboardRow rowD READ rowD NOTIFY rowDChanged)
		Q_PROPERTY(KeyboardRow rowE READ rowE NOTIFY rowEChanged)
	public:
		explicit KeyboardLayout(QObject *parent = nullptr);
		KeyboardLayout(QString id, QObject *parent = nullptr);
		QString layoutId(void);
		void setLayoutId(QString id);
		QString variant(void);
		void setVariant(QString variant);
		KeyboardRow rowA(void);
		KeyboardRow rowB(void);
		KeyboardRow rowC(void);
		KeyboardRow rowD(void);
		KeyboardRow rowE(void);

	private:
		void init(void);
		QVariantList parse(QString data);
		QString nestedData(int *pos, QString data, QString startToken, QString endToken);
		QString keyText(QString id);
		QPoint keyPos(QString keyId);
		void addKey(Key key, int x, int y);
		QString m_layoutId;
		QString m_variant;
		KeyboardRow m_rowA;
		KeyboardRow m_rowB;
		KeyboardRow m_rowC;
		KeyboardRow m_rowD;
		KeyboardRow m_rowE;

	signals:
		void layoutIdChanged(QString id);
		void variantChanged(QString variant);
		void rowAChanged(KeyboardRow row);
		void rowBChanged(KeyboardRow row);
		void rowCChanged(KeyboardRow row);
		void rowDChanged(KeyboardRow row);
		void rowEChanged(KeyboardRow row);
};

#endif // KEYBOARDLAYOUT_H
