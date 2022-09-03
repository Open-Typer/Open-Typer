/*
 * KeyboardWidget.cpp
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

#include "widgets/KeyboardWidget.h"

/*! Constructs KeyboardWidget. */
KeyboardWidget::KeyboardWidget(QWidget *parent) :
	QFrame(parent),
	mainLayout(new QVBoxLayout(this)),
	keyboardFrame(new QFrame(this)),
	keyboardLayout(new QVBoxLayout(keyboardFrame)),
	currentRow(-1),
	currentColumn(0),
	keyboardVisible(true),
	settings(fileUtils::mainSettingsLocation(), QSettings::IniFormat)
{
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	mainLayout->setSpacing(0);
	keyboardLayout->setSpacing(3);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	keyboardLayout->setContentsMargins(0, 0, 0, 0);
	keyboardFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	mainLayout->addWidget(keyboardFrame);
	keyboardFrame->installEventFilter(this);
	keys.clear();
	keyLabels.clear();
	keyMap.clear();
	keyTypes.clear();
	keyBaseStyleSheets.clear();
	keyColors.clear();
	keyFingerColors.clear();
	// Close button
	closeButton = new QPushButton(this);
	setKeyboardVisible(settings.value("view/keyboardvisible", "true").toBool());
	closeButton->setIconSize(QSize(32, 32));
	closeButton->setFocusPolicy(Qt::NoFocus);
	mainLayout->addWidget(closeButton);
	closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mainLayout->setAlignment(closeButton, Qt::AlignCenter);
	// Numeric row
	nextRow();
	for(int i = 0; i < 13; i++)
		addKey();
	addKey("⌫", Qt::Key_Backspace, 125);
	// Above main row
	nextRow();
	addKey("Tab ⭾", Qt::Key_Tab, 75);
	for(int i = 0; i < 12; i++)
		addKey();
	// Main row
	nextRow();
	addKey("Caps lock", Qt::Key_CapsLock, 100);
	for(int i = 0; i < 12; i++)
		addKey();
	addKey("⏎", Qt::Key_Return, 75);
	// Below main row
	nextRow();
	addKey("⇧ Shift", Qt::Key_Shift, 125);
	for(int i = 0; i < 10; i++)
		addKey();
	addKey("Shift ⇧", -2, 156); // Qt doesn't recognize left and right shift; -2 is a special code for right shift
	// Bottom row
	nextRow();
	addKey("Ctrl", Qt::Key_Control, 75);
	addKey("Alt", Qt::Key_Alt, 75);
	addKey("", Qt::Key_Space, 475);
	addKey("Alt", Qt::Key_AltGr, 75);
	addKey("Ctrl", -3, 103); // Qt doesn't recognize left and right control; -3 is a special code for right control
	// Connections
	connect(closeButton, &QPushButton::clicked, this, &KeyboardWidget::toggleKeyboard);
}

/*! Adds a key. */
void KeyboardWidget::addKey(QString keyLabelText, int keyCode, int keyMinimumWidth)
{
	if(currentRowLayout == nullptr)
		return;
	if(!keyLabelText.contains("\n"))
		keyLabelText += "\n";
	// Create key frame
	QFrame *newKey = new QFrame(this);
	newKey->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	newKey->setMinimumWidth(keyMinimumWidth);
	newKey->setFrameShape(QFrame::WinPanel);
	newKey->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
	Finger finger = keyFinger(currentColumn, currentRow);
	QColor keyColor(0, 0, 0);
	switch(finger)
	{
		case Finger_LeftIndex:
		case Finger_RightIndex:
			keyColor = QColor(255, 255, 0);
			break;
		case Finger_LeftMiddle:
		case Finger_RightMiddle:
			keyColor = QColor(100, 255, 0);
			break;
		case Finger_LeftRing:
		case Finger_RightRing:
			keyColor = QColor(0, 100, 255);
			break;
		case Finger_LeftLittle:
		case Finger_RightLittle:
			keyColor = QColor(255, 25, 25);
			break;
		default:
			break;
	}
	keyBaseStyleSheets.insert(newKey, "QFrame { border-radius: 5px; }");
	keyFingerColors.insert(newKey, keyColor);
	newKey->setStyleSheet(keyBaseStyleSheets[newKey]);
	currentRowLayout->addWidget(newKey);
	keyMap.insert(QPair<int, int>(currentColumn, currentRow), newKey);
	currentColumn++;
	// Create key label
	QHBoxLayout *keyLayout = new QHBoxLayout(newKey);
	QLabel *keyLabel = new QLabel(keyLabelText, newKey);
	keyLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	keyLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	keyLabel->setMargin(0);
	keyLayout->addWidget(keyLabel);
	keyLayout->setContentsMargins(1, 1, 1, 1);
	keyLabels.insert(newKey, keyLabel);
	// Save the key
	keys.insert(newKey, keyCode);
	keyTypes.insert(newKey, 0);
}

