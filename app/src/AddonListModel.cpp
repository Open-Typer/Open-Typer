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

/*! Constructs AddonListModel. */
AddonListModel::AddonListModel(QObject *parent) :
	QObject(parent) { }

/*! Loads the available addons. Emits loaded() when loaded. */
void AddonListModel::load(void)
{
	m_items.clear();
	emit itemsChanged();

	QString repoUrl = "https://raw.githubusercontent.com/Open-Typer/addons/test";
	itemMap.clear();
	addonCounter = 0;
	addonCount = 0;

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, &QNetworkAccessManager::finished, [this, repoUrl](QNetworkReply *reply) {
		// Loop through all addon JSON files
		while(!reply->atEnd())
		{
			QString line = reply->readLine();
			line = line.remove("\n");
			QNetworkAccessManager *manager = new QNetworkAccessManager(this);
			connect(manager, &QNetworkAccessManager::finished, [this](QNetworkReply *reply) {
				// Create item model
				QByteArray json = reply->readAll();
				auto item = AddonItemModel::fromJson(json, this);
				if(item->downloadUrls().isEmpty())
					item->deleteLater();
				else
				{
					// Append addonCounter to the list to avoid duplicate keys
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
			manager->get(QNetworkRequest(QUrl(repoUrl + "/" + line + ".json")));
		}
	});
	// Download list of addons
	manager->get(QNetworkRequest(QUrl(repoUrl + "/addon-list")));
}

/*! List of currently loaded items. */
QList<AddonItemModel *> AddonListModel::items()
{
	return m_items;
}
