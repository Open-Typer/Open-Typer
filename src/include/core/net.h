/*
 * net.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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
#include <QByteArray>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDataStream>
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>

/*! \brief The Net class provides network functions. */
class Net : public QObject
{
	Q_OBJECT
	public:
		bool internetConnected();
};

/*!
 * \brief The downloader class provides functions for file downloading.
 *
 * Usage example:
 * \code
 * class myClass : public QObject
 * {
 	Q_OBJECT
 * 	public:
 * 		explicit myClass(QObject *parent = nullptr);
 * 		void downloadFile(QUrl url, QString destination);
 * 		Downloader *dw;
 * 		QString dest;
 *
 * 	private slots:
 * 		void saveFile(void);
 * };
 *
 * void myClass::downloadFile(QUrl url, QString destination)
 * {
 * 	dest = destination;
 * 	dw = new Downloader(url,this);
 * 	connect(updatedProgram,SIGNAL(downloaded()),this,SLOT(saveFile()));
 * }
 *
 * void myClass::saveFile(void)
 * {
 * 	QSaveFile file(dest);
	file.open(QIODevice::WriteOnly);
	file.write(dw->downloadedData());
	file.commit();
 * }
 * \endcode
 *
 * \see Net
 * \see Updater
 */
class Downloader : public QObject
{
	Q_OBJECT
	public:
		explicit Downloader(QUrl url, QObject *parent = nullptr);
		virtual ~Downloader();
		QByteArray downloadedData() const;
		int downloadProgressPercentage;

	signals:
		/*! A signal, which is emitted when the download finishes. */
		void downloaded();
		/*!
		 * A signal, which is emitted when the progress percentage changes.\n
		 * The parameter is the progress percentage.
		 */
		void progressChanged(int);
	
	private slots:
		void fileDownloaded(QNetworkReply* pReply);
		void downloadProgress(qint64 current, qint64 max);

	public slots:
		void cancelDownload(void);
	
	private:
		QNetworkAccessManager m_WebCtrl;
		QByteArray m_DownloadedData;
		QNetworkReply *reply;
};

/*! \brief The monitorClient class is used to communicate with the class monitor server. */
class monitorClient : public QObject
{
	Q_OBJECT
	public:
		explicit monitorClient(QObject *parent = nullptr);
		QByteArray sendRequest(QString method, QList<QByteArray> data);

	private:
		QTcpSocket *socket;
		QByteArray response;
		QByteArray convertData(bool *ok, QList<QByteArray> input);

	signals:
		/*! A signal, which is emitted after readResponse() reads the response. \see readResponse() */
		void responseReady();

	private slots:
		void readResponse(void);
		void errorOccurred(QAbstractSocket::SocketError error);
};

#endif // NET_H
