/*
 * MonitorClient.cpp
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

#include "core/MonitorClient.h"

/*! Constructs MonitorClient. */
MonitorClient::MonitorClient(bool errDialogs, QObject *parent) :
	QObject(parent),
	connected(false),
	waitingForResponse(false)
{
	setErrorDialogs(errDialogs);
	connect(&socket, &QWebSocket::textMessageReceived, this, &MonitorClient::readResponse);
	connect(&socket, &QWebSocket::disconnected, this, &MonitorClient::disconnected);
	connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &MonitorClient::errorOccurred);
#ifndef Q_OS_WASM
	connect(&socket, QOverload<const QList<QSslError> &>::of(&QWebSocket::sslErrors), this, &MonitorClient::sslErrorsOccurred);
#endif // Q_OS_WASM
}

/*! Destroys the MonitorClient object. */
MonitorClient::~MonitorClient()
{
	// Disconnect everything from socket because the connections cause segfaults
	disconnect(&socket, nullptr, nullptr, nullptr);
}

/*! Closes the connection. */
void MonitorClient::close(void)
{
	socket.close();
}

/*! Enables or disables connection error dialogs. */
void MonitorClient::setErrorDialogs(bool errDialogs)
{
	errorDialogs = errDialogs;
}

/*! Returns server address. */
QHostAddress MonitorClient::serverAddress(void)
{
	return QHostAddress(Settings::serverAddress());
}

/*! Returns server port. */
quint16 MonitorClient::serverPort(void)
{
	return Settings::serverPort();
}

/*! Returns the local IP address, e. g. 192.168.0.100. */
QHostAddress MonitorClient::localAddress(void)
{
#ifndef Q_OS_WASM
	QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
	for(QHostAddress address : addresses)
	{
		if((address.protocol() == QAbstractSocket::IPv4Protocol) && !address.isLoopback())
			return address;
	}
#endif // Q_OS_WASM
	return QHostAddress(QHostAddress::Null);
}

/*! Returns true if client is enabled in the settings. */
bool MonitorClient::enabled(void)
{
	return (Settings::networkEnabled() && (Settings::networkMode() == 2));
}

/*! Returns true if class monitor server connection is enabled in the settings and the server is available. */
bool MonitorClient::available(void)
{
	if(enabled())
		return (sendRequest("check", {}).value(0) == "ok");
	else
		return false;
}

/*! Returns true if full mode is enabled on the server. */
bool MonitorClient::fullMode(void)
{
	auto response = sendRequest("get", { "serverMode" });
	if(response[0] == "ok")
		return (response[1] == "full");
	else
		return false;
}

/*! Returns true if this device is paired with the server or full mode is enabled. */
bool MonitorClient::isPaired(void)
{
	if(fullMode())
		return true;
	return (sendRequest("get", { "paired" }).value(0) == "ok");
}

/*! Enables client again after connection failure. */
void MonitorClient::enableClient(void)
{
	clientDisabled = false;
	Settings::setClientDisabled(clientDisabled);
}

/*!
 * Sends a request and returns the response.\n
 * \param[in] method Request method.
 * \param[in] data Request data.
 */
QStringList MonitorClient::sendRequest(QString method, QStringList data)
{
	clientDisabled = Settings::clientDisabled();
	if(clientDisabled)
		return { "clientDisabled" };
	connected = (socket.state() == QAbstractSocket::ConnectedState);
	if(!connected)
	{
		connecting = true;
		QTimer timer;
		QEventLoop eventLoop;
		connect(&socket, &QWebSocket::connected, &eventLoop, &QEventLoop::quit);
		QPointer<QEventLoop> eventLoopPtr = &eventLoop;
		auto errorSlot = [this, eventLoopPtr]() {
			clientDisabled = true;
			Settings::setClientDisabled(clientDisabled);
			if(connecting && eventLoopPtr)
				eventLoopPtr->quit();
		};
		connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, errorSlot);
		connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
		connect(&timer, &QTimer::timeout, this, errorSlot);
		timer.start(5000);
		socket.open(QUrl("wss://" + QHostAddress(serverAddress().toIPv4Address()).toString() + ":" + QString::number(serverPort())));
		eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
		connecting = false;
		connected = (socket.state() == QAbstractSocket::ConnectedState);
	}
	if(connected)
	{
		enableClient();
		bool ok;
		QStringList reqList;
		reqList.clear();
		reqList += method;
		reqList += data;
		waitingForResponse = true;
		QTimer timer;
		timer.setSingleShot(true);
		QEventLoop eventLoop;
		connect(this, &MonitorClient::responseReady, &eventLoop, &QEventLoop::quit);
		connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
		timer.start(5000);
		socket.sendTextMessage(convertData(&ok, reqList));
		if(!ok)
			return { "requestError" };
		// Wait for response
		QApplication::setOverrideCursor(Qt::WaitCursor);
		eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
		bool readStatus = timer.isActive();
		waitingForResponse = false;
		QApplication::restoreOverrideCursor();
		if(!readStatus)
		{
			errorOccurred(QAbstractSocket::SocketTimeoutError);
			return { "timeout" };
		}
		else
			return readData(response);
	}
	else
		return { "connectFailure" };
}

