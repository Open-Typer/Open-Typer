/*
 * AddonItemModel.cpp
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

#include <QJsonDocument>
#include <QJsonObject>
#include "AddonItemModel.h"

/*! Constructs AddonItemModel. */
AddonItemModel::AddonItemModel(QObject *parent) :
	QObject(parent) { }

/*! Creates an AddonItemModel from the given JSON. */
AddonItemModel *AddonItemModel::fromJson(QByteArray json, QObject *parent)
{
	QJsonDocument document = QJsonDocument::fromJson(json);
	QJsonObject rootObj = document.object();
	AddonItemModel *ret = new AddonItemModel(parent);
	ret->setName(rootObj["name"].toString());
	ret->setDescription(rootObj["description"].toString());
	ret->setVersion(rootObj["version"].toString());
	ret->setIconUrl(rootObj["icon"].toString());
	if(rootObj.contains("proprietary"))
		ret->setProprietary(rootObj["proprietary"].toBool());
	return ret;
}

/*! The addon name. */
QString AddonItemModel::name(void)
{
	return m_name;
}

void AddonItemModel::setName(QString newName)
{
	if(m_name == newName)
		return;
	m_name = newName;
	emit nameChanged();
}

/*! The addon description. */
QString AddonItemModel::description(void)
{
	return m_description;
}

void AddonItemModel::setDescription(QString newDescription)
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
QString AddonItemModel::version(void)
{
	return m_version;
}

void AddonItemModel::setVersion(QString newVersion)
{
	if(m_version == newVersion)
		return;
	m_version = newVersion;
	emit versionChanged();
}

/*! The URL of the addon icon. */
QString AddonItemModel::iconUrl(void)
{
	return m_iconUrl;
}

void AddonItemModel::setIconUrl(QString newIconUrl)
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
