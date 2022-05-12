/*
 * net.h
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

#ifndef NET_H
#define NET_H

#include <QObject>
#include <QApplication>
#include <QByteArray>
#include <QMessageBox>
#include <QEventLoop>
#include <QHostAddress>
#include <QTimer>
#include <QTcpSocket>
#include <QSslSocket>
#include <QSettings>
#include <QFile>
#include <QSslConfiguration>
#include "core/utils.h"

/*! \brief The monitorClient class is used to communicate with the class monitor server. */
class monitorClient : public QObject
{
	Q_OBJECT
	public:
		explicit monitorClient(bool errDialogs = true, QObject *parent = nullptr);
		~monitorClient();
		void close(void);
		void setErrorDialogs(bool errDialogs);
		QList<QByteArray> sendRequest(QString method, QList<QByteArray> data);
		static QHostAddress serverAddress(void);
		static quint16 serverPort(void);
		static bool enabled(void);
		bool available(void);

	private:
		bool errorDialogs, legacy = false;
#ifdef Q_OS_WASM
		QTcpSocket *socket;
#else
		QSslSocket *socket;
#endif
		QByteArray response, receivedData = "";
		bool connected, waitingForResponse;
		QByteArray convertData(bool *ok, QList<QByteArray> input);
		QList<QByteArray> readData(QByteArray input);
		QSettings settings;

	signals:
		/*! A signal, which is emitted after readResponse() reads the response. \see readResponse() */
		void responseReady();
		/*! A signal, which is emitted when connection is lost. */
		void disconnected();
		/*! A signal, which is emitted when an exercise is received from the server. */
		void exerciseReceived(QByteArray, int, bool, int, int, bool, bool, bool);

	private slots:
		void readResponse(void);
		void errorOccurred(QAbstractSocket::SocketError error);
};

#endif // NET_H
