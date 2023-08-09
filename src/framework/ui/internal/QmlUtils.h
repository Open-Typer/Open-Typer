/*
 * QmlUtils.h
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

#ifndef QMLUTILS_H
#define QMLUTILS_H

#include <QObject>
#include <QQuickItem>
#include <QFont>
#include "validator/MistakeRecord.h"
#include "validator/CharacterRecord.h"

/*! \brief The QmlUtils class acts like a bridge for QML code. */
class Q_DECL_EXPORT QmlUtils : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QQuickItem *blurSource READ blurSource WRITE setBlurSource NOTIFY blurSourceChanged) // deprecated
		Q_PROPERTY(QQuickItem *bgBlur READ bgBlur WRITE setBgBlur NOTIFY bgBlurChanged)
		Q_PROPERTY(QQuickItem *menuBarBlur READ menuBarBlur WRITE setMenuBarBlur NOTIFY menuBarBlurChanged)
		Q_PROPERTY(QQuickItem *tabBarBlur READ tabBarBlur WRITE setTabBarBlur NOTIFY tabBarBlurChanged)
		Q_PROPERTY(QQuickItem *activeFocusItem READ activeFocusItem WRITE setActiveFocusItem NOTIFY activeFocusItemChanged)
	public:
		enum StandardIcon
		{
			NoIcon = 0,
			Information = 1,
			Warning = 2,
			Critical = 3,
			Question = 4
		};
		Q_ENUM(StandardIcon)

		Q_DECL_DEPRECATED QQuickItem *blurSource(void);
		Q_DECL_DEPRECATED void setBlurSource(QQuickItem *item);
		QQuickItem *bgBlur(void);
		void setBgBlur(QQuickItem *item);
		QQuickItem *menuBarBlur(void);
		void setMenuBarBlur(QQuickItem *item);
		QQuickItem *tabBarBlur() const;
		void setTabBarBlur(QQuickItem *newTabBarBlur);
		QQuickItem *activeFocusItem(void);
		void setActiveFocusItem(QQuickItem *newActiveFocusItem);
		Q_INVOKABLE static bool nativeMenuBar(void);
		Q_INVOKABLE static int qtVersionMajor(void);
		Q_INVOKABLE static int qtVersionMinor(void);
		Q_INVOKABLE static int qtVersionPatch(void);
		Q_INVOKABLE static int compareQtVersions(int major1, int minor1, int patch1, int major2, int minor2, int patch2);
		Q_INVOKABLE static QString applicationRevision(void);
		Q_INVOKABLE static int applicationBuildYear(void);
		Q_INVOKABLE static void restartApplication(void);
		Q_INVOKABLE static bool osWindows(void);
		Q_INVOKABLE static bool osWasm(void);
		Q_INVOKABLE static bool osMac(void);
		Q_INVOKABLE void reloadMenuBar(void);
		static QPixmap standardIcon(StandardIcon icon);
		Q_INVOKABLE static QString standardIconStr(StandardIcon icon);
		Q_INVOKABLE static QStringList fontFamilies(bool fixedPitch = false);
		Q_INVOKABLE static QString translateStandardButton(QString buttonText);
		Q_INVOKABLE static MistakeRecord createMistakeRecord(void);
		Q_INVOKABLE static CharacterRecord createCharacterRecord(void);
		Q_INVOKABLE static QQuickItem *findFirstControl(QQuickItem *rootItem);
		Q_INVOKABLE static bool itemHasChild(QQuickItem *item, QQuickItem *child);

	private:
		static QString convertPixmap(QPixmap pixmap);
		static QList<QQuickItem *> itemChildren(QQuickItem *item);
		QQuickItem *m_blurSource = nullptr;
		QQuickItem *m_bgBlur = nullptr;
		QQuickItem *m_menuBarBlur = nullptr;
		QQuickItem *m_tabBarBlur = nullptr;
		QQuickItem *m_activeFocusItem = nullptr;

	signals:
		void blurSourceChanged(QQuickItem *item);
		void bgBlurChanged(QQuickItem *item);
		void menuBarBlurChanged(QQuickItem *item);
		void tabBarBlurChanged(QQuickItem *item);
		void activeFocusItemChanged(void);
		void menuBarReloadTriggered(void);
		void screenKeyboardChanged(bool layoutChanged); // used to update the keyboard after changing settings related to it
};

#endif // QMLUTILS_H
