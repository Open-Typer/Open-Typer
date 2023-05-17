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
#include "internal/QmlFileDialog.h"
#include "internal/QmlWidget.h"
#include "view/DialogView.h"

std::string UiComponentsModule::moduleName() const
{
	return "uicomponents";
}

void UiComponentsModule::registerResources()
{
	Q_INIT_RESOURCE(uicomponents);
}

void UiComponentsModule::registerUiTypes()
{
	qmlRegisterType<QmlFileDialog>("OpenTyper.UiComponents", 1, 0, "QmlFileDialog");
	qmlRegisterType<QmlWidget>("OpenTyper.UiComponents", 1, 0, "Widget");
	qmlRegisterType<QWidget>("OpenTyper.UiComponents", 1, 0, "QWidget");
	qmlRegisterType<DialogView>("OpenTyper.UiComponents", 1, 0, "DialogView");
	// TODO: Remove this after fully switching to Qt 6
	qmlRegisterModule("Qt5Compat.GraphicalEffects", 1, 0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qmlRegisterModule("QtGraphicalEffects", 1, 0);
#endif
}
