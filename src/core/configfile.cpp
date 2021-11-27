/*
 * configfile.cpp
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

#include "core/configfile.h"

configParser::configParser(QObject *parent) :
	QObject(parent)
{
	configFile = new QFile;
}

bool configParser::open(const QString fileName)
{
	configFile->close();
	configFile->setFileName(fileName);
	return configFile->open(QIODevice::ReadOnly | QIODevice::Text);
}

bool configParser::reopen(QIODevice::OpenMode mode)
{
	if(configFile->fileName() == "")
		return false;
	configFile->close();
	return configFile->open(mode);
}

int configParser::lessonCount(void)
{
	configFile->seek(0);
	QTextStream fileStream(configFile);
	fileStream.setCodec("UTF-8");
	QList<int> lessonIDs;
	while(!fileStream.atEnd())
	{
		QString line = fileStream.readLine();
		int lessonID = exerciseID(line,1);
		if(!lessonIDs.contains(lessonID))
			lessonIDs += lessonID;
	}
	return lessonIDs.count();
}

int configParser::sublessonCount(int lesson)
{
	configFile->seek(0);
	QTextStream fileStream(configFile);
	fileStream.setCodec("UTF-8");
	QList<int> sublessonIDs;
	while(!fileStream.atEnd())
	{
		QString line = fileStream.readLine();
		int lessonID = exerciseID(line,1);
		if(lessonID == lesson)
		{
			int sublessonID = exerciseID(line,2);
			if(!sublessonIDs.contains(sublessonID))
				sublessonIDs += sublessonID;
		}
	}
	return sublessonIDs.count();
}

int configParser::exerciseCount(int lesson, int sublesson)
{
	configFile->seek(0);
	QTextStream fileStream(configFile);
	fileStream.setCodec("UTF-8");
	QList<int> exerciseIDs;
	while(!fileStream.atEnd())
	{
		QString line = fileStream.readLine();
		int lessonID = exerciseID(line,1);
		if(lessonID == lesson)
		{
			int sublessonID = exerciseID(line,2);
			if(sublessonID == sublesson)
			{
				int _exerciseID = exerciseID(line,3);
				if(!exerciseIDs.contains(_exerciseID))
					exerciseIDs += _exerciseID;
			}
		}
	}
	return exerciseIDs.count();
}

int configParser::exerciseLine(int lesson, int sublesson, int exercise)
{
	configFile->seek(0);
	QTextStream fileStream(configFile);
	fileStream.setCodec("UTF-8");
	int lineID = 0;
	while(!fileStream.atEnd())
	{
		lineID++;
		QString line = fileStream.readLine();
		if((exerciseID(line,1) == lesson) && (exerciseID(line,2) == sublesson) && (exerciseID(line,3) == exercise))
			return lineID;
	}
	return 0;
}

bool configParser::exerciseRepeatBool(int lesson, int sublesson, int exercise)
{
	return exerciseRepeatBool(exerciseRepeatConfig(lineOf(lesson,sublesson,exercise)));
}

QString configParser::exerciseRepeatType(int lesson, int sublesson, int exercise)
{
	return exerciseRepeatType(exerciseRepeatConfig(lineOf(lesson,sublesson,exercise)));
}

int configParser::exerciseRepeatLimit(int lesson, int sublesson, int exercise)
{
	return exerciseAttribute(exerciseAttributes(lineOf(lesson,sublesson,exercise)),0).toInt();
}

int configParser::exerciseLineLength(int lesson, int sublesson, int exercise)
{
	return exerciseAttribute(exerciseAttributes(lineOf(lesson,sublesson,exercise)),1).toInt();
}

QString configParser::lessonDesc(int lesson)
{
	configFile->seek(0);
	QTextStream fileStream(configFile);
	fileStream.setCodec("UTF-8");
	while(!fileStream.atEnd())
	{
		QString line = fileStream.readLine();
		if(exerciseID(line,1) == lesson)
		{
			QString out = exerciseAttribute(exerciseAttributes(line),2);
			if(out != "")
				return out;
		}
	}
	return "";
}

QString configParser::parseDesc(QString desc)
{
	QString out = "";
	int i;
	bool bracket=false;
	for(i=0; i < desc.count(); i++)
	{
		if(desc[i] == '%')
		{
			i++;
			if(bracket)
				out += '}';
			if(desc[i] == 'r')
				out += QObject::tr("Revision");
			else if(desc[i] == '%')
				out += '%';
			// %b is reserved (it's used to separate 2 sets)
			if(desc[i] == 's')
			{
				if(!bracket)
				{
					out += '{';
					bracket=true;
				}
				out += QObject::tr("Shift","Shift key");
			}
			else
				bracket=false;
		}
		else
		{
			if(!bracket)
			{
				out += '{';
				bracket=true;
			}
			out += desc[i];
		}
	}
	if(bracket)
		out += '}';
	return out;
}

QString configParser::sublessonName(int id)
{
	switch(id) {
		case 1:
			return QObject::tr("Touch");
			break;
		case 2:
			return QObject::tr("Words");
			break;
		case 3:
			return QObject::tr("Sentences");
			break;
		case 4:
			return QObject::tr("Text");
			break;
		default:
			return QObject::tr("Sublesson") + " " + QString::number(id);
			break;
	}
}

QString configParser::exerciseRawText(int lesson, int sublesson, int exercise)
{
	return exerciseRawText(lineOf(lesson,sublesson,exercise));
}

QString configParser::exerciseText(int lesson, int sublesson, int exercise)
{
	QString line = lineOf(lesson,sublesson,exercise);
	QString repeatConfig = exerciseRepeatConfig(line);
	QString attributes = exerciseAttributes(line);
	return generateText(exerciseRawText(line),
		exerciseRepeatBool(repeatConfig),
		exerciseRepeatType(repeatConfig),
		exerciseAttribute(attributes,0).toInt());
}

bool configParser::exerciseRepeatBool(const QString config)
{
	QString out = "";
	int i;
	for(i=0; i < config.count(); i++)
	{
		if(config[i] == '\\')
		{
			i++;
			if(i < config.count())
				out += config[i];
		}
		else if(config[i] == ',')
			return (out == "1");
		else
			out += config[i];
	}
	return "";
}

QString configParser::exerciseRepeatType(const QString config)
{
	QString out = "";
	bool repeatTypeReached = false;
	int i;
	for(i=0; i < config.count(); i++)
	{
		if(config[i] == '\\')
		{
			i++;
			if(i < config.count())
				out += config[i];
		}
		else if(config[i] == ',')
			repeatTypeReached = true;
		else
		{
			if(repeatTypeReached)
				out += config[i];
		}
	}
	return out;
}

QString configParser::exerciseRepeatConfig(const QString line)
{
	QString out = "";
	bool repeatConfigReached = false;
	int i;
	for(i=0; i < line.count(); i++)
	{
		if(line[i] == '\\')
		{
			i++;
			if(i < line.count())
				out += line[i];
		}
		else if(line[i] == ':')
			repeatConfigReached = true;
		else if((line[i] == ' ') || (line[i] == ';'))
			return out;
		else
		{
			if(repeatConfigReached)
				out += line[i];
		}
	}
	return out;
}

QString configParser::exerciseAttribute(const QString config, const int id)
{
	QString out = "";
	int i, currentID = 0;
	for(i=0; i < config.count(); i++)
	{
		if(config[i] == '\\')
		{
			i++;
			if(i < config.count())
				out += config[i];
		}
		else if(config[i] == ',')
		{
			if(currentID == id)
				return out;
			out = "";
			currentID++;
		}
		else
			out += config[i];
	}
	if(currentID == id)
		return out;
	else
		return "";
}

QString configParser::exerciseAttributes(const QString line)
{
	QString out = "";
	bool lengthConfigReached = false;
	int i;
	for(i=0; i < line.count(); i++)
	{
		if(line[i] == '\\')
		{
			i++;
			if(i < line.count())
				out += '\\' + line[i];
		}
		else if(line[i] == ';')
			lengthConfigReached = true;
		else if(line[i] == ' ')
			return out;
		else
		{
			if(lengthConfigReached)
				out += line[i];
		}
	}
	return out;
}

QString configParser::exerciseRawText(const QString line)
{
	QString out = "";
	bool textReached = false;
	int i;
	for(i=0; i < line.count(); i++)
	{
		if(line[i] == '\\')
		{
			i++;
			if(i < line.count())
			{
				if(line[i] == '\n')
					out += '\n';
				else if(textReached)
					out += line[i];
			}
		}
		else if((line[i] == ' ') && !textReached)
			textReached = true;
		else
		{
			if(textReached)
				out += line[i];
		}
	}
	return out;
}

int configParser::exerciseID(const QString line, const int part)
{
	QString out = "";
	int i, currentPart = 0;
	for(i=0; i < line.count(); i++)
	{
		if(line[i] == '\\')
		{
			i++;
			out += line[i];
		}
		else
		{
			if((line[i] == '.') || (line[i] == ' ') || (line[i] == ':'))
			{
				currentPart++;
				/*
				 * 1: lesson ID
				 * 2: sublesson ID
				 * 3: exercise ID
				 */
				if(currentPart == part)
					return out.toInt();
				out = "";
				i++;
			}
			out += line[i];
		}
	}
	if(currentPart+1 == part)
		return out.toInt();
	else
		return 0;
}

