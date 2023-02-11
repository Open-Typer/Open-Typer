/*
 * AddonManager.h
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

#ifndef ADDONMANAGER_H
#define ADDONMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include "AddonModel.h"
#include "AddonItemModel.h"

class QPluginLoader;

/*!
 * \brief The AddonManager class is used to load, install, uninstall and update addons.
 * Don't create instances of AddonManager. Use globalAddonManager instead.
 */
class AddonManager : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QList<AddonModel *> addons READ addons NOTIFY addonsChanged)
	public:
		explicit AddonManager(QObject *parent = nullptr);

		QList<AddonModel *> addons(void);

		Q_INVOKABLE AddonModel *findAddon(QString addonId);
		Q_INVOKABLE void installAddon(AddonItemModel *itemModel);
		Q_INVOKABLE void uninstallAddon(QString id);
		Q_INVOKABLE void loadAddons(void);
		Q_INVOKABLE void unloadAddons(void);

	private:
		QList<QPluginLoader *> loadAddons(QString path);
		void unloadAddon(QString id);
		void saveAddon(AddonModel *model);
		QList<AddonModel *> m_addons;
		QJsonDocument document;
		QMap<QString, QList<QPluginLoader *>> pluginLoaders;

	signals:
		void addonsChanged();
};

extern AddonManager globalAddonManager;

#endif // ADDONMANAGER_H
