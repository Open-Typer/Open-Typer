/*
 * simplecolordialog.h
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

#ifndef SIMPLECOLORDIALOG_H
#define SIMPLECOLORDIALOG_H

#include <QDialog>

namespace Ui {
class SimpleColorDialog;
}

class SimpleColorDialog : public QDialog
{
	Q_OBJECT
	public:
		explicit SimpleColorDialog(QWidget *parent = nullptr);
		~SimpleColorDialog();
		int redColor, greenColor, blueColor;
		void setColor(int r, int g, int b);
		void updateColor(void);

	private:
		Ui::SimpleColorDialog *ui;
	
	private slots:
		void setRedColorValue(int val);
		void setGreenColorValue(int val);
		void setBlueColorValue(int val);
		void setRedColor(void);
		void setBrownColor(void);
		void setOrangeColor(void);
		void setYellowColor(void);
		void setGreenColor(void);
		void setLightBlueColor(void);
		void setBlueColor(void);
		void setPinkColor(void);
		void setPurpleColor(void);
		void setBlackColor(void);
		void setGrayColor(void);
		void setWhiteColor(void);
};

#endif // SIMPLECOLORDIALOG_H
