/*
 * GradeCalculator.h
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

#ifndef GRADECALCULATOR_H
#define GRADECALCULATOR_H

#include <QObject>
#include "ClassManager.h"
#include "global/ISettings.h"
#include "validator/IExerciseValidator.h"

/*! \brief The GradeCalculator class provides methods for grades. \since Open-Typer 5.1.0 */
class GradeCalculator : public QObject
{
		Q_OBJECT
		INJECT(ISettings, settings)
		Q_PROPERTY(IExerciseValidator *validator READ validator WRITE setValidator NOTIFY validatorChanged)
		Q_PROPERTY(int targetHitsPerMinute READ targetHitsPerMinute WRITE setTargetHitsPerMinute NOTIFY targetHitsPerMinuteChanged)
		Q_PROPERTY(bool useNetHitsForGrading READ useNetHitsForGrading WRITE setUseNetHitsForGrading NOTIFY useNetHitsForGradingChanged)
		Q_PROPERTY(ClassManager::GradingMethod gradingMethod READ gradingMethod WRITE setGradingMethod NOTIFY gradingMethodChanged)
		Q_PROPERTY(QString grade READ grade NOTIFY gradeChanged)
	public:
		GradeCalculator();
		IExerciseValidator *validator() const;
		void setValidator(IExerciseValidator *newValidator);
		int targetHitsPerMinute(void) const;
		void setTargetHitsPerMinute(int newTargetHitsPerMinute);
		bool useNetHitsForGrading(void) const;
		void setUseNetHitsForGrading(bool newUseNetHitsForGrading);
		const ClassManager::GradingMethod &gradingMethod(void) const;
		void setGradingMethod(const ClassManager::GradingMethod &newGradingMethod);
		const QString &grade(void) const;

	private:
		IExerciseValidator *m_validator = nullptr;
		void updateGrade(void);
		int m_targetHitsPerMinute = 0;
		bool m_useNetHitsForGrading = true;
		ClassManager::GradingMethod m_gradingMethod = ClassManager::GradingMethod_Numbers;
		QString m_grade;

	signals:
		void validatorChanged();
		void targetHitsPerMinuteChanged();
		void useNetHitsForGradingChanged();
		void gradingMethodChanged();
		void gradeChanged();
};

#endif // GRADECALCULATOR_H
