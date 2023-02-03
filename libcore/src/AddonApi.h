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
#include "IAddon.h"
#include "AddonButton.h"
#include "AppMenuModel.h"

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

/*! \brief The AddonApi class provides an API for addons. */
class CORE_LIB_EXPORT AddonApi : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QList<AppMenuModel *> menus READ menus NOTIFY menusChanged)
		Q_PROPERTY(QList<AddonButton *> mainButtons READ mainButtons NOTIFY mainButtonsChanged)
		Q_PROPERTY(QList<AddonButton *> exOptionsButtons READ exOptionsButtons NOTIFY exOptionsButtonsChanged)
		Q_PROPERTY(QList<AddonButton *> navigationButtons READ navigationButtons NOTIFY navigationButtonsChanged)
		Q_PROPERTY(QList<AddonButton *> exInfoButtons READ exInfoButtons NOTIFY exInfoButtonsChanged)
	public:
		static AddonApi *instance(void);
		static void addLoadExTarget(int id, QString name);
		static void clearLoadExTargets(void);
		static QMap<int, QString> loadExTargets(void);
		static bool blockLoadedEx(void);
		static void setBlockLoadedEx(bool value);
		static bool addSettingsCategory(QString categoryName, QIcon icon, QString className);
		static QList<QVariantMap> settingsCategories(void);
		static void clearSettingsCategories(void);
		static void initSettingsCategories(bool clear = false);
		static void sendEvent(IAddon::Event type, QVariantMap args = QVariantMap());

		void addMenu(AppMenuModel *menu);
		QList<AppMenuModel *> menus(void);
		void setMenus(QList<AppMenuModel *> menus);

		AddonButton *addMainButton(QString text, QString toolTip, QString iconName, QString iconSource);
		QList<AddonButton *> mainButtons(void);

		AddonButton *addExOptionsButton(QString text, QString toolTip, QString iconName, QString iconSource);
		QList<AddonButton *> exOptionsButtons(void);

		AddonButton *addNavigationButton(QString text, QString toolTip, QString iconName, QString iconSource);
		QList<AddonButton *> navigationButtons(void);

		AddonButton *addExInfoButton(QString text, QString toolTip, QString iconName, QString iconSource);
		QList<AddonButton *> exInfoButtons(void);

	private:
		void deleteMenus(void);
		void deleteMainButtons(void);
		void deleteExOptionsButtons(void);
		void deleteNavigationButtons(void);
		void deleteExInfoButtons(void);
		void deleteButtons(QList<AddonButton *> *buttonList);
		AddonButton *createButton(QString text, QString toolTip, QString iconName, QString iconSource);
		static AddonApi m_instance;
		static QMap<int, QString> m_loadExTargets;
		static bool m_blockLoadedEx;
		static QList<QVariantMap> m_settingsCategories;
		QList<AppMenuModel *> m_menus;
		QList<AddonButton *> m_mainButtons;
		QList<AddonButton *> m_exOptionsButtons;
		QList<AddonButton *> m_navigationButtons;
		QList<AddonButton *> m_exInfoButtons;

	signals:
		void changeMode(int mode);
		void startTypingTest(QByteArray text, int lineLength, bool includeNewLines, int mode, int time, bool correctMistakes, bool lockUi, bool hideText);
		void menusChanged(QList<AppMenuModel *> menus);
		void mainButtonsChanged(QList<AddonButton *> buttons);
		void exOptionsButtonsChanged(QList<AddonButton *> buttons);
		void navigationButtonsChanged(QList<AddonButton *> buttons);
		void exInfoButtonsChanged(QList<AddonButton *> buttons);
};

#endif // ADDONAPI_H
