/*
 * AddonApi.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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

#ifndef ADDONAPI_H
#define ADDONAPI_H

#include <QObject>
#include <QIcon>
#include <QMessageBox>
#include <QVariantMap>
#include <QMenu>
#include <QPushButton>
#include <QLayout>
#include "IAddonApi.h"
#include "app/settings/SettingsCategory.h"
#include "AddonButton.h"
#include "ui/menubar/AppMenuModel.h"
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
Q_MOC_INCLUDE("IAddon.h")
#endif

class Q_DECL_EXPORT IAddon;

/*! \brief The AddonApi class provides an API for addons. */
class Q_DECL_EXPORT AddonApi : public IAddonApi
{
		Q_OBJECT
		Q_PROPERTY(QVector<IAddon *> loadedAddons READ loadedAddons WRITE setLoadedAddons)
		Q_PROPERTY(QQmlListProperty<SettingsCategory> settingsCategories READ settingsCategories NOTIFY settingsCategoriesChanged)
		Q_PROPERTY(QQmlListProperty<AppMenuModel> menus READ menus NOTIFY menusChanged)
		Q_PROPERTY(QQmlListProperty<AddonButton> mainButtons READ mainButtons NOTIFY mainButtonsChanged)
		Q_PROPERTY(QQmlListProperty<AddonButton> exOptionsButtons READ exOptionsButtons NOTIFY exOptionsButtonsChanged)
		Q_PROPERTY(QQmlListProperty<AddonButton> navigationButtons READ navigationButtons NOTIFY navigationButtonsChanged)
		Q_PROPERTY(QQmlListProperty<AddonButton> exInfoButtons READ exInfoButtons NOTIFY exInfoButtonsChanged)
	public:
		static std::shared_ptr<AddonApi> instance();
		Q_INVOKABLE void sendEvent(Event type, QVariantMap args = QVariantMap());

		QVector<IAddon *> loadedAddons(void);
		void setLoadedAddons(QVector<IAddon *> newLoadedAddons);

		bool addSettingsCategory(QString categoryName, QString qmlFileName, QString iconName, QString iconSource = "");
		QQmlListProperty<SettingsCategory> settingsCategories(void);

		void addMenu(AppMenuModel *menu);
		QQmlListProperty<AppMenuModel> menus(void);

		AddonButton *addMainButton(QString text, QString toolTip, QString iconName, QString iconSource = "");
		QQmlListProperty<AddonButton> mainButtons(void);

		AddonButton *addExOptionsButton(QString text, QString toolTip, QString iconName, QString iconSource = "");
		QQmlListProperty<AddonButton> exOptionsButtons(void);

		AddonButton *addNavigationButton(QString text, QString toolTip, QString iconName, QString iconSource = "");
		QQmlListProperty<AddonButton> navigationButtons(void);

		AddonButton *addExInfoButton(QString text, QString toolTip, QString iconName, QString iconSource = "");
		QQmlListProperty<AddonButton> exInfoButtons(void);

	private:
		void deleteSettingsCategories(void);
		void deleteMenus(void);
		void deleteMainButtons(void);
		void deleteExOptionsButtons(void);
		void deleteNavigationButtons(void);
		void deleteExInfoButtons(void);
		void deleteButtons(QList<AddonButton *> *buttonList);
		void deleteObjects(QList<QObject *> *objList);
		AddonButton *createButton(QString text, QString toolTip, QString iconName, QString iconSource);
		static std::shared_ptr<AddonApi> m_instance;
		QVector<IAddon *> m_loadedAddons;
		QList<SettingsCategory *> m_settingsCategories;
		QList<AppMenuModel *> m_menus;
		QList<AddonButton *> m_mainButtons;
		QList<AddonButton *> m_exOptionsButtons;
		QList<AddonButton *> m_navigationButtons;
		QList<AddonButton *> m_exInfoButtons;
};

#endif // ADDONAPI_H
