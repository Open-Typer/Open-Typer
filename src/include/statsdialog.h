/*
 * statsdialog.h
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

#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>
#include <QtCharts>
#include "core/net.h"

using namespace QtCharts;

namespace Ui {
	class statsDialog;
}

/*!
 * \brief The statsDialog class is a dialog, which shows exercise history.
 * \image html statsDialog.png
 */
class statsDialog : public QDialog
{
	Q_OBJECT
	public:
		explicit statsDialog(monitorClient *client, QString configName, int lesson, int sublesson, int exercise, QWidget *parent = nullptr);
		~statsDialog();

	private:
		Ui::statsDialog *ui;
		QChart *speedChart, *mistakesChart, *timeChart;
		QLineSeries *speedSeries, *mistakesSeries, *timeSeries;
};

#endif // STATSDIALOG_H
