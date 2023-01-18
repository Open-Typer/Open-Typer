/*
 * KeyboardLayout.cpp
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

#include <QFile>
#include <xkbcommon.h>
#include <xkbcommon-keysyms.h>
#include <ks_tables.h>
#include <keysym.h>
#include "KeyboardLayout.h"

/*! Constructs KeyboardLayout. */
KeyboardLayout::KeyboardLayout(QObject *parent) :
	QObject(parent) { }

/*! Constructs KeyboardLayout with the given ID. */
KeyboardLayout::KeyboardLayout(QString id, QObject *parent) :
	QObject(parent)
{
	setLayoutId(id);
}

/*! The layout ID, for example "de" for the German keyboard. */
QString KeyboardLayout::layoutId(void)
{
	return m_layoutId;
}

void KeyboardLayout::setLayoutId(QString id)
{
	m_layoutId = id;
	init();
	emit layoutIdChanged(id);
}

/*! The layout variant, for example "basic" or "qwerty". */
QString KeyboardLayout::variant(void)
{
	return m_variant;
}

void KeyboardLayout::setVariant(QString variant)
{
	m_variant = variant;
	init();
	emit variantChanged(variant);
}

/*! Initializes the keyboard layout (reads all keys from xkeyboard-config). */
void KeyboardLayout::init(void)
{
	rowB.clear();
	rowC.clear();
	rowD.clear();
	rowE.clear();

	QFile layoutFile(":/res/xkeyboard-config/symbols/" + m_layoutId);
	Q_ASSERT(layoutFile.open(QFile::ReadOnly | QFile::Text));
	QString rawData = QString::fromUtf8(layoutFile.readAll());
	//QString data = " [      plus,          1,       exclam,   dead_tilde ] ";

	// Remove comments
	QString data;
	QString buffer;
	for(int i = 0; i < rawData.length(); i++)
	{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		if(buffer.length() >= 2 && buffer.last(2) == "//")
#else
		if(buffer.length() >= 2 && buffer.right(2) == "//")
#endif
		{
			while(i < rawData.length() && rawData[i] != "\n")
				i++;
			buffer.clear();
			continue;
		}
		if(rawData[i] == "\n")
		{
			data += buffer + "\n";
			buffer.clear();
		}
		else
			buffer += rawData[i];
	}
	if(!buffer.isEmpty())
		data += buffer;

	QVariantList config = parse(data);

	// Read data
	for(int i = 0; i < config.length(); i++)
	{
		QVariantList entry = config[i].toList();
		if(entry.contains("xkb_symbols"))
		{
			// TODO: Implement inclusion of common layouts
			Q_ASSERT(entry.indexOf("xkb_symbols") + 4 < entry.length());
			Q_ASSERT(entry[entry.indexOf("xkb_symbols") + 1] == "str");
			Q_ASSERT(entry[entry.indexOf("xkb_symbols") + 3] == "{}");
			if(m_variant == entry[entry.indexOf("xkb_symbols") + 2])
			{
				QVariantList layoutEntries = entry[entry.indexOf("xkb_symbols") + 4].toList();
				for(int j = 0; j < layoutEntries.length(); j++)
				{
					QVariantList layoutEntry = layoutEntries[j].toList();
					if(layoutEntry.length() > 0 && layoutEntry[0] == "key")
					{
						Q_ASSERT(layoutEntry.length() == 4);
						QString rawKeyId = layoutEntry[1].toString();
						Q_ASSERT(rawKeyId.length() >= 2);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
						QString keyId = rawKeyId.sliced(1, rawKeyId.length() - 2); // Remove '<' and '>'
#else
						QString keyId = rawKeyId.mid(1, rawKeyId.length() - 2); // Remove '<' and '>'
#endif
						Q_ASSERT(layoutEntry[2] == "{}");
						Q_ASSERT(layoutEntry[3].toList().length() == 1);
						Q_ASSERT(layoutEntry[3].toList()[0].toList().length() == 2);
						Q_ASSERT(layoutEntry[3].toList()[0].toList()[0] == "[]");
						QVariantList keyData = layoutEntry[3].toList()[0].toList()[1].toList();
						Key key;
						key.setText(keyChar(keyData[0].toList()[0].toString()));
						key.setShiftText(keyChar(keyData[1].toList()[0].toString()));
						// TODO: Get key position
					}
				}
			}
		}
	}
}

/*! Reads keyboard layout configuration. */
QVariantList KeyboardLayout::parse(QString data)
{
	QVariantList out;
	QString buffer;
	QVariantList args;
	bool quotes = false, skipEntry = false;
	for(int i = 0; i < data.length(); i++)
	{
		if(quotes)
		{
			if(data[i] == "\"")
			{
				args.append("str");
				args.append(buffer);
				buffer.clear();
				quotes = false;
			}
			else
				buffer += data[i];
		}
		else if(data[i] == "\"")
		{
			buffer.clear();
			quotes = true;
		}
		else if(data[i] == " " || data[i] == "\t" || (data[i] == "\n" && !skipEntry))
		{
			if(!buffer.isEmpty())
			{
				if(buffer == "include")
					skipEntry = true;
				args.append(buffer);
				buffer.clear();
			}
		}
		else if(data[i] == "{")
		{
			args.append("{}");
			args.append((QVariant) parse(nestedData(&i, data, "{", "}")));
		}
		else if(data[i] == "[")
		{
			args.append("[]");
			args.append((QVariant) parse(nestedData(&i, data, "[", "]")));
		}
		else if((data[i] == ";" || data[i] == ",") || (data[i] == "\n" && skipEntry))
		{
			if(!buffer.isEmpty())
				args.append(buffer);
			out.append((QVariant) args);
			args.clear();
			buffer.clear();
			skipEntry = false;
		}
		else if(data[i] != "\n")
			buffer += data[i];
	}
	return out;
}

/*! Returns text nested in brackets. */
QString KeyboardLayout::nestedData(int *pos, QString data, QString startToken, QString endToken)
{
	bool quotes = false;
	int bracketCount = 1;
	QString out;
	int i = *pos;
	while(i < data.length() && !(data[i] == endToken && bracketCount == 0))
	{
		i++;
		if(!quotes)
		{
			if(data[i] == startToken)
				bracketCount++;
			if(data[i] == endToken)
				bracketCount--;
		}
		if(quotes)
		{
			if(data[i] == "\"")
			{
				out += "\"";
				quotes = false;
			}
			else
				out += data[i];
		}
		else if(data[i] == "\"")
		{
			out += "\"";
			quotes = true;
		}
		else if(bracketCount > 0)
			out += data[i];
		Q_ASSERT(bracketCount >= -1);
	}
	Q_ASSERT(data[i] == endToken);
	*pos = i;
	Q_ASSERT(!out.isEmpty());
	if(out.back() != ";")
		out += ";";
	return out;
}

/*! Convert key ID (e. g. "backslash" or "bracketleft") to QChar. */
QChar KeyboardLayout::keyChar(QString id)
{
	return QChar(xkb_keysym_from_name(id.toStdString().c_str(), XKB_KEYSYM_CASE_INSENSITIVE));
}
