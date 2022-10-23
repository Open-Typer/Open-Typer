/*
 * DatabaseManager.cpp
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

#include "DatabaseManager.h"

DatabaseManager dbMgr;

/*! Constructs DatabaseManager. */
DatabaseManager::DatabaseManager(QObject *parent) :
	QObject(parent),
	openStatus(false) { }

/*! Opens the database. */
bool DatabaseManager::open(void)
{
#ifndef Q_OS_WASM
	QSqlDatabase db;
	if(QSqlDatabase::connectionNames().count() == 0)
		db = QSqlDatabase::addDatabase("QSQLITE");
	else
		db = QSqlDatabase::database();
	db.setDatabaseName(FileUtils::configLocation() + "/database.db");
	openStatus = db.open();
	if(openStatus)
	{
		QStringList tables = db.tables();
		if(!(
			   (tables.contains("user")) && (tables.contains("users")) && (tables.contains("class")) && (tables.contains("exercise_result")) && (tables.contains("device"))))
		{
			QSqlQuery query;
			QStringList tables = db.tables();
			for(int i = 0; i < tables.count(); i++)
				query.exec(QString("DROP TABLE %1").arg(tables[i]));
			// user
			query.exec(QString("CREATE TABLE user (") + QString("id INTEGER PRIMARY KEY NOT NULL,") + QString("name VARCHAR(64) NOT NULL,") + QString("nickname VARCHAR(128) NOT NULL UNIQUE,") + QString("password VARCHAR(64) NOT NULL,") + QString("role INT NOT NULL)"));
			// class
			query.exec(QString("CREATE TABLE class (") + QString("id INTEGER PRIMARY KEY NOT NULL,") + QString("name VARCHAR(20) NOT NULL,") + QString("icon INT NOT NULL,") + QString("used_time TIMESTAMP NOT NULL)"));
			// users
			query.exec(QString("CREATE TABLE users (") + QString("user INT NOT NULL,") + QString("class INT NOT NULL,") + QString("PRIMARY KEY (user, class),") + QString("FOREIGN KEY (user) REFERENCES user(id) ON DELETE CASCADE,") + QString("FOREIGN KEY (class) REFERENCES class(id) ON DELETE CASCADE)"));
			// exercise_result
			query.exec(QString("CREATE TABLE exercise_result (") + QString("id INTEGER PRIMARY KEY NOT NULL,") + QString("user INT NOT NULL,") + QString("class INT NOT NULL,") + QString("upload_time TIMESTAMP NOT NULL,") + QString("pack VARCHAR(64) NOT NULL,") + QString("lesson INT NOT NULL,") + QString("sublesson INT NOT NULL,") + QString("exercise INT NOT NULL,") + QString("speed INT NOT NULL,") + QString("mistakes INT NOT NULL,") + QString("duration INT NOT NULL,") + QString("FOREIGN KEY (user) REFERENCES user(id) ON DELETE CASCADE,") + QString("FOREIGN KEY (class) REFERENCES class(id) ON DELETE CASCADE)"));
			// device
			query.exec(QString("CREATE TABLE device (") + QString("id INTEGER PRIMARY KEY NOT NULL,") + QString("ip_address VARCHAR(15) NOT NULL UNIQUE,") + QString("name VARCHAR(20) NOT NULL)"));
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

/*! Returns true if the DatabaseManager.has been successfully opened. */
bool DatabaseManager::status(void)
{
	return openStatus;
}

/*! Returns list of translated user role names. */
QStringList DatabaseManager::roles(bool includeStudentRole)
{
	// All new roles must be added to the Role enum!
	QStringList out;
	out += tr("Teacher", "Teacher user role");
	out += tr("Administrator", "Administrator user role");
	if(includeStudentRole)
		out += tr("Student", "Student user role");
	return out;
}

/*! Returns the name of the user. */
QString DatabaseManager::userName(int userID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(userID);
	return QString();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM user WHERE id = " + QString::number(userID));
	query.next();
	Q_ASSERT(query.value(0).isValid());
	return query.value(0).toString();
#endif
}

/*! Returns the nickname of the user. */
QString DatabaseManager::userNickname(int userID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(userID);
	return QString();
#else
	QSqlQuery query;
	query.exec("SELECT nickname FROM user WHERE id = " + QString::number(userID));
	query.next();
	Q_ASSERT(query.value(0).isValid());
	return query.value(0).toString();
#endif
}

/*! Returns the role of the user. */
DatabaseManager::Role DatabaseManager::userRole(int userID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(userID);
	return Role_Invalid;
#else
	QSqlQuery query;
	query.exec("SELECT role FROM user WHERE id = " + QString::number(userID));
	query.next();
	Q_ASSERT(query.value(0).isValid());
	return (Role) query.value(0).toInt();
#endif
}

/*! Returns list of users. */
QStringList DatabaseManager::userNames(void)
{
#ifdef Q_OS_WASM
	return QStringList();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM user");
	QStringList out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toString();
	}
	return out;
#endif
}

/*! Returns list of teachers or administrators. */
QStringList DatabaseManager::teacherNames(void)
{
#ifdef Q_OS_WASM
	return QStringList();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM user WHERE role = " + QString::number(Role_Teacher) + " OR role = " + QString::number(Role_Administrator));
	QStringList out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toString();
	}
	return out;
#endif
}

/*! Returns list of administrators. */
QStringList DatabaseManager::administratorNames(void)
{
#ifdef Q_OS_WASM
	return QStringList();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM user WHERE role = " + QString::number(Role_Administrator));
	QStringList out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toString();
	}
	return out;
#endif
}

