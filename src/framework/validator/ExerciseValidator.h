/*
 * ExerciseValidator.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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

#ifndef EXERCISEVALIDATOR_H
#define EXERCISEVALIDATOR_H

#include <QObject>
#include "MistakeRecord.h"
#include "CharacterRecord.h"
#include "grades/ClassManager.h"

/*! \brief The ExerciseValidator class provides methods for exercise validation to QML code. */
class Q_DECL_EXPORT ExerciseValidator : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString exerciseText READ exerciseText WRITE setExerciseText NOTIFY exerciseTextChanged)
		Q_PROPERTY(QString inputText READ inputText WRITE setInputText NOTIFY inputTextChanged)
		Q_PROPERTY(QList<MistakeRecord> mistakes READ mistakes WRITE setMistakes NOTIFY mistakesChanged)
		Q_PROPERTY(QList<CharacterRecord> characters READ characters WRITE setCharacters NOTIFY charactersChanged)
		Q_PROPERTY(bool timed READ timed WRITE setTimed NOTIFY timedChanged)
		Q_PROPERTY(qreal time READ time WRITE setTime NOTIFY timeChanged)
		Q_PROPERTY(int targetHitsPerMinute READ targetHitsPerMinute WRITE setTargetHitsPerMinute NOTIFY targetHitsPerMinuteChanged)
		Q_PROPERTY(bool useNetHitsForGrading READ useNetHitsForGrading WRITE setUseNetHitsForGrading NOTIFY useNetHitsForGradingChanged)
		Q_PROPERTY(ClassManager::GradingMethod gradingMethod READ gradingMethod WRITE setGradingMethod NOTIFY gradingMethodChanged)
		Q_PROPERTY(QString grade READ grade NOTIFY gradeChanged)
	public:
		ExerciseValidator(QObject *parent = nullptr);
		QString exerciseText(void);
		void setExerciseText(QString text);
		QString inputText(void);
		void setInputText(QString text);
		Q_INVOKABLE void clearMistakes(void);
		Q_INVOKABLE void addMistake(MistakeRecord mistake);
		QList<MistakeRecord> mistakes(void);
		void setMistakes(QList<MistakeRecord> mistakeList);
		Q_INVOKABLE void clearCharacters(void);
		Q_INVOKABLE void addCharacter(CharacterRecord character);
		QList<CharacterRecord> characters(void);
		void setCharacters(QList<CharacterRecord> characterList);
		bool timed(void);
		Q_DECL_DEPRECATED bool isTimed(void);
		void setTimed(bool value);
		qreal time(void);
		void setTime(qreal seconds);
		Q_INVOKABLE void validate(void);
		Q_INVOKABLE void validate(int grossHits, QStringList errorWords);
		Q_INVOKABLE int grossHits(void);
		Q_INVOKABLE int mistakeCount(void);
		Q_INVOKABLE QStringList errorWords(void);
		Q_INVOKABLE void generateMistakeText(bool correctMistakes);
		Q_INVOKABLE QString generatedInputText(void);
		Q_INVOKABLE QString generatedMistakeText(void);
		Q_INVOKABLE QString textWithMistakes(void);
		int targetHitsPerMinute(void) const;
		void setTargetHitsPerMinute(int newTargetHitsPerMinute);
		bool useNetHitsForGrading(void) const;
		void setUseNetHitsForGrading(bool newUseNetHitsForGrading);
		const ClassManager::GradingMethod &gradingMethod(void) const;
		void setGradingMethod(const ClassManager::GradingMethod &newGradingMethod);
		const QString &grade(void) const;

	private:
		void updateGrade(void);
		QList<MistakeRecord> compareLists(QList<QVariant> source, QList<QVariant> target, QVector<CharacterRecord> *recordedCharacters = nullptr, int *hits = nullptr, int *inputPos = nullptr);
		QList<MistakeRecord> compareStrings(QString source, QString target, QVector<CharacterRecord> *recordedCharacters = nullptr, int *hits = nullptr, int *inputPos = nullptr);
		QMap<int, MistakeRecord> generateDiffList(QStringList *sourceWords, QStringList *targetWords, QList<int> *mergeList = nullptr);
		QList<MistakeRecord> findMistakes(QString exerciseText, QString input, QVector<CharacterRecord> recordedCharacters, int *totalHits = nullptr, QStringList *errorWords = nullptr);
		QList<MistakeRecord> validateExercise(QString exerciseText, QString inputText, QVector<CharacterRecord> recordedCharacters, int *totalHits, int *mistakeCount, QStringList *errorWords = nullptr, bool timed = false, int timeSecs = 0);
		QString addMistakes(QString exerciseText, QList<MistakeRecord *> recordedMistakes);
		QStringList splitWordsByPunct(QStringList source);
		QString m_exerciseText;
		QString m_inputText;
		QList<MistakeRecord> m_mistakes;
		QList<CharacterRecord> m_characters;
		bool m_timed = false;
		qreal m_time = 0;
		int m_grossHits = 0;
		int m_mistakeCount = 0;
		QStringList m_errorWords;
		QString m_generatedInputText;
		QString m_generatedMistakeText;
		int m_targetHitsPerMinute;
		bool m_useNetHitsForGrading;
		ClassManager::GradingMethod m_gradingMethod;
		QString m_grade;

	signals:
		void exerciseTextChanged(QString text);
		void inputTextChanged(QString text);
		void mistakesChanged(QList<MistakeRecord> mistakeList);
		void charactersChanged(QList<CharacterRecord> characterList);
		void timedChanged(bool value);
		void timeChanged(qreal seconds);
		void targetHitsPerMinuteChanged();
		void useNetHitsForGradingChanged();
		void gradingMethodChanged();
		void gradeChanged();
};

#endif // EXERCISEVALIDATOR_H
