/*
 * server.h
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

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QSettings>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSslSocket>
#include <QNetworkInterface>
#include <QTimer>
#include <QSslCertificate>
#include <QSslKey>
#include <QPointer>
#include "core/database.h"

/*! \brief The monitorServer class is a QTcpServer that is used to communicate with Open-Typer clients. */
class monitorServer : public QTcpServer
{
	Q_OBJECT
	public:
		explicit monitorServer(bool silent = false, QObject *parent = nullptr);
		~monitorServer();
		static quint16 port(void);
		static QHostAddress address(void);
		void sendSignal(QByteArray name, QList<QByteArray> data, QList<QByteArray> usernames);
		bool isLoggedIn(QString username);
		QList<int> runningExerciseStudents(void);
		const QSslCertificate &getSslLocalCertificate() const;
		const QSslKey &getSslPrivateKey() const;
		QSsl::SslProtocol getSslProtocol() const;
		void setSslLocalCertificate(const QSslCertificate &certificate);
		bool setSslLocalCertificate(const QString &path, QSsl::EncodingFormat format = QSsl::Pem);
		void setSslPrivateKey(const QSslKey &key);
		bool setSslPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm = QSsl::Rsa, QSsl::EncodingFormat format = QSsl::Pem, const QByteArray &passPhrase = QByteArray());
		void setSslProtocol(QSsl::SslProtocol protocol);

	protected:
		void incomingConnection(qintptr socketDescriptor);

	signals:
		/*! A signal, which is emitted, when a student uploads a monitored exercise result. */
		void resultUploaded(int, QList<QVariantMap>, QString, int, int, double, int);
		/*! A signal, which is emitted, when a student aborts a monitored exercise. */
		void exerciseAborted(int);

	private slots:
		void acceptConnection(void);
		void disconnectClient(void);
		void sendResponse(void);

	private:
		QList<QSslSocket*> clientSockets;
		QByteArray convertData(bool *ok, QList<QByteArray> input);
		QByteArray convertData(QList<QByteArray> input);
		QList<QByteArray> readData(QByteArray input);
		bool studentAuthAvailable(QString nickname);
		QMap<QSslSocket*,QString> sessions; /*!< Stores student sessions (socket, username). */
		QList<QSslSocket*> exerciseSockets;
		QMap<QSslSocket*, QList<QVariantMap>> recordedMistakes;
		QSslCertificate m_sslLocalCertificate;
		QSslKey m_sslPrivateKey;
		QSsl::SslProtocol m_sslProtocol;
};

extern QPointer<monitorServer> serverPtr;

#endif // SERVER_H
