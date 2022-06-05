/*
 * server.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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

#include "core/server.h"

QPointer<monitorServer> serverPtr = nullptr;

/*! Constructs monitorServer. */
monitorServer::monitorServer(bool silent, QObject *parent) :
	QWebSocketServer("Open-Typer", QWebSocketServer::SecureMode, parent),
	clientSockets(),
	sessions(),
	exerciseSockets(),
	settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat)
{
	generateRandomCertKey();
	if(!listen(QHostAddress::Any,port()))
	{
		if(!silent)
		{
			QMessageBox errBox;
			errBox.setWindowTitle("Error");
			errBox.setText(tr("Unable to start server on port %1.").arg(port()));
			errBox.setInformativeText(errorString());
			errBox.setIcon(QMessageBox::Critical);
			errBox.exec();
		}
		return;
	}
	// Connections
	connect(this, &QWebSocketServer::newConnection, this, &monitorServer::acceptConnection);
}

/*! Returns the IP address of the network interface. */
QHostAddress monitorServer::address(void)
{
	QList<QHostAddress> addressList = QNetworkInterface::allAddresses();
	for(int i=0; i < addressList.count(); i++)
	{
		if((addressList[i] != QHostAddress::LocalHost) && (addressList[i].toIPv4Address()))
			return addressList[i];
	}
	return QHostAddress(QHostAddress::LocalHost);
}

/*! Returns the port, which was set by the user (or the default port). */
quint16 monitorServer::port(void)
{
	QSettings settings(fileUtils::configLocation() + "/config.ini",QSettings::IniFormat);
	return settings.value("server/port",57100).toUInt();
}

/*!
 * Accepts a new connection.
 *
 * \see sendResponse()
 */
void monitorServer::acceptConnection(void)
{
	QWebSocket *clientSocket = nextPendingConnection();
	connect(clientSocket, &QWebSocket::textMessageReceived, this, &monitorServer::sendResponse);
	connect(clientSocket, &QWebSocket::disconnected, this, &monitorServer::disconnectClient);
	clientSockets += clientSocket;
	if(!settings.value("server/fullmode", false).toBool())
		emit connectedDevicesChanged();
}

/*! Connected from QWebSocket::disconnected(). */
void monitorServer::disconnectClient(void)
{
	QWebSocket *clientSocket = (QWebSocket*) sender();
	clientSockets.removeAll(clientSocket);
	sessions.remove(clientSocket);
	emit loggedInStudentsChanged();
	exerciseSockets.removeAll(clientSocket);
	emit exerciseAborted(dbMgr.findUser(sessions[clientSocket]));
	clientSocket->deleteLater();
	if(!settings.value("server/fullmode", false).toBool())
		emit connectedDevicesChanged();
}

/*!
 * Connected from QWebSocket::binaryMessageReceived().\n
 * Sends a response back to the client.
 */
