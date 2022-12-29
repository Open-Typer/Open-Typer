/*
 * ConfigParser.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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

#include "ConfigParser.h"

namespace publicPos {
	Q_NAMESPACE
	int currentLesson = 0, currentSublesson = 0, currentExercise = 0;
}

/*! Constructs ConfigParser. */
ConfigParser::ConfigParser(QObject *parent) :
	QObject(parent)
{
	currentDevice = &configFile;
}

/*!
 * Opens a pack file.\n
 * Returns true if successful.
 * \see reopen()
 */
bool ConfigParser::open(const QString fileName)
{
	currentDevice->close();
	configFile.setFileName(fileName);
	currentDevice = &configFile;
	return configFile.open(QIODevice::ReadOnly | QIODevice::Text);
}

/*! Closes opened file and loads pack content to the buffer. */
void ConfigParser::loadToBuffer(const QByteArray content)
{
	close();
	configBuffer.setData(nullptr);
	configBuffer.open(QBuffer::WriteOnly);
	configBuffer.write(content);
	configBuffer.close();
	configBuffer.open(QIODevice::ReadOnly);
	currentDevice = &configBuffer;
}

/*! Returns current data in the opened file or buffer. */
QByteArray ConfigParser::data(void)
{
	if(!currentDevice->isReadable())
		return 0;
	currentDevice->seek(0);
	return currentDevice->readAll();
}

/*! Returns true if there's a buffer opened. */
bool ConfigParser::bufferOpened(void)
{
	return (currentDevice != &configFile);
}

/*!
 * ConfigParser provides read and write support.
 * The pack file or buffer is opened for reading by default,
 * so it must be reopened for writing while performing
 * write operations.\n
 *
 * This function reopens the opened file or buffer in a new mode.\n
 * Returns true if successful.
 *
 * \param[in] mode Open mode, like in the QIODevice#open() function.
 *
 * \see open()
 */
bool ConfigParser::reopen(QIODevice::OpenMode mode)
{
	if(currentDevice == &configFile)
	{
		if(configFile.fileName() == "")
			return false;
	}
	currentDevice->close();
	return currentDevice->open(mode);
}

/*! Closes the opened pack file or buffer. */
void ConfigParser::close(void)
{
	currentDevice->close();
}

/*! Returns the file name of the opened pack file. */
QString ConfigParser::fileName(void)
{
	if(currentDevice == &configFile)
		return configFile.fileName();
	else
		return QString();
}

/*! Returns the number of lessons in the pack file or buffer. */
int ConfigParser::lessonCount(void)
{
	if(!currentDevice->isReadable())
		return 0;
	currentDevice->seek(0);
	QList<int> lessonIDs;
	while(!currentDevice->atEnd())
	{
		QString line = QString(currentDevice->readLine()).remove('\n');
		int lessonID = exerciseID(line, 1);
		if(!lessonIDs.contains(lessonID))
			lessonIDs += lessonID;
	}
	return lessonIDs.count();
}

/*!
 * Returns the number of sublessons in a lesson.\n
 * Note: Empty sublessons are allowed and will not be counted by this function.
 * The exerciseCount() function can be used to detect an empty sublesson.
 */
int ConfigParser::sublessonCount(int lesson)
{
	if(!currentDevice->isReadable())
		return 0;
	currentDevice->seek(0);
	QList<int> sublessonIDs;
	while(!currentDevice->atEnd())
	{
		QString line = QString(currentDevice->readLine()).remove('\n');
		int lessonID = exerciseID(line, 1);
		if(lessonID == lesson)
		{
			int sublessonID = exerciseID(line, 2);
			if(!sublessonIDs.contains(sublessonID))
				sublessonIDs += sublessonID;
		}
	}
	return sublessonIDs.count();
}

/*! Returns the number of exercises in a sublesson. */
int ConfigParser::exerciseCount(int lesson, int sublesson)
{
	if(!currentDevice->isReadable())
		return 0;
	currentDevice->seek(0);
	QList<int> exerciseIDs;
	while(!currentDevice->atEnd())
	{
		QString line = QString(currentDevice->readLine()).remove('\n');
		int lessonID = exerciseID(line, 1);
		if(lessonID == lesson)
		{
			int sublessonID = exerciseID(line, 2);
			if(sublessonID == sublesson)
			{
				int _exerciseID = exerciseID(line, 3);
				if(!exerciseIDs.contains(_exerciseID))
					exerciseIDs += _exerciseID;
			}
		}
	}
	return exerciseIDs.count();
}

