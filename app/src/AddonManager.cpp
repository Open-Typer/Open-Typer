/*
 * AddonManager.cpp
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

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QPluginLoader>
#include <QProcessEnvironment>
#include <QCoreApplication>
#include "AddonManager.h"
#include "FileUtils.h"
#include "IAddon.h"

AddonManager globalAddonManager;

/*! Constructs AddonManager. */
AddonManager::AddonManager(QObject *parent) :
	QObject(parent)
{
	QFile jsonFile(FileUtils::addonConfigLocation());
	if(jsonFile.open(QFile::ReadOnly | QFile::Text))
	{
		document = QJsonDocument::fromJson(jsonFile.readAll());
		if(!document.isObject())
			return;
		QJsonObject obj = document.object();
		QStringList keys = obj.keys();
		for(int i = 0; i < keys.length(); i++)
		{
			QJsonObject addonObj = obj[keys[i]].toObject();
			AddonModel *model = new AddonModel(this);
			model->setId(keys[i]);
			model->setName(addonObj["name"].toString());
			model->setDescription(addonObj["description"].toString());
			model->setIconFileName(addonObj["iconFileName"].toString());
			model->setRepositoryUrl(addonObj["repositoryUrl"].toString());
			model->setVersion(QVersionNumber::fromString(addonObj["version"].toString()));
			model->setQtMajor(addonObj["qtMajor"].toInt());
			model->setInstalled(true);
			m_addons.append(model);
		}
		emit addonsChanged();
	}
}

/*! List of installed addons (and addons that are currently being installed). */
QList<AddonModel *> AddonManager::addons(void)
{
	return m_addons;
}

/*! Returns the addon with the given ID, or nullptr if it isn't in the list. */
AddonModel *AddonManager::findAddon(QString addonId)
{
	for(int i = 0; i < m_addons.length(); i++)
	{
		auto addon = m_addons[i];
		if(addon->id() == addonId)
			return addon;
	}
	return nullptr;
}

/*! Installs a new addon. */
void AddonManager::installAddon(AddonItemModel *itemModel)
{
	AddonModel *model = new AddonModel(this);
	model->setId(itemModel->id());
	model->setName(itemModel->name());
	model->setDescription(itemModel->description());
	model->setRepositoryUrl(itemModel->repositoryUrl());
	model->setVersion(itemModel->version());
	model->setQtMajor(QT_VERSION_MAJOR);
	model->setInstalled(false);
	model->setInstallationProgress(0);
	model->downloadIcon(itemModel->iconUrl());
	model->downloadFiles(itemModel->downloadUrls());
	m_addons.append(model);
	emit addonsChanged();

	connect(model, &AddonModel::installedChanged, [this, model]() {
		if(model->installed())
			saveAddon(model);
	});
}

/*! Uninstalls an addon. */
void AddonManager::uninstallAddon(QString id)
{
	unloadAddon(id);
	auto model = findAddon(id);
	if(model == nullptr)
		return;
	if(!model->installed())
		return;
	QDir addonDir(FileUtils::addonDirectory() + "/" + model->id());
	addonDir.removeRecursively();
	m_addons.removeAll(model);
	emit addonsChanged();
	model->deleteLater();

	QJsonObject obj = document.object();
	obj.remove(id);
	document.setObject(obj);
	QFile jsonFile(FileUtils::addonConfigLocation());
	if(jsonFile.open(QFile::WriteOnly | QFile::Text))
		jsonFile.write(document.toJson(QJsonDocument::Compact));
}

/*! Loads an addon from the given path. */
QList<QPluginLoader *> AddonManager::loadAddons(QString path)
{
	QList<QPluginLoader *> loaderList;
	QDir pluginsDir(path);
	const QStringList entries = pluginsDir.entryList(QDir::Files);
	for(const QString &fileName : entries)
	{
		QPluginLoader *pluginLoader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName), this);
		QObject *plugin = pluginLoader->instance();
		if(plugin)
		{
			IAddon *addonInterface = qobject_cast<IAddon *>(plugin);
			if(addonInterface)
			{
				QString className = plugin->metaObject()->className();
				QStringList addonMinVersion = addonInterface->version().split(".");
				QStringList appVersion = QCoreApplication::applicationVersion().split(".");
				if(addonMinVersion[0].toInt() != appVersion[0].toInt())
					continue;
				if(addonMinVersion[1].toInt() > appVersion[1].toInt())
					continue;
				if(loadedAddonsClasses.contains(className))
					continue;
				loadedAddons.append(addonInterface);
				loadedAddonsClasses += className;
				loaderList.append(pluginLoader);
			}
			else
			{
				pluginLoader->unload();
				pluginLoader->deleteLater();
			}
		}
	}
	AddonApi::setLoadedAddons(loadedAddons);
	return loaderList;
}

/*! Loads all installed addons. */
void AddonManager::loadAddons(void)
{
	pluginLoaders.clear();
	for(int i = 0; i < m_addons.length(); i++)
		pluginLoaders.insert(m_addons[i]->id(), loadAddons(FileUtils::addonDirectory() + "/" + m_addons[i]->id()));
}

/*! Unloads all addons. */
void AddonManager::unloadAddons(void)
{
	QStringList keys = pluginLoaders.keys();
	for(int i = 0; i < keys.length(); i++)
		unloadAddon(keys[i]);
	Q_ASSERT(pluginLoaders.count() == 0);
}

/*! Unloads the addon with the given id. */
void AddonManager::unloadAddon(QString id)
{
	auto list = pluginLoaders[id];
	for(int i = 0; i < list.length(); i++)
	{
		loadedAddons.removeAll(qobject_cast<IAddon *>(list[i]->instance()));
		list[i]->unload();
		list[i]->deleteLater();
	}
	pluginLoaders.remove(id);
	AddonApi::setLoadedAddons(loadedAddons);
}

/*! Saves the installed addon in the JSON file. */
void AddonManager::saveAddon(AddonModel *model)
{
	QJsonObject addonObj;
	addonObj["name"] = model->name();
	addonObj["description"] = model->description();
	addonObj["iconFileName"] = model->iconFileName();
	addonObj["repositoryUrl"] = model->repositoryUrl();
	addonObj["version"] = model->version().toString();
	addonObj["qtMajor"] = model->qtMajor();
	QJsonObject obj = document.object();
	obj[model->id()] = addonObj;
	document.setObject(obj);
	QFile jsonFile(FileUtils::addonConfigLocation());
	if(jsonFile.open(QFile::WriteOnly | QFile::Text))
		jsonFile.write(document.toJson(QJsonDocument::Compact));
}
