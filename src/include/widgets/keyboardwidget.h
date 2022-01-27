/*
 * keyboardwidget.h
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

#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QFrame>
#include <QLayout>
#include <QLabel>
#include <QMap>
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsColorizeEffect>
#include <QTimer>

/*!
 * \brief The keyboardWidget class provides a simple virtual keyboard widget.
 *
 * \image html keyboardWidget.png
 */
class keyboardWidget : public QFrame
{
	Q_OBJECT
	public:
		explicit keyboardWidget(QWidget *parent = nullptr);
		void setKeyStyleSheet(QString styleSheet);
		void loadLayout(QLocale::Language language, QLocale::Country country, QString variant);
		void highlightKey(int keyCode);
		void dehighlightKey(int keyCode);

	private:
		QVBoxLayout *mainLayout;
		QMultiMap<QFrame*,int> keys;
		QMap<QFrame*,QLabel*> keyLabels;
		QMap<QPair<int,int>,QFrame*> keyMap;
		int currentRow, currentColumn;
		QHBoxLayout *currentRowLayout = nullptr;
		void addKey(QString keyLabelText = "", int keyCode = -1, int keyMinimumWidth = 50);
		void nextRow(void);
		void registerKey(int row, int column, QString keyLabelText, int keyCode, int shiftKeyCode);
};

#endif // KEYBOARDWIDGET_H
