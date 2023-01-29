/*
 * ExportWidget.cpp
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

#include "ExportProvider.h"

/*! Constructs ExportProvider. */
ExportProvider::ExportProvider(QObject *parent) :
	QObject(parent) { }

/*! The ExerciseValidator used to get the input text and mistakes. */
ExerciseValidator *ExportProvider::validator(void)
{
	return m_validator;
}

void ExportProvider::setValidator(ExerciseValidator *validator)
{
	m_validator = validator;
	if(!validator)
		return;
	QString inputText = validator->inputText();
	auto mistakes = validator->mistakes();
	QString finalText = "";
	QStringList lines = inputText.split('\n');
	int longestLineLength = 0;
	for(int i = 0; i < lines.count(); i++)
	{
		if(lines[i].count() > longestLineLength)
			longestLineLength = lines[i].count();
	}
	QMap<int, MistakeRecord *> mistakesMap;
	for(int i = 0; i < mistakes.length(); i++)
		mistakesMap[mistakes[i]->position()] = mistakes[i];
	int pos = 0;
	for(int i = 0; i < lines.count(); i++)
	{
		QString line = lines[i];
		int lineMistakes = 0;
		for(int i2 = 0; i2 <= line.count(); i2++)
		{
			QString append;
			if(i2 < line.count())
				append = QString(line[i2]).toHtmlEscaped().replace(" ", "&nbsp;");
			else
				append = "";
			if(mistakesMap.contains(pos))
			{
				MistakeRecord *currentMistake = mistakesMap[pos];
				if(currentMistake->isEnabled())
					lineMistakes++;
				if(append == "")
					append.prepend("&nbsp;");
				finalText += "<u>" + append + "</u>";
			}
			else
				finalText += append;
			pos++;
		}
		finalText += QString("&nbsp;").repeated(longestLineLength - line.count() + 4) + QString("/").repeated(lineMistakes);
		finalText += "<br>";
	}
	m_exportText = finalText;
	emit exportTextChanged(m_exportText);
	emit validatorChanged(validator);
}

/*! The exported input text. */
QString ExportProvider::exportText(void)
{
	return m_exportText;
}

/*! The ExportTable that is going to be printed. */
ExportTable *ExportProvider::table(void)
{
	return m_table;
}

void ExportProvider::setTable(ExportTable *table)
{
	m_table = table;
	emit tableChanged(table);
}
