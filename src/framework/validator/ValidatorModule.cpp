/*
 * ValidatorModule.cpp
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
#include "ValidatorModule.h"
#include "ExerciseValidator.h"
#include "global/ISettings.h"

std::string ValidatorModule::moduleName() const
{
	return "validator";
}

void ValidatorModule::initSettings()
{
	INIT_SETTINGS_KEY("mistakeLimit", "main/mistakelimit", true);
	INIT_SETTINGS_KEY("mistakeChars", "main/mistakechars", 6);
}

void ValidatorModule::registerUiTypes()
{
	qmlRegisterType<ExerciseValidator>("OpenTyper.Validator", 1, 0, "ExerciseValidator");
	qRegisterMetaType<CharacterRecord>();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qmlRegisterUncreatableMetaObject(MistakeRecord::staticMetaObject, "OpenTyper.Validator", 1, 0, "MistakeRecord", "Please use QmlUtils.createMistakeRecord()");
#else
	qmlRegisterUncreatableType<MistakeRecord>("OpenTyper.Validator", 1, 0, "MistakeRecord", "Please use QmlUtils.createMistakeRecord()");
#endif
	qRegisterMetaType<QList<MistakeRecord>>();
}
