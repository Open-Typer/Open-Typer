/*
 * classcontrols.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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

#ifndef CLASSCONTROLS_H
#define CLASSCONTROLS_H

#include <QWidget>
#include <QtCharts>
#include "server/studentedit.h"
#include "server/loadexercisedialog.h"
#include "paperconfigdialog.h"
#include "core/database.h"
#include "core/configfile.h"
#ifndef Q_OS_WASM
#include "core/server.h"
#endif // Q_OS_WASM

#if QT_VERSION < QT_VERSION_CHECK(6, 1, 0)
using namespace QtCharts;
#endif

namespace Ui {
	class classControls;
}

/*!
 * \brief The classControls class is a widget for class management.
 *
 * \image html classControls.png
 */
class classControls : public QWidget
{
	Q_OBJECT
	public:
		explicit classControls(int openClassID, QWidget *parent = nullptr);
		~classControls();

	private:
		Ui::classControls *ui;
		int classID;
		QChart *speedChart, *mistakesChart, *timeChart;
		void setupTable(void);

	protected:
		void changeEvent(QEvent *event);

	signals:
		/*! A signal, which is emitted when the details button is clicked. \see openDetails() */
		void detailsClicked(int);

	private slots:
		void verify(void);
		void addStudent(void);
		void removeStudent(void);
		void editStudent(void);
		void openDetails(void);
		void refreshCharts(void);
		void loadExercise(void);
};

#endif // CLASSCONTROLS_H
