/*
 * IAddonButton.h
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

#ifndef IADDONBUTTON_H
#define IADDONBUTTON_H

#include <QObject>

class IAddonButton : public QObject
{
		Q_OBJECT
	public:
		virtual ~IAddonButton() { }

		virtual bool visible() const = 0;
		virtual void setVisible(bool newVisible) = 0;

		virtual QString text() = 0;
		virtual void setText(QString newText) = 0;

		virtual QString toolTip() = 0;
		virtual void setToolTip(QString newToolTip) = 0;

		virtual QString iconName() = 0;
		virtual void setIconName(QString newIconName) = 0;

		virtual QString iconSource() = 0;
		virtual void setIconSource(QString newIconSource) = 0;

	signals:
		void visibleChanged();
		void textChanged();
		void toolTipChanged();
		void iconNameChanged();
		void iconSourceChanged();
		void clicked();
};

#endif // IADDONBUTTON_H
