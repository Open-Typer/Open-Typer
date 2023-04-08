/*
 * AddonItemModel.cpp
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

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QApplication>
#include "AddonItemModel.h"

const QString AddonItemModel::downloadLinkEntryTemplate = "download_%1_qt%2_%3";
const QString AddonItemModel::appVersionProperty = "app_version";
const QString AddonItemModel::addonNameProperty = "name";
const QString AddonItemModel::addonDescriptionProperty = "description";
const QString AddonItemModel::addonVersionProperty = "version";
const QString AddonItemModel::addonIconPathProperty = "icon";
const QString AddonItemModel::addonIsProprietaryProperty = "proprietary";

/*! Constructs AddonItemModel. */
AddonItemModel::AddonItemModel(QObject *parent) :
	QObject(parent) { }

/*! Creates an AddonItemModel from the given JSON. */
AddonItemModel *AddonItemModel::fromJson(const QByteArray &json, const QString &id, QObject *parent)
{
#if defined(Q_OS_LINUX)
	const QString osName = "linux";
#elif defined(Q_OS_MACOS)
	const QString osName = "macos";
#elif defined(Q_OS_WINDOWS)
	const QString osName = "windows";
#endif
	const QString qtMajor = QString::number(QT_VERSION_MAJOR);
	const QString arch = QSysInfo::buildCpuArchitecture();
	const QString downloadLinkEntry = downloadLinkEntryTemplate.arg(osName, qtMajor, arch);

	const QJsonDocument document = QJsonDocument::fromJson(json);
	const QJsonObject rootObj = document.object();
	const QVersionNumber version = QVersionNumber::fromString(rootObj[appVersionProperty].toString());
	const QVersionNumber appVersion = QVersionNumber::fromString(qApp->applicationVersion());
	if(version > appVersion || appVersion.majorVersion() != version.majorVersion())
		return new AddonItemModel(parent);
	AddonItemModel *ret = new AddonItemModel(parent);
	ret->setId(id);
	ret->setName(rootObj[addonNameProperty].toString());
	ret->setDescription(rootObj[addonDescriptionProperty].toString());
	ret->setVersion(QVersionNumber::fromString(rootObj[addonVersionProperty].toString()));
	ret->setIconUrl(rootObj[addonIconPathProperty].toString());
	if(rootObj.contains(addonIsProprietaryProperty))
		ret->setProprietary(rootObj[addonIsProprietaryProperty].toBool());
	if(rootObj.contains(downloadLinkEntry))
	{
		QVariantList list = rootObj[downloadLinkEntry].toArray().toVariantList();
		QStringList urlList;
		for(int i = 0; i < list.length(); i++)
			urlList.append(list[i].toString());
		ret->setDownloadUrls(urlList);
	}
	return ret;
}

/*! Addon ID. */
const QString &AddonItemModel::id(void)
{
	return m_id;
}

void AddonItemModel::setId(const QString &newId)
{
	if(m_id == newId)
		return;
	m_id = newId;
	emit idChanged();
}

/*! The addon name. */
const QString &AddonItemModel::name(void)
{
	return m_name;
}

void AddonItemModel::setName(const QString &newName)
{
	if(m_name == newName)
		return;
	m_name = newName;
	emit nameChanged();
}

/*! The addon description. */
const QString &AddonItemModel::description(void)
{
	return m_description;
}

void AddonItemModel::setDescription(const QString &newDescription)
{
	if(m_description == newDescription)
		return;
	m_description = newDescription;
	emit descriptionChanged();
	emit shortDescriptionChanged();
}

/*! A short part of the description. */
QString AddonItemModel::shortDescription(void)
{
	if(m_description.length() > 32)
		return m_description;
	else
	{
		QString ret = m_description;
		ret.truncate(29);
		return ret + "...";
	}
}

/*! The addon version. */
const QVersionNumber &AddonItemModel::version(void)
{
	return m_version;
}

void AddonItemModel::setVersion(const QVersionNumber &newVersion)
{
	if(m_version == newVersion)
		return;
	m_version = newVersion;
	emit versionChanged();
}

/*! The URL of the addon icon. */
const QString &AddonItemModel::iconUrl(void)
{
	return m_iconUrl;
}

void AddonItemModel::setIconUrl(const QString &newIconUrl)
{
	if(m_iconUrl == newIconUrl)
		return;
	m_iconUrl = newIconUrl;
	emit iconUrlChanged();
}

/*! Whether this addon is proprietary. */
bool AddonItemModel::proprietary(void)
{
	return m_proprietary;
}

void AddonItemModel::setProprietary(bool newProprietary)
{
	if(m_proprietary == newProprietary)
		return;
	m_proprietary = newProprietary;
	emit proprietaryChanged();
}

/*! List of download links of the files for the addon. */
QStringList AddonItemModel::downloadUrls(void)
{
	return m_downloadUrls;
}

void AddonItemModel::setDownloadUrls(QStringList newDownloadUrls)
{
	if(m_downloadUrls == newDownloadUrls)
		return;
	m_downloadUrls = newDownloadUrls;
	emit downloadUrlsChanged();
}

/*! URL of the addon repository. */
const QString &AddonItemModel::repositoryUrl(void)
{
	return m_repositoryUrl;
}

void AddonItemModel::setRepositoryUrl(const QString &newRepositoryUrl)
{
	if(m_repositoryUrl == newRepositoryUrl)
		return;
	m_repositoryUrl = newRepositoryUrl;
	emit repositoryUrlChanged();
}