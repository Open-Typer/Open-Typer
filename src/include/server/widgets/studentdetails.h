/*
 * studentdetails.h
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

#ifndef STUDENTDETAILS_H
#define STUDENTDETAILS_H

#include <QWidget>
#include <QtCharts>
#include "core/database.h"
#include "core/configfile.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 1, 0)
using namespace QtCharts;
#endif

namespace Ui {
	class studentDetails;
}

/*!
 * \brief The studentDetails class is a widget for student results and statistics, used by MonitorWindow.
 *
 * \image html studentDetails.png
 */
class studentDetails : public QWidget
{
	Q_OBJECT
	public:
		explicit studentDetails(int openClassID, int id, QWidget *parent = nullptr);
		~studentDetails();

	private:
		Ui::studentDetails *ui;
		void refreshCharts(void);
		int classID, studentID;
		QChart *speedChart, *mistakesChart, *timeChart;
		QLineSeries *speedSeries, *mistakesSeries, *timeSeries;

	protected:
		void changeEvent(QEvent *event);

	signals:
		/*! A signal, which is emitted when the back button is clicked. */
		void backClicked();

	private slots:
		void refresh(void);
		void refreshTable(void);
};

#endif // STUDENTDETAILS_H
