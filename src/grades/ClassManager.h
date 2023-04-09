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
#include "global/ISettings.h"
#include "global/IFileUtils.h"

/*!
 * \brief The ClassManager class provides access to grading configuration of each class.
 *
 * \since Open-Typer 5.1.0
 */
class Q_DECL_EXPORT ClassManager : public QObject
{
		Q_OBJECT
		INJECT(ISettings, settings)
		INJECT(IFileUtils, fileUtils)
		Q_PROPERTY(QQmlListProperty<Class> classes READ classes NOTIFY classesChanged)
		Q_PROPERTY(QStringList classNames READ classNames NOTIFY classNamesChanged)
	public:
		enum GradingMethod
		{
			GradingMethod_Numbers = 0,
			GradingMethod_Letters = 1
		};
		Q_ENUM(GradingMethod)

		ClassManager(QObject *parent = nullptr);
		void init();

		QQmlListProperty<Class> classes(void);
		void setClasses(QList<Class *> newClasses);

		QStringList classNames(void);

		Q_INVOKABLE void createNewClass(void);
		Q_INVOKABLE void removeClass(Class *classPtr);
		Q_INVOKABLE static int targetHitsPerMinute(void);
		Q_INVOKABLE static int targetHitsPerMinute(int selectedClass);

	signals:
		void classesChanged();
		void classNamesChanged();

	private:
		QString configLocation();
		void write(void);
		QList<Class *> m_classes;
		QStringList m_classNames;
		QJsonDocument doc;

		static const QString nameProperty;
		static const QString descriptionProperty;
		static const QString gradingProperty;
		static const QString targetHitsProperty;
};

extern ClassManager Q_DECL_EXPORT globalClassManager;

#endif // CLASSMANAGER_H
