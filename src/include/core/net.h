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

class Net : public QObject
{
	Q_OBJECT
	public:
		bool internetConnected();
};

class Downloader : public QObject
{
	Q_OBJECT
	public:
		explicit Downloader(QUrl url, QObject *parent = nullptr);
		virtual ~Downloader();
		QByteArray downloadedData() const;
		int downloadProgressPercentage;

	signals:
		void downloaded();
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

#endif // NET_H
