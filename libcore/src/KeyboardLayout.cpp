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
#include <QMetaEnum>
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-keysyms.h>
#include <ks_tables.h>
#include <keysym.h>
#include "KeyboardLayout.h"
#include "KeyboardUtils.h"

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

/*! Row A (control, alt, space, etc.) */
KeyboardRow KeyboardLayout::rowA(void)
{
	return m_rowA;
}

/*! Row B (shift, backslash, y/z, x, c, etc.) */
KeyboardRow KeyboardLayout::rowB(void)
{
	return m_rowB;
}

/*! Row C (caps lock, a, s, d, f, etc.) */
KeyboardRow KeyboardLayout::rowC(void)
{
	return m_rowC;
}

/*! Row D (tab, q, w, e, r, t, z/y, etc.) */
KeyboardRow KeyboardLayout::rowD(void)
{
	return m_rowD;
}

/*! Row E (tilde, 1, 2, 3, 4, 5, etc.) */
KeyboardRow KeyboardLayout::rowE(void)
{
	return m_rowE;
}

/*! Initializes the keyboard layout (reads all keys from xkeyboard-config). */
void KeyboardLayout::init(void)
{
	m_rowA.clear();
	m_rowB.clear();
	m_rowC.clear();
	m_rowD.clear();
	m_rowE.clear();
	for(int i = 0; i < 14; i++)
		m_rowE.append(Key());
	for(int i = 0; i < 13; i++)
		m_rowD.append(Key());
	for(int i = 0; i < 13; i++)
		m_rowC.append(Key());
	for(int i = 0; i < 13; i++)
		m_rowB.append(Key());
	for(int i = 0; i < 8; i++)
		m_rowA.append(Key());

	if(m_layoutId.isEmpty() || m_variant.isEmpty())
		return;
	QFile layoutFile(":/res/xkeyboard-config/symbols/" + m_layoutId);
	bool ret = layoutFile.open(QFile::ReadOnly | QFile::Text);
	Q_ASSERT(ret);
	QString rawData = QString::fromUtf8(layoutFile.readAll());
	loadLayout(rawData, m_variant);
	emit rowAChanged(m_rowA);
	emit rowBChanged(m_rowB);
	emit rowCChanged(m_rowC);
	emit rowDChanged(m_rowD);
	emit rowEChanged(m_rowE);
}

/*! Loads keyboard layout from the given text. */
void KeyboardLayout::loadLayout(QString rawData, QString variantName)
{
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
			while(i < rawData.length() && rawData[i] != '\n')
				i++;
			buffer.clear();
			continue;
		}
		if(rawData[i] == '\n')
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
			Q_ASSERT(entry.indexOf("xkb_symbols") + 4 < entry.length());
			Q_ASSERT(entry[entry.indexOf("xkb_symbols") + 1] == "str");
			Q_ASSERT(entry[entry.indexOf("xkb_symbols") + 3] == "{}");
			if(variantName == entry[entry.indexOf("xkb_symbols") + 2])
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
						Q_ASSERT(layoutEntry[3].toList().length() >= 1);
						Q_ASSERT(layoutEntry[3].toList()[0].toList().length() == 2);
						Q_ASSERT(layoutEntry[3].toList()[0].toList()[0] == "[]");
						QVariantList keyData = layoutEntry[3].toList()[0].toList()[1].toList();
						Key key;
						key.setText(keyText(keyData[0].toList()[0].toString()));
						key.setShiftText(keyText(keyData[1].toList()[0].toString()));
						Key::Type keyType;
						QPoint pos = keyPos(keyId, &keyType);
						key.setTypeFromEnum(keyType);
						addKey(key, pos.x(), pos.y());
					}
					else if(layoutEntry.length() > 0 && layoutEntry[0] == "include")
					{
						Q_ASSERT(layoutEntry.length() == 3);
						Q_ASSERT(layoutEntry[1] == "str");
						QString includeDest = layoutEntry[2].toString();
						bool bracket = false;
						QString destFile, destVariant;
						for(int k = 0; k < includeDest.length(); k++)
						{
							if(includeDest[k] == "(")
								bracket = true;
							else if(includeDest[k] == ")")
								bracket = false;
							else if(bracket)
								destVariant += includeDest[k];
							else
								destFile += includeDest[k];
						}
						if(destVariant.isEmpty())
							destVariant = "basic";
						QFile layoutFile(":/res/xkeyboard-config/symbols/" + destFile);
						bool ret = layoutFile.open(QFile::ReadOnly | QFile::Text);
						Q_ASSERT(ret);
						QString layoutData = QString::fromUtf8(layoutFile.readAll());
						loadLayout(layoutData, destVariant);
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
			if(data[i] == '\"')
			{
				args.append("str");
				args.append(buffer);
				buffer.clear();
				quotes = false;
			}
			else
				buffer += data[i];
		}
		else if(data[i] == '\"')
		{
			buffer.clear();
			quotes = true;
		}
		else if(data[i] == ' ' || data[i] == '\t' || (data[i] == '\n' && !skipEntry))
		{
			if(!buffer.isEmpty())
			{
				if(buffer == "include")
					skipEntry = true;
				args.append(buffer);
				buffer.clear();
			}
		}
		else if(data[i] == '{')
		{
			args.append("{}");
			args.append((QVariant) parse(nestedData(&i, data, "{", "}")));
		}
		else if(data[i] == '[')
		{
			args.append("[]");
			args.append((QVariant) parse(nestedData(&i, data, "[", "]")));
		}
		else if((data[i] == ';' || data[i] == ',') || (data[i] == '\n' && skipEntry))
		{
			if(!buffer.isEmpty())
				args.append(buffer);
			out.append((QVariant) args);
			args.clear();
			buffer.clear();
			skipEntry = false;
		}
		else if(data[i] != '\n')
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
			if(data[i] == '\"')
			{
				out += "\"";
				quotes = false;
			}
			else
				out += data[i];
		}
		else if(data[i] == '\"')
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
	if(out.back() != ';')
		out += ";";
	return out;
}

