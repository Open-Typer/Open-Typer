/*
 * AddonModel.cpp
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

#include <QNetworkAccessManager>
#include <QImageReader>
#include <QNetworkReply>
#include <QDir>
#include <QFile>
#include "AddonModel.h"
#include "framework/global/FileUtils.h"

/*! Constructs AddonModel. */
AddonModel::AddonModel(QObject *parent) :
	QObject(parent) { }

/*! Downloads the icon and sets iconFileName. */
void AddonModel::downloadIcon(QString url)
{
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	connect(manager, &QNetworkAccessManager::finished, [this, manager](QNetworkReply *reply) {
		auto redirectAttribute = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
		if(redirectAttribute.isValid())
		{
			manager->get(QNetworkRequest(redirectAttribute.toUrl()));
			return;
		}
		QImageReader reader(reply);
		Q_ASSERT(!reader.format().isEmpty());
		QFile iconFile(addonDirectory() + "/icon." + reader.format());
		if(iconFile.open(QFile::WriteOnly))
			iconFile.write(reply->readAll());
		QFileInfo fileInfo(iconFile.fileName());
		setIconFileName(fileInfo.fileName());
		updateProgress();
	});

	auto reply = manager->get(QNetworkRequest(QUrl(url)));

	connect(reply, &QNetworkReply::downloadProgress, [this](qint64 received, qint64 total) {
		if(total == 0)
			iconDownloadProgress = 1;
		else
			iconDownloadProgress = received / total;
		updateProgress();
	});
}

/*! Downloads addon files. */
void AddonModel::downloadFiles(QStringList urls)
{
	fileDownloadProgress.clear();
	for(int i = 0; i < urls.length(); i++)
	{
		QString url = urls[i];
		QNetworkAccessManager *manager = new QNetworkAccessManager(this);
		auto reply = manager->get(QNetworkRequest(QUrl(url)));
		fileDownloadProgress.append(0);

		connect(manager, &QNetworkAccessManager::finished, [this, url, manager](QNetworkReply *reply) {
			auto redirectAttribute = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
			if(redirectAttribute.isValid())
			{
				manager->get(QNetworkRequest(redirectAttribute.toUrl()));
				return;
			}
			QFileInfo fileInfo(url);
			QFile file(addonDirectory() + "/" + fileInfo.fileName());
			if(file.open(QFile::WriteOnly))
				file.write(reply->readAll());
		});

		connect(reply, &QNetworkReply::downloadProgress, [this, i](qint64 received, qint64 total) {
			if(total == 0)
				fileDownloadProgress[i] = 1;
			else
				fileDownloadProgress[i] = received / total;
			updateProgress();
		});
	}
}

/*! Returns the path to the directory of the addon. */
QString AddonModel::addonDirectory()
{
	QDir addonDir(FileUtils::addonDirectory() + "/" + m_id);
	if(!addonDir.exists())
		addonDir.mkpath(".");
	return addonDir.absolutePath();
}

/*! Updates the installationProgress property. */
void AddonModel::updateProgress(void)
{
	qreal progress = iconDownloadProgress;
	for(int i = 0; i < fileDownloadProgress.length(); i++)
		progress += fileDownloadProgress[i];
	setInstallationProgress(progress / (1 + fileDownloadProgress.length()));
	if(m_installationProgress == 1 && !iconFileName().isEmpty())
		setInstalled(true);
}

/*! Addon ID. */
QString AddonModel::id(void)
{
	return m_id;
}

void AddonModel::setId(QString newId)
{
	if(m_id == newId)
		return;
	m_id = newId;
	emit idChanged();
}

/*! Addon name. */
QString AddonModel::name(void)
{
	return m_name;
}

void AddonModel::setName(QString newName)
{
	if(m_name == newName)
		return;
	m_name = newName;
	emit nameChanged();
}

/*! Addon description. */
QString AddonModel::description(void)
{
	return m_description;
}

void AddonModel::setDescription(QString newDescription)
{
	if(m_description == newDescription)
		return;
	m_description = newDescription;
	emit descriptionChanged();
}

/*! URL of the addon repository. */
QString AddonModel::repositoryUrl(void)
{
	return m_repositoryUrl;
}

void AddonModel::setRepositoryUrl(QString newRepositoryUrl)
{
	if(m_repositoryUrl == newRepositoryUrl)
		return;
	m_repositoryUrl = newRepositoryUrl;
	emit repositoryUrlChanged();
}

/*! Current addon version. */
QVersionNumber AddonModel::version(void)
{
	return m_version;
}

void AddonModel::setVersion(QVersionNumber newVersion)
{
	if(m_version == newVersion)
		return;
	m_version = newVersion;
	emit versionChanged();
}

/*! The major version of Qt. If current major version of Qt is different, the addon is incompatible. */
int AddonModel::qtMajor(void)
{
	return m_qtMajor;
}

void AddonModel::setQtMajor(int newQtMajor)
{
	if(m_qtMajor == newQtMajor)
		return;
	m_qtMajor = newQtMajor;
	emit qtMajorChanged();
}

/*! Whether the addon is installed. */
bool AddonModel::installed(void)
{
	return m_installed;
}

void AddonModel::setInstalled(bool newInstalled)
{
	if(m_installed == newInstalled)
		return;
	m_installed = newInstalled;
	emit installedChanged();
}

/*! Use this while the addon is installing. */
qreal AddonModel::installationProgress(void)
{
	return m_installationProgress;
}

void AddonModel::setInstallationProgress(qreal newInstallationProgress)
{
	if(qFuzzyCompare(m_installationProgress, newInstallationProgress))
		return;
	m_installationProgress = newInstallationProgress;
	emit installationProgressChanged();
}

/*! The name of the icon file. */
QString AddonModel::iconFileName(void)
{
	return m_iconFileName;
}

void AddonModel::setIconFileName(QString newIconFileName)
{
	if(m_iconFileName == newIconFileName)
		return;
	m_iconFileName = newIconFileName;
	emit iconFileNameChanged();
}