/*! Starts a new row. */
void KeyboardWidget::nextRow(void)
{
	QFrame *currentRowFrame = new QFrame(this);
	currentRowFrame->setContentsMargins(0, 0, 0, 0);
	currentRowFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	currentRowFrame->setFrameShape(QFrame::NoFrame);
	currentRowFrame->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
	currentRowLayout = new QHBoxLayout(currentRowFrame);
	currentRowLayout->setSizeConstraint(QLayout::SetFixedSize);
	currentRowLayout->setSpacing(3);
	currentRowLayout->setContentsMargins(0, 0, 0, 0);
	keyboardLayout->addWidget(currentRowFrame);
	currentRow++;
	currentColumn = 0;
}

/*! Adds a layout-specific key. */
void KeyboardWidget::registerKey(int x, int y, QString keyLabelText, int keyCode, int shiftKeyCode)
{
	QPair<int, int> keyPos = QPair<int, int>(x, y);
	if(!keyMap.contains(keyPos))
		return;
	if(!keyLabelText.contains("\n"))
		keyLabelText += "\n";
	QFrame *targetKeyFrame = keyMap[keyPos];
	keys.remove(targetKeyFrame);
	keys.insert(targetKeyFrame, keyCode);
	keys.insert(targetKeyFrame, shiftKeyCode);
	keyLabels[targetKeyFrame]->setText(keyLabelText);
	keyTypes[targetKeyFrame] = 1;
}

/*! Sets color of all keys. */
void KeyboardWidget::setKeyColor(QColor color, QColor borderColor)
{
	QList<QFrame *> keyList = keys.keys();
	for(int i = 0; i < keyList.count(); i++)
	{
		QColor fingerColor = keyFingerColors[keyList[i]];
		QColor newColor = color;
		if(!((fingerColor.red() == 0) && (fingerColor.green() == 0) && (fingerColor.blue() == 0)))
		{
			newColor = QColor::fromRgb(color.red() + (fingerColor.red() - color.red()) / 7.5,
				color.green() + (fingerColor.green() - color.green()) / 7.5,
				color.blue() + (fingerColor.blue() - color.blue()) / 7.5);
		}
		keyList[i]->setStyleSheet(keyBaseStyleSheets[keyList[i]]);
		keyList[i]->setStyleSheet(keyBaseStyleSheets[keyList[i]] + "QFrame { background-color: rgb(" + QString::number(newColor.red()) + ", " + QString::number(newColor.green()) + ", " + QString::number(newColor.blue()) + "); border: 1px solid rgb(" + QString::number(borderColor.red()) + ", " + QString::number(borderColor.green()) + ", " + QString::number(borderColor.blue()) + "); }" + "QLabel { border: 0px; }");
		keyColors[keyList[i]] = QPair<QColor, QColor>(color, borderColor);
	}
}

