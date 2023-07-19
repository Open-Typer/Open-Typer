/*
 * PackEditorModel.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

#ifndef PACKEDITORMODEL_H
#define PACKEDITORMODEL_H

#include <QObject>
#include "lessonpack/IConfigParser.h"
#include "translations/ILanguageManager.h"

class PackEditorModel : public QObject
{
		Q_OBJECT
		INJECT(IConfigParser, configParser)
		INJECT(ILanguageManager, languageManager)
		Q_PROPERTY(IConfigParser *parser READ parser NOTIFY parserChanged)
		Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
		Q_PROPERTY(bool opened READ opened NOTIFY openedChanged)
		Q_PROPERTY(bool saved READ saved NOTIFY savedChanged)
		Q_PROPERTY(bool readOnly READ readOnly NOTIFY readOnlyChanged)
		Q_PROPERTY(int lesson READ lesson WRITE setLesson NOTIFY lessonChanged)
		Q_PROPERTY(int sublesson READ sublesson WRITE setSublesson NOTIFY sublessonChanged)
		Q_PROPERTY(int exercise READ exercise WRITE setExercise NOTIFY exerciseChanged)
		Q_PROPERTY(QStringList lessonList READ lessonList NOTIFY lessonListChanged)
		Q_PROPERTY(QStringList sublessonList READ sublessonList NOTIFY sublessonListChanged)
		Q_PROPERTY(QStringList exerciseList READ exerciseList NOTIFY exerciseListChanged)
		Q_PROPERTY(QString currentText READ currentText NOTIFY currentTextChanged)
	public:
		explicit PackEditorModel(QObject *parent = nullptr);
		~PackEditorModel();

		IConfigParser *parser() const;

		const QString &fileName() const;
		void setFileName(const QString &newFileName);

		bool opened() const;

		bool saved() const;

		bool readOnly() const;

		int lesson() const;
		void setLesson(int newLesson);

		int sublesson() const;
		void setSublesson(int newSublesson);

		int exercise() const;
		void setExercise(int newExercise);

		const QStringList &lessonList() const;

		const QStringList &sublessonList() const;

		const QStringList &exerciseList() const;

		QString currentText() const;

		Q_INVOKABLE void open();

		Q_INVOKABLE void nextExercise();
		Q_INVOKABLE void previousExercise();

	private:
		void nextSublesson();
		void previousSublesson();
		void nextLesson(bool updateExList = true);
		void previousLesson(bool updateExList = true);
		void updateLists();
		void updateLessonList();
		void updateSublessonList();
		void updateExerciseList();
		void updateAbsoluteSublesson();

		std::shared_ptr<IConfigParser> m_parser = nullptr;
		QString m_fileName;
		bool m_opened = false;
		bool m_saved = false;
		bool m_readOnly = false;
		int m_lesson = 0;
		int m_sublesson = 0;
		int m_absoluteSublesson = 0;
		int m_exercise = 0;
		QStringList m_lessonList;
		QStringList m_sublessonList;
		QStringList m_exerciseList;

	signals:
		void openedChanged();
		void parserChanged();
		void fileNameChanged();
		void savedChanged();
		void readOnlyChanged();
		void lessonChanged();
		void sublessonChanged();
		void exerciseChanged();
		void lessonListChanged();
		void sublessonListChanged();
		void exerciseListChanged();
		void currentTextChanged();
};

#endif // PACKEDITORMODEL_H
