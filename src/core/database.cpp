/*
 * database.cpp
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

#include "core/database.h"

databaseManager dbMgr;

/*! Constructs databaseManager. */
databaseManager::databaseManager(QObject *parent) :
	QObject(parent),
	openStatus(false) { }

/*! Opens the database. */
bool databaseManager::open(void)
{
#ifndef Q_OS_WASM
	QSqlDatabase db;
	if(QSqlDatabase::connectionNames().count() == 0)
		db = QSqlDatabase::addDatabase("QSQLITE");
	else
		db = QSqlDatabase::database();
	db.setDatabaseName(fileUtils::configLocation() + "/database.db");
	openStatus = db.open();
	if(openStatus)
	{
		QStringList tables = db.tables();
		if(!(
			(tables.contains("user")) &&
			(tables.contains("class")) &&
			(tables.contains("exercise_result"))
		))
		{
			QSqlQuery query;
			QStringList tables = db.tables();
			for(int i=0; i < tables.count(); i++)
				query.exec(QString("DROP TABLE %1").arg(tables[i]));
			// user
			query.exec(QString("CREATE TABLE user (") +
				QString("id INTEGER PRIMARY KEY NOT NULL,") +
				QString("name VARCHAR(64) NOT NULL,") +
				QString("nickname VARCHAR(128) NOT NULL UNIQUE,") +
				QString("password VARCHAR(64) NOT NULL,") +
				QString("role INT NOT NULL)"));
			// class
			query.exec(QString("CREATE TABLE class (") +
				QString("id INTEGER PRIMARY KEY NOT NULL,") +
				QString("name VARCHAR(20) NOT NULL,") +
				QString("icon INT NOT NULL,") +
				QString("used_time TIMESTAMP NOT NULL)"));
			// users
			query.exec(QString("CREATE TABLE users (") +
				QString("user INT NOT NULL,") +
				QString("class INT NOT NULL,") +
				QString("PRIMARY KEY (user, class),") +
				QString("FOREIGN KEY (user) REFERENCES user(id) ON DELETE CASCADE,") +
				QString("FOREIGN KEY (class) REFERENCES class(id) ON DELETE CASCADE)"));
			// exercise_result
			query.exec(QString("CREATE TABLE exercise_result (") +
				QString("id INTEGER PRIMARY KEY NOT NULL,") +
				QString("user INT NOT NULL,") +
				QString("class INT NOT NULL,") +
				QString("upload_time TIMESTAMP NOT NULL,") +
				QString("pack VARCHAR(64) NOT NULL,") +
				QString("lesson INT NOT NULL,") +
				QString("sublesson INT NOT NULL,") +
				QString("exercise INT NOT NULL,") +
				QString("speed INT NOT NULL,") +
				QString("mistakes INT NOT NULL,") +
				QString("duration INT NOT NULL,") +
				QString("FOREIGN KEY (user) REFERENCES user(id) ON DELETE CASCADE,") +
				QString("FOREIGN KEY (class) REFERENCES class(id) ON DELETE CASCADE)"));
		}
		QSqlQuery query;
		query.exec("PRAGMA foreign_keys=on"); // required for foreign keys
	}
	else
		QMessageBox::critical(nullptr, QString(), tr("Couldn't open the database."));
#else
	openStatus = false;
#endif
	return openStatus;
}

/*! Returns true if the database has been successfully opened. */
bool databaseManager::status(void)
{
	return openStatus;
}

/*! Returns list of translated user role names. */
QStringList databaseManager::roles(bool includeStudentRole)
{
	// All new roles must be added to the Role enum!
	QStringList out;
	out += tr("Teacher","Teacher user role");
	out += tr("Administrator","Administrator user role");
	if(includeStudentRole)
		out += tr("Student","Student user role");
	return out;
}

/*! Returns the name of the user. */
QString databaseManager::userName(int userID)
{
#ifdef Q_OS_WASM
	return QString();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM user WHERE id = " + QString::number(userID));
	query.next();
	return query.value(0).toString();
#endif
}

/*! Returns the nickname of the user. */
QString databaseManager::userNickname(int userID)
{
#ifdef Q_OS_WASM
	return QString();
#else
	QSqlQuery query;
	query.exec("SELECT nickname FROM user WHERE id = " + QString::number(userID));
	query.next();
	return query.value(0).toString();
#endif
}

/*! Returns the role of the user. */
databaseManager::Role databaseManager::userRole(int userID)
{
#ifdef Q_OS_WASM
	return Role_Invalid;
#else
	QSqlQuery query;
	query.exec("SELECT role FROM user WHERE id = " + QString::number(userID));
	query.next();
	return (Role) query.value(0).toInt();
#endif
}

