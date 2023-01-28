/*
 * QmlWidget.cpp
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

#include <QPainter>
#include "QmlWidget.h"

/*! Constructs QmlWidget. */
QmlWidget::QmlWidget(QQuickItem *parent) :
	QQuickPaintedItem(parent) { }

/*! The widget which is displayed in QML. */
QWidget *QmlWidget::widget(void)
{
	return m_widget;
}

void QmlWidget::setWidget(QWidget *widget)
{
	m_widget = widget;
	m_widget->setParent(nullptr);
	emit widgetChanged(widget);
}

void QmlWidget::paint(QPainter *painter)
{
	if(m_widget)
	{
		m_widget->setGeometry(0, 0, width(), height());
		m_widget->render(painter, QPoint(), QRegion(0, 0, width(), height()));
	}
}
