/*
 * AddonManager.h
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

#ifndef ADDONMANAGER_H
#define ADDONMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QPluginLoader>
#include <QMap>
#include "IAddonManager.h"
#include "AddonListModel.h"
#include "global/IFileUtils.h"
#include "global/ISettings.h"
#include "network/INetUtils.h"

class IAddon;

/*! \brief The AddonManager class is used to load, install, uninstall and update addons. */
class Q_DECL_EXPORT AddonManager : public IAddonManager
{
		Q_OBJECT
		INJECT(IFileUtils, fileUtils)
		INJECT(ISettings, settings)
		INJECT(INetUtils, netUtils)
		Q_PROPERTY(QList<AddonModel *> addons READ addons NOTIFY addonsChanged)
	public:
		static std::shared_ptr<AddonManager> instance();
		void init();

		QString addonDirectory() override;
		QList<AddonModel *> addons(void) override;

		Q_INVOKABLE AddonModel *findAddon(const QString &addonId) override;
		Q_INVOKABLE AddonModel *installAddon(AddonItemModel *itemModel) override;
		Q_INVOKABLE void uninstallAddon(const QString &id) override;
		Q_INVOKABLE void loadAddons(void) override;
		Q_INVOKABLE void unloadAddons(void) override;

		Q_INVOKABLE void getAddonUpdates(void) override;
		Q_INVOKABLE bool addonUpdateAvailable(void) override;
		Q_INVOKABLE void updateAddons(void) override;

	private:
		QString configLocation();
		QList<QPluginLoader *> loadAddons(const QString &path);
		void unloadAddon(const QString &id);
		void saveAddon(AddonModel *model);
		static std::shared_ptr<AddonManager> m_instance;
		QList<AddonModel *> m_addons;
		QJsonDocument document;
		QMap<QString, QList<QPluginLoader *>> pluginLoaders;
		QVector<IAddon *> loadedAddons;
		QStringList loadedAddonsClasses;
		AddonListModel updateListModel;
		QList<AddonItemModel *> updatableAddons;

		static const QString addonModelNameProperty;
		static const QString addonModelDescriptionProperty;
		static const QString addonModelIconFileNameProperty;
		static const QString addonModelRepositoryUrlProperty;
		static const QString addonModelQtMajorProperty;
		static const QString addonModelVersionProperty;
};

#endif // ADDONMANAGER_H
