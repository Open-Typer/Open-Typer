/*
 * TextView.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <QWidget>
#include <QTextEdit>
#include <QWheelEvent>
#include <QLayout>

/*!
 * \brief The TextView class is a QTextEdit used to display text.
 *
 * It contains modifications that ignore mouse wheel events and make the widget resize using fixed size according to the text document size.
 */
class TextView : public QTextEdit
{
		Q_OBJECT
	public:
		explicit TextView(QWidget *parent = nullptr);
		~TextView();
		void setHorizontalAdjust(bool value);
		void setVerticalAdjust(bool value);

	private:
		bool horizontalAdjust = true, verticalAdjust = true;

	private slots:
		void updateWidgetSize(void);

	protected:
		void changeEvent(QEvent *event);
		void wheelEvent(QWheelEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
};

#endif // TEXTVIEW_H