/*! Returns list of user IDs. */
QList<int> DatabaseManager::userIDs(void)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec("SELECT id FROM user");
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Finds user by nickname. */
int DatabaseManager::findUser(QString nickname)
{
#ifdef Q_OS_WASM
	Q_UNUSED(nickname);
	return 0;
#else
	QSqlQuery query;
	query.exec("SELECT id FROM user WHERE nickname = " + quotesEnclosed(nickname));
	if(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		return query.value(0).toInt();
	}
	else
		return 0;
#endif
}

/*! Returns list of student IDs in the given class. */
QList<int> DatabaseManager::studentIDs(int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT id FROM user INNER JOIN users ON users.user = user.id AND users.class = %1 AND user.role = %2").arg(QString::number(classID), QString::number(Role_Student)));
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Returns list of teacher or administrator IDs. */
QList<int> DatabaseManager::teacherIDs(void)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec("SELECT id FROM user WHERE role = " + QString::number(Role_Teacher) + " OR role = " + QString::number(Role_Administrator));
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Returns list of administrator IDs. */
QList<int> DatabaseManager::administratorIDs(void)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec("SELECT id FROM user WHERE role = " + QString::number(Role_Administrator));
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Returns the ID of currently logged in user. */
int DatabaseManager::loginID(void)
{
	return userLoginID;
}

/*! Opens an authentication dialog logs in as the given user. */
bool DatabaseManager::auth(int userID, QString password, bool forceDialogs)
{
#ifdef Q_OS_WASM
	Q_UNUSED(userID);
	Q_UNUSED(password);
	Q_UNUSED(forceDialogs);
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
	{
		if(forceDialogs)
			silent = false;
		goto validate;
	}
	return false;
validate:
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(password.toUtf8());
	QSqlQuery query;
	query.exec("SELECT password FROM user WHERE id = " + QString::number(userID));
	QStringList out;
	query.next();
	Q_ASSERT(query.value(0).isValid());
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
void DatabaseManager::addUser(QString name, DatabaseManager::Role role, QString password, QString nickname, int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(name);
	Q_UNUSED(role);
	Q_UNUSED(password);
	Q_UNUSED(nickname);
	Q_UNUSED(classID);
#else
	bool result;
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(password.toUtf8());
	QString passwordStr = QString(hash.result().toHex());
	QSqlQuery query;
	if((role == Role_Teacher) || (role == Role_Administrator))
	{
		result = query.exec(QString("INSERT INTO user (name, nickname, password, role) VALUES (%1, '', %2, %3)").arg(quotesEnclosed(name), quotesEnclosed(passwordStr), QString::number(role)));
		Q_ASSERT(result);
		result = query.exec("UPDATE user SET nickname = 'teacher_' || (SELECT id FROM user WHERE nickname = '') WHERE nickname = ''");
		Q_ASSERT(result);
	}
	else
	{
		result = query.exec(QString("INSERT INTO user (name, nickname, password, role) VALUES (%1, '', %2, %3)").arg(quotesEnclosed(name), quotesEnclosed(passwordStr), QString::number(role)));
		Q_ASSERT(result);
		result = query.exec(QString("INSERT INTO users (user, class) VALUES ((SELECT id FROM user WHERE nickname = ''), %1)").arg(QString::number(classID)));
		Q_ASSERT(result);
		result = query.exec(QString("UPDATE user SET nickname = (SELECT id FROM user WHERE nickname = '') || %1 WHERE nickname = ''").arg(quotesEnclosed(nickname)));
		Q_ASSERT(result);
	}
#endif
}

/*! Adds a student to the given class. */
void DatabaseManager::addStudentToClass(int userID, int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(userID);
	Q_UNUSED(classID);
#else
	QSqlQuery query;
	if(userRole(userID) == Role_Student)
	{
		bool result = query.exec(QString("INSERT INTO users (user, class) VALUES (%1, %2)").arg(QString::number(userID), QString::number(classID)));
		Q_ASSERT(result);
	}
#endif
}

/*! Edits a user. */
void DatabaseManager::editUser(int userID, QString name, DatabaseManager::Role role, QString password, QString nickname)
{
#ifdef Q_OS_WASM
	Q_UNUSED(userID);
	Q_UNUSED(name);
	Q_UNUSED(role);
	Q_UNUSED(password);
	Q_UNUSED(nickname);
#else
	bool result;
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(password.toUtf8());
	QString passwordStr = QString(hash.result().toHex());
	QSqlQuery query;
	if((role == Role_Teacher) || (role == Role_Administrator))
	{
		if(password == "")
		{
			result = query.exec(QString("UPDATE user SET name = %1, role = %2 WHERE id = %3").arg(quotesEnclosed(name), QString::number(role), QString::number(userID)));
			Q_ASSERT(result);
		}
		else
		{
			result = query.exec(QString("UPDATE user SET name = %1, password = %2, role = %3 WHERE id = %4").arg(quotesEnclosed(name), quotesEnclosed(passwordStr), QString::number(role), QString::number(userID)));
			Q_ASSERT(result);
		}
	}
	else
	{
		if(password == "")
		{
			result = query.exec(QString("UPDATE user SET name = %1, nickname = %2, role = %3 WHERE id = %4").arg(quotesEnclosed(name), quotesEnclosed(nickname), QString::number(role), QString::number(userID)));
			Q_ASSERT(result);
		}
		else
		{
			result = query.exec(QString("UPDATE user SET name = %1, nickname = %2, password = %3, role = %4 WHERE id = %5").arg(quotesEnclosed(name), quotesEnclosed(nickname), quotesEnclosed(passwordStr), QString::number(role), QString::number(userID)));
			Q_ASSERT(result);
		}
	}
#endif
}

/*! Removes a user. */
void DatabaseManager::removeUser(int userID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(userID);
#else
	bool result;
	QSqlQuery query;
	if(userRole(userID) != Role_Student)
	{
		result = query.exec(QString("DELETE FROM class WHERE id IN (SELECT id FROM class INNER JOIN users ON users.user = %1 AND users.class = class.id)").arg(QString::number(userID)));
		Q_ASSERT(result);
	}
	result = query.exec("DELETE FROM user WHERE id = " + QString::number(userID));
	Q_ASSERT(result);
	removeOrphanedStudents();
#endif
}

/*! Removes the student from the given class. */
void DatabaseManager::removeStudentFromClass(int userID, int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(userID);
	Q_UNUSED(classID);
#else
	QSqlQuery query;
	if(userRole(userID) == Role_Student)
	{
		bool result = query.exec(QString("DELETE FROM users WHERE user = %1 AND class = %2").arg(QString::number(userID), QString::number(classID)));
		Q_ASSERT(result);
		removeOrphanedStudents();
	}
#endif
}

/*! Returns the name of the class. */
QString DatabaseManager::className(int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	return QString();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM class WHERE id = " + QString::number(classID));
	query.next();
	Q_ASSERT(query.value(0).isValid());
	return query.value(0).toString();
#endif
}

/*! Returns the icon of the class. */
int DatabaseManager::classIcon(int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	return 0;
#else
	QSqlQuery query;
	query.exec("SELECT icon FROM class WHERE id = " + QString::number(classID));
	query.next();
	Q_ASSERT(query.value(0).isValid());
	return query.value(0).toInt();
#endif
}

/*! Returns the timestamp of the class. */
unsigned long long DatabaseManager::classTimestamp(int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	return 0;
#else
	QSqlQuery query;
	query.exec("SELECT strftime(\"%s\", used_time) FROM class WHERE id = " + QString::number(classID));
	query.next();
	Q_ASSERT(query.value(0).isValid());
	return query.value(0).toULongLong();
#endif
}

/*! Returns the owner of the class */
int DatabaseManager::classOwner(int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	return 0;
#else
	QSqlQuery query;
	query.exec(QString("SELECT user FROM users INNER JOIN user ON user.id = users.user AND users.class = %1 AND user.role != %2").arg(QString::number(classID), QString::number(Role_Student)));
	query.next();
	Q_ASSERT(query.value(0).isValid());
	return query.value(0).toInt();
#endif
}

/*! Returns list of classes. */
QStringList DatabaseManager::classNames(bool sort)
{
#ifdef Q_OS_WASM
	Q_UNUSED(sort);
	return QStringList();
#else
	QSqlQuery query;
	if(sort)
		query.exec("SELECT name FROM class ORDER BY used_time DESC");
	else
		query.exec("SELECT name FROM class");
	QStringList out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toString();
	}
	return out;
#endif
}

/*! Returns list of class IDs. */
QList<int> DatabaseManager::classIDs(bool sort)
{
#ifdef Q_OS_WASM
	Q_UNUSED(sort);
	return QList<int>();
#else
	QSqlQuery query;
	if(sort)
		query.exec("SELECT id FROM class ORDER BY used_time DESC");
	else
		query.exec("SELECT id FROM class");
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Adds a class. */
void DatabaseManager::addClass(QString name, int owner, int icon)
{
#ifdef Q_OS_WASM
	Q_UNUSED(name);
	Q_UNUSED(owner);
	Q_UNUSED(icon);
#else
	bool result;
	QSqlQuery query;
	result = query.exec(QString("INSERT INTO class (name, icon, used_time) VALUES ('', %1, CURRENT_TIMESTAMP)").arg(QString::number(icon)));
	Q_ASSERT(result);
	result = query.exec(QString("INSERT INTO users (user, class) VALUES (%1, (SELECT id FROM class WHERE name = ''))").arg(QString::number(owner)));
	Q_ASSERT(result);
	result = query.exec(QString("UPDATE class SET name = %1 WHERE name = ''").arg(quotesEnclosed(name)));
	Q_ASSERT(result);
#endif
}

/*! Edits a class. */
void DatabaseManager::editClass(int classID, QString name, int owner, int icon)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(name);
	Q_UNUSED(owner);
	Q_UNUSED(icon);
#else
	bool result;
	QSqlQuery query;
	result = query.exec(QString("UPDATE class SET name = %1, icon = %2 WHERE id = %3").arg(quotesEnclosed(name), QString::number(icon), QString::number(classID)));
	Q_ASSERT(result);
	result = query.exec(QString("UPDATE users SET user = %1 WHERE user = (SELECT user FROM users INNER JOIN class INNER JOIN user ON class.id = %2 AND users.class = %2 AND user.id = users.user AND (user.role = 0 OR user.role = 1))").arg(QString::number(owner), QString::number(classID)));
	Q_ASSERT(result);
#endif
}

/*! Updates the timestamp of the class. */
void DatabaseManager::updateClassTimestamp(int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
#else
	QSqlQuery query;
	bool result = query.exec("UPDATE class SET used_time = CURRENT_TIMESTAMP WHERE id = " + QString::number(classID));
	Q_ASSERT(result);
#endif
}

/*! Removes a class. */
void DatabaseManager::removeClass(int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
#else
	QSqlQuery query;
	bool result = query.exec(QString("DELETE FROM class WHERE id = %1").arg(QString::number(classID)));
	Q_ASSERT(result);
	removeOrphanedStudents();
#endif
}

/*! Returns list of all packs in recorded exercise results of the given class. */
QStringList DatabaseManager::recordedPacks(int classID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	return QStringList();
#else
	QSqlQuery query;
	query.exec("SELECT DISTINCT pack FROM exercise_result WHERE class = " + QString::number(classID));
	QStringList out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toString();
	}
	return out;
#endif
}

