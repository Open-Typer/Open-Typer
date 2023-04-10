/*
 * IAddonApi.h
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

#ifndef IADDONAPI_H
#define IADDONAPI_H

#include <QObject>
#include <QQmlListProperty>
#include "global/modularity/ioc.h"
#include "IAddonButton.h"
#include "ui/menubar/AppMenuModel.h"
#include "app/settings/SettingsCategory.h"

class IAddonApi : public QObject, MODULE_EXPORT_INTERFACE
{
		Q_OBJECT
	public:
		virtual ~IAddonApi() { }

		enum Event
		{
			Event_InitApp = 0,
			Event_RefreshApp = 1,
			Event_InitExercise = 2,
			Event_EndStockExercise = 3,
			Event_EndTypingTest = 4,
			Event_ChangeMode = 5,
			Event_CustomExLoaded = 6
		};
		Q_ENUM(Event)

		virtual bool addSettingsCategory(QString categoryName, QString qmlFileName, QString iconName, QString iconSource = "") = 0;
		virtual QQmlListProperty<SettingsCategory> settingsCategories(void) = 0;

		virtual void addMenu(AppMenuModel *menu) = 0;
		virtual QQmlListProperty<AppMenuModel> menus(void) = 0;

		virtual IAddonButton *addMainButton(QString text, QString toolTip, QString iconName, QString iconSource = "") = 0;
		virtual QQmlListProperty<IAddonButton> mainButtons(void) = 0;

		virtual IAddonButton *addExOptionsButton(QString text, QString toolTip, QString iconName, QString iconSource = "") = 0;
		virtual QQmlListProperty<IAddonButton> exOptionsButtons(void) = 0;

		virtual IAddonButton *addNavigationButton(QString text, QString toolTip, QString iconName, QString iconSource = "") = 0;
		virtual QQmlListProperty<IAddonButton> navigationButtons(void) = 0;

		virtual IAddonButton *addExInfoButton(QString text, QString toolTip, QString iconName, QString iconSource = "") = 0;
		virtual QQmlListProperty<IAddonButton> exInfoButtons(void) = 0;

	signals:
		void settingsCategoriesChanged();
		void menusChanged();
		void mainButtonsChanged();
		void exOptionsButtonsChanged();
		void navigationButtonsChanged();
		void exInfoButtonsChanged();
};

#endif // IADDONAPI_H
