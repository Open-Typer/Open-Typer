/*
 * ExportModule.cpp
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

#include <QQmlEngine>
#include "ExportModule.h"
#include "internal/ExportProvider.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
static ExportTable *tablePtr = nullptr;
#endif

std::string ExportModule::moduleName() const
{
	return "export";
}

void ExportModule::registerUiTypes()
{
	qmlRegisterType<ExportProvider>("OpenTyper.Export", 1, 0, "ExportProvider");
	QQmlEngine::setObjectOwnership(m_table, QQmlEngine::CppOwnership);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	qmlRegisterSingletonType<ExportTable>("OpenTyper.Export", 1, 0, "ExportTable", [this](QQmlEngine *, QJSEngine *) -> QObject * {
		return m_table;
	});
#else
	tablePtr = m_table;
	qmlRegisterSingletonType<ExportTable>("OpenTyper.Export", 1, 0, "ExportTable", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return tablePtr;
	});
#endif
}

void ExportModule::onPreInit()
{
	m_table = new ExportTable;
}
