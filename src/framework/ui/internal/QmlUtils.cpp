/*
 * QmlUtils.cpp
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

#include <QStyle>
#include <QFontDatabase>
#include <QFontInfo>
#include <QApplication>
#include <QBuffer>
#ifndef Q_OS_WASM
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QTextEdit>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#endif
#ifdef Q_OS_LINUX
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#endif
#include "QmlUtils.h"
#include "ThemeEngine.h"
#include "global/global.h"

/*!
 * Returns QML background blur source.
 * \deprecated Use the bgBlur property to set a custom blur item.
 */
QQuickItem *QmlUtils::blurSource(void)
{
	return m_blurSource;
}

void QmlUtils::setBlurSource(QQuickItem *item)
{
	m_blurSource = item;
	emit blurSourceChanged(item);
}

/*! QML background blur. \since Open-Typer 5.1.0 */
QQuickItem *QmlUtils::bgBlur(void)
{
	return m_bgBlur;
}

void QmlUtils::setBgBlur(QQuickItem *item)
{
	m_bgBlur = item;
	emit bgBlurChanged(item);
}

/*! QML menu bar blur item. */
QQuickItem *QmlUtils::menuBarBlur(void)
{
	return m_menuBarBlur;
}

void QmlUtils::setMenuBarBlur(QQuickItem *item)
{
	m_menuBarBlur = item;
	emit menuBarBlurChanged(item);
}

/*! QML tab bar blur item. \since Open-Typer 5.2.0 */
QQuickItem *QmlUtils::tabBarBlur() const
{
	return m_tabBarBlur;
}

void QmlUtils::setTabBarBlur(QQuickItem *newTabBarBlur)
{
	if(m_tabBarBlur == newTabBarBlur)
		return;
	m_tabBarBlur = newTabBarBlur;
	emit tabBarBlurChanged(m_tabBarBlur);
}

/*! Same as Window.activeFocusItem. \since Open-Typer 5.0.1 */
QQuickItem *QmlUtils::activeFocusItem(void)
{
	return m_activeFocusItem;
}

void QmlUtils::setActiveFocusItem(QQuickItem *newActiveFocusItem)
{
	if(m_activeFocusItem == newActiveFocusItem)
		return;
	m_activeFocusItem = newActiveFocusItem;
	emit activeFocusItemChanged();
}

/*! Returns true if the application uses native menu bar. */
bool QmlUtils::nativeMenuBar(void)
{
#if defined(Q_OS_MACOS)
	return true;
#elif defined(Q_OS_LINUX)
	const QDBusConnection connection = QDBusConnection::sessionBus();
	static const QString registrarService = QStringLiteral("com.canonical.AppMenu.Registrar");
	if(const auto iface = connection.interface())
		return iface->isServiceRegistered(registrarService);
	else
		return false;
#else
	return false;
#endif
}

/*! Returns major version of Qt. */
int QmlUtils::qtVersionMajor(void)
{
	return QT_VERSION_MAJOR;
}

/*! Returns minor version of Qt. */
int QmlUtils::qtVersionMinor(void)
{
	return QT_VERSION_MINOR;
}

/*! Returns patch version of Qt. */
int QmlUtils::qtVersionPatch(void)
{
	return QT_VERSION_PATCH;
}

/*!
 * Returns the application git revision.\n
 * Use QCoreApplication#applicationVersion() or Qt.application.version in QML to get the version.
 */
QString QmlUtils::applicationRevision(void)
{
	return BUILD_REVISION;
}

/*! Returns build year of the application. */
int QmlUtils::applicationBuildYear(void)
{
	QString buildDate = __DATE__;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	QString buildYear = buildDate.sliced(7, 4);
#else
	QString buildYear = buildDate.mid(7, 4);
#endif
	return buildYear.toInt();
}

/*! Restarts the application. */
void QmlUtils::restartApplication(void)
{
	qApp->exit(EXIT_CODE_REBOOT);
}

/*! Returns true if the platform is Windows. */
bool QmlUtils::osWindows(void)
{
#ifdef Q_OS_WINDOWS
	return true;
#else
	return false;
#endif
}

/*! Returns true if the platform is WebAssembly. */
bool QmlUtils::osWasm(void)
{
#ifdef Q_OS_WASM
	return true;
#else
	return false;
#endif
}

/*! Returns true if the platform is macOS. \since Open-Typer 5.0.3 */
bool QmlUtils::osMac(void)
{
#ifdef Q_OS_MACOS
	return true;
#else
	return false;
#endif
}

/*! Reloads the menu bar (e. g. after language change). */
void QmlUtils::reloadMenuBar(void)
{
	emit menuBarReloadTriggered();
}