/*! Returns the line the exercise is located in the pack file or buffer. */
int ConfigParser::exerciseLine(int lesson, int sublesson, int exercise)
{
	if(!currentDevice->isReadable())
		return -1;
	currentDevice->seek(0);
	int lineID = 0;
	while(!currentDevice->atEnd())
	{
		lineID++;
		QString line = QString(currentDevice->readLine()).remove('\n');
		if((exerciseID(line, 1) == lesson) && (exerciseID(line, 2) == sublesson) && (exerciseID(line, 3) == exercise))
			return lineID;
	}
	return 0;
}

/*! Returns true if repeating is enabled in the exercise. */
bool ConfigParser::exerciseRepeatBool(int lesson, int sublesson, int exercise)
{
	return exerciseRepeatBool(exerciseRepeatConfig(lineOf(lesson, sublesson, exercise)));
}

/*! Returns repeat configuration of the exercise. */
QString ConfigParser::exerciseRepeatType(int lesson, int sublesson, int exercise)
{
	return exerciseRepeatType(exerciseRepeatConfig(lineOf(lesson, sublesson, exercise)));
}

/*! Returns the maximum number of characters of the exercise (if repeating is enabled). */
int ConfigParser::exerciseRepeatLimit(int lesson, int sublesson, int exercise)
{
	return exerciseAttribute(exerciseAttributes(lineOf(lesson, sublesson, exercise)), 0).toInt();
}

/*! Returns the exercise's maximum number of characters in one line. */
int ConfigParser::exerciseLineLength(int lesson, int sublesson, int exercise)
{
	return exerciseAttribute(exerciseAttributes(lineOf(lesson, sublesson, exercise)), 1).toInt();
}

/*! Returns the description of a lesson (what new characters are learned in it). */
QString ConfigParser::lessonDesc(int lesson)
{
	if(!currentDevice->isReadable())
		return "";
	currentDevice->seek(0);
	while(!currentDevice->atEnd())
	{
		QString line = QString(currentDevice->readLine()).remove('\n');
		if(exerciseID(line, 1) == lesson)
		{
			QString out = exerciseAttribute(exerciseAttributes(line), 2);
			if(out != "")
				return out;
		}
	}
	return "";
}

/*!
 * Converts a lesson description into a human-readable description.\n
 * For example:\n
 * \c dfjk  -> {dfjk}\n
 * \c %%r    -> Revision\n
 * \c %%s    -> Shift\n
 * \c sl%%be -> {sl}{e}\n
 *
 * \see lessonDesc()
 */
QString ConfigParser::parseDesc(QString desc)
{
	QString out = "";
	int i;
	bool bracket = false;
	for(i = 0; i < desc.count(); i++)
	{
		if(desc[i] == '%')
		{
			i++;
			if(bracket && (desc[i] != '%'))
				out += '}';
			if(desc[i] == 'r')
				out += QObject::tr("Revision");
			else if(desc[i] == '%')
			{
				out += '%';
				if(bracket)
					out += '}';
			}
			// %b is reserved (it's used to separate 2 sets)
			if(desc[i] == 's')
			{
				if(!bracket)
				{
					out += '{';
					bracket = true;
				}
				out += QObject::tr("Shift", "Shift key");
			}
			else
				bracket = false;
		}
		else
		{
			if(!bracket)
			{
				out += '{';
				bracket = true;
			}
			out += desc[i];
		}
	}
	if(bracket)
		out += '}';
	return out;
}

/*! Returns the name of a sublesson.\n
 * For example:\n
 * 1 -> Touch
 */
QString ConfigParser::sublessonName(int id)
{
	switch(id)
	{
		case 1:
			return tr("Touch");
			break;
		case 2:
			return tr("Words");
			break;
		case 3:
			return tr("Sentences");
			break;
		case 4:
			return tr("Text");
			break;
		default:
			return sublessonTr(id);
			break;
	}
}

/*! Returns translated "Lesson (id)" string. */
QString ConfigParser::lessonTr(int id)
{
	return tr("Lesson %1").arg(QString::number(id));
}

/*! Returns translated "Sublesson (id)" string. */
QString ConfigParser::sublessonTr(int id)
{
	return tr("Sublesson %1").arg(QString::number(id));
}

/*! Returns translated "Exercise (id)" string. */
QString ConfigParser::exerciseTr(int id)
{
	return tr("Exercise %1").arg(QString::number(id));
}

/*!
 * Returns raw text of the exercise (no repeating or line wrapping).
 * \see exerciseText()
 * \see initExercise()
 */