QString configParser::lineOf(int lesson, int sublesson, int exercise)
{
	configFile->seek(0);
	QTextStream fileStream(configFile);
	fileStream.setCodec("UTF-8");
	while(!fileStream.atEnd())
	{
		QString line = fileStream.readLine();
		if((exerciseID(line,1) == lesson) && (exerciseID(line,2) == sublesson) && (exerciseID(line,3) == exercise))
			return line;
	}
	return "";
}

QString configParser::generateText(QString rawText, bool repeat, QString repeatType, int repeatLimit)
{
	if(repeat && (repeatType == "w")) // repeating words
	{
		if(rawText == "")
			return "";
		int i, words = stringUtils::wordCount(rawText);
		QString out = "";
		i = 1;
		while(true)
		{
			QString nextWord = stringUtils::word(rawText,i);
			int space = 0;
			if(out.count() > 0)
				space = 1; // for space between current text and new word
			if((out.count() + space + nextWord.count()) <= repeatLimit)
			{
				if(space != 0)
					out += ' ';
				out += nextWord;
			}
			else
				return out;
			i++;
			if(i > words)
				i = 1;
		}
	}
	else
		return rawText;
}

bool configParser::addExercise(int lesson, int sublesson, int exercise, bool repeat, QString repeatType, int repeatLimit, int lineLength, QString desc, QString rawText)
{
	// Reopen for appending
	if(!reopen(QIODevice::Append | QIODevice::Text))
		return false;
	// Add exercise line
	configFile->write(QString(QString::number(lesson) + "." + QString::number(sublesson) + "." + QString::number(exercise) + ":").toUtf8());
	if(repeat)
		configFile->write("1");
	else
		configFile->write("0");
	configFile->write(QString("," + repeatType + ";" + QString::number(repeatLimit) + "," + QString::number(lineLength)).toUtf8());
	if(desc != "")
		configFile->write(QString("," + desc).toUtf8());
	configFile->write(QString(" " + rawText).toUtf8());
	// Reopen for reading
	if(!reopen(QIODevice::ReadOnly | QIODevice::Text)) // This shouldn't happen
		return false;
	return true;
}
