/*
 * opentyper.h
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

#ifndef OPENTYPER_H
#define OPENTYPER_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QElapsedTimer>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class OpenTyper; }
QT_END_NAMESPACE

class OpenTyper : public QMainWindow
{
	Q_OBJECT

public:
	void updateColors(void);
	OpenTyper(QWidget *parent = nullptr);
	~OpenTyper();
	void loadConfigs(void);
	char *loadConfig(QString configName);
	void startLevel(FILE *cr, int lesson, int sublesson, int level);
	QString level, displayLevel, input, displayInput, publicConfigName;
	int lessonCount, sublessonCount, levelCount, currentLesson, currentSublesson, currentLevel, levelPos, displayPos, levelMistakes;
	QElapsedTimer levelTimer;
	bool levelInProgress, mistake;
	int lastTime;
	QString getConfigLoc(void);
	bool isSpecialKey(QKeyEvent *event);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	Ui::OpenTyper *ui;

private slots:
    void packListIndexChanged(int index);
    void repeatLevel(void);
    void nextLevel(void);
    void previousLevel(void);
    void updateCurrentTime(void);
    void lessonSelectionListIndexChanged(int index);
    void sublessonSelectionListIndexChanged(int index);
    void levelSelectionListIndexChanged(int index);
    void randomOrderCheckBoxChanged(bool checked);
};
#endif // OPENTYPER_H