QString ConfigParser::exerciseRawText(int lesson, int sublesson, int exercise)
{
	return exerciseRawText(lineOf(lesson, sublesson, exercise));
}

/*!
 * Returns exercise text (with repeating and without line wrapping).
 * \see exerciseRawText()
 * \see initExercise()
 */
QString ConfigParser::exerciseText(int lesson, int sublesson, int exercise)
{
	QString line = lineOf(lesson, sublesson, exercise);
	QString repeatConfig = exerciseRepeatConfig(line);
	QString attributes = exerciseAttributes(line);
	return generateText(exerciseRawText(line),
		exerciseRepeatBool(repeatConfig),
		exerciseRepeatType(repeatConfig),
		exerciseAttribute(attributes, 0).toInt());
}

/*!
 * Adds line wrapping to exercise text.
 * \see exerciseRawText()
 * \see exerciseText()
 */
QString ConfigParser::initExercise(QString exercise, int lineLength)
{
	int len, len2, i, line_pos = 0;
	QString out = "";
	len = exercise.count();
	QString word = "";
	bool first_word = true;
	for(i = 0; i < len; i++)
	{
		if(((exercise[i] == ' ') || (exercise[i] == '\n')) || (i + 1 >= len))
		{
			if(i + 1 >= len)
				word += exercise[i];
			len2 = word.count();
			if(line_pos + len2 > lineLength)
			{
				out += '\n';
				line_pos = 0;
			}
			if(!first_word && (line_pos > 0))
				out += ' ';
			first_word = false;
			out += word;
			word = "";
			if(exercise[i] == '\n')
			{
				out += '\n';
				line_pos = 0;
			}
			else
			{
				if(!first_word)
					line_pos++;
				line_pos = line_pos + len2;
			}
		}
		else
			word += exercise[i];
	}
	return out;
}

/*!
 * Adds line wrapping to exercise text and (optionally) limits number of lines.\n
 * \param[in] lineCountLimit Whether to limit number of lines.
 * \param[in] currentLine The line the shortened text starts at. Note that lines start by 0 in this function.
 */
QString ConfigParser::initExercise(QString exercise, int lineLength, bool lineCountLimit, int currentLine)
{
	QString text = initExercise(exercise, lineLength);
	QString out = "";
	int i, line = 0;
	for(i = 0; i < text.count(); i++)
	{
		if(text[i] == '\n')
		{
			if((line >= currentLine) && ((line + 1 < currentLine + 3) || !lineCountLimit))
				out += text[i];
			line++;
		}
		else
		{
			if((line >= currentLine) && ((line < currentLine + 3) || !lineCountLimit))
				out += text[i];
		}
		if((line >= currentLine + 3) && lineCountLimit)
			break;
	}
	return out;
}

/*! Generates text without line wrapping nor repeating. */
QString ConfigParser::initText(QString rawText)
{
	return generateText(rawText, false, "", 0);
}

