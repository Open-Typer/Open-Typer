/*
 * SettingsCategory.h
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

#ifndef SETTINGSCATEGORY_H
#define SETTINGSCATEGORY_H

#include <QObject>

/*! \brief The SettingsCategory class provides a model for settings categories. */
class Q_DECL_EXPORT SettingsCategory : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QString iconName READ iconName WRITE setIconName NOTIFY iconNameChanged)
		Q_PROPERTY(QString iconSource READ iconSource WRITE setIconSource NOTIFY iconSourceChanged)
		Q_PROPERTY(QString qmlFileName READ qmlFileName WRITE setQmlFileName NOTIFY qmlFileNameChanged)
		Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)

	public:
		explicit SettingsCategory(QObject *parent = nullptr);

		QString name(void);
		void setName(QString newName);

		QString iconName(void);
		void setIconName(QString newIconName);

		QString iconSource(void);
		void setIconSource(QString newIconSource);

		QString qmlFileName(void);
		void setQmlFileName(QString newQmlFileName);

		bool visible(void);
		void setVisible(bool newVisible);

	private:
		QString m_name;
		QString m_iconName;
		QString m_iconSource;
		QString m_qmlFileName;
		bool m_visible = true;

	signals:
		void nameChanged(QString name);
		void iconNameChanged(QString iconName);
		void iconSourceChanged(QString iconSource);
		void qmlFileNameChanged(QString qmlFileName);
		void visibleChanged(bool visible);
};

#endif // SETTINGSCATEGORY_H
