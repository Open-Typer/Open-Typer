/*
 * levelsummary.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

#ifndef LEVELSUMMARY_H
#define LEVELSUMMARY_H

#include <QDialog>

namespace Ui {
class levelSummary;
}

/*!
 * \brief A summary dialog window, which shows the exercise result.
 *
 * Each value can be set using the functions.\n
 * They're all optional and unset values will be hidden.
 * \c AlignLeft
 * \image html levelSummary.png
 *
 * The dialog supports 2 modes.\n
 * The first mode asks the user whether to continue to the next exercise or repeat it.\n
 * The second mode shows the summary with an OK button. It can be enabled by calling showOK().
 */
class levelSummary : public QDialog
{
	Q_OBJECT

public:
	explicit levelSummary(QWidget *parent = nullptr);
	~levelSummary();
	void showOK(void);
	void setTotalTime(double time);
	void setHitCount(int hits);
	void setHits(int hits);
	void setMistakes(int mistakes);

private:
	Ui::levelSummary *ui;

};

#endif // LEVELSUMMARY_H
