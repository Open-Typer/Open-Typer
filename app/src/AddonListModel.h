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
#include "AddonItemModel.h"

/*! \brief The AddonListModel class provides a model for the addon list in addon settings. */
class AddonListModel : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QList<AddonItemModel *> items READ items NOTIFY itemsChanged)
	public:
		explicit AddonListModel(QObject *parent = nullptr);
		Q_INVOKABLE void load(void);

		QList<AddonItemModel *> items();

	private:
		QList<AddonItemModel *> m_items;
		QMap<QString, AddonItemModel *> itemMap;
		int addonCounter, addonCount;

	signals:
		void loaded();
		void itemsChanged();
};

#endif // ADDONLISTMODEL_H