/*! Returns the given message box standard icon. */
QPixmap QmlUtils::standardIcon(QmlUtils::StandardIcon icon)
{
	QStyle *style = QApplication::style();
	int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize, 0, nullptr);
	QIcon tmpIcon;
	switch(icon)
	{
		case StandardIcon::Information:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxInformation, 0, nullptr);
			break;
		case StandardIcon::Warning:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, nullptr);
			break;
		case StandardIcon::Critical:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxCritical, 0, nullptr);
			break;
		case StandardIcon::Question:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxQuestion, 0, nullptr);
			break;
		default:
			break;
	}
	return tmpIcon.pixmap(QSize(iconSize, iconSize));
}

/*! Returns the given message box standard icon as a string loadable from QML. \see standardIcon(). */
QString QmlUtils::standardIconStr(StandardIcon icon)
{
	return convertPixmap(standardIcon(icon));
}

/*! Saves the given pixmap in a string. */
QString QmlUtils::convertPixmap(QPixmap pixmap)
{
	QByteArray pixmapData = "PNG";
	QBuffer buffer(&pixmapData);
	buffer.open(QBuffer::WriteOnly);
	pixmap.save(&buffer, "PNG");
	QString imageStr = "data:image/png;base64,";
	imageStr += QString::fromLatin1(pixmapData.toBase64().data());
	return imageStr;
}

/*! Returns list of all (or only fixed pitch) font families. */
QStringList QmlUtils::fontFamilies(bool fixedPitch)
{
	QStringList out;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	QStringList families = QFontDatabase::families();
#else
	QFontDatabase fontDatabase;
	QStringList families = fontDatabase.families();
#endif
	for(int i = 0; i < families.count(); i++)
	{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		if((!fixedPitch || QFontDatabase::isFixedPitch(families[i])) && !QFontDatabase::isPrivateFamily(families[i]))
#else
		if((!fixedPitch || fontDatabase.isFixedPitch(families[i])) && !fontDatabase.isPrivateFamily(families[i]))
#endif
			out += families[i];
	}
	return out;
}

/*! Returns translated standard button text. */
QString QmlUtils::translateStandardButton(QString buttonText)
{
	QString translated = QCoreApplication::translate("QDialogButtonBox", buttonText.toStdString().c_str());
	if(translated == buttonText)
		// Some button translations are in QPlatformTheme
		translated = QCoreApplication::translate("QPlatformTheme", buttonText.toStdString().c_str());
	return translated;
}

/*! Returns an empty MistakeRecord. */
MistakeRecord QmlUtils::createMistakeRecord(void)
{
	return MistakeRecord();
}

/*! Returns an empty CharacterRecord. */
CharacterRecord QmlUtils::createCharacterRecord(void)
{
	return CharacterRecord();
}

/*! Finds the first QML Control in children items of the given item. \since Open-Typer 5.0.1 */
QQuickItem *QmlUtils::findFirstControl(QQuickItem *rootItem)
{
	if(!rootItem)
		return nullptr;
	QQuickItem *ret = nullptr;
	auto children = itemChildren(rootItem);
	for(int i = 0; i < children.length(); i++)
	{
		auto element = children[i];
		if(element->property("hovered").isValid()) // all Controls have the "hovered" property
		{
			ret = qobject_cast<QQuickItem *>(element);
			break;
		}
		else if(itemChildren(element).length() > 0)
		{
			auto control = findFirstControl(qobject_cast<QQuickItem *>(element));
			if(control != nullptr)
				return control;
		}
	}
	return ret;
}

/*! Returns true if the item has the child (this function searches recursively). \since Open-Typer 5.0.1 */
bool QmlUtils::itemHasChild(QQuickItem *item, QQuickItem *child)
{
	if(!item || !child)
		return false;
	auto children = itemChildren(item);
	for(int i = 0; i < children.length(); i++)
	{
		if(children[i] == child)
			return true;
		else if(itemChildren(children[i]).length() > 0)
		{
			auto castItem = qobject_cast<QQuickItem *>(children[i]);
			if(castItem == nullptr)
				continue;
			bool hasChild = itemHasChild(castItem, child);
			if(hasChild)
				return true;
		}
	}
	return false;
}

/*! Returns children items of the given items, including items in Repeaters (this function doesn't search recursively). \since Open-Typer 5.1.0 */
QList<QQuickItem *> QmlUtils::itemChildren(QQuickItem *item)
{
	QList<QQuickItem *> out;
	if(strcmp(item->metaObject()->className(), "QQuickRepeater") == 0)
	{
		int count = item->property("count").toInt();
		for(int i = 0; i < count; i++)
		{
			QQuickItem *child;
			QMetaObject::invokeMethod(item, "itemAt", Q_RETURN_ARG(QQuickItem *, child), Q_ARG(int, i));
			out.append(child);
		}
	}
	else
	{
		auto children = item->children();
		for(int i = 0; i < children.length(); i++)
		{
			QQuickItem *currentItem = qobject_cast<QQuickItem *>(children[i]);
			if(currentItem)
				out.append(currentItem);
		}
	}
	Q_ASSERT(!out.contains(nullptr));
	return out;
}
