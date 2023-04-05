/*
 * main.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2023 - adazem009
 * Copyright (C) 2022-2023 - Roker2
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

#include "App.h"
#include "AppModule.h"
#include "keyboard/KeyboardModule.h"
#include "lessonpack/LessonPackModule.h"
#include "translations/TranslationsModule.h"
#include "ui/UiModule.h"
#include "uicomponents/UiComponentsModule.h"
#include "utils/UtilsModule.h"
#include "validator/ValidatorModule.h"
#ifndef Q_OS_WASM
#include "addons/AddonsModule.h"
#endif
#include "export/ExportModule.h"

int main(int argc, char *argv[])
{
	App app;
	// Framework
	app.addModule(new KeyboardModule);
	app.addModule(new LessonPackModule);
	app.addModule(new TranslationsModule);
	app.addModule(new UiModule);
	app.addModule(new UiComponentsModule);
	app.addModule(new UtilsModule);
	app.addModule(new ValidatorModule);
	// Application modules
	app.addModule(new AppModule);
#ifndef Q_OS_WASM
	app.addModule(new AddonsModule);
#endif
	app.addModule(new ExportModule);
	return app.run(argc, argv);
}
