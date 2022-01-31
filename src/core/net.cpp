/*
 * net.cpp
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

#include "core/net.h"

/*! Constructs monitorClient. */
monitorClient::monitorClient(bool errDialogs, QObject *parent) :
	QObject(parent),
	connected(false)
{
	setErrorDialogs(errDialogs);
#ifdef Q_OS_WASM
	socket = new QTcpSocket;
#else
	socket = new QSslSocket;
	QFile *certFile = new QFile(":certs/server.pem");
	certFile->open(QIODevice::ReadOnly | QIODevice::Text);
	QSslCertificate cert(certFile);
	socket->addCaCertificate(cert);
	socket->setProtocol(QSsl::TlsV1_2);
	socket->ignoreSslErrors({QSslError(QSslError::HostNameMismatch,cert)});
#endif
	connect(socket,&QIODevice::readyRead,this,&monitorClient::readResponse);
	connect(socket,&QAbstractSocket::disconnected,this,&monitorClient::disconnected);
	connect(socket,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&monitorClient::errorOccurred);
}

/*! Destroys the monitorClient object. */
monitorClient::~monitorClient()
{
	socket->close();
}

/*! Closes the connection. */
void monitorClient::close(void)
{
	socket->close();
}

/*! Enables or disables connection error dialogs. */
void monitorClient::setErrorDialogs(bool errDialogs)
{
	errorDialogs = errDialogs;
}

/*! Returns server address. */
QHostAddress monitorClient::serverAddress(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(),QSettings::IniFormat);
	return QHostAddress(settings.value("server/address","127.0.0.1").toString());
}

/*! Returns server port. */
quint16 monitorClient::serverPort(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(),QSettings::IniFormat);
	return settings.value("server/port","57100").toUInt();
}

/*! Returns true if class monitor server connection is enabled in the settings. */
bool monitorClient::enabled(void)
{
	QSettings settings(fileUtils::mainSettingsLocation(),QSettings::IniFormat);
	return settings.value("server/enabled","false").toBool();
}

/*! Returns true if class monitor server connection is enabled in the settings and the server is available. */
bool monitorClient::available(bool hang)
{
	if(enabled())
		return (sendRequest("check",{},hang).value(0) == "ok");
	else
		return false;
}

/*!
 * Sends a request and returns the response.\n
 * \param[in] method Request method.
 * \param[in] data Request data.
 * \param[in] hang Whether to block user input events while waiting for response.
 */
QList<QByteArray> monitorClient::sendRequest(QString method, QList<QByteArray> data, bool hang)
{
	connected = (socket->state() == QAbstractSocket::ConnectedState);
	if(!connected)
	{
#ifdef Q_OS_WASM
		socket->connectToHost(QHostAddress(serverAddress().toIPv4Address()).toString(),serverPort());
		connected = socket->waitForConnected();
#else
		socket->connectToHostEncrypted(QHostAddress(serverAddress().toIPv4Address()).toString(),serverPort());
		connected = socket->waitForEncrypted();
#endif
	}
	if(connected)
	{
		bool ok;
		QList<QByteArray> reqList;
		reqList.clear();
		reqList += method.toUtf8();
		for(int i=0; i < data.count(); i++)
			reqList += data[i];
		socket->write(convertData(&ok,reqList));
		if(!ok)
			return QList<QByteArray>({"requestError"});
		// Wait for response
		QApplication::setOverrideCursor(Qt::WaitCursor);
		QTimer responseTimer;
		responseTimer.setSingleShot(true);
		responseTimer.setInterval(5000); // Maximum wait time
		QEventLoop reqLoop;
		connect(&responseTimer,SIGNAL(timeout()),&reqLoop,SLOT(quit()));
		connect(this,SIGNAL(responseReady()),&reqLoop,SLOT(quit()));
		responseTimer.start();
		if(hang)
			reqLoop.exec(QEventLoop::ExcludeUserInputEvents);
		else
			reqLoop.exec();
		QApplication::restoreOverrideCursor();
		if(responseTimer.remainingTime() == -1)
		{
			errorOccurred(QAbstractSocket::SocketTimeoutError);
			return QList<QByteArray>({"timeout"});
		}
		else
			return readData(response);
	}
	else
		return QList<QByteArray>({"connectFailure"});
}

/*!
 * Connected from socket->readyRead().\n
 * Reads the response and emits responseReady().
 * \see responseReady()
 */
void monitorClient::readResponse(void)
{
	response = socket->readAll();
	emit responseReady();
}

/*!
 * Connected from socket->error().\n
 * Displays connection error message.
 */
void monitorClient::errorOccurred(QAbstractSocket::SocketError error)
{
	if(!errorDialogs)
		return;
	QMessageBox errBox;
	errBox.setWindowTitle(tr("Error"));
	errBox.setText(tr("Unable to connect to class monitor server."));
	switch(error) {
		case QAbstractSocket::SocketTimeoutError:
			errBox.setInformativeText("Connection timed out.");
			break;
		default:
			errBox.setInformativeText(socket->errorString());
			break;
	}
	errBox.setIcon(QMessageBox::Critical);
	errBox.exec();
}

/*! Converts list of QByteArrays to a single QByteArray, which can be used for a request. */
QByteArray monitorClient::convertData(bool *ok, QList<QByteArray> input)
{
	QByteArray out;
	out.clear();
	for(int i = 0; i < input.count(); i++)
	{
		QByteArray sizeNum, dataSize;
		// Data size
		sizeNum.setNum(input[i].size(),16);
		dataSize = QByteArray::fromHex(sizeNum);
		if(sizeNum.size() <= 2)
			dataSize.prepend(QByteArray::fromHex("0"));
		else if(sizeNum.size() > 4)
		{
			if(ok != nullptr)
				*ok = false;
			return QByteArray();
		}
		out += dataSize;
		// Data
		out += input[i];
	}
	if(ok != nullptr)
		*ok = true;
	return out;
}

/*! Returns a list of QByteArrays from the input QByteArray. */
QList<QByteArray> monitorClient::readData(QByteArray input)
{
	QList<QByteArray> out;
	out.clear();
	quint16 dataSize, i2;
	QByteArray dataSizeArr, data;
	int i = 0;
	while(i < input.count())
	{
		// Read data size
		dataSizeArr.clear();
		dataSizeArr += input[i];
		dataSizeArr += input[i+1];
		i += 2;
		dataSize = dataSizeArr.toHex().toUInt(nullptr,16);
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