void monitorServer::sendResponse(QString message)
{
	QWebSocket *clientSocket = (QWebSocket*) sender();
	QStringList requestList = readData(message.toUtf8());
	if(sessions.contains(clientSocket) && !studentAuthAvailable(sessions[clientSocket]))
		sessions.remove(clientSocket);
	QList<QWebSocket*> socketsToRemove;
	for(int i=0; i < exerciseSockets.count(); i++)
	{
		if(!sessions.contains(exerciseSockets[i]))
			socketsToRemove += exerciseSockets[i];
	}
	for(int i=0; i < socketsToRemove.count(); i++)
		exerciseSockets.removeAll(socketsToRemove[i]);
	if(requestList[0] == "check")
	{
		clientSocket->sendTextMessage(convertData({"ok"}));
		return;
	}
	if(settings.value("server/fullmode", false).toBool())
	{
		if((requestList[0] == "auth") && (requestList.count() >= 3))
		{
			if(studentAuthAvailable(requestList[1]))
			{
				int userID = dbMgr.findUser(requestList[1]);
				if((requestList[2] != "") && dbMgr.auth(userID, requestList[2]))
				{
					sessions.insert(clientSocket,requestList[1]);
					emit loggedInStudentsChanged();
					clientSocket->sendTextMessage(convertData({"ok"}));
				}
				else
					clientSocket->sendTextMessage(convertData({"fail", "password"}));
			}
			else
				clientSocket->sendTextMessage(convertData({"fail", "inactive_class"}));
			return;
		}
		else if(requestList[0] == "logout")
		{
			sessions.remove(clientSocket);
			emit loggedInStudentsChanged();
			clientSocket->sendTextMessage(convertData({"ok"}));
			return;
		}
		else if((requestList[0] == "get") && (requestList.count() >= 2))
		{
			if(requestList[1] == "username")
			{
				if(sessions.contains(clientSocket))
				{
					QString username = sessions.value(clientSocket);
					clientSocket->sendTextMessage(convertData({"ok",username.toUtf8()}));
					return;
				}
			}
			else if(requestList[1] == "name")
			{
				if(sessions.contains(clientSocket))
				{
					QString username = sessions.value(clientSocket);
					clientSocket->sendTextMessage(convertData({ "ok", dbMgr.userName(dbMgr.findUser(username)) }));
					return;
				}
			}
			else if((requestList[1] == "resultcount") && (requestList.count() >= 6))
			{
				if(sessions.contains(clientSocket))
				{
					QString username = sessions.value(clientSocket);
					int studentID = dbMgr.findUser(username);
					clientSocket->sendTextMessage(convertData({"ok", QByteArray::number(dbMgr.historyEntries(dbMgr.activeClass, studentID, QString(requestList[2]), requestList[3].toInt(), requestList[4].toInt(), requestList[5].toInt()).count())}));
					return;
				}
			}
			else if((requestList[1] == "result") && (requestList.count() >= 7))
			{
				if(sessions.contains(clientSocket))
				{
					QString username = sessions.value(clientSocket);
					int studentID = dbMgr.findUser(username);
					QVariantMap entry = dbMgr.historyEntries(dbMgr.activeClass, studentID, QString(requestList[2]), requestList[3].toInt(), requestList[4].toInt(), requestList[5].toInt()).at(requestList[6].toInt());
					QStringList responseList;
					responseList.clear();
					responseList += "ok";
					responseList += QString::number(entry["speed"].toInt());
					responseList += QString::number(entry["mistakes"].toInt());
					responseList += QString::number(entry["duration"].toInt());
					clientSocket->sendTextMessage(convertData(responseList));
					return;
				}
			}
			else if((requestList[1] == "betterstudents") && (requestList.count() >= 6))
			{
				if(sessions.contains(clientSocket))
				{
					QString username = sessions.value(clientSocket);
					int studentID = dbMgr.findUser(username);
					int betterStudents = dbMgr.compareWithStudents(dbMgr.activeClass, studentID, QString(requestList[2]), requestList[3].toInt(), requestList[4].toInt(), requestList[5].toInt(), true);
					clientSocket->sendTextMessage(convertData({"ok",QByteArray::number(betterStudents)}));
					return;
				}
			}
			else if((requestList[1] == "worsestudents") && (requestList.count() >= 6))
			{
				if(sessions.contains(clientSocket))
				{
					QString username = sessions.value(clientSocket);
					int studentID = dbMgr.findUser(username);
					int worseStudents = dbMgr.compareWithStudents(dbMgr.activeClass, studentID, QString(requestList[2]), requestList[3].toInt(), requestList[4].toInt(), requestList[5].toInt(), false);
					clientSocket->sendTextMessage(convertData({"ok",QByteArray::number(worseStudents)}));
					return;
				}
			}
		}
		else if((requestList[0] == "put") && (requestList.count() >= 2))
		{
			if((requestList[1] == "result") && (requestList.count() >= 9))
			{
				if(sessions.contains(clientSocket))
				{
					QString username = sessions.value(clientSocket);
					int studentID = dbMgr.findUser(username);
					QVariantMap resultData;
					resultData["speed"] = requestList[6];
					resultData["mistakes"] = requestList[7];
					resultData["duration"] = requestList[8];
					dbMgr.addHistoryEntry(dbMgr.activeClass, studentID, QString(requestList[2]), requestList[3].toInt(), requestList[4].toInt(), requestList[5].toInt(), resultData);
					clientSocket->sendTextMessage(convertData({"ok"}));
					return;
				}
			}
		}
	}
	else
	{
		bool paired = dbMgr.deviceAddresses().contains(QHostAddress(clientSocket->peerAddress().toIPv4Address()));
		if((requestList[0] == "get") && (requestList.count() >= 2))
		{
			if(requestList[1] == "paired")
			{
				if(paired)
				{
					clientSocket->sendTextMessage(convertData({"ok"}));
					return;
				}
			}
		}
		else if((requestList[0] == "put") && (requestList.count() >= 2))
		{
			if((requestList[1] == "name") && requestList.count() >= 3)
			{
				if(paired)
				{
					deviceStudentNames[dbMgr.findDevice(clientSocket->peerAddress())] = requestList[2];
					clientSocket->sendTextMessage(convertData({"ok"}));
					emit deviceConfigurationChanged();
					return;
				}
			}
		}
	}
	if((requestList[0] == "get") && (requestList.count() >= 2))
	{
		if(requestList[1] == "serverMode")
		{
			clientSocket->sendTextMessage(convertData({ "ok", settings.value("server/fullmode", false).toBool() ? "full" : "easy" }));
			return;
		}
	}
	else if((requestList[0] == "put") && (requestList.count() >= 2))
	{
		if(requestList[1] == "clearRecordedMistakes")
		{
			if(settings.value("server/fullmode", false).toBool() || (dbMgr.findDevice(clientSocket->peerAddress()) != 0))
			{
				if(!recordedMistakes.contains(clientSocket))
					recordedMistakes[clientSocket] = QList<QVariantMap>();
				recordedMistakes[clientSocket].clear();
				clientSocket->sendTextMessage(convertData({"ok"}));
				return;
			}
		}
		else if(requestList[1] == "recordedMistake")
		{
			if(settings.value("server/fullmode", false).toBool() || (dbMgr.findDevice(clientSocket->peerAddress()) != 0))
			{
				if(!recordedMistakes.contains(clientSocket))
					recordedMistakes[clientSocket] = QList<QVariantMap>();
				if(requestList.count() % 2 == 0)
				{
					QVariantMap map;
					for(int i=2; i < requestList.count(); i++)
					{
						// Key
						QString key = requestList[i];
						i++;
						// Value
						if(key == "pos")
							map[key] = requestList[i].toInt();
						else
							map[key] = QString(requestList[i]);
					}
					recordedMistakes[clientSocket].append(map);
					clientSocket->sendTextMessage(convertData({"ok"}));
					return;
				}
			}
		}
		else if((requestList[1] == "monitorResult") && (requestList.count() >= 7))
		{
			if(settings.value("server/fullmode", false).toBool() || (dbMgr.findDevice(clientSocket->peerAddress()) != 0))
			{
				int id;
				if(sessions.contains(clientSocket))
					id = dbMgr.findUser(sessions[clientSocket]);
				else
					id = dbMgr.findDevice(QHostAddress(clientSocket->peerAddress().toIPv4Address()));
				if(recordedMistakes.contains(clientSocket))
				{
					emit resultUploaded(id, recordedMistakes[clientSocket], requestList[2], requestList[3].toInt(), requestList[4].toInt(), requestList[5].toDouble(), requestList[6].toInt(), requestList[7].toDouble());
					recordedMistakes[clientSocket].clear();
					exerciseSockets.removeAll(clientSocket);
					clientSocket->sendTextMessage(convertData({"ok"}));
					return;
				}
			}
		}
		else if(requestList[1] == "abortExercise")
		{
			if(settings.value("server/fullmode", false).toBool() || (dbMgr.findDevice(clientSocket->peerAddress()) != 0))
			{
				exerciseSockets.removeAll(clientSocket);
				if(sessions.contains(clientSocket))
					emit exerciseAborted(dbMgr.findUser(sessions[clientSocket]));
				else if(!settings.value("server/fullmode", false).toBool())
					emit exerciseAborted(dbMgr.findDevice(QHostAddress(clientSocket->peerAddress().toIPv4Address())));
			}
			clientSocket->sendTextMessage(convertData({"ok"}));
			return;
		}
	}
	clientSocket->sendTextMessage(convertData({"fail"}));
}