/*! Convert key ID (e. g. "backslash" or "bracketleft") to unicode representation of the key. */
QString KeyboardLayout::keyText(QString id)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	bool dead = id.length() > 5 && id.first(5) == "dead_";
#else
	bool dead = id.length() > 5 && id.left(5) == "dead_";
#endif
	if(dead)
	{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		QString idPart = id.last(id.length() - 5);
#else
		QString idPart = id.right(id.length() - 5);
#endif
		idPart.replace(0, 1, idPart[0].toUpper());
		QMetaEnum metaEnum = QMetaEnum::fromType<Qt::Key>();
		bool ok = false;
		Qt::Key keyId = (Qt::Key) metaEnum.keyToValue(QString("Key_Dead_" + idPart).toStdString().c_str(), &ok);
		if(!ok)
			qCritical("%s", QString("Unsupported dead key: " + id).toStdString().c_str());
		Q_ASSERT(ok);
		return KeyboardUtils::deadKeyToString(keyId);
	}

	char *buffer = (char *) malloc(8);
	memset(buffer, 0, 8);
	xkb_keysym_t keysym = xkb_keysym_from_name(id.toStdString().c_str(), XKB_KEYSYM_CASE_INSENSITIVE);
	int ret = xkb_keysym_to_utf8(keysym, buffer, 8);
	Q_ASSERT(ret >= 0);
	QString out = QString::fromUtf8(buffer, ret - 1);
	free(buffer);
	return out;
}

/*!
 * Returns the position of the key (key - left to right, row - bottom to top, ).\n
 * Optionally gets key type.
 */
QPoint KeyboardLayout::keyPos(QString keyId, Key::Type *type)
{
	if(type)
		*type = Key::Type_Any;
	if(keyId == "TLDE")
		return QPoint(0, 4);
	else if(keyId == "BKSL")
		return QPoint(12, 2);
	else if(keyId == "LSGT")
		return QPoint(0, 1);
	else if(keyId == "SPCE")
	{
		if(type)
			*type = Key::Type_Space;
		return QPoint(3, 0);
	}
	else if(keyId[0] == 'A')
	{
		QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		return QPoint(QString(QString(keyId[2]) + keyId[3]).toInt(), alphabet.indexOf(keyId[1]));
	}
	else
	{
		// This shouldn't happen...
		qCritical("%s", QString("Unsupported key: " + keyId).toStdString().c_str());
		Q_ASSERT(false);
		return QPoint(-1, -1);
	}
}

/*! Adds a key to the layout. */
void KeyboardLayout::addKey(Key key, int x, int y)
{
	KeyboardRow *row;
	switch(y)
	{
		case 0:
			row = &m_rowA;
			break;
		case 1:
			row = &m_rowB;
			break;
		case 2:
			row = &m_rowC;
			break;
		case 3:
			row = &m_rowD;
			break;
		case 4:
			row = &m_rowE;
			break;
		default:
			row = nullptr;
			break;
	}
	Q_ASSERT(row != nullptr);
	row->replace(x, key);
}
