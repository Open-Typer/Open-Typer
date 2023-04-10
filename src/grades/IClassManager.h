/*
 * IClassManager.h
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

#ifndef ICLASSMANAGER_H
#define ICLASSMANAGER_H

#include <QObject>
#include <QQmlListProperty>
#include "global/modularity/ioc.h"
#include "internal/Class.h"

class IClassManager : public QObject, MODULE_EXPORT_INTERFACE
{
		Q_OBJECT
	public:
		virtual ~IClassManager() { }

		enum GradingMethod
		{
			GradingMethod_Numbers = 0,
			GradingMethod_Letters = 1
		};
		Q_ENUM(GradingMethod)

		virtual QQmlListProperty<Class> classes(void) = 0;
		virtual void setClasses(QList<Class *> newClasses) = 0;

		virtual QStringList classNames(void) = 0;

		virtual void createNewClass(void) = 0;
		virtual void removeClass(Class *classPtr) = 0;
		virtual int targetHitsPerMinute(void) = 0;
		virtual int targetHitsPerMinute(int selectedClass) = 0;

	signals:
		void classesChanged();
		void classNamesChanged();
};

#endif // ICLASSMANAGER_H
