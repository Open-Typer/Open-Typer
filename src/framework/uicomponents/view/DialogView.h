/*
 * DialogView.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

#ifndef DIALOGVIEW_H
#define DIALOGVIEW_H

#include <QQuickItem>
#include "internal/QuickWindow.h"

/*! \brief The DialogView class is a native dialog for QML. */
class DialogView : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QQuickItem *contentItem READ contentItem WRITE setContentItem NOTIFY contentItemChanged)
		Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
		Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
		Q_PROPERTY(bool modal READ modal WRITE setModal NOTIFY modalChanged)
		Q_PROPERTY(int minimumWidth READ minimumWidth WRITE setMinimumWidth NOTIFY minimumWidthChanged)
		Q_PROPERTY(int minimumHeight READ minimumHeight WRITE setMinimumHeight NOTIFY minimumHeightChanged)
		Q_PROPERTY(int maximumWidth READ maximumWidth WRITE setMaximumWidth NOTIFY maximumWidthChanged)
		Q_PROPERTY(int maximumHeight READ maximumHeight WRITE setMaximumHeight NOTIFY maximumHeightChanged)
		Q_PROPERTY(bool autoClose READ autoClose WRITE setAutoClose NOTIFY autoCloseChanged)
	public:
		explicit DialogView(QObject *parent = nullptr);

		QQuickItem *contentItem() const;
		void setContentItem(QQuickItem *newContentItem);

		bool visible() const;
		void setVisible(bool newVisible);

		QString title() const;
		void setTitle(const QString &newTitle);

		bool modal() const;
		void setModal(bool newModal);

		int minimumWidth() const;
		void setMinimumWidth(int newMinimumWidth);

		int minimumHeight() const;
		void setMinimumHeight(int newMinimumHeight);

		int maximumWidth() const;
		void setMaximumWidth(int newMaximumWidth);

		int maximumHeight() const;
		void setMaximumHeight(int newMaximumHeight);

		Q_INVOKABLE void setMaximizedState();
		Q_INVOKABLE void setNormalState();
		Q_INVOKABLE void showMaximized();
		Q_INVOKABLE void showNormal();

		bool autoClose() const;
		void setAutoClose(bool newAutoClose);

	signals:
		void contentItemChanged();
		void visibleChanged();
		void titleChanged();
		void modalChanged();
		void minimumWidthChanged();
		void minimumHeightChanged();
		void maximumWidthChanged();
		void maximumHeightChanged();
		void autoCloseChanged();

	private:
		QuickWindow m_window;
		QQuickItem *m_contentItem;
		bool m_modal = true;
};

#endif // DIALOGVIEW_H
