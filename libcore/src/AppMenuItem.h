/*
 * AppMenuItem.h
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

#ifndef APPMENUITEM_H
#define APPMENUITEM_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>

class CORE_LIB_EXPORT AppMenuModel;

/*! \brief The AppMenuItem class provides a model for a menu item. */
class CORE_LIB_EXPORT AppMenuItem : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
		Q_PROPERTY(AppMenuModel *submenu READ submenu WRITE setSubmenu NOTIFY submenuChanged)
		Q_PROPERTY(bool isSeparator READ isSeparator WRITE setIsSeparator NOTIFY isSeparatorChanged)
	public:
		explicit AppMenuItem(QObject *parent = nullptr);

		QString text(void);
		void setText(QString newText);

		AppMenuModel *submenu(void);
		void setSubmenu(AppMenuModel *newSubmenu);

		bool isSeparator(void);
		void setIsSeparator(bool newIsSeparator);

	private:
		QString m_text;
		AppMenuModel *m_submenu = nullptr;
		bool m_isSeparator = false;

	signals:
		void textChanged(QString text);
		void submenuChanged(AppMenuModel *menu);
		void isSeparatorChanged(bool value);
		void clicked();
};

#endif // APPMENUITEM_H