/*! Sends a signal to clients with username from a list. */
void monitorServer::sendSignal(QByteArray name, QStringList data, QList<QByteArray> usernames)
{
	QStringList rawData;
	rawData.clear();
	rawData += name;
	for(int i=0; i < data.count(); i++)
		rawData += data[i];
	QString finalData = convertData(rawData);
	for(int i=0; i < clientSockets.count(); i++)
	{
		if(sessions.contains(clientSockets[i]) && usernames.contains(sessions[clientSockets[i]].toUtf8()))
		{
			if(!((name == "loadExercise") && exerciseSockets.contains(clientSockets[i])))
			{
				if(name == "loadExercise")
					exerciseSockets += clientSockets[i];
				clientSockets[i]->sendTextMessage(finalData);
			}
		}
	}
}

/*! Sends a signal to clients with address from a list. */
void monitorServer::sendSignal(QByteArray name, QStringList data, QList<QHostAddress> addresses)
{
	QStringList rawData;
	rawData.clear();
	rawData += name;
	for(int i=0; i < data.count(); i++)
		rawData += data[i];
	QString finalData = convertData(rawData);
	for(int i=0; i < clientSockets.count(); i++)
	{
		QHostAddress address(clientSockets[i]->peerAddress().toIPv4Address());
		if(addresses.contains(address) && (dbMgr.findDevice(address) != 0))
		{
			if(!((name == "loadExercise") && exerciseSockets.contains(clientSockets[i])))
			{
				if(name == "loadExercise")
					exerciseSockets += clientSockets[i];
				clientSockets[i]->sendTextMessage(finalData);
			}
		}
	}
}

