/*
 * AddonModel.h
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

#ifndef ADDONMODEL_H
#define ADDONMODEL_H

#include <QObject>
#include <QVersionNumber>
#include "global/IFileUtils.h"

/*! \brief The AddonModel class provides a model for installed addons. */
class Q_DECL_EXPORT AddonModel : public QObject
{
		Q_OBJECT
		INJECT(IFileUtils, fileUtils)
		Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
		Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
		Q_PROPERTY(QString iconFileName READ iconFileName WRITE setIconFileName NOTIFY iconFileNameChanged)
		Q_PROPERTY(QString repositoryUrl READ repositoryUrl WRITE setRepositoryUrl NOTIFY repositoryUrlChanged)
		Q_PROPERTY(QVersionNumber version READ version WRITE setVersion NOTIFY versionChanged)
		Q_PROPERTY(int qtMajor READ qtMajor WRITE setQtMajor NOTIFY qtMajorChanged)
		Q_PROPERTY(bool installed READ installed WRITE setInstalled NOTIFY installedChanged)
		Q_PROPERTY(qreal installationProgress READ installationProgress WRITE setInstallationProgress NOTIFY installationProgressChanged)
	public:
		explicit AddonModel(QObject *parent = nullptr);

		Q_INVOKABLE void downloadIcon(QString url);
		Q_INVOKABLE void downloadFiles(QStringList urls);

		QString id(void);
		void setId(QString newId);

		QString name(void);
		void setName(QString newName);

		QString description(void);
		void setDescription(QString newDescription);

		QString iconFileName(void);
		void setIconFileName(QString newIconFileName);

		QString repositoryUrl(void);
		void setRepositoryUrl(QString newRepositoryUrl);

		QVersionNumber version(void);
		void setVersion(QVersionNumber newVersion);

		int qtMajor(void);
		void setQtMajor(int newQtMajor);

		bool installed(void);
		void setInstalled(bool newInstalled);

		qreal installationProgress(void);
		void setInstallationProgress(qreal newInstallationProgress);

	private:
		QString addonDirectory(void);
		void updateProgress(void);

		QString m_id;
		QString m_name;
		QString m_description;
		QString m_repositoryUrl;
		QVersionNumber m_version;
		int m_qtMajor;
		bool m_installed;
		qreal m_installationProgress;
		QString m_iconFileName;

		qreal iconDownloadProgress = 0;
		QList<qreal> fileDownloadProgress;

	signals:
		void idChanged();
		void nameChanged();
		void descriptionChanged();
		void repositoryUrlChanged();
		void versionChanged();
		void qtMajorChanged();
		void installedChanged();
		void installationProgressChanged();
		void iconFileNameChanged();
};

#endif // ADDONMODEL_H