/*! Returns list of users. */
QStringList databaseManager::userNames(void)
{
#ifdef Q_OS_WASM
	return QStringList();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM user");
	QStringList out;
	while(query.next())
		out += query.value(0).toString();
	return out;
#endif
}

/*! Returns list of teachers or administrators. */
QStringList databaseManager::teacherNames(void)
{
#ifdef Q_OS_WASM
	return QStringList();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM user WHERE role = " + QString::number(Role_Teacher) + " OR role = " + QString::number(Role_Administrator));
	QStringList out;
	while(query.next())
		out += query.value(0).toString();
	return out;
#endif
}

/*! Returns list of administrators. */
QStringList databaseManager::administratorNames(void)
{
#ifdef Q_OS_WASM
	return QStringList();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM user WHERE role = " + QString::number(Role_Administrator));
	QStringList out;
	while(query.next())
		out += query.value(0).toString();
	return out;
#endif
}

/*! Returns list of user IDs. */
QList<int> databaseManager::userIDs(void)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec("SELECT id FROM user");
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Returns list of student IDs in the given class. */
QList<int> databaseManager::studentIDs(int classID)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT id FROM user INNER JOIN users ON users.user = user.id AND users.class = %1 AND user.role = %2").arg(QString::number(classID), QString::number(Role_Student)));
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Returns list of teacher or administrator IDs. */
QList<int> databaseManager::teacherIDs(void)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec("SELECT id FROM user WHERE role = " + QString::number(Role_Teacher) + " OR role = " + QString::number(Role_Administrator));
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Returns list of administrator IDs. */
QList<int> databaseManager::administratorIDs(void)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec("SELECT id FROM user WHERE role = " + QString::number(Role_Administrator));
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Returns the ID of currently logged in user. */
int databaseManager::loginID(void)
{
	return userLoginID;
}

/*! Opens an authentication dialog logs in as the given user. */
bool databaseManager::auth(int userID, QString password)
{
#ifdef Q_OS_WASM
	return false;
#else
	bool silent = true;
	if(password == "")
	{
		silent = false;
		QInputDialog dialog;
		dialog.setLabelText(tr("Password for user %1:").arg(userName(userID)));
		dialog.setInputMode(QInputDialog::TextInput);
		dialog.setTextEchoMode(QLineEdit::Password);
		if(dialog.exec() == QDialog::Accepted)
		{
			password = dialog.textValue();
			goto validate;
		}
	}
	else
		goto validate;
	return false;
	validate:
		QCryptographicHash hash(QCryptographicHash::Sha256);
		hash.addData(password.toUtf8());
		QSqlQuery query;
		query.exec("SELECT password FROM user WHERE id = " + QString::number(userID));
		QStringList out;
		query.next();
		if(query.value(0).toString().toUtf8() == hash.result().toHex())
		{
			userLoginID = userID;
			return true;
		}
		else if(!silent)
			QMessageBox::warning(nullptr, QString(), tr("Incorrect password!"));
		return false;
#endif
}

/*! Adds a user. */
void databaseManager::addUser(QString name, databaseManager::Role role, QString password, QString nickname, int classID)
{
#ifndef Q_OS_WASM
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(password.toUtf8());
	QString passwordStr = QString(hash.result().toHex());
	QSqlQuery query;
	if((role == Role_Teacher) || (role == Role_Administrator))
	{
		query.exec(QString("INSERT INTO user (name, nickname, password, role) VALUES (%1, '', %2, %3)").arg(quotesEnclosed(name), quotesEnclosed(passwordStr), QString::number(role)));
		query.exec("UPDATE user SET nickname = 'teacher_' || (SELECT id FROM user WHERE nickname = '') WHERE nickname = ''");
	}
	else
	{
		query.exec(QString("INSERT INTO user (name, nickname, password, role) VALUES (%1, '', %2, %3)").arg(quotesEnclosed(name), quotesEnclosed(passwordStr), QString::number(role)));
		query.exec(QString("INSERT INTO users (user, class) VALUES ((SELECT id FROM user WHERE nickname = ''), %1)").arg(QString::number(classID)));
		query.exec(QString("UPDATE user SET nickname = (SELECT id FROM user WHERE nickname = '') || %1 WHERE nickname = ''").arg(quotesEnclosed(nickname)));
	}
#endif
}

/*! Adds a student to the given class. */
void databaseManager::addStudentToClass(int userID, int classID)
{
#ifndef Q_OS_WASM
	QSqlQuery query;
	if(userRole(userID) == Role_Student)
		query.exec(QString("INSERT INTO users (user, class) VALUES (%1, %2)").arg(QString::number(userID, classID)));
#endif
}

