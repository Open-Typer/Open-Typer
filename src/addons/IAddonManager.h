/*
 * IAddonManager.h
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

#ifndef IADDONMANAGER_H
#define IADDONMANAGER_H

#include "global/modularity/ioc.h"
#include "internal/AddonModel.h"
#include "internal/AddonItemModel.h"

class IAddonManager : public QObject, MODULE_EXPORT_INTERFACE
{
		Q_OBJECT
	public:
		virtual ~IAddonManager() { }

		virtual QString addonDirectory() = 0;
		virtual QList<AddonModel *> addons(void) = 0;

		virtual AddonModel *findAddon(const QString &addonId) = 0;
		virtual AddonModel *installAddon(AddonItemModel *itemModel) = 0;
		virtual void uninstallAddon(const QString &id) = 0;
		virtual void loadAddons(void) = 0;
		virtual void unloadAddons(void) = 0;

		virtual void getAddonUpdates(void) = 0;
		virtual bool addonUpdateAvailable(void) = 0;
		virtual void updateAddons(void) = 0;

	signals:
		void addonsChanged();
};

#endif // IADDONMANAGER_H
