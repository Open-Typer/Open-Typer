/*
 * KeyboardUtils.cpp
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

#include <QMetaEnum>
#include "KeyboardUtils.h"

/*!
 * Returns true if the key is a special key.\n
 * Special keys are ignored by default.
 */
bool KeyboardUtils::isSpecialKey(QKeyEvent *event)
{
	return isSpecialKey(QmlKeyboardHandler::convertEvent(event));
}

/*! Implementation of isSpecialKey() for QVariantMap events. */
bool KeyboardUtils::isSpecialKey(QVariantMap event)
{
	int keyID = event["key"].toInt();
	if((event["text"].toString() == "") && (keyID != Qt::Key_Return) && (keyID != Qt::Key_Enter))
		return true;
	switch(keyID)
	{
		case Qt::Key_Delete:
			return true;
			break;
		case Qt::Key_Backspace:
			return true;
			break;
		case Qt::Key_Backtab:
			return true;
			break;
		case Qt::Key_Escape:
			return true;
			break;
		default:
			return false;
			break;
	}
}

/*! Returns true if the key code belongs to a dead key. */
bool KeyboardUtils::isDeadKey(int key)
{
	QMetaEnum metaEnum = QMetaEnum::fromType<Qt::Key>();
	return QString(metaEnum.valueToKey(key)).contains("Dead");
}

/*! Returns the text for the given dead key. */
QString KeyboardUtils::deadKeyToString(Qt::Key key)
{
	// Source: https://github.com/qt/qtwebengine/blob/9ef040a6bef904cbbf9e39135135333cbbae80b1/src/core/web_event_factory.cpp#L917-L983
	switch(key)
	{
		case Qt::Key_Dead_Grave:
			return QChar(0x0060);
		case Qt::Key_Dead_Acute:
			return QChar(0x00B4);
		case Qt::Key_Dead_Circumflex:
			return QChar(0x005E);
		case Qt::Key_Dead_Tilde:
			return QChar(0x02DC);
		case Qt::Key_Dead_Macron:
			return QChar(0x00AF);
		case Qt::Key_Dead_Breve:
			return QChar(0x02D8);
		case Qt::Key_Dead_Abovedot:
			return QChar(0x02D9);
		case Qt::Key_Dead_Diaeresis:
			return QChar(0x00A8);
		case Qt::Key_Dead_Abovering:
			return QChar(0x02DA);
		case Qt::Key_Dead_Doubleacute:
			return QChar(0x02DD);
		case Qt::Key_Dead_Caron:
			return QChar(0x02C7);
		case Qt::Key_Dead_Cedilla:
			return QChar(0x00B8);
		case Qt::Key_Dead_Ogonek:
			return QChar(0x02DB);
		case Qt::Key_Dead_Iota:
			return QChar(0x037A);
		case Qt::Key_Dead_Voiced_Sound:
			return QChar(0x309B);
		case Qt::Key_Dead_Semivoiced_Sound:
			return QChar(0x309C);
		case Qt::Key_Dead_Belowdot:
			return QChar(0xFBB3);
		/*case Qt::Key_Dead_Hook:
			return QChar(0x0294); // U+0294 is the most similar character to U+0309
		case Qt::Key_Dead_Horn:
			return QChar(0x2019); // U+2019 is the most similar character to U+031B
		case Qt::Key_Dead_Stroke:
			return QChar(0x0338);
		case Qt::Key_Dead_Abovecomma:
			return QChar(0x0313);
		case Qt::Key_Dead_Abovereversedcomma:
			return QChar(0x0314);*/
		case Qt::Key_Dead_Doublegrave:
			return QChar(0x02F5);
		case Qt::Key_Dead_Belowring:
			return QChar(0x02DA); // same as ring above because it doesn't matter on a keyboard
		case Qt::Key_Dead_Belowmacron:
			return QChar(0x00AF); // same as macron because it doesn't matter on a keyboard
		case Qt::Key_Dead_Belowcircumflex:
			return QChar(0xA788);
		case Qt::Key_Dead_Belowtilde:
			return QChar(0x02F7);
		case Qt::Key_Dead_Belowbreve:
			return QChar(0x23D1);
		case Qt::Key_Dead_Belowdiaeresis:
			return QChar(0x00A8); // same as diaeresis because it doesn't matter on a keyboard
		/*case Qt::Key_Dead_Invertedbreve:
			return QChar(0x0311);*/
		case Qt::Key_Dead_Belowcomma:
			return QChar(0x002C);
		case Qt::Key_Dead_Currency:
			return QChar(0x00A4);
		case Qt::Key_Dead_Greek:
			return QChar(0x037E);
		default:
			return QChar();
	}
}
