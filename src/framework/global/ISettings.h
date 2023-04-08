/*
 * ISettings.h
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

#ifndef ISETTINGS_H
#define ISETTINGS_H

#include <QVariant>
#include "global/modularity/ioc.h"

#define INIT_SETTINGS_KEY(keyName, key, defaultValue) \
	modularity::ioc()->resolve<ISettings>()->addKey(QString::fromStdString(moduleName()), keyName, key, defaultValue)

class ISettings : public QObject, MODULE_EXPORT_INTERFACE
{
		Q_OBJECT
	public:
		virtual ~ISettings() { }

		virtual void addKey(QString moduleName, QString keyName, QString key, QVariant defaultValue) = 0;
		virtual void setValue(QString moduleName, QString keyName, QVariant value) = 0;
		virtual QVariant getValue(QString moduleName, QString keyName) = 0;
		virtual bool containsKey(QString moduleName, QString keyName) = 0;

		virtual void freeze() = 0;
		virtual void saveChanges() = 0;
		virtual void discardChanges() = 0;
		virtual bool isFrozen() = 0;

	signals:
		void stateChanged();
		void saved();
		void discarded();
};

#endif // ISETTINGS_H