/*! Returns list of all packs in recorded exercise results of the given student. */
QStringList DatabaseManager::studentPacks(int classID, int userID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(userID);
	return QStringList();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT pack FROM exercise_result WHERE class = %1 AND user = %2").arg(QString::number(classID), QString::number(userID)));
	QStringList out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toString();
	}
	return out;
#endif
}

/*! Returns list of all recorded lessons in the given pack. */
QList<int> DatabaseManager::recordedLessons(int classID, QString pack)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(pack);
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT lesson FROM exercise_result WHERE class = %1 AND pack = %2").arg(QString::number(classID), quotesEnclosed(pack)));
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Returns list of all lessons in the pack of the given student. */
QList<int> DatabaseManager::studentLessons(int classID, int userID, QString pack)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(userID);
	Q_UNUSED(pack);
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT lesson FROM exercise_result WHERE class = %1 AND user = %2 AND pack = %3").arg(QString::number(classID), QString::number(userID), quotesEnclosed(pack)));
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Returns list of all recorded sublessons in the given lesson. */
QList<int> DatabaseManager::recordedSublessons(int classID, QString pack, int lesson)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(pack);
	Q_UNUSED(lesson);
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT sublesson FROM exercise_result WHERE class = %1 AND pack = %2 AND lesson = %3").arg(QString::number(classID), quotesEnclosed(pack), QString::number(lesson)));
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Returns list of all sublessons in the lesson of the given student. */
QList<int> DatabaseManager::studentSublessons(int classID, int userID, QString pack, int lesson)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(userID);
	Q_UNUSED(pack);
	Q_UNUSED(lesson);
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT sublesson FROM exercise_result WHERE class = %1 AND user = %2 AND pack = %3 AND lesson = %4").arg(QString::number(classID), QString::number(userID), quotesEnclosed(pack), QString::number(lesson)));
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Returns list of all recorded exercises in the given sublesson. */
QList<int> DatabaseManager::recordedExercises(int classID, QString pack, int lesson, int sublesson)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(pack);
	Q_UNUSED(lesson);
	Q_UNUSED(sublesson);
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT exercise FROM exercise_result WHERE class = %1 AND pack = %2 AND lesson = %3 AND sublesson = %4").arg(QString::number(classID), quotesEnclosed(pack), QString::number(lesson), QString::number(sublesson)));
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Returns list of all exercises in the sublesson of the given student. */
QList<int> DatabaseManager::studentExercises(int classID, int userID, QString pack, int lesson, int sublesson)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(userID);
	Q_UNUSED(pack);
	Q_UNUSED(lesson);
	Q_UNUSED(sublesson);
	return QList<int>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT DISTINCT exercise FROM exercise_result WHERE class = %1 AND user = %2 AND pack = %3 AND lesson = %4 AND sublesson = %5").arg(QString::number(classID), QString::number(userID), quotesEnclosed(pack), QString::number(lesson), QString::number(sublesson)));
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Returns list of all entries in the exercise of the given student. */
QList<QVariantMap> DatabaseManager::historyEntries(int classID, int userID, QString pack, int lesson, int sublesson, int exercise)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(userID);
	Q_UNUSED(pack);
	Q_UNUSED(lesson);
	Q_UNUSED(sublesson);
	Q_UNUSED(exercise);
	return QList<QVariantMap>();
