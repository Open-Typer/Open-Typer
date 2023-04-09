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
#include "../IExerciseValidator.h"
#include "MistakeRecord.h"
#include "CharacterRecord.h"
#include "global/ISettings.h"
#include "global/IStringUtils.h"

/*! \brief The ExerciseValidator class provides methods for exercise validation to QML code. */
class Q_DECL_EXPORT ExerciseValidator : public IExerciseValidator
{
		Q_OBJECT
		INJECT(ISettings, settings)
		INJECT(IStringUtils, stringUtils)
		Q_PROPERTY(QString exerciseText READ exerciseText WRITE setExerciseText NOTIFY exerciseTextChanged)
		Q_PROPERTY(QString inputText READ inputText WRITE setInputText NOTIFY inputTextChanged)
		Q_PROPERTY(QList<MistakeRecord> mistakes READ mistakes WRITE setMistakes NOTIFY mistakesChanged)
		Q_PROPERTY(QList<CharacterRecord> characters READ characters WRITE setCharacters NOTIFY charactersChanged)
		Q_PROPERTY(bool timed READ timed WRITE setTimed NOTIFY timedChanged)
		Q_PROPERTY(qreal time READ time WRITE setTime NOTIFY timeChanged)
	public:
		QString exerciseText(void) override;
		void setExerciseText(QString text) override;
		QString inputText(void) override;
		void setInputText(QString text) override;
		Q_INVOKABLE void clearMistakes(void) override;
		Q_INVOKABLE void addMistake(MistakeRecord mistake) override;
		QList<MistakeRecord> mistakes(void) override;
		void setMistakes(QList<MistakeRecord> mistakeList) override;
		Q_INVOKABLE void clearCharacters(void) override;
		Q_INVOKABLE void addCharacter(CharacterRecord character) override;
		QList<CharacterRecord> characters(void) override;
		void setCharacters(QList<CharacterRecord> characterList) override;
		bool timed(void) override;
		void setTimed(bool value) override;
		qreal time(void) override;
		void setTime(qreal seconds) override;
		Q_INVOKABLE void validate(void) override;
		Q_INVOKABLE void validate(int grossHits, QStringList errorWords) override;
		Q_INVOKABLE int grossHits(void) override;
		Q_INVOKABLE int mistakeCount(void) override;
		Q_INVOKABLE QStringList errorWords(void) override;
		Q_INVOKABLE void generateMistakeText(bool correctMistakes) override;
		Q_INVOKABLE QString generatedInputText(void) override;
		Q_INVOKABLE QString generatedMistakeText(void) override;
		Q_INVOKABLE QString textWithMistakes(void) override;

	private:
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
};

#endif // EXERCISEVALIDATOR_H