/*! Edits a user. */
void databaseManager::editUser(int userID, QString name, databaseManager::Role role, QString password, QString nickname)
{
#ifndef Q_OS_WASM
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(password.toUtf8());
	QString passwordStr = QString(hash.result().toHex());
	QSqlQuery query;
	if((role == Role_Teacher) || (role == Role_Administrator))
	{
		if(password == "")
			query.exec(QString("UPDATE user SET name = %1, role = %2 WHERE id = %3").arg(quotesEnclosed(name), QString::number(role), QString::number(userID)));
		else
			query.exec(QString("UPDATE user SET name = %1, password = %2, role = %3 WHERE id = %4").arg(quotesEnclosed(name), quotesEnclosed(passwordStr), QString::number(role), QString::number(userID)));
	}
	else
	{
		if(password == "")
			query.exec(QString("UPDATE user SET name = %1, nickname = %2, role = %3 WHERE id = %4").arg(quotesEnclosed(name), quotesEnclosed(nickname), QString::number(role), QString::number(userID)));
		else
			query.exec(QString("UPDATE user SET name = %1, nickname = %2, password = %3, role = %4 WHERE id = %5").arg(quotesEnclosed(name), quotesEnclosed(nickname), quotesEnclosed(passwordStr), QString::number(role), QString::number(userID)));
	}
#endif
}

/*! Removes a user. */
void databaseManager::removeUser(int userID)
{
#ifndef Q_OS_WASM
	QSqlQuery query;
	if(userRole(userID) != Role_Student)
		query.exec(QString("DELETE FROM class WHERE id IN (SELECT id FROM class INNER JOIN users ON users.user = %1 AND users.class = class.id)").arg(QString::number(userID)));
	query.exec("DELETE FROM user WHERE id = " + QString::number(userID));
#endif
}

/*! Returns the name of the class. */
QString databaseManager::className(int classID)
{
#ifdef Q_OS_WASM
	return QString();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM class WHERE id = " + QString::number(classID));
	query.next();
	return query.value(0).toString();
#endif
}

/*! Returns the icon of the class. */
int databaseManager::classIcon(int classID)
{
#ifdef Q_OS_WASM
	return 0;
#else
	QSqlQuery query;
	query.exec("SELECT icon FROM class WHERE id = " + QString::number(classID));
	query.next();
	return query.value(0).toInt();
#endif
}

/*! Returns the timestamp of the class. */
unsigned long long databaseManager::classTimestamp(int classID)
{
#ifdef Q_OS_WASM
	return 0;
#else
	QSqlQuery query;
	query.exec("SELECT strftime(\"%s\", used_time) FROM class WHERE id = " + QString::number(classID));
	query.next();
	return query.value(0).toULongLong();
#endif
}

/*! Returns list of classes. */
QStringList databaseManager::classNames(bool sort)
{
#ifdef Q_OS_WASM
	return QStringList();
#else
	QSqlQuery query;
	if(sort)
		query.exec("SELECT name FROM class ORDER BY used_time DESC");
	else
		query.exec("SELECT name FROM class");
	QStringList out;
	while(query.next())
		out += query.value(0).toString();
	return out;
#endif
}

/*! Returns list of class IDs. */
QList<int> databaseManager::classIDs(bool sort)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	if(sort)
		query.exec("SELECT id FROM class ORDER BY used_time DESC");
	else
		query.exec("SELECT id FROM class");
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Adds a class. */
void databaseManager::addClass(QString name, int owner, int icon)
{
#ifndef Q_OS_WASM
	QSqlQuery query;
	query.exec(QString("INSERT INTO class (name, icon, used_time) VALUES ('', %1, CURRENT_TIMESTAMP)").arg(QString::number(icon)));
	query.exec(QString("INSERT INTO users (user, class) VALUES (%1, (SELECT id FROM class WHERE name = ''))").arg(QString::number(owner)));
	query.exec(QString("UPDATE class SET name = %1 WHERE name = ''").arg(quotesEnclosed(name)));
	
#endif
}

/*! Edits a class. */
void databaseManager::editClass(int classID, QString name, int owner, int icon)
{
#ifndef Q_OS_WASM
	QSqlQuery query;
	query.exec(QString("UPDATE class SET name = %1, icon = %2 WHERE id = %3").arg(quotesEnclosed(name), QString::number(icon), QString::number(classID)));
	query.exec(QString("UPDATE users SET user = %1 WHERE user = (SELECT user FROM users INNER JOIN class INNER JOIN user ON class.id = %2 AND users.class = %2 AND user.id = users.user AND (user.role = 0 OR user.role = 1))").arg(QString::number(owner), QString::number(classID)));
#endif
}

