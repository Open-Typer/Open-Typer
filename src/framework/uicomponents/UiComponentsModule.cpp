/*
 * UiComponentsModule.cpp
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
#include "UiComponentsModule.h"
#include "QmlFileDialog.h"
#include "QmlWidget.h"

std::string UiComponentsModule::moduleName() const
{
	return "uicomponents";
}

void UiComponentsModule::registerUiTypes()
{
	qmlRegisterType<QmlFileDialog>("OpenTyper", 1, 0, "QmlFileDialog");
	qmlRegisterType<QmlWidget>("OpenTyper", 1, 0, "Widget");
}
