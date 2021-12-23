/*
 * net.cpp
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

#include "core/net.h"

/*!
 * Constructs Downloader, which starts to download immediately.
 * \param[in] url Remote file URL.
 */
Downloader::Downloader(QUrl url, QObject *parent) :
	QObject(parent)
{
	downloadProgressPercentage = 0;
	QNetworkRequest request(url);
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);
	reply = m_WebCtrl.get(request);
	connect(&m_WebCtrl,SIGNAL(finished(QNetworkReply*)),this,SLOT(fileDownloaded(QNetworkReply*)));
	connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
}

/*! Destroys the Downloader object. */
Downloader::~Downloader() { }

/*!
 * Connected from m_WebCtrl.finished().\n
 * Reads downloaded data and emits downloaded().
 * \see downloaded()
 */
void Downloader::fileDownloaded(QNetworkReply* pReply)
{
	if(!pReply->isOpen())
		return;
	m_DownloadedData = pReply->readAll();
	pReply->deleteLater();
	emit downloaded();
}

/*!
 * Returns downloaded data.
 * \see downloaded()
 */
QByteArray Downloader::downloadedData() const
{
	return m_DownloadedData;
}

/*!
 * Connected from reply->downloadProgress().\n
 * Calculates download progress percentage and emits progressChanged().
 * \see progressChanged()
 */
void Downloader::downloadProgress(qint64 current, qint64 max)
{
	downloadProgressPercentage = (current*100)/max;
	emit progressChanged(downloadProgressPercentage);
}

/*! Aborts the download. */
void Downloader::cancelDownload(void)
{
	reply->abort();
}

/*!
 * Returns true if an internet connection is available.\n
 * This functions pings 8.8.8.8 to check connection availability.
 */
bool Net::internetConnected()
{
	QTcpSocket* sock = new QTcpSocket(this);
	sock->connectToHost("8.8.8.8", 53);
	bool connected = sock->waitForConnected(30000);//ms
	if (!connected)
	{
		sock->abort();
		return false;
	}
	sock->close();
	return true;
}

/*! Constructs monitorClient. */
monitorClient::monitorClient(QObject *parent) :
	QObject(parent)
{
	socket = new QTcpSocket;
	// Connections
	connect(socket,&QIODevice::readyRead,this,&monitorClient::readResponse);
	connect(socket,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&monitorClient::errorOccurred);
}

/*!
 * Sends a request and returns the response.\n
 * \param[in] method Request method.
 * \param[in] data Request data.
 */
QByteArray monitorClient::sendRequest(QString method, QByteArray data)
{
	socket->abort();
	socket->connectToHost("localhost",57100);
	if(socket->waitForConnected())
	{
		bool ok;
		socket->write(convertData(&ok,{method.toUtf8(),data}));
		if(!ok)
		{
			socket->close();
			return QByteArray();
		}
		// Wait for response
		QTimer responseTimer;
		responseTimer.setSingleShot(true);
		responseTimer.setInterval(5000); // Maximum wait time
		QEventLoop reqLoop;
		connect(&responseTimer,SIGNAL(timeout()),&reqLoop,SLOT(quit()));
		connect(this,SIGNAL(responseReady()),&reqLoop,SLOT(quit()));
		responseTimer.start();
		reqLoop.exec();
		socket->close();
		if(responseTimer.remainingTime() == -1)
		{
			errorOccurred(QAbstractSocket::SocketTimeoutError);
			return QByteArray();
		}
		else
			return response;
	}
	else
		return QByteArray();
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
	QMessageBox errBox;
	errBox.setWindowTitle(tr("Error"));
	errBox.setText(tr("Unable to connect to class monitor server."));
	switch(error) {
		case QAbstractSocket::SocketTimeoutError:
			errBox.setInformativeText(tr("Connection timed out."));
			break;
		default:
			errBox.setInformativeText(socket->errorString());
			break;
	}
	errBox.setIcon(QMessageBox::Critical);
	errBox.exec();
}

/*!
 * Converts list of QByteArrays to a single QByteArray, which can be used for a request. */
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
			*ok = false;
			return QByteArray();
		}
		out += dataSize;
		// Data
		out += input[i];
	}
	*ok = true;
	return out;
}
