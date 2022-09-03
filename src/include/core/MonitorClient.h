/*
 * MonitorClient.h
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

#ifndef MONITORCLIENT_H
#define MONITORCLIENT_H

#include <QObject>
#include <QApplication>
#include <QByteArray>
#include <QMessageBox>
#include <QEventLoop>
#include <QHostAddress>
#include <QTimer>
#include <QWebSocket>
#include <QSettings>
#include <QFile>
#include <QWebSocketServer>
#include <QNetworkInterface>
#include <QPointer>
#include "core/FileUtils.h"

/*! \brief The MonitorClient class is used to communicate with the class monitor server. */
class MonitorClient : public QObject
{
		Q_OBJECT
	public:
		explicit MonitorClient(bool errDialogs = true, QObject *parent = nullptr);
		~MonitorClient();
		void close(void);
		void setErrorDialogs(bool errDialogs);
		bool fullMode(void);
		bool isPaired(void);
		void enableClient(void);
		QStringList sendRequest(QString method, QStringList data);
		static QHostAddress serverAddress(void);
		static quint16 serverPort(void);
		static QHostAddress localAddress(void);
		static bool enabled(void);
		bool available(void);

	private:
		bool errorDialogs;
		QWebSocket socket;
		QString response, receivedData = "";
		bool connected, waitingForResponse;
		QString convertData(bool *ok, QStringList input);
		QStringList readData(QString input);
		QSettings settings;
		bool clientDisabled = false, connecting = false;

	signals:
		/*! A signal, which is emitted after readResponse() reads the response. \see readResponse() */
		void responseReady();
		/*! A signal, which is emitted when connection is lost. */
		void disconnected();
		/*! A signal, which is emitted when a started exercise is received from the server. */
		void exerciseReceived(QByteArray, int, bool, int, int, bool, bool, bool);
		/*! A signal, which is emitted when an exercise is received from the server. */
		void initExReceived(QString, int);
		/*! A signal, which is emitted when the student's name changes. */
		void studentNameChanged(QString name);

	private slots:
		void readResponse(QString message);
		void errorOccurred(QAbstractSocket::SocketError error);
#ifndef Q_OS_WASM
		void sslErrorsOccurred(const QList<QSslError> &errors);
#endif // Q_OS_WASM
};

#endif // MONITORCLIENT_H
