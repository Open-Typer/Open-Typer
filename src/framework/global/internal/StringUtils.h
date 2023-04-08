/*
 * StringUtils.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2023 - adazem009
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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QVariant>
#include "../IStringUtils.h"

/*! \brief The StringUtils class contains functions related to strings. */
class Q_DECL_EXPORT StringUtils : public QObject, public IStringUtils
{
		Q_OBJECT
	public:
		static std::shared_ptr<StringUtils> instance();
		Q_INVOKABLE int wordCount(QString str) override;
		Q_INVOKABLE int charCount(QString str, QChar ch) override;
		Q_INVOKABLE QString word(QString str, int id) override;
		Q_INVOKABLE QString wordAt(QString str, int index) override;
		Q_INVOKABLE QString repeatString(QString string, int n) override;
		Q_INVOKABLE QList<QVariant> longestCommonSubsequence(QList<QVariant> source, QList<QVariant> target) override;
		Q_INVOKABLE QString longestCommonSubsequence(QString source, QString target) override;
		Q_INVOKABLE QString normalizeString(QString str) override;

	private:
		static std::shared_ptr<StringUtils> m_instance;
		static int lcsLen(QList<QVariant> source, QList<QVariant> target);
		static int lcsLen(QList<QVariant> source, QList<QVariant> target, QMap<int, QMap<int, int>> *lcsTable);
};

#endif // STRINGUTILS_H