/*!
 * Connected from socket->textMessageReceived().\n
 * Reads the response and emits responseReady().\n
 * If there isn't any request in progress, it reads the data as a signal.
 * \see responseReady()
 */
void MonitorClient::readResponse(QString message)
{
	receivedData += message;
	if(receivedData[receivedData.count() - 1] != ';')
		return;
	receivedData.remove(receivedData.count() - 1, 1);
	if(waitingForResponse)
	{
		response = receivedData;
		emit responseReady();
	}
	else
	{
		QStringList signal = readData(receivedData);
		if(signal[0] == "initExercise")
		{
			if(signal.count() >= 3)
				emit initExReceived(signal[1], signal[2].toInt());
		}
		else if(signal[0] == "loadExercise")
		{
			if(signal.count() >= 9)
				emit exerciseReceived(signal[1].toUtf8(), signal[2].toInt(), (signal[3] == "true"), signal[4].toInt(), signal[5].toInt(), (signal[6] == "true"), (signal[7] == "true"), (signal[8] == "true"));
		}
		else if(signal[0] == "changeName")
		{
			if(signal.count() >= 1)
				emit studentNameChanged(signal[1]);
		}
	}
	receivedData = "";
}

/*!
 * Connected from socket->error().\n
 * Displays connection error message.
 */
void MonitorClient::errorOccurred(QAbstractSocket::SocketError error)
{
	if(!errorDialogs)
		return;
	QMessageBox errBox;
	errBox.setWindowTitle(tr("Error"));
	errBox.setText(tr("Unable to connect to class monitor server."));
	switch(error)
	{
		case QAbstractSocket::SocketTimeoutError:
			errBox.setInformativeText("Connection timed out.");
			break;
		default:
			errBox.setInformativeText(socket.errorString());
			break;
	}
	errBox.setIcon(QMessageBox::Critical);
	errBox.exec();
}

#ifndef Q_OS_WASM
/*!
 * Ignores SSL errors.
 */
void MonitorClient::sslErrorsOccurred(const QList<QSslError> &errors)
{
	Q_UNUSED(errors);
	// Reason for ignoring SSL errors: The server generates random certificate and key when it starts.
	QSslCertificate cert = errors[0].certificate();
	if(errors.count() == 2 && errors.contains(QSslError(QSslError::HostNameMismatch, cert)) && errors.contains(QSslError(QSslError::SelfSignedCertificate, cert)))
		socket.ignoreSslErrors();
}
#endif // Q_OS_WASM

/*! Converts list of QStrings to a single QString, which can be used for a request. */
QString MonitorClient::convertData(bool *ok, QStringList input)
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
	if(ok != nullptr)
		*ok = true;
	return out;
}

/*! Returns a list of QStrings from the input QString. */
QStringList MonitorClient::readData(QString input)
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
		for(j = 0; j < 10; j++)
		{
			if(i + j >= input.count())
				break;
			dataSizeStr += input[i + j];
		}
		i += j;
		dataSize = dataSizeStr.toInt();
		// Read data
		data.clear();
		for(i2 = 0; i2 < dataSize; i2++)
		{
			data += input[i];
			i++;
		}
		out += data;
	}
	return out;
}