/*! Resets color of a key and returns new color. */
QColor KeyboardWidget::resetKeyColor(QFrame *targetKey)
{
	if(keyColors.contains(targetKey))
	{
		QColor color = keyColors[targetKey].first;
		QColor borderColor = keyColors[targetKey].second;
		QColor fingerColor = keyFingerColors[targetKey];
		if(!((fingerColor.red() == 0) && (fingerColor.green() == 0) && (fingerColor.blue() == 0)))
		{
			color = QColor::fromRgb(color.red() + (fingerColor.red() - color.red()) / 7.5,
				color.green() + (fingerColor.green() - color.green()) / 7.5,
				color.blue() + (fingerColor.blue() - color.blue()) / 7.5);
		}
		targetKey->setStyleSheet(keyBaseStyleSheets[targetKey]);
		targetKey->setStyleSheet(keyBaseStyleSheets[targetKey] + "QFrame { background-color: rgb(" + QString::number(color.red()) + ", " + QString::number(color.green()) + ", " + QString::number(color.blue()) + "); border: 1px solid rgb(" + QString::number(borderColor.red()) + ", " + QString::number(borderColor.green()) + ", " + QString::number(borderColor.blue()) + "); }" + "QLabel { border: 0px; }");
		return color;
	}
	return QColor();
}

/*! Loads a keyboard layout. */
bool KeyboardWidget::loadLayout(QLocale::Language language, QLocale::Country country, QString variant)
{
	if(variant == "")
		variant = "QWERTY";
	QLocale inputLocale(language, country);
	QString layoutPath = ":res/keyboard-layouts/" + inputLocale.name() + "-" + variant + ".json";
	QFile layoutFile(layoutPath);
	if(layoutFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QJsonDocument layoutDocument = QJsonDocument::fromJson(layoutFile.readAll());
		QJsonObject layoutObject = layoutDocument.object();
		QJsonArray layoutKeys = layoutObject["keys"].toArray();
		for(int i = 0; i < layoutKeys.count(); i++)
		{
			QJsonObject layoutKey = layoutKeys[i].toObject();
			QStringList keyStrings = layoutKey["label"].toString().split('\n');
			int keyCode = -1, shiftKeyCode = -1;
			for(int i = 0; i < keyStrings.count(); i++)
			{
				QKeySequence keySequence(keyStrings[i]);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
				int code = keySequence[0].key();
#else
				int code = keySequence[0];
#endif
				switch(i)
				{
					case 0:
						shiftKeyCode = code;
						break;
					case 1:
						keyCode = code;
						break;
				}
			}
			registerKey(layoutKey["x"].toInt(), layoutKey["y"].toInt(), layoutKey["label"].toString(), keyCode, shiftKeyCode);
		}
		return true;
	}
	return false;
}

/*! Highlights a key. */
void KeyboardWidget::highlightKey(int keyCode)
{
	QList<int> keyCodes = keys.values();
	if(keyCodes.contains(keyCode))
	{
		QFrame *targetKey = keys.key(keyCode);
		QColor oldColor = resetKeyColor(targetKey);
		QColor keyBgColor = QColor(0, 175, 255);
		keyBgColor = QColor::fromRgb(keyBgColor.red() + (oldColor.red() - keyBgColor.red()) / 1.2,
			keyBgColor.green() + (oldColor.green() - keyBgColor.green()) / 1.2,
			keyBgColor.blue() + (oldColor.blue() - keyBgColor.blue()) / 1.2);
		targetKey->setStyleSheet(keyBaseStyleSheets[targetKey] + "QFrame { background-color: rgb(" + QString::number(keyBgColor.red()) + ", " + QString::number(keyBgColor.green()) + ", " + QString::number(keyBgColor.blue()) + "); }");
	}
}

/*! Dehighlights a key. */
void KeyboardWidget::dehighlightKey(int keyCode)
{
	QList<int> keyCodes = keys.values();
	if(keyCodes.contains(keyCode))
	{
		QFrame *targetKey = keys.key(keyCode);
		resetKeyColor(targetKey);
	}
}

