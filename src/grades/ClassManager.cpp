/*
 * ClassManager.cpp
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

#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include "ClassManager.h"
#include "global/internal/Settings.h"

static const QString module = "grades";
static const ISettings::Key TARGET_HPM(module, "targetHitsPerMinute");
static const ISettings::Key SELECTED_CLASS(module, "selectedClass");

const QString ClassManager::nameProperty = "name";
const QString ClassManager::descriptionProperty = "description";
const QString ClassManager::gradingProperty = "grading";
const QString ClassManager::targetHitsProperty = "targetHitsPerMinute";

ClassManager globalClassManager;

/*! Constructs ClassManager. */
ClassManager::ClassManager(QObject *parent) :
	QObject(parent)
{
}

/*! Initializes the ClassManager instance. */
void ClassManager::init()
{
	// Open class configuration
	QFile file(fileUtils()->classConfigLocation());
	if(!file.exists())
	{
		bool ret = file.open(QFile::WriteOnly | QFile::Text);
		Q_ASSERT(ret);
		if(!ret)
			return;
		file.close();
	}
	bool ret = file.open(QFile::ReadOnly | QFile::Text);
	Q_ASSERT(ret);
	if(!ret)
		return;
	doc = QJsonDocument::fromJson(file.readAll());

	// Create list of classes
	QJsonArray arr = doc.array();
	for(int i = 0; i < arr.count(); i++)
	{
		Q_ASSERT(arr[i].isObject());
		QJsonObject obj = arr[i].toObject();
		Class *currentClass = new Class(this);
		currentClass->setName(obj[nameProperty].toString());
		currentClass->setDescription(obj[descriptionProperty].toString());
		currentClass->setTargetHitsPerMinute(obj[targetHitsProperty].toInt());

		Q_ASSERT(obj[gradingProperty].isObject());
		QJsonObject gradingConfig = obj[gradingProperty].toObject();
		QStringList keys = gradingConfig.keys();
		for(int j = 0; j < keys.length(); j++)
		{
			QString key = keys[j];
			currentClass->setTargetHitsForMonth(key.toInt(), gradingConfig[key].toInt());
		}

		connect(currentClass, &Class::modified, this, &ClassManager::write);
		m_classes.append(currentClass);
	}
	connect(this, &ClassManager::classesChanged, this, &ClassManager::classNamesChanged);
	emit classesChanged();
}

/*! List of classes. */
QQmlListProperty<Class> ClassManager::classes(void)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	return QQmlListProperty<Class>(this, &m_classes);
#else
	return QQmlListProperty<Class>(this, m_classes);
#endif
}

void ClassManager::setClasses(QList<Class *> newClasses)
{
	if(m_classes == newClasses)
		return;
	m_classes = newClasses;
	emit classesChanged();
}

/*! Creates a new class and adds it to the list of classes. */
void ClassManager::createNewClass(void)
{
	Class *newClass = new Class(this);
	newClass->setName(tr("Unnamed class"));
	newClass->setTargetHitsPerMinute(settings()->getValue(TARGET_HPM).toInt());
	connect(newClass, &Class::modified, this, &ClassManager::write);
	m_classes.append(newClass);
	write();
	emit classesChanged();
}

/*! Removes the given class. */
void ClassManager::removeClass(Class *classPtr)
{
	m_classes.removeAll(classPtr);
	write();
	emit classesChanged();
}

/*! Returns target hits per minute based on selected class and current month. */
int ClassManager::targetHitsPerMinute(void)
{
	return targetHitsPerMinute(settings()->getValue(SELECTED_CLASS).toInt());
}

/*! Returns target hits per minute based on the given class and current month. */
int ClassManager::targetHitsPerMinute(int selectedClass)
{
	if(selectedClass == -1)
		return settings()->getValue(TARGET_HPM).toInt();
	else
	{
		Class *classPtr = globalClassManager.m_classes[selectedClass];
		int hitsForMonth = classPtr->targetHitsForMonth(QDate::currentDate().month());
		if(hitsForMonth == 0)
			return classPtr->targetHitsPerMinute();
		else
			return hitsForMonth;
	}
}

/*! Writes all classes into the configuration file. */
void ClassManager::write(void)
{
	QJsonArray arr;
	for(int i = 0; i < m_classes.length(); i++)
	{
		Class *currentClass = m_classes[i];
		QJsonObject obj;
		obj.insert(nameProperty, currentClass->name());
		obj.insert(descriptionProperty, currentClass->description());
		obj.insert(targetHitsProperty, currentClass->targetHitsPerMinute());

		QMap<int, int> gradeConfigMap = currentClass->gradeConfig();
		QJsonObject gradingConfig;
		QList<int> keys = gradeConfigMap.keys();
		for(int j = 0; j < keys.length(); j++)
			gradingConfig.insert(QString::number(keys[j]), gradeConfigMap[keys[j]]);
		obj.insert(gradingProperty, gradingConfig);

		arr.append(obj);
	}
	doc.setArray(arr);

	QFile file(fileUtils()->classConfigLocation());
	bool ret = file.open(QFile::WriteOnly | QFile::Text);
	Q_ASSERT(ret);
	if(ret)
		file.write(doc.toJson(QJsonDocument::Compact));
}

/*! List of class names. */
QStringList ClassManager::classNames(void)
{
	QStringList out;
	for(int i = 0; i < m_classes.length(); i++)
		out.append(m_classes[i]->name());
	return out;
}
