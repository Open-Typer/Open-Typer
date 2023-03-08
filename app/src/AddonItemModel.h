/*
 * AddonItemModel.h
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

#ifndef ADDONITEMMODEL_H
#define ADDONITEMMODEL_H

#include <QObject>
#include <QVersionNumber>

/*! \brief The AddonItemModel class provides a model for a single item in a list of addons. */
class AddonItemModel : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
		Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
		Q_PROPERTY(QString shortDescription READ shortDescription NOTIFY shortDescriptionChanged)
		Q_PROPERTY(QVersionNumber version READ version WRITE setVersion NOTIFY versionChanged)
		Q_PROPERTY(QString iconUrl READ iconUrl WRITE setIconUrl NOTIFY iconUrlChanged)
		Q_PROPERTY(bool proprietary READ proprietary WRITE setProprietary NOTIFY proprietaryChanged)
		Q_PROPERTY(QStringList downloadUrls READ downloadUrls WRITE setDownloadUrls NOTIFY downloadUrlsChanged)
		Q_PROPERTY(QString repositoryUrl READ repositoryUrl WRITE setRepositoryUrl NOTIFY repositoryUrlChanged)
	public:
		explicit AddonItemModel(QObject *parent = nullptr);
		static AddonItemModel *fromJson(const QByteArray &json, const QString &id, QObject *parent = nullptr);

		const QString &id(void);
		void setId(const QString &newId);

		const QString &name(void);
		void setName(const QString &newName);

		const QString &description(void);
		void setDescription(const QString &newDescription);

		QString shortDescription(void);

		const QVersionNumber &version(void);
		void setVersion(const QVersionNumber &newVersion);

		const QString &iconUrl(void);
		void setIconUrl(const QString &newIconUrl);

		bool proprietary(void);
		void setProprietary(bool newProprietary);

		QStringList downloadUrls(void);
		void setDownloadUrls(QStringList newDownloadUrls);

		const QString &repositoryUrl(void);
		void setRepositoryUrl(const QString &newRepositoryUrl);

	private:
		QString m_id;
		QString m_name;
		QString m_description;
		QVersionNumber m_version;
		QString m_iconUrl;
		bool m_proprietary = true;
		QStringList m_downloadUrls;
		QString m_repositoryUrl;

		static const QString downloadLinkEntryTemplate;
		static const QString appVersionProperty;
		static const QString addonNameProperty;
		static const QString addonDescriptionProperty;
		static const QString addonVersionProperty;
		static const QString addonIconPathProperty;
		static const QString addonIsProprietaryProperty;

	signals:
		void idChanged();
		void nameChanged();
		void descriptionChanged();
		void shortDescriptionChanged();
		void iconUrlChanged();
		void proprietaryChanged();
		void versionChanged();
		void downloadUrlsChanged();
		void repositoryUrlChanged();
};

#endif // ADDONITEMMODEL_H