#else
	QSqlQuery query;
	query.exec(QString("SELECT * FROM exercise_result WHERE class = %1 AND user = %2 AND pack = %3 AND lesson = %4 AND sublesson = %5 AND exercise = %6 ORDER BY upload_time ASC").arg(QString::number(classID), QString::number(userID), quotesEnclosed(pack), QString::number(lesson), QString::number(sublesson), QString::number(exercise)));
	QList<QVariantMap> out;
	while(query.next())
	{
		QVariantMap entry;
		Q_ASSERT(query.value("speed").isValid());
		Q_ASSERT(query.value("mistakes").isValid());
		Q_ASSERT(query.value("duration").isValid());
		entry["speed"] = query.value("speed").toInt();
		entry["mistakes"] = query.value("mistakes").toInt();
		entry["duration"] = query.value("duration").toInt();
		out += entry;
	}
	return out;
#endif
}

/*! Adds an entry to the exercise history. */
void DatabaseManager::addHistoryEntry(int classID, int userID, QString pack, int lesson, int sublesson, int exercise, QVariantMap record)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(userID);
	Q_UNUSED(pack);
	Q_UNUSED(lesson);
	Q_UNUSED(sublesson);
	Q_UNUSED(exercise);
	Q_UNUSED(record);
#else
	QSqlQuery query;
	bool result = query.exec(QString("INSERT INTO exercise_result (user, class, upload_time, pack, lesson, sublesson, exercise, speed, mistakes, duration) VALUES (%1, %2, CURRENT_TIMESTAMP, %3, %4, %5, %6, %7, %8, %9)").arg(QString::number(userID), QString::number(classID), quotesEnclosed(pack), QString::number(lesson), QString::number(sublesson), QString::number(exercise), QString::number(record["speed"].toInt()), QString::number(record["mistakes"].toInt()), QString::number(record["duration"].toInt())));
	Q_ASSERT(result);
