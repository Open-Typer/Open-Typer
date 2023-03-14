/*
 * ClassManager.h
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

#ifndef CLASSMANAGER_H
#define CLASSMANAGER_H

#include <QObject>
#include <QQmlListProperty>
#include <QJsonDocument>
#include "Class.h"

/*!
 * \brief The ClassManager class provides access to grading configuration of each class.
 *
 * \since Open-Typer 5.1.0
 */
class ClassManager : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QQmlListProperty<Class> classes READ classes NOTIFY classesChanged)
	public:
		ClassManager(QObject *parent = nullptr);

		QQmlListProperty<Class> classes(void);
		void setClasses(QList<Class *> newClasses);

		Q_INVOKABLE void createNewClass(void);
		Q_INVOKABLE void removeClass(Class *classPtr);

	signals:
		void classesChanged();

	private:
		void write(void);
		QList<Class *> m_classes;
		QJsonDocument doc;

		static const QString nameProperty;
		static const QString descriptionProperty;
		static const QString gradingProperty;
		static const QString targetHitsProperty;
};

extern ClassManager globalClassManager;

#endif // CLASSMANAGER_H
