/*
 * GradeCalculator.cpp
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

#include "GradeCalculator.h"

static const QString module = "grades";
static const QString appModule = "app";
static const ISettings::Key GRADE_NET_HITS(module, "gradeNetHits");
static const ISettings::Key GRADING_METHOD(module, "gradingMethod");
static const ISettings::Key GRADE_START_NUM(module, "gradeStartNumber");
static const ISettings::Key GRADE_END_NUM(module, "gradeEndNumber");
static const ISettings::Key GRADE_START_LETTER(module, "gradeStartLetter");
static const ISettings::Key GRADE_END_LETTER(module, "gradeEndLetter");
static const ISettings::Key ERROR_PENALTY(appModule, "errorPenalty");

/*! Constructs GradeCalculator. */
GradeCalculator::GradeCalculator()
{
	m_useNetHitsForGrading = settings()->getValue(GRADE_NET_HITS).toBool();
	m_gradingMethod = static_cast<ClassManager::GradingMethod>(settings()->getValue(GRADING_METHOD).toInt());
	connect(this, &GradeCalculator::targetHitsPerMinuteChanged, this, &GradeCalculator::updateGrade);
	connect(this, &GradeCalculator::useNetHitsForGradingChanged, this, &GradeCalculator::updateGrade);
	connect(this, &GradeCalculator::gradingMethodChanged, this, &GradeCalculator::updateGrade);
	connect(this, &GradeCalculator::validatorChanged, this, [this]() {
		connect(m_validator, &IExerciseValidator::validated, this, &GradeCalculator::updateGrade);
	});
}

/*! The ExerciseValidator used for the calculation. */
IExerciseValidator *GradeCalculator::validator() const
{
	return m_validator;
}

void GradeCalculator::setValidator(IExerciseValidator *newValidator)
{
	if(m_validator == newValidator)
		return;
	m_validator = newValidator;
	emit validatorChanged();
}

/*! Number of hits per minute for the best grade. */
int GradeCalculator::targetHitsPerMinute(void) const
{
	return m_targetHitsPerMinute;
}

void GradeCalculator::setTargetHitsPerMinute(int newTargetHitsPerMinute)
{
	if(m_targetHitsPerMinute == newTargetHitsPerMinute)
		return;
	m_targetHitsPerMinute = newTargetHitsPerMinute;
	emit targetHitsPerMinuteChanged();
}

/*!
 * Whether to use net hits per minute instead of gross hits for grading.\n
 * Default value: Settings#gradeNetHits()
 */
bool GradeCalculator::useNetHitsForGrading(void) const
{
	return m_useNetHitsForGrading;
}

void GradeCalculator::setUseNetHitsForGrading(bool newUseNetHitsForGrading)
{
	if(m_useNetHitsForGrading == newUseNetHitsForGrading)
		return;
	m_useNetHitsForGrading = newUseNetHitsForGrading;
	emit useNetHitsForGradingChanged();
}

/*!
 * The grading method, for example numbers or letters.\n
 * Default value: Settings#gradingMethod
 */
const ClassManager::GradingMethod &GradeCalculator::gradingMethod(void) const
{
	return m_gradingMethod;
}

void GradeCalculator::setGradingMethod(const ClassManager::GradingMethod &newGradingMethod)
{
	if(m_gradingMethod == newGradingMethod)
		return;
	m_gradingMethod = newGradingMethod;
	emit gradingMethodChanged();
}

/*! The grade. */
const QString &GradeCalculator::grade(void) const
{
	return m_grade;
}

/*! Calculates the grade and updates the grade property. */
void GradeCalculator::updateGrade(void)
{
	Q_ASSERT(m_validator);
	qreal hits = m_useNetHitsForGrading ? std::max(0, m_validator->grossHits() - m_validator->mistakeCount() * settings()->getValue(ERROR_PENALTY).toInt()) : m_validator->grossHits();
	qreal hpm = hits * (60 / m_validator->time());
	QVariantList grades;
	if(m_gradingMethod == ClassManager::GradingMethod_Numbers)
	{
		int start = settings()->getValue(GRADE_START_NUM).toInt();
		int end = settings()->getValue(GRADE_END_NUM).toInt();
		if(start < end)
		{
			for(int i = start; i <= end; i++)
				grades.append(i);
		}
		else
		{
			for(int i = start; i >= end; i--)
				grades.append(i);
		}
	}
	else if(m_gradingMethod == ClassManager::GradingMethod_Letters)
	{
		QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		int start = alphabet.indexOf(settings()->getValue(GRADE_START_LETTER).toChar());
		int end = alphabet.indexOf(settings()->getValue(GRADE_END_LETTER).toChar());
		Q_ASSERT(start >= 0);
		Q_ASSERT(end >= 0);
		if(start < end)
		{
			for(int i = start; i <= end; i++)
				grades.append(QChar(alphabet[i]));
		}
		else
		{
			for(int i = start; i >= end; i--)
				grades.append(QChar(alphabet[i]));
		}
	}
	Q_ASSERT(grades.length() != 0);
	int index = std::min(grades.length() - 1.0, hpm / m_targetHitsPerMinute * grades.length() - 1.0);
	if(index < 0)
		index = 0;
	QVariant grade = grades[index];
	if(m_gradingMethod == ClassManager::GradingMethod_Numbers)
		m_grade = QString::number(grade.toInt());
	else if(m_gradingMethod == ClassManager::GradingMethod_Letters)
		m_grade = grade.toChar();
	emit gradeChanged();
}
