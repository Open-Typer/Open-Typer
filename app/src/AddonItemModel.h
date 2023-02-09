/*
 * AddonItemModel.h
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

#ifndef ADDONITEMMODEL_H
#define ADDONITEMMODEL_H

#include <QObject>

/*! \brief The AddonItemModel class provides a model for a single item in a list of addons. */
class AddonItemModel : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
		Q_PROPERTY(QString shortDescription READ shortDescription NOTIFY shortDescriptionChanged)
		Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
		Q_PROPERTY(QString iconUrl READ iconUrl WRITE setIconUrl NOTIFY iconUrlChanged)
		Q_PROPERTY(bool proprietary READ proprietary WRITE setProprietary NOTIFY proprietaryChanged)
	public:
		explicit AddonItemModel(QObject *parent = nullptr);
		static AddonItemModel *fromJson(QByteArray json, QObject *parent = nullptr);

		QString name(void);
		void setName(QString newName);

		QString description(void);
		void setDescription(QString newDescription);

		QString shortDescription(void);

		QString version(void);
		void setVersion(QString newVersion);

		QString iconUrl(void);
		void setIconUrl(QString newIconUrl);

		bool proprietary(void);
		void setProprietary(bool newProprietary);

	private:
		QString m_name;
		QString m_description;
		QString m_version;
		QString m_iconUrl;
		bool m_proprietary = true;

	signals:
		void nameChanged();
		void descriptionChanged();
		void shortDescriptionChanged();
		void iconUrlChanged();
		void proprietaryChanged();
		void versionChanged();
};

#endif // ADDONITEMMODEL_H
