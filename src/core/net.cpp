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
