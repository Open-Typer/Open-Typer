/*
 * database.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVariantMap>
#include <QHostAddress>

#ifndef Q_OS_WASM
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QInputDialog>
#include <QCryptographicHash>
#include "core/utils.h"
#endif // Q_OS_WASM

/*! \brief The databaseManager class manages the server database. */
class databaseManager : public QObject
{
	Q_OBJECT
	public:
		databaseManager(QObject *parent = nullptr);
		bool open(void);

		enum Role {
			Role_Invalid = -1,
			Role_Teacher = 0,
			Role_Administrator = 1,
			Role_Student = 2
		};

		bool status(void);
		static QStringList roles(bool includeStudentRole = true);
		QString userName(int userID);
		QString userNickname(int userID);
		Role userRole(int userID);
		QStringList userNames(void);
		QStringList teacherNames(void);
		QStringList administratorNames(void);
		QList<int> userIDs(void);
		int findUser(QString nickname);
		QList<int> studentIDs(int classID);
		QList<int> teacherIDs(void);
		QList<int> administratorIDs(void);
		int loginID(void);
		bool auth(int userID, QString password = "");
		void addUser(QString name, Role role, QString password, QString nickname = QString(), int classID = 0);
		void addStudentToClass(int userID, int classID);
		void editUser(int userID, QString name, Role role, QString password = "", QString nickname = QString());
		void removeUser(int userID);
		void removeStudentFromClass(int userID, int classID);
		QString className(int classID);
		int classIcon(int classID);
		unsigned long long classTimestamp(int classID);
		int classOwner(int classID);
		QStringList classNames(bool sort = false);
		QList<int> classIDs(bool sort = false);
		void addClass(QString name, int owner, int icon);
		void editClass(int classID, QString name, int owner, int icon);
		void updateClassTimestamp(int classID);
		void removeClass(int classID);
		QStringList recordedPacks(int classID);
		QStringList studentPacks(int classID, int userID);
		QList<int> recordedLessons(int classID, QString pack);
		QList<int> studentLessons(int classID, int userID, QString pack);
		QList<int> recordedSublessons(int classID, QString pack, int lesson);
		QList<int> studentSublessons(int classID, int userID, QString pack, int lesson);
		QList<int> recordedExercises(int classID, QString pack, int lesson, int sublesson);
		QList<int> studentExercises(int classID, int userID, QString pack, int lesson, int sublesson);
		QList<QVariantMap> historyEntries(int classID, int userID, QString pack, int lesson, int sublesson, int exercise);
		void addHistoryEntry(int classID, int userID, QString pack, int lesson, int sublesson, int exercise, QVariantMap record);
		int compareWithStudents(int classID, int studentID, QString pack, int lesson, int sublesson, int exercise, bool better);
		QList<int> deviceIDs(void);
		QList<QHostAddress> deviceAddresses(void);
		QHostAddress deviceAddress(int deviceID);
		QString deviceName(int deviceID);
		int findDevice(QHostAddress address);
		void addDevice(QString name, QHostAddress address);
		void editDevice(int deviceID, QString name, QHostAddress address);
		void removeDevice(int deviceID);
		int activeClass = 0;

	private:
		bool openStatus = false;
		int userLoginID = -1;
		void removeOrphanedStudents(void);
		QString quotesEnclosed(QString str);
};

extern databaseManager dbMgr;

#endif // DATABASE_H