#endif
}

/*!
 * Returns number of better or worse students based on their average number of points.\n
 * The number of points for each entry is speed - mistakes*10.
 */
int DatabaseManager::compareWithStudents(int classID, int studentID, QString pack, int lesson, int sublesson, int exercise, bool better)
{
#ifdef Q_OS_WASM
	Q_UNUSED(classID);
	Q_UNUSED(studentID);
	Q_UNUSED(pack);
	Q_UNUSED(lesson);
	Q_UNUSED(sublesson);
	Q_UNUSED(exercise);
	Q_UNUSED(better);
	return 0;
#else
	int out = 0;
	QList<int> recordedStudents, students = studentIDs(classID);
	for(int i = 0; i < students.count(); i++)
	{
		QSqlQuery query;
		query.exec(QString("SELECT * FROM exercise_result WHERE user = %1 AND class = %2 AND pack = %3 AND lesson = %4 AND sublesson = %5 AND exercise = %6").arg(QString::number(students[i]), QString::number(classID), quotesEnclosed(pack), QString::number(lesson), QString::number(sublesson), QString::number(exercise)));
		int count = 0;
		while(query.next())
			count++;
		if(count > 0)
			recordedStudents += students[i];
	}
	QList<QVariantMap> entries = historyEntries(classID, studentID, pack, lesson, sublesson, exercise);
	int count = entries.count();
	QList<int> points;
	points.clear();
	int sum = 0;
	for(int i2 = 0; i2 < count; i2++)
	{
		QVariantMap entry = entries[i2];
		int pointCount = entry["speed"].toInt() - entry["mistakes"].toInt() * 10;
		points += pointCount;
		sum += pointCount;
	}
	int myAvgPoints = sum / count;
	for(int i = 0; i < recordedStudents.count(); i++)
	{
		entries = historyEntries(classID, recordedStudents[i], pack, lesson, sublesson, exercise);
		count = entries.count();
		points.clear();
		sum = 0;
		for(int i2 = 0; i2 < count; i2++)
		{
			QVariantMap entry = entries[i2];
			int pointCount = entry["speed"].toInt() - entry["mistakes"].toInt() * 10;
			points += pointCount;
			sum += pointCount;
		}
		int avgPoints = sum / count;
		if(better)
		{
			if(avgPoints > myAvgPoints)
				out++;
		}
		else
		{
			if(avgPoints < myAvgPoints)
				out++;
		}
	}
	return out;
#endif // Q_OS_WASM
}

