/*
 * keyboardwidget.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

#include "widgets/keyboardwidget.h"

/*! Constructs keyboardWidget. */
keyboardWidget::keyboardWidget(QWidget *parent) :
	QFrame(parent),
	mainLayout(new QVBoxLayout(this)),
	currentRow(-1),
	currentColumn(0)
{
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	mainLayout->setSpacing(3);
	mainLayout->setContentsMargins(0,0,0,0);
	keys.clear();
	keyLabels.clear();
	keyMap.clear();
	// Numeric row
	nextRow();
	for(int i=0; i < 13; i++)
		addKey();
	addKey("⌫",Qt::Key_Backspace,125);
	// Above main row
	nextRow();
	addKey("Tab ⭾",Qt::Key_Tab,75);
	for(int i=0; i < 12; i++)
		addKey();
	// Main row
	nextRow();
	addKey("Caps lock",Qt::Key_CapsLock,100);
	for(int i=0; i < 12; i++)
		addKey();
	addKey("⏎",Qt::Key_Return,75);
	// Below main row
	nextRow();
	addKey("⇧ Shift",Qt::Key_Shift,125);
	for(int i=0; i < 10; i++)
		addKey();
	addKey("Shift ⇧",Qt::Key_Shift,156);
	// Bottom row
	nextRow();
	addKey("Ctrl",Qt::Key_Control,75);
	addKey("Alt",Qt::Key_Alt,75);
	addKey("",Qt::Key_Space,475);
	addKey("Alt",Qt::Key_AltGr,75);
	addKey("Ctrl",Qt::Key_Control,103);
}

/*! Adds a key. */
void keyboardWidget::addKey(QString keyLabelText, int keyCode, int keyMinimumWidth)
{
	if(currentRowLayout == nullptr)
		return;
	if(!keyLabelText.contains("\n"))
		keyLabelText += "\n";
	// Create key frame
	QFrame *newKey = new QFrame(this);
	newKey->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	newKey->setMinimumWidth(keyMinimumWidth);
	newKey->setFrameShape(QFrame::WinPanel);
	newKey->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
	currentRowLayout->addWidget(newKey);
	keyMap.insert(QPair<int,int>(currentRow,currentColumn),newKey);
	currentRow++;
	// Create key label
	QHBoxLayout *keyLayout = new QHBoxLayout(newKey);
	QLabel *keyLabel = new QLabel(keyLabelText,newKey);
	keyLabel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
	keyLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	keyLabel->setMargin(0);
	keyLayout->addWidget(keyLabel);
	keyLayout->setContentsMargins(1,1,1,1);
	keyLabels.insert(newKey,keyLabel);
	// Save the key
	keys.insert(newKey,keyCode);
}

/*! Starts a new row. */
void keyboardWidget::nextRow(void)
{
	QFrame *currentRowFrame = new QFrame(this);
	currentRowFrame->setContentsMargins(0,0,0,0);
	currentRowFrame->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
	currentRowFrame->setFrameShape(QFrame::NoFrame);
	currentRowFrame->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
	currentRowLayout = new QHBoxLayout(currentRowFrame);
	currentRowLayout->setSizeConstraint(QLayout::SetFixedSize);
	currentRowLayout->setSpacing(3);
	currentRowLayout->setContentsMargins(0,0,0,0);
	mainLayout->addWidget(currentRowFrame);
	currentRow++;
	currentColumn = 0;
}

/*! Adds a layout-specific key. */
void keyboardWidget::registerKey(int row, int column, QString keyLabelText, int keyCode, int shiftKeyCode)
{
	QPair<int,int> keyPos = QPair<int,int>(row,column);
	if(!keyMap.contains(keyPos))
		return;
	QFrame *targetKeyFrame = keyMap[keyPos];
	keys.insert(targetKeyFrame,keyCode);
	keys.insert(targetKeyFrame,shiftKeyCode);
	keyLabels[targetKeyFrame]->setText(keyLabelText);
}

/*! Sets style sheet of all keys. */
void keyboardWidget::setKeyStyleSheet(QString styleSheet)
{
	QList<QFrame*> keyList = keys.keys();
	for(int i=0; i < keyList.count(); i++)
		keyList[i]->setStyleSheet(styleSheet);
}

/*! Loads a keyboard layout. */
void keyboardWidget::loadLayout(QLocale::Language language, QLocale::Country country, QString variant)
{
	if(variant == "")
		variant = "QWERTY";
	QLocale inputLocale(language,country);
	QString layoutPath = ":res/keyboard-layouts/" + inputLocale.name() + "-" + variant + ".json";
	QFile layoutFile(layoutPath);
	if(layoutFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QJsonDocument layoutDocument = QJsonDocument::fromJson(layoutFile.readAll());
		QJsonObject layoutObject = layoutDocument.object();
		QJsonArray layoutKeys = layoutObject["keys"].toArray();
		for(int i=0; i < layoutKeys.count(); i++)
		{
			QJsonObject layoutKey = layoutKeys[i].toObject();
			registerKey(layoutKey["x"].toInt(),layoutKey["y"].toInt(),layoutKey["label"].toString(),layoutKey["code"].toInt(),layoutKey["shiftCode"].toInt());
		}
	}
}
/*! Highlights a key. */
void keyboardWidget::highlightKey(int keyCode)
{
	QList<int> keyCodes = keys.values();
	if(keyCodes.contains(keyCode))
	{
		QFrame *targetKey = keys.key(keyCode);
		targetKey->setGraphicsEffect(new QGraphicsColorizeEffect);
	}
}

/*! Dehighlights a key. */
void keyboardWidget::dehighlightKey(int keyCode)
{
	QList<int> keyCodes = keys.values();
	if(keyCodes.contains(keyCode))
	{
		QFrame *targetKey = keys.key(keyCode);
		targetKey->setGraphicsEffect(nullptr);
	}
}
