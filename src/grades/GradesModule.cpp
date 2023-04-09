/*
 * GradesModule.cpp
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
#include "GradesModule.h"
#include "ClassManager.h"
#include "internal/GradeCalculator.h"
#include "global/ISettings.h"

std::string GradesModule::moduleName() const
{
	return "grades";
}

void GradesModule::registerResources()
{
	Q_INIT_RESOURCE(grades);
}

void GradesModule::initSettings()
{
	INIT_SETTINGS_KEY("targetHitsPerMinute", "grading/targethits", 130);
	INIT_SETTINGS_KEY("gradeNetHits", "grading/gradenethits", true);
	INIT_SETTINGS_KEY("selectedClass", "grading/class", -1);
	INIT_SETTINGS_KEY("gradingMethod", "grading/gradingmethod", ClassManager::GradingMethod_Numbers);
	INIT_SETTINGS_KEY("gradeStartNumber", "grading/startnumber", 5);
	INIT_SETTINGS_KEY("gradeEndNumber", "grading/endnumber", 1);
	INIT_SETTINGS_KEY("gradeStartLetter", "grading/startletter", 'F');
	INIT_SETTINGS_KEY("gradeEndLetter", "grading/endletter", 'A');
}

void GradesModule::registerUiTypes()
{
	QQmlEngine::setObjectOwnership(&globalClassManager, QQmlEngine::CppOwnership);
	qmlRegisterSingletonType<ClassManager>("OpenTyper.Grades", 1, 0, "ClassManager", [](QQmlEngine *, QJSEngine *) -> QObject * {
		return &globalClassManager;
	});
	qmlRegisterType<Class>("OpenTyper.Grades", 1, 0, "Class");
	qmlRegisterType<GradeCalculator>("OpenTyper.Grades", 1, 0, "GradeCalculator");
	qRegisterMetaType<ClassManager::GradingMethod>();
	qRegisterMetaType<QMap<int, int>>();
}

void GradesModule::onPreInit()
{
	globalClassManager.init();
}
