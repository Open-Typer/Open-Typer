/*
 * AddonListModel.cpp
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
#include <QNetworkReply>
#include "AddonListModel.h"
#include "AddonManager.h"
#include "FileUtils.h"
#include "StringUtils.h"

/*! Constructs AddonListModel. */
AddonListModel::AddonListModel(QObject *parent) :
	QObject(parent) { }

/*! Loads the available addons. Emits loaded() when loaded. */
void AddonListModel::load(QString filter)
{
	emit loadingAborted();
	if(m_localAddons)
	{
		localItemMap.clear();
		auto addons = globalAddonManager.addons();
		for(int i = 0; i < addons.length(); i++)
		{
			auto model = addons[i];
			if(!filter.isEmpty())
			{
				bool nameMatches = model->name().contains(filter, Qt::CaseInsensitive);
				bool descMatches = model->description().contains(filter, Qt::CaseInsensitive);
				if(!(nameMatches || descMatches))
					continue;
			}
			AddonItemModel *itemModel = new AddonItemModel(this);
			itemModel->setId(model->id());
			itemModel->setName(model->name());
			itemModel->setDescription(model->description());
			itemModel->setVersion(model->version());
			itemModel->setIconUrl("file://" + FileUtils::addonDirectory() + "/" + model->id() + "/" + model->iconFileName());
			QStringList downloadUrls;
			QDir addonDir(FileUtils::addonDirectory() + "/" + model->id());
			QStringList addonFiles = addonDir.entryList(QDir::Files);
			addonFiles.removeAll(model->iconFileName());
			for(int i = 0; i < addonFiles.length(); i++)
				downloadUrls.append("file://" + FileUtils::addonDirectory() + "/" + model->id() + "/" + addonFiles[i]);
			itemModel->setDownloadUrls(downloadUrls);
			// Append i to the name to avoid duplicate keys
			localItemMap.insert(itemModel->name() + QString::number(i), itemModel);
		}
		int i;
		for(i = 0; i < m_localItems.length(); i++)
		{
			if(m_localItems[i])
				m_localItems[i]->deleteLater();
		}
		m_localItems.clear();
		// Append item pointers from sorted map
		QStringList keys = localItemMap.keys();
		for(i = 0; i < keys.length(); i++)
			m_localItems.append(localItemMap[keys[i]]);
		emit itemsChanged();
		emit loaded();
		return;
	}

	QString repoUrl = "https://raw.githubusercontent.com/Open-Typer/addons/test";
	itemMap.clear();
	addonCounter = 0;
	addonCount = 0;

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, &QNetworkAccessManager::finished, [this, manager, repoUrl, filter](QNetworkReply *reply) {
		if(reply->error() != QNetworkReply::NoError)
			return;
		auto redirectAttribute = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
		if(redirectAttribute.isValid())
		{
			manager->get(QNetworkRequest(redirectAttribute.toUrl()));
			return;
		}
		// Loop through all addon JSON files
		while(!reply->atEnd())
		{
			QString line = reply->readLine();
			line = line.remove("\n");
			QNetworkAccessManager *manager = new QNetworkAccessManager(this);
			connect(manager, &QNetworkAccessManager::finished, [this, line, manager, filter](QNetworkReply *reply) {
				if(reply->error() != QNetworkReply::NoError)
					return;
				auto redirectAttribute = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
				if(redirectAttribute.isValid())
				{
					manager->get(QNetworkRequest(redirectAttribute.toUrl()));
					return;
				}
				// Create item model
				QByteArray json = reply->readAll();
				auto item = AddonItemModel::fromJson(json, line, this);
				bool matches = true;
				if(!filter.isEmpty())
				{
					bool nameMatches = item->name().contains(filter, Qt::CaseInsensitive);
					bool descMatches = item->description().contains(filter, Qt::CaseInsensitive);
					matches = (nameMatches || descMatches);
				}
				if(!matches || item->downloadUrls().isEmpty())
					item->deleteLater();
				else
				{
					// Append addonCounter to the name to avoid duplicate keys
					itemMap.insert(item->name() + QString::number(addonCounter), item);
				}
				addonCounter++;
				if(addonCounter >= addonCount)
				{
					int i;
					for(i = 0; i < m_items.length(); i++)
					{
						if(m_items[i])
							m_items[i]->deleteLater();
					}
					m_items.clear();
					// Append item pointers from sorted map
					QStringList keys = itemMap.keys();
					for(i = 0; i < keys.length(); i++)
						m_items.append(itemMap[keys[i]]);
					emit itemsChanged();
					emit loaded();
				}
			});
			auto nextReply = manager->get(QNetworkRequest(QUrl(repoUrl + "/" + line + ".json")));
			connect(this, &AddonListModel::loadingAborted, nextReply, &QNetworkReply::abort);
		}
	});
	// Download list of addons
	auto reply = manager->get(QNetworkRequest(QUrl(repoUrl + "/addon-list")));
	connect(this, &AddonListModel::loadingAborted, reply, &QNetworkReply::abort);
}

/*! List of currently loaded items. */
QQmlListProperty<AddonItemModel> AddonListModel::items()
{
	QList<AddonItemModel *> *list;
	if(m_localAddons)
		list = &m_localItems;
	else
		list = &m_items;
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	return QQmlListProperty<AddonItemModel>(this, list);
#else
	return QQmlListProperty<AddonItemModel>(this, *list);
#endif
}

/*! Returns the list of currently loaded items. */
QList<AddonItemModel *> AddonListModel::getItems()
{
	if(m_localAddons)
		return m_localItems;
	else
		return m_items;
}

/*! Whether to load installed addons instead of online addons. */
bool AddonListModel::localAddons(void)
{
	return m_localAddons;
}

void AddonListModel::setLocalAddons(bool newLocalAddons)
{
	if(m_localAddons == newLocalAddons)
		return;
	m_localAddons = newLocalAddons;
	emit localAddonsChanged();
}
