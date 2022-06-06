/*
 * testwaitdialog.h
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

#ifndef TESTWAITDIALOG_H
#define TESTWAITDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QScrollBar>
#include "core/net.h"
#include "core/theme.h"

namespace Ui {
	class testWaitDialog;
}

/*!
 * \brief The testWaitDialog class is a dialog, which is displayed when client receives an exercise.
 * \image html testWaitDialog.png
 */
class testWaitDialog : public QDialog
{
	Q_OBJECT
	public:
		explicit testWaitDialog(monitorClient *client, QWidget *parent = nullptr);
		~testWaitDialog();
		QString name(void);
		void setName(QString name);
		bool nameReadOnly(void);
		void setNameReadOnly(bool readOnly);
		void setText(QString text);

	private:
		Ui::testWaitDialog *ui;
		monitorClient *m_client;
		QFont font;

	protected:
		void closeEvent(QCloseEvent *event);
};

#endif // TESTWAITDIALOG_H
