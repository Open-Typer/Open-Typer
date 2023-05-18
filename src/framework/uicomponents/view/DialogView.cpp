/*
 * DialogView.cpp
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

#include "DialogView.h"
#include <QQuickItem>

/*! Constructs DialogView. */
DialogView::DialogView(QObject *parent) :
	QObject(parent)
{
	if(m_modal)
		m_window.setModality(Qt::ApplicationModal);
	else
		m_window.setModality(Qt::NonModal);
	m_window.setFlag(Qt::Window, false);
	m_window.setFlag(Qt::Dialog, true);
	m_window.setFlag(Qt::WindowCloseButtonHint, true);

	connect(&m_window, &QQuickWindow::visibleChanged, this, &DialogView::visibleChanged);
	connect(&m_window, &QQuickWindow::windowTitleChanged, this, &DialogView::titleChanged);
	connect(&m_window, &QQuickWindow::minimumWidthChanged, this, &DialogView::minimumWidthChanged);
	connect(&m_window, &QQuickWindow::minimumHeightChanged, this, &DialogView::minimumHeightChanged);
	connect(&m_window, &QQuickWindow::maximumWidthChanged, this, &DialogView::maximumWidthChanged);
	connect(&m_window, &QQuickWindow::maximumHeightChanged, this, &DialogView::maximumHeightChanged);
	connect(&m_window, &QuickWindow::autoCloseChanged, this, &DialogView::autoCloseChanged);
	connect(&m_window, &QuickWindow::closableChanged, this, &DialogView::closableChanged);
}

/*! The root item of the dialog. */
QQuickItem *DialogView::contentItem() const
{
	return m_contentItem;
}

void DialogView::setContentItem(QQuickItem *newContentItem)
{
	if(m_contentItem == newContentItem)
		return;
	m_contentItem = newContentItem;
	m_contentItem->setParentItem(m_window.contentItem());
	emit contentItemChanged();
}

/*! Whether the dialog is visible. */
bool DialogView::visible() const
{
	return m_window.isVisible();
}

void DialogView::setVisible(bool newVisible)
{
	m_window.setVisible(newVisible);
}

/*! The title of the dialog. */
QString DialogView::title() const
{
	return m_window.title();
}

void DialogView::setTitle(const QString &newTitle)
{
	m_window.setTitle(newTitle);
}

/*! Whether the dialog is modal. True by default. */
bool DialogView::modal() const
{
	return m_modal;
}

void DialogView::setModal(bool newModal)
{
	if(m_modal == newModal)
		return;
	m_modal = newModal;
	if(m_modal)
		m_window.setModality(Qt::ApplicationModal);
	else
		m_window.setModality(Qt::NonModal);
	emit modalChanged();
}

/*! The minimum width of the dialog. */
int DialogView::minimumWidth() const
{
	return m_window.minimumWidth();
}

void DialogView::setMinimumWidth(int newMinimumWidth)
{
	m_window.setMinimumWidth(newMinimumWidth);
}

/*! The minimum height of the dialog. */
int DialogView::minimumHeight() const
{
	return m_window.minimumHeight();
}

void DialogView::setMinimumHeight(int newMinimumHeight)
{
	m_window.setMinimumHeight(newMinimumHeight);
}

/*! The maximum width of the dialog. */
int DialogView::maximumWidth() const
{
	return m_window.maximumWidth();
}

void DialogView::setMaximumWidth(int newMaximumWidth)
{
	m_window.setMaximumWidth(newMaximumWidth);
}

/*! The maximum height of the dialog. */
int DialogView::maximumHeight() const
{
	return m_window.maximumHeight();
}

void DialogView::setMaximumHeight(int newMaximumHeight)
{
	m_window.setMaximumHeight(newMaximumHeight);
}

/*! Sets the state to maximized. */
void DialogView::setMaximizedState()
{
	m_window.setWindowStates(Qt::WindowMaximized);
}

/*! Sets the state to normal, i.e. neither maximized, minimized, nor fullscreen. */
void DialogView::setNormalState()
{
	m_window.setWindowState(Qt::WindowNoState);
}

/*! Shows the dialog as maximized. */
void DialogView::showMaximized()
{
	m_window.showMaximized();
}

/*! Shows the dialog as normal, i.e. neither maximized, minimized, nor fullscreen. */
void DialogView::showNormal()
{
	m_window.showNormal();
}

/*! Whether to close the dialog when escape is pressed. */
bool DialogView::autoClose() const
{
	return m_window.autoClose();
}

void DialogView::setAutoClose(bool newAutoClose)
{
	m_window.setAutoClose(newAutoClose);
}

/*! Whether to allow closing the dialog using the close button or escape key when autoClose is true. */
bool DialogView::closable() const
{
	return m_window.closable();
}

void DialogView::setClosable(bool newClosable)
{
	m_window.setClosable(newClosable);
}
