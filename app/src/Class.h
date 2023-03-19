/*
 * Class.h
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

#ifndef CLASS_H
#define CLASS_H

#include <QObject>
#include <QMap>

/*!
 * \brief The Class class encapsulates a class in grading configuration.
 *
 * \since Open-Typer 5.1.0
 */
class Class : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
		Q_PROPERTY(int targetHitsPerMinute READ targetHitsPerMinute WRITE setTargetHitsPerMinute NOTIFY targetHitsPerMinuteChanged)
		Q_PROPERTY(QMap<int, int> gradeConfig READ gradeConfig WRITE setGradeConfig NOTIFY gradeConfigChanged)

	public:
		Class(QObject *parent = nullptr);

		const QString &name(void) const;
		void setName(const QString &newName);

		const QString &description(void) const;
		void setDescription(const QString &newDescription);

		int targetHitsPerMinute(void) const;
		void setTargetHitsPerMinute(int newTargetHitsPerMinute);

		const QMap<int, int> &gradeConfig(void) const;
		void setGradeConfig(const QMap<int, int> &newGradeConfig);

		Q_INVOKABLE int targetHitsForMonth(int month);
		Q_INVOKABLE void setTargetHitsForMonth(int month, int targetHits);

	private:
		QString m_name;
		QString m_description;
		int m_targetHitsPerMinute;
		QMap<int, int> m_gradeConfig;

	signals:
		void modified();
		void nameChanged();
		void gradeConfigChanged();
		void descriptionChanged();
		void targetHitsPerMinuteChanged();
};

#endif // CLASS_H
