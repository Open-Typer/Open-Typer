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

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include "MistakeRecord.h"
#include "CharacterRecord.h"

/*! \brief The ExerciseValidator class provides methods for exercise validation to QML code. */
class CORE_LIB_EXPORT ExerciseValidator : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QString exerciseText READ exerciseText WRITE setExerciseText NOTIFY exerciseTextChanged)
		Q_PROPERTY(QString inputText READ inputText WRITE setInputText NOTIFY inputTextChanged)
		Q_PROPERTY(QList<MistakeRecord> mistakes READ mistakes WRITE setMistakes NOTIFY mistakesChanged)
		Q_PROPERTY(QList<CharacterRecord> characters READ characters WRITE setCharacters NOTIFY charactersChanged)
		Q_PROPERTY(bool timed READ isTimed WRITE setTimed NOTIFY timedChanged)
		Q_PROPERTY(int time READ time WRITE setTime NOTIFY timeChanged)
	public:
		void setExerciseText(QString text);
		QString exerciseText(void);
		void setInputText(QString text);
		QString inputText(void);
		void setMistakes(QList<MistakeRecord> mistakeList);
		Q_INVOKABLE void clearMistakes(void);
		Q_INVOKABLE void addMistake(MistakeRecord mistake);
		QList<MistakeRecord> mistakes(void);
		void setCharacters(QList<CharacterRecord> characterList);
		Q_INVOKABLE void clearCharacters(void);
		Q_INVOKABLE void addCharacter(CharacterRecord character);
		QList<CharacterRecord> characters(void);
		void setTimed(bool value);
		bool isTimed(void);
		void setTime(qreal seconds);
		qreal time(void);
		Q_INVOKABLE void validate(void);
		Q_INVOKABLE int grossHits(void);
		Q_INVOKABLE int mistakeCount(void);
		Q_INVOKABLE QStringList errorWords(void);
		Q_INVOKABLE void generateMistakeText(bool correctMistakes);
		Q_INVOKABLE QString generatedInputText(void);
		Q_INVOKABLE QString generatedMistakeText(void);
		Q_INVOKABLE QString textWithMistakes(void);

	private:
		QString m_exerciseText;
		QString m_inputText;
		QList<MistakeRecord> m_mistakes;
		QList<CharacterRecord> m_characters;
		bool m_isTimed = false;
		qreal m_time = 0;
		int m_grossHits = 0;
		int m_mistakeCount = 0;
		QStringList m_errorWords;
		QString m_generatedInputText;
		QString m_generatedMistakeText;

	signals:
		void exerciseTextChanged(QString text);
		void inputTextChanged(QString text);
		void mistakesChanged(QList<MistakeRecord> mistakeList);
		void charactersChanged(QList<CharacterRecord> characterList);
		void timedChanged(bool value);
		void timeChanged(qreal seconds);
};

#endif // EXERCISEVALIDATOR_H
