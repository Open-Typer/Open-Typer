/*
 * exportdialog.h
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

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QDate>

#ifndef Q_OS_WASM
#include <QPrinter>
#include <QPrintDialog>
#endif // Q_OS_WASM

#include <QPainter>
#include "core/theme.h"

namespace Ui {
	class exportDialog;
}

/*!
 * \brief The exportDialog class is a dialog for typewriting result exports.
 *
 * \image html exportDialog.png
 */
class exportDialog : public QDialog
{
	Q_OBJECT
	public:
		explicit exportDialog(QString text, QVariantMap result, QWidget *parent = nullptr);
		~exportDialog();

	private:
		Ui::exportDialog *ui;
		QString inputText;
		QVariantMap performanceResult;

	private slots:
		void updateTable(void);
		void printResult(void);
};

#endif // EXPORTDIALOG_H