/*! Removes orphaned students. */
void DatabaseManager::removeOrphanedStudents(void)
{
#ifndef Q_OS_WASM
	QSqlQuery query1;
	query1.exec(QString("SELECT id FROM user WHERE role = %1").arg(QString::number(Role_Student)));
	while(query1.next())
	{
		QSqlQuery query2;
		Q_ASSERT(query1.value(0).isValid());
		int id = query1.value(0).toInt();
		query2.exec("SELECT user FROM users WHERE user = " + QString::number(id));
		if(!query2.next()) // This student is not a member of any class
			removeUser(id);
	}
#endif // Q_OS_WASM
}

/*! Returns list of paired device IDs. */
QList<int> DatabaseManager::deviceIDs(void)
{
#ifdef Q_OS_WASM
	return QList<int>();
#else
	QSqlQuery query;
	query.exec("SELECT id FROM device");
	QList<int> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += query.value(0).toInt();
	}
	return out;
#endif
}

/*! Returns list of paired device IPv4 addresses. */
QList<QHostAddress> DatabaseManager::deviceAddresses(void)
{
#ifdef Q_OS_WASM
	return QList<QHostAddress>();
#else
	QSqlQuery query;
	query.exec("SELECT ip_address FROM device");
	QList<QHostAddress> out;
	while(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		out += QHostAddress(query.value(0).toString());
	}
	return out;
#endif
}

