/*
 * QmlWidget.h
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

#ifndef QMLWIDGET
#define QMLWIDGET

#include <QQuickPaintedItem>
#include <QWidget>

/*!
 * \brief The QmlWidget class provides a way to display QWidgets in a QML scene.
 * Note: It does not support any events and needs to be repainted manually.\n
 * The widget can be only created in C++ and used in QML using QQmlContext#setContextProperty().
 */
class QmlWidget : public QQuickPaintedItem
{
		Q_OBJECT
		Q_PROPERTY(QWidget *widget READ widget WRITE setWidget NOTIFY widgetChanged)
	public:
		QmlWidget(QQuickItem *parent = nullptr);
		QWidget *widget(void);
		void setWidget(QWidget *widget);
		void paint(QPainter *painter) override;

	private:
		QWidget *m_widget = nullptr;

	signals:
		void widgetChanged(QWidget *widget);
};

#endif // QMLWIDGET
