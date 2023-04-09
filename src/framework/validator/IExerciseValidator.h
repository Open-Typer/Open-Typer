/*
 * IExerciseValidator.h
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

#ifndef IEXERCISEVALIDATOR_H
#define IEXERCISEVALIDATOR_H

#include <QObject>
#include "MistakeRecord.h"
#include "CharacterRecord.h"
#include "global/modularity/ioc.h"

class IExerciseValidator : public QObject, MODULE_EXPORT_INTERFACE
{
		Q_OBJECT
	public:
		virtual ~IExerciseValidator() { }

		virtual QString exerciseText(void) = 0;
		virtual void setExerciseText(QString text) = 0;
		virtual QString inputText(void) = 0;
		virtual void setInputText(QString text) = 0;
		virtual void clearMistakes(void) = 0;
		virtual void addMistake(MistakeRecord mistake) = 0;
		virtual QList<MistakeRecord> mistakes(void) = 0;
		virtual void setMistakes(QList<MistakeRecord> mistakeList) = 0;
		virtual void clearCharacters(void) = 0;
		virtual void addCharacter(CharacterRecord character) = 0;
		virtual QList<CharacterRecord> characters(void) = 0;
		virtual void setCharacters(QList<CharacterRecord> characterList) = 0;
		virtual bool timed(void) = 0;
		virtual void setTimed(bool value) = 0;
		virtual qreal time(void) = 0;
		virtual void setTime(qreal seconds) = 0;
		virtual void validate(void) = 0;
		virtual void validate(int grossHits, QStringList errorWords) = 0;
		virtual int grossHits(void) = 0;
		virtual int mistakeCount(void) = 0;
		virtual QStringList errorWords(void) = 0;
		virtual void generateMistakeText(bool correctMistakes) = 0;
		virtual QString generatedInputText(void) = 0;
		virtual QString generatedMistakeText(void) = 0;
		virtual QString textWithMistakes(void) = 0;

	signals:
		void validated();
		void exerciseTextChanged();
		void inputTextChanged();
		void mistakesChanged();
		void charactersChanged();
		void timedChanged();
		void timeChanged();
};

#endif // IEXERCISEVALIDATOR_H
