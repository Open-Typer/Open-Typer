/*
 * ExerciseSummary.h
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

#ifndef EXERCISESUMMARY_H
#define EXERCISESUMMARY_H

#include <QDialog>

namespace Ui {
	class ExerciseSummary;
}

/*!
 * \brief A summary dialog window, which shows the exercise result.
 *
 * Each value can be set using the functions.\n
 * They're all optional and unset values will be hidden.
 *
 * \image html ExerciseSummary.png
 */
class ExerciseSummary : public QDialog
{
		Q_OBJECT
	public:
		explicit ExerciseSummary(QWidget *parent = nullptr);
		~ExerciseSummary();
		Q_INVOKABLE void setTotalTime(int time);
		Q_INVOKABLE void setTotalHits(int hits);
		Q_INVOKABLE void setNetHits(int hits);
		Q_INVOKABLE void setGrossHits(int hits);
		Q_INVOKABLE void setMistakes(int mistakes);
		Q_INVOKABLE void setAccuracy(double accuracy);

	private:
		Ui::ExerciseSummary *ui;
};

#endif // EXERCISESUMMARY_H