/*! Updates the timestamp of the class. */
void databaseManager::updateClassTimestamp(int classID)
{
#ifndef Q_OS_WASM
	QSqlQuery query;
	query.exec("UPDATE class SET used_time = CURRENT_TIMESTAMP WHERE id = " + QString::number(classID));
#endif
}

/*! Removes a class. */
void databaseManager::removeClass(int classID)
{
#ifndef Q_OS_WASM
	QSqlQuery query;
	query.exec(QString("DELETE FROM class WHERE id = %1").arg(QString::number(classID)));
#endif
}

/*! Returns list of all packs in recorded exercise results of the given class. */
QStringList databaseManager::recordedPacks(int classID)
{
#ifdef Q_OS_WASM
	return QStringList();
#else
	QSqlQuery query;
	query.exec("SELECT DISTINCT pack FROM exercise_result WHERE class = " + QString::number(classID));
	QStringList out;
	while(query.next())
		out += query.value(0).toString();
	return out;
#endif
}

/*! Returns list of all packs in recorded exercise results of the given student. */
QStringList databaseManager::studentPacks(int classID, int userID)
{
#ifdef Q_OS_WASM
	return QStringList();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT pack FROM exercise_result WHERE class = %1 AND user = %2").arg(QString::number(classID), QString::number(userID)));
	QStringList out;
	while(query.next())
		out += query.value(0).toString();
	return out;
#endif
}

/*! Returns list of all recorded lessons in the given pack. */
QList<int> databaseManager::recordedLessons(int classID, QString pack)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT lesson FROM exercise_result WHERE class = %1 AND pack = %2").arg(QString::number(classID), quotesEnclosed(pack)));
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Returns list of all lessons in the pack of the given student. */
QList<int> databaseManager::studentLessons(int classID, int userID, QString pack)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT lesson FROM exercise_result WHERE class = %1 AND user = %2 AND pack = %3").arg(QString::number(classID), QString::number(userID), quotesEnclosed(pack)));
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Returns list of all recorded sublessons in the given lesson. */
QList<int> databaseManager::recordedSublessons(int classID, QString pack, int lesson)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT sublesson FROM exercise_result WHERE class = %1 AND pack = %2 AND lesson = %3").arg(QString::number(classID), quotesEnclosed(pack), QString::number(lesson)));
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Returns list of all sublessons in the lesson of the given student. */
QList<int> databaseManager::studentSublessons(int classID, int userID, QString pack, int lesson)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT sublesson FROM exercise_result WHERE class = %1 AND user = %2 AND pack = %3 AND lesson = %4").arg(QString::number(classID), QString::number(userID), quotesEnclosed(pack), QString::number(lesson)));
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Returns list of all recorded exercises in the given sublesson. */
QList<int> databaseManager::recordedExercises(int classID, QString pack, int lesson, int sublesson)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT exercise FROM exercise_result WHERE class = %1 AND pack = %2 AND lesson = %3 AND sublesson = %4").arg(QString::number(classID), quotesEnclosed(pack), QString::number(lesson), QString::number(sublesson)));
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Returns list of all exercises in the sublesson of the given student. */
QList<int> databaseManager::studentExercises(int classID, int userID, QString pack, int lesson, int sublesson)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT exercise FROM exercise_result WHERE class = %1 AND user = %2 AND pack = %3 AND lesson = %4 AND sublesson = %5").arg(QString::number(classID), QString::number(userID), quotesEnclosed(pack), QString::number(lesson), QString::number(sublesson)));
	QList<int> out;
	while(query.next())
		out += query.value(0).toInt();
	return out;
#endif
}

/*! Returns list of all entries in the exercise of the given student. */
QList<QVariantMap> databaseManager::historyEntries(int classID, int userID, QString pack, int lesson, int sublesson, int exercise)
{
#ifdef Q_OS_WASM
	return QList<QVariantMap>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT * FROM exercise_result WHERE class = %1 AND user = %2 AND pack = %3 AND lesson = %4 AND sublesson = %5 AND exercise = %6 ORDER BY upload_time ASC").arg(QString::number(classID), QString::number(userID), quotesEnclosed(pack), QString::number(lesson), QString::number(sublesson), QString::number(exercise)));
	QList<QVariantMap> out;
	while(query.next())
	{
		QVariantMap entry;
		entry["speed"] = query.value("speed").toInt();
		entry["mistakes"] = query.value("mistakes").toInt();
		entry["duration"] = query.value("duration").toInt();
		out += entry;
	}
	return out;
#endif
}

/*! Returns the given string enclosed by quotes compatible with SQLite (special characters are automatically escaped). */
QString databaseManager::quotesEnclosed(QString str)
{
	QString out = "\"";
	for(int i=0; i < str.count(); i++)
	{
		if(str[i] == '"')
			out += '"';
		out += str[i];
	}
	out += '"';
	return out;
}
