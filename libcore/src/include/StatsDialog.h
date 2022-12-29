/*
 * StatsDialog.h
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

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QDialog>
#include <QtCharts>
#include "ConfigParser.h"
#include "HistoryParser.h"
#include "Settings.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 1, 0)
using namespace QtCharts;
#endif

namespace Ui {
	class StatsDialog;
}

/*!
 * \brief The StatsDialog class is a dialog, which shows exercise history.
 * \image html StatsDialog.png
 */
class CORE_LIB_EXPORT StatsDialog : public QDialog
{
		Q_OBJECT
		Q_PROPERTY(QString packName READ packName WRITE setPackName NOTIFY packNameChanged)
		Q_PROPERTY(int lesson READ lesson WRITE setLesson NOTIFY lessonChanged)
		Q_PROPERTY(int sublesson READ sublesson WRITE setSublesson NOTIFY sublessonChanged)
		Q_PROPERTY(int exercise READ exercise WRITE setExercise NOTIFY exerciseChanged)
	public:
		explicit StatsDialog(bool offline, QList<QStringList> data, QPair<int, int> studentComparison, QString configName, int lesson, int sublesson, int exercise, QWidget *parent = nullptr);
		~StatsDialog();
		Q_INVOKABLE void loadData(void);
		void setPackName(QString name);
		QString packName(void);
		void setLesson(int lesson);
		int lesson(void);
		void setSublesson(int sublesson);
		int sublesson(void);
		void setExercise(int exercise);
		int exercise(void);

	private:
		Ui::StatsDialog *ui;
		QChart *speedChart, *mistakesChart, *timeChart;
		QChartView *speedChartView, *mistakesChartView, *timeChartView;
		QLineSeries *speedSeries, *mistakesSeries, *timeSeries;
		bool chartsCreated = false;
		QString m_packName;
		int m_lesson;
		int m_sublesson;
		int m_exercise;
		bool m_offline;
		QList<QStringList> m_data;
		QPair<int, int> m_comparison;

	signals:
		void packNameChanged(QString name);
		void lessonChanged(int lesson);
		void sublessonChanged(int sublesson);
		void exerciseChanged(int exercise);
};

#endif // STATSDIALOG_H