/*! Returns the finger that should be used to press the key. */
KeyboardWidget::Finger KeyboardWidget::keyFinger(int keyX, int keyY)
{
	QPoint keyPos(keyX, keyY);
	switch(keyPos.y())
	{
		case 0:
			switch(keyPos.x())
			{
				case 0:
					return Finger_LeftLittle;
					break;
				case 1:
					return Finger_LeftLittle;
					break;
				case 2:
					return Finger_LeftLittle;
					break;
				case 3:
					return Finger_LeftRing;
					break;
				case 4:
					return Finger_LeftMiddle;
					break;
				case 5:
					return Finger_LeftIndex;
					break;
				case 6:
					return Finger_LeftIndex;
					break;
				case 7:
					return Finger_RightIndex;
					break;
				case 8:
					return Finger_RightIndex;
					break;
				case 9:
					return Finger_RightMiddle;
					break;
				case 10:
					return Finger_RightRing;
					break;
				case 11:
					return Finger_RightLittle;
					break;
				case 12:
					return Finger_RightLittle;
					break;
				case 13:
					return Finger_RightLittle;
					break;
				default:
					return Finger_Invalid;
					break;
			}
			break;
		case 1:
			switch(keyPos.x())
			{
				case 0:
					return Finger_LeftLittle;
					break;
				case 1:
					return Finger_LeftLittle;
					break;
				case 2:
					return Finger_LeftRing;
					break;
				case 3:
					return Finger_LeftMiddle;
					break;
				case 4:
					return Finger_LeftIndex;
					break;
				case 5:
					return Finger_LeftIndex;
					break;
				case 6:
					return Finger_RightIndex;
					break;
				case 7:
					return Finger_RightIndex;
					break;
				case 8:
					return Finger_RightMiddle;
					break;
				case 9:
					return Finger_RightRing;
					break;
				case 10:
					return Finger_RightLittle;
					break;
				case 11:
					return Finger_RightLittle;
					break;
				case 12:
					return Finger_RightLittle;
					break;
				default:
					return Finger_Invalid;
					break;
			}
			break;
		case 2:
			switch(keyPos.x())
			{
				case 0:
					return Finger_LeftLittle;
					break;
				case 1:
					return Finger_LeftLittle;
					break;
				case 2:
					return Finger_LeftRing;
					break;
				case 3:
					return Finger_LeftMiddle;
					break;
				case 4:
					return Finger_LeftIndex;
					break;
				case 5:
					return Finger_LeftIndex;
					break;
				case 6:
					return Finger_RightIndex;
					break;
				case 7:
					return Finger_RightIndex;
					break;
				case 8:
					return Finger_RightMiddle;
					break;
				case 9:
					return Finger_RightRing;
					break;
				case 10:
					return Finger_RightLittle;
					break;
				case 11:
					return Finger_RightLittle;
					break;
				case 12:
					return Finger_RightLittle;
					break;
				case 13:
					return Finger_RightLittle;
					break;
				default:
					return Finger_Invalid;
					break;
			}
			break;
		case 3:
			switch(keyPos.x())
			{
				case 0:
					return Finger_LeftLittle;
					break;
				case 1:
					return Finger_LeftLittle;
					break;
				case 2:
					return Finger_LeftRing;
					break;
				case 3:
					return Finger_LeftMiddle;
					break;
				case 4:
					return Finger_LeftIndex;
					break;
				case 5:
					return Finger_LeftIndex;
					break;
				case 6:
					return Finger_RightIndex;
					break;
				case 7:
					return Finger_RightIndex;
					break;
				case 8:
					return Finger_RightMiddle;
					break;
				case 9:
					return Finger_RightRing;
					break;
				case 10:
					return Finger_RightLittle;
					break;
				case 11:
					return Finger_RightLittle;
					break;
				case 12:
					return Finger_RightLittle;
					break;
				default:
					return Finger_Invalid;
					break;
			}
			break;
		case 4:
			switch(keyPos.x())
			{
				case 2:
					// TODO: Add an option to set left thumb finger for space bar
					return Finger_RightThumb;
					break;
				default:
					return Finger_Invalid;
					break;
			}
			break;
		default:
			return Finger_Invalid;
			break;
	}
}

