/*
 * UiEngine.cpp
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

#include "UiEngine.h"

/*! Returns a pointer to the UiEngine global instance. */
UiEngine *UiEngine::instance()
{
	static UiEngine instance;
	return &instance;
}

/*! Returns a pointer to the QQmlEngine. */
QQmlEngine *UiEngine::qmlEngine()
{
	return m_qmlEngine;
}

/*! Sets the QQmlEngine. */
void UiEngine::setQmlEngine(QQmlEngine *engine)
{
	m_qmlEngine = engine;
}

/*! Adds a QML import path. */
void UiEngine::addSourceImportPath(const QString &path)
{
	Q_ASSERT(m_qmlEngine);
	if(m_qmlEngine)
		m_qmlEngine->addImportPath(path);
}
