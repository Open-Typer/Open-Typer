/*
 * QmlUtils.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#include "QmlUtils.h"
#include <QMenuBar>

/*! Sets QML root item. */
void QmlUtils::setBlurSource(QQuickItem *item)
{
	m_blurSource = item;
	emit blurSourceChanged(item);
}

/*! Returns QML root item. */
QQuickItem *QmlUtils::blurSource(void)
{
	return m_blurSource;
}

/*! Returns true if the application uses native menu bar. */
bool QmlUtils::nativeMenuBar(void)
{
	QMenuBar menuBar;
	return menuBar.isNativeMenuBar();
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

// TODO: Remove this after dropping Qt 5.9 support
/*! Emits menuBarReloadTriggered() to fix empty menu bar in Qt 5.9. */
void QmlUtils::reloadMenuBar(void)
{
	emit menuBarReloadTriggered();
}