/*! Finds a key that contains label and returns its position. */
QPoint KeyboardWidget::findKey(QString label)
{
	QList<QLabel *> labels = keyLabels.values();
	for(int i = 0; i < labels.count(); i++)
	{
		if((labels[i]->text().contains(label, Qt::CaseInsensitive)) && (keyTypes[keyLabels.key(labels[i])] == 1))
		{
			QFrame *targetKey = keyLabels.key(labels[i]);
			QPair<int, int> keyPos = keyMap.key(targetKey);
			return QPoint(keyPos.first, keyPos.second);
		}
	}
	return QPoint();
}

/*!
 * Returns the hand of a finger.\n
 * -1 - Invalid\n
 * 0 - Left\n
 * 1 - Right
 */
int KeyboardWidget::fingerHand(KeyboardWidget::Finger finger)
{
	if(finger == Finger_Invalid)
		return -1;
	else if((finger == Finger_LeftThumb) || (finger == Finger_LeftIndex) || (finger == Finger_LeftMiddle) || (finger == Finger_LeftRing) || (finger == Finger_LeftLittle))
		return 0;
	return 1;
}

/*! Toggles keyboard visibility. */
void KeyboardWidget::toggleKeyboard(void)
{
	QPropertyAnimation *anim = new QPropertyAnimation(keyboardFrame, "geometry");
	anim->setDuration(250);
	setKeyboardVisible(!keyboardVisible, false);
	if(keyboardVisible)
	{
		anim->setEasingCurve(QEasingCurve::OutCubic);
		closeButton->setEnabled(false);
		keyboardFrame->show();
		keyboardFrame->adjustSize();
		QRect rect = keyboardFrame->geometry();
		QRect endRect = rect;
		rect.setY(rect.height());
		rect.setHeight(0);
		keyboardFrame->setGeometry(rect);
		anim->setStartValue(rect);
		anim->setEndValue(endRect);
		connect(anim, &QPropertyAnimation::finished, [this]() {
			closeButton->setEnabled(true);
		});
	}
	else
	{
		anim->setEasingCurve(QEasingCurve::InCubic);
		QRect rect = keyboardFrame->geometry();
		anim->setStartValue(rect);
		rect.setY(rect.height());
		rect.setHeight(0);
		anim->setEndValue(rect);
		closeButton->setEnabled(false);
		connect(anim, &QPropertyAnimation::finished, keyboardFrame, &QWidget::hide);
		connect(anim, &QPropertyAnimation::finished, [this]() {
			closeButton->hide();
			closeButton->setEnabled(true);
			closeButton->show();
		}); // hide and then show to prevent geometry glitches
	}
	anim->start();
}

/*! Sets keyboard visibility. */
void KeyboardWidget::setKeyboardVisible(bool visible, bool changeVisibility)
{
	settings.setValue("view/keyboardvisible", visible);
	if(visible)
	{
		closeButton->setIcon(QIcon(":/res/images/down.png"));
		closeButton->setToolTip(tr("Hide keyboard"));
	}
	else
	{
		closeButton->setIcon(QIcon(":/res/images/up.png"));
		closeButton->setToolTip(tr("Show keyboard"));
	}
	if(changeVisibility)
		keyboardFrame->setVisible(visible);
	keyboardVisible = visible;
}

/*! Overrides QWidget::changeEvent(). */
void KeyboardWidget::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::LanguageChange)
	{
		setKeyboardVisible(keyboardVisible, false);
		event->accept();
	}
	else
		QWidget::changeEvent(event);
}

/*! Overrides QFrame::eventFilter(). */
bool KeyboardWidget::eventFilter(QObject *obj, QEvent *event)
{
	if(obj == keyboardFrame)
	{
		if(event->type() == QEvent::Show)
			emit visibilityChanged(true);
		else if(event->type() == QEvent::Hide)
			emit visibilityChanged(false);
	}
	return QFrame::eventFilter(obj, event);
}