/*! Returns true if the given student is logged in. */
bool monitorServer::isLoggedIn(QString username)
{
	return sessions.values().contains(username);
}

/*! Returns true if the given device is online. */
bool monitorServer::isConnected(QHostAddress address)
{
	for(int i=0; i < clientSockets.count(); i++)
	{
		if(QHostAddress(clientSockets[i]->peerAddress().toIPv4Address()) == QHostAddress(address.toIPv4Address()))
			return true;
	}
	return false;
}

/*! Returns list of students with an exercise in progress. */
QList<int> monitorServer::runningExerciseStudents(void)
{
	QList<int> out;
	for(int i=0; i < exerciseSockets.count(); i++)
		out += dbMgr.findUser(sessions.value(exerciseSockets[i]));
	return out;
}

/*! Returns the name entered by student on the given device. */
QString monitorServer::deviceStudentName(int deviceID)
{
	if(deviceStudentNames.contains(deviceID))
		return deviceStudentNames[deviceID];
	else
		return "";
}

/*! Converts list of QStrings to a single QString, which can be used for a response or signal. */
QString monitorServer::convertData(bool *ok, QStringList input)
{
	QString out;
	out.clear();
	for(int i = 0; i < input.count(); i++)
	{
		// Data size
		QString dataSize = QString::number(input[i].size());
		if(dataSize.count() > 10)
		{
			if(ok != nullptr)
				*ok = false;
			return QByteArray();
		}
		dataSize.prepend(QString("0").repeated(10 - dataSize.count()));
		out += dataSize;
		// Data
		out += input[i];
	}
	// Termination byte
	out += ";";
	if(ok != nullptr)
		*ok = true;
	return out;
}

/*! Implementation of convertData() without status boolean. */
QString monitorServer::convertData(QStringList input)
{
	return convertData(nullptr,input);
}