/*! Returns the IPv4 address of the given device. */
QHostAddress DatabaseManager::deviceAddress(int deviceID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(deviceID);
	return QHostAddress();
#else
	QSqlQuery query;
	query.exec("SELECT ip_address FROM device WHERE id = " + QString::number(deviceID));
	query.next();
	Q_ASSERT(query.value(0).isValid());
	return QHostAddress(query.value(0).toString());
#endif
}

/*! Returns the name of the given device. */
QString DatabaseManager::deviceName(int deviceID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(deviceID);
	return QString();
#else
	QSqlQuery query;
	query.exec("SELECT name FROM device WHERE id = " + QString::number(deviceID));
	query.next();
	Q_ASSERT(query.value(0).isValid());
	return query.value(0).toString();
#endif
}

/*! Finds device by IPv4 address. */
int DatabaseManager::findDevice(QHostAddress address)
{
#ifdef Q_OS_WASM
	Q_UNUSED(address);
	return 0;
#else
	QSqlQuery query;
	query.exec("SELECT id FROM device WHERE ip_address = " + quotesEnclosed(QHostAddress(address.toIPv4Address()).toString()));
	if(query.next())
	{
		Q_ASSERT(query.value(0).isValid());
		return query.value(0).toInt();
	}
	else
		return 0;
#endif
}

/*! Adds a new device. */
void DatabaseManager::addDevice(QString name, QHostAddress address)
{
#ifdef Q_OS_WASM
	Q_UNUSED(name);
	Q_UNUSED(address);
#else
	QSqlQuery query;
	bool result = query.exec(QString("INSERT INTO device (name, ip_address) VALUES (%1, %2)").arg(quotesEnclosed(name), quotesEnclosed(QHostAddress(address.toIPv4Address()).toString())));
	Q_ASSERT(result);
#endif
}

/*! Edits the given device. */
void DatabaseManager::editDevice(int deviceID, QString name, QHostAddress address)
{
#ifdef Q_OS_WASM
	Q_UNUSED(deviceID);
	Q_UNUSED(name);
	Q_UNUSED(address);
#else
	QSqlQuery query;
	bool result = query.exec(QString("UPDATE device SET name = %1, ip_address = %2 WHERE id = %3").arg(quotesEnclosed(name), quotesEnclosed(QHostAddress(address.toIPv4Address()).toString()), QString::number(deviceID)));
	Q_ASSERT(result);
#endif
}

/*! Removes the given device. */
void DatabaseManager::removeDevice(int deviceID)
{
#ifdef Q_OS_WASM
	Q_UNUSED(deviceID);
#else
	QSqlQuery query;
	bool result = query.exec(QString("DELETE FROM device WHERE id = %1").arg(QString::number(deviceID)));
	Q_ASSERT(result);
#endif
}

/*! Returns the given string enclosed by quotes compatible with SQLite (special characters are automatically escaped). */
QString DatabaseManager::quotesEnclosed(QString str)
{
	QString out = "\"";
	for(int i = 0; i < str.count(); i++)
	{
		if(str[i] == '"')
			out += '"';
		out += str[i];
	}
	out += '"';
	return out;
}
