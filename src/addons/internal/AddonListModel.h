/*
 * AddonListModel.h
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

#ifndef ADDONLISTMODEL_H
#define ADDONLISTMODEL_H

#include <QObject>
#include <QMap>
#include <QQmlListProperty>
#include "AddonItemModel.h"
#include "global/IFileUtils.h"

/*! \brief The AddonListModel class provides a model for the addon list in addon settings. */
class Q_DECL_EXPORT AddonListModel : public QObject
{
		Q_OBJECT
		INJECT(IFileUtils, fileUtils)
		Q_PROPERTY(QQmlListProperty<AddonItemModel> items READ items NOTIFY itemsChanged)
		Q_PROPERTY(bool localAddons READ localAddons WRITE setLocalAddons NOTIFY localAddonsChanged)
	public:
		explicit AddonListModel(QObject *parent = nullptr);
		Q_INVOKABLE void load(QString filter = QString());

		QQmlListProperty<AddonItemModel> items();
		QList<AddonItemModel *> getItems(void);

		bool localAddons(void);
		void setLocalAddons(bool newLocalAddons);

	private:
		QList<AddonItemModel *> m_items;
		QMap<QString, AddonItemModel *> itemMap;
		QList<AddonItemModel *> m_localItems;
		QMap<QString, AddonItemModel *> localItemMap;
		bool m_localAddons = false;
		int addonCounter, addonCount;

	signals:
		void loaded();
		void itemsChanged();
		void localAddonsChanged();
		void loadingAborted();
};

#endif // ADDONLISTMODEL_H