/*! Returns a list of QStrings from the input QString. */
QStringList monitorServer::readData(QString input)
{
	QStringList out;
	out.clear();
	quint16 dataSize, i2;
	QString dataSizeStr, data;
	int i = 0;
	while(i < input.count())
	{
		// Read data size
		dataSizeStr.clear();
		int j;
		for(j=0; j < 10; j++)
		{
			if(i + j >= input.count())
				break;
			dataSizeStr += input[i + j];
		}
		i += j;
		dataSize = dataSizeStr.toInt();
		// Read data
		data.clear();
		for(i2=0; i2 < dataSize; i2++)
		{
			data += input[i];
			i++;
		}
		out += data;
	}
	return out;
}

/*! Returns true if the given student is in the active class. */
bool monitorServer::studentAuthAvailable(QString nickname)
{
	QList<int> students = dbMgr.studentIDs(dbMgr.activeClass);
	for(int i=0; i < students.count(); i++)
	{
		if(dbMgr.userNickname(students[i]) == nickname)
			return true;
	}
	return false;
}

/*! Generates and sets random SSL key and certificate. */
void monitorServer::generateRandomCertKey(void)
{
	EVP_PKEY * pkey = nullptr;
	RSA * rsa = nullptr;
	X509 * x509 = nullptr;
	X509_NAME * name = nullptr;
	BIO * bp_public = nullptr, * bp_private = nullptr;
	const char * buffer = nullptr;
	long size;
	pkey = EVP_PKEY_new();
	q_check_ptr(pkey);
	rsa = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);
	q_check_ptr(rsa);
	EVP_PKEY_assign_RSA(pkey, rsa);
	x509 = X509_new();
	q_check_ptr(x509);
	ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
	X509_gmtime_adj(X509_get_notBefore(x509), 0);
	X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);
	X509_set_pubkey(x509, pkey);
	name = X509_get_subject_name(x509);
	q_check_ptr(name);
	X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)"US", -1, -1, 0);
	X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)"Open-Typer", -1, -1, 0);
	X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"Open-Typer", -1, -1, 0);
	X509_set_issuer_name(x509, name);
	X509_sign(x509, pkey, EVP_sha1());
	bp_private = BIO_new(BIO_s_mem());
	q_check_ptr(bp_private);
	int ret = PEM_write_bio_PrivateKey(bp_private, pkey, nullptr, nullptr, 0, nullptr, nullptr);
	if(ret != 1)
	{
		EVP_PKEY_free(pkey);
		X509_free(x509);
		BIO_free_all(bp_private);
		QString errorMessage = QString("PEM_write_bio_PrivateKey exited with code %1").arg(QString::number(ret));
		QMessageBox::critical(nullptr, "Error", errorMessage);
		return;
	}
	bp_public = BIO_new(BIO_s_mem());
	q_check_ptr(bp_public);
	ret = PEM_write_bio_X509(bp_public, x509);
	if(ret != 1)
	{
		EVP_PKEY_free(pkey);
		X509_free(x509);
		BIO_free_all(bp_public);
		BIO_free_all(bp_private);
		QString errorMessage = QString("PEM_write_bio_X509 exited with code %1").arg(QString::number(ret));
		QMessageBox::critical(nullptr, "Error", errorMessage);
		return;
	}
	size = BIO_get_mem_data(bp_public, &buffer);
	q_check_ptr(buffer);
	QSslConfiguration sslConfig;
	sslConfig.setLocalCertificate(QSslCertificate(QByteArray(buffer, size)));
	if(sslConfig.localCertificate().isNull())
	{
		QString errorMessage = "Failed to generate a random certificate";
		QMessageBox::critical(nullptr, "Error", errorMessage);
		return;
	}
	size = BIO_get_mem_data(bp_private, &buffer);
	q_check_ptr(buffer);
	sslConfig.setPrivateKey(QSslKey(QByteArray(buffer, size), QSsl::Rsa));
	if(sslConfig.privateKey().isNull())
	{
		QString errorMessage = "Failed to generate a random private key";
		QMessageBox::critical(nullptr, "Error", errorMessage);
		return;
	}
	setSslConfiguration(sslConfig);
	EVP_PKEY_free(pkey);
	X509_free(x509);
	BIO_free_all(bp_public);
	BIO_free_all(bp_private);
}
