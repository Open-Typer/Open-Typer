/*
 * AddonButton.h
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

#ifndef ADDONBUTTON_H
#define ADDONBUTTON_H

#include <QObject>

class AddonApi;

/*! \brief The AddonButton class provides a button for addons. */
class AddonButton : public QObject
{
		Q_OBJECT
		Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
		Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
		Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip NOTIFY toolTipChanged)
		Q_PROPERTY(QString iconName READ iconName WRITE setIconName NOTIFY iconNameChanged)
		Q_PROPERTY(QString iconSource READ iconSource WRITE setIconSource NOTIFY iconSourceChanged)
		Q_PROPERTY(AddonButton::TopBarSection topBarSection READ topBarSection WRITE setTopBarSection NOTIFY topBarSectionChanged)

	public:
		enum TopBarSection
		{
			TopBarSection_Home = 0,
			TopBarSection_Navigation = 1,
			TopBarSection_ExOptions = 2,
			TopBarSection_ExOptionsBelow = 3,
			TopBarSecion_LastValue = 4 // do not use this
		};
		Q_ENUM(TopBarSection)

		explicit AddonButton(QObject *parent = nullptr);

		bool visible() const;
		void setVisible(bool newVisible);

		QString text();
		void setText(QString newText);

		QString toolTip();
		void setToolTip(QString newToolTip);

		QString iconName();
		void setIconName(QString newIconName);

		QString iconSource();
		void setIconSource(QString newIconSource);

		TopBarSection topBarSection();
		void setTopBarSection(TopBarSection newTopBarSection);

	private:
		bool m_visible;
		QString m_text;
		QString m_toolTip;
		QString m_iconName;
		QString m_iconSource;
		TopBarSection m_topBarSection;

	signals:
		void visibleChanged(bool visible);
		void textChanged(QString text);
		void toolTipChanged(QString text);
		void iconNameChanged(QString name);
		void iconSourceChanged(QString source);
		void topBarSectionChanged(TopBarSection section);
};

#endif // ADDONBUTTON_H
