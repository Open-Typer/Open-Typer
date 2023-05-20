/*
 * QuickWindow.cpp
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

#include "QuickWindow.h"

QuickWindow::QuickWindow(QWindow *parent) :
	QQuickWindow(parent)
{
	m_accessibleWidget = new QAccessibleWidget(&m_widget, QAccessible::Dialog);
	m_accessibleWidget->setText(QAccessible::Name, title());
	m_widget.setAccessibleName(title());
	QAccessibleEvent accessibleEvent(m_accessibleWidget, QAccessible::NameChanged);
	QAccessible::updateAccessibility(&accessibleEvent);

	connect(this, &QQuickWindow::windowTitleChanged, [this](const QString &title) {
		m_accessibleWidget->setText(QAccessible::Name, title);
		m_widget.setAccessibleName(title);
		QAccessibleEvent accessibleEvent(m_accessibleWidget, QAccessible::NameChanged);
		QAccessible::updateAccessibility(&accessibleEvent);
	});
}

void QuickWindow::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Escape && m_autoClose && m_closable)
		close();
	QQuickWindow::keyPressEvent(event);
}

void QuickWindow::showEvent(QShowEvent *event)
{
	QQuickWindow::showEvent(event);
	QAccessibleEvent accessibleEvent(m_accessibleWidget, QAccessible::Focus);
	QAccessible::updateAccessibility(&accessibleEvent);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QuickWindow::closeEvent(QCloseEvent *event)
{
	if(m_closable)
		QQuickWindow::closeEvent(event);
	else
		event->ignore();
}
#else
bool QuickWindow::event(QEvent *event)
{
	if(event->type() == QEvent::Close)
	{
		if(m_closable)
			return QQuickWindow::event(event);
		return true;
	}
	else
		return QQuickWindow::event(event);
}
#endif

void QuickWindow::setAutoClose(bool newAutoClose)
{
	if(m_autoClose == newAutoClose)
		return;
	m_autoClose = newAutoClose;
	emit autoCloseChanged();
}

bool QuickWindow::closable() const
{
	return m_closable;
}

void QuickWindow::setClosable(bool newClosable)
{
	if(m_closable == newClosable)
		return;
	m_closable = newClosable;
	emit closableChanged();
}

bool QuickWindow::autoClose() const
{
	return m_autoClose;
}