/*! Implementation of exerciseRepeatBool() for a config string. */
bool ConfigParser::exerciseRepeatBool(const QString config)
{
	QString out = "";
	int i;
	for(i = 0; i < config.count(); i++)
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

/*! Implementation of exerciseRepeatType() for a config string. */
QString ConfigParser::exerciseRepeatType(const QString config)
{
	QString out = "";
	bool repeatTypeReached = false;
	int i;
	for(i = 0; i < config.count(); i++)
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

/*! Gets config string from a line. */
QString ConfigParser::exerciseRepeatConfig(const QString line)
{
	QString out = "";
	bool repeatConfigReached = false;
	int i;
	for(i = 0; i < line.count(); i++)
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

/*! Gets a specific exercise attribute from a config string. */
QString ConfigParser::exerciseAttribute(const QString config, const int id)
{
	QString out = "";
	int i, currentID = 0;
	for(i = 0; i < config.count(); i++)
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

/*! Gets a config string from a line. */
QString ConfigParser::exerciseAttributes(const QString line)
{
	QString out = "";
	bool lengthConfigReached = false;
	int i;
	for(i = 0; i < line.count(); i++)
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

/*! Implementation of exerciseRawText() for a line. */
QString ConfigParser::exerciseRawText(const QString line)
{
	QString out = "";
	bool textReached = false;
	int i;
	for(i = 0; i < line.count(); i++)
	{
		if(line[i] == '\\')
		{
			i++;
			if(i < line.count())
			{
				if(line[i] == 'n')
					out += "\\n";
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

/*! Returns a part of the exercise ID (from a line).
 *
 * Part 1 - lesson ID\n
 * Part 2 - sublesson ID\n
 * Part 3 - exercise ID\n
 */
int ConfigParser::exerciseID(const QString line, const int part)
{
	QString out = "";
	int i, currentPart = 0;
	for(i = 0; i < line.count(); i++)
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
	if(currentPart + 1 == part)
		return out.toInt();
	else
		return 0;
}

/*! Returns line string of the exercise. */
QString ConfigParser::lineOf(int lesson, int sublesson, int exercise)
{
	if(!currentDevice->isReadable())
		return "";
	currentDevice->seek(0);
	while(!currentDevice->atEnd())
	{
		QString line = QString(currentDevice->readLine()).remove('\n');
		if((exerciseID(line, 1) == lesson) && (exerciseID(line, 2) == sublesson) && (exerciseID(line, 3) == exercise))
			return line;
	}
	return "";
}

/*!
 * Adds repeating to a raw text.
 * \see exerciseRawText()
 */
QString ConfigParser::generateText(QString rawText, bool repeat, QString repeatType, int repeatLimit)
{
	if(repeat && (repeatType == "w")) // repeating words
	{
		if(rawText == "")
			return "";
		int i, words = StringUtils::wordCount(rawText);
		QString out = "";
		i = 1;
		while(true)
		{
			QString nextWord = StringUtils::word(rawText, i);
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
	{
		QString out = "";
		for(int i2 = 0; i2 < rawText.count(); i2++)
		{
			if(rawText[i2] == '\\')
			{
				i2++;
				if(i2 < rawText.count())
				{
					if(rawText[i2] == 'n')
						out += '\n';
				}
			}
			else
				out += rawText[i2];
		}
		return out;
	}
}

/*!
 * Adds an exercise to the opened pack file.
 *
 * \param[in] lesson Lesson ID.
 * \param[in] sublesson Sublesson ID.
 * \param[in] exercise Exercise ID.
 * \param[in] repeat Whether to enable repeating.
 * \param[in] repeatType Repeat type string.
 * \param[in] repeatLimit Maximum number of characters (if repeating is enabled).
 * \param[in] lineLength Maximum number of characters in one line.
 * \param[in] desc Lesson description. Should be used only in the first exercise of the lesson.
 * \param[in] rawText Exercise raw text.
 */
bool ConfigParser::addExercise(int lesson, int sublesson, int exercise, bool repeat, QString repeatType, int repeatLimit, int lineLength, QString desc, QString rawText)
{
	// Reopen for appending
	if(!reopen(QIODevice::Append | QIODevice::Text))
		return false;
	// Add exercise line
	currentDevice->write(QString(QString::number(lesson) + "." + QString::number(sublesson) + "." + QString::number(exercise) + ":").toUtf8());
	if(repeat)
		currentDevice->write("1");
	else
		currentDevice->write("0");
	currentDevice->write(QString("," + repeatType + ";" + QString::number(repeatLimit) + "," + QString::number(lineLength)).toUtf8());
	if(desc != "")
		currentDevice->write(QString("," + desc).toUtf8());
	currentDevice->write(QString(" " + rawText + '\n').toUtf8());
	// Reopen for reading
	if(!reopen(QIODevice::ReadOnly | QIODevice::Text)) // This shouldn't happen
		return false;
	return true;
}

/*! Returns default repeat limit (text length). */
int ConfigParser::defaultRepeatLimit(void)
{
	return m_defaultRepeatLimit;
}

/*! Returns default line length. */
int ConfigParser::defaultLineLength(void)
{
	return m_defaultLineLength;
}

/*! Sets selected lesson (should only be used in QML). */
void ConfigParser::setCurrentLesson(int value)
{
	publicPos::currentLesson = value;
}

/*! Returns selected lesson. */
int ConfigParser::currentLesson(void)
{
	return publicPos::currentLesson;
}

/*! Sets selected sublesson (should only be used in QML). */
void ConfigParser::setCurrentSublesson(int value)
{
	publicPos::currentSublesson = value;
}

/*! Returns selected sublesson. */
int ConfigParser::currentSublesson(void)
{
	return publicPos::currentSublesson;
}

/*! Sets selected exercise (should only be used in QML). */
void ConfigParser::setCurrentExercise(int value)
{
	publicPos::currentExercise = value;
}

/*! Returns selected exercise. */
int ConfigParser::currentExercise(void)
{
	return publicPos::currentExercise;
}
