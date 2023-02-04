/*
 * AddonSettingsCategory.h
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

#ifndef ADDONSETTINGSCATEGORY_H
#define ADDONSETTINGSCATEGORY_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>

/*! \brief The AddonSettingsCategory provides a model for settings categories added by addons. */
class CORE_LIB_EXPORT AddonSettingsCategory : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QString iconName READ iconName WRITE setIconName NOTIFY iconNameChanged)
		Q_PROPERTY(QString iconSource READ iconSource WRITE setIconSource NOTIFY iconSourceChanged)
		Q_PROPERTY(QString qmlFileName READ qmlFileName WRITE setQmlFileName NOTIFY qmlFileNameChanged)

	public:
		explicit AddonSettingsCategory(QObject *parent = nullptr);

		QString name(void);
		void setName(QString newName);

		QString iconName(void);
		void setIconName(QString newIconName);

		QString iconSource(void);
		void setIconSource(QString newIconSource);

		QString qmlFileName(void);
		void setQmlFileName(QString newQmlFileName);

	private:
		QString m_name;
		QString m_iconName;
		QString m_iconSource;
		QString m_qmlFileName;

	signals:
		void nameChanged(QString name);
		void iconNameChanged(QString iconName);
		void iconSourceChanged(QString iconSource);
		void qmlFileNameChanged(QString qmlFileName);
};

#endif // ADDONSETTINGSCATEGORY_H
